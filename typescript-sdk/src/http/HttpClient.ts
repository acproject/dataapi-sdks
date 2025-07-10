import axios, { AxiosInstance, AxiosRequestConfig, AxiosResponse } from 'axios';
import { ClientConfig } from '../config/ClientConfig';
import { AuthenticationProvider } from '../auth/AuthenticationProvider';
import { HttpRequestConfig, HttpResponse, ApiResponse } from '../types';
import { HttpError, DataApiError } from '../errors';

/**
 * HTTP客户端类
 */
export class HttpClient {
  private axiosInstance: AxiosInstance;
  private config: ClientConfig;
  private authProvider: AuthenticationProvider;
  
  constructor(config: ClientConfig, authProvider: AuthenticationProvider) {
    this.config = config;
    this.authProvider = authProvider;
    
    this.axiosInstance = axios.create({
      baseURL: config.baseUrl,
      timeout: config.timeout,
      headers: config.defaultHeaders,
      validateStatus: () => true // 让我们自己处理状态码
    });
    
    this.setupInterceptors();
  }
  
  /**
   * 设置请求和响应拦截器
   */
  private setupInterceptors(): void {
    // 请求拦截器
    this.axiosInstance.interceptors.request.use(
      async (config) => {
        // 添加认证头
        const authHeaders = await this.authProvider.getAuthenticationHeaders();
        config.headers = { ...config.headers, ...authHeaders };
        
        if (this.config.enableLogging) {
          console.log(`[HTTP] ${config.method?.toUpperCase()} ${config.url}`);
        }
        
        return config;
      },
      (error) => {
        if (this.config.enableLogging) {
          console.error('[HTTP] Request error:', error);
        }
        return Promise.reject(error);
      }
    );
    
    // 响应拦截器
    this.axiosInstance.interceptors.response.use(
      (response) => {
        if (this.config.enableLogging) {
          console.log(`[HTTP] ${response.status} ${response.config.method?.toUpperCase()} ${response.config.url}`);
        }
        return response;
      },
      (error) => {
        if (this.config.enableLogging) {
          console.error('[HTTP] Response error:', error);
        }
        return Promise.reject(error);
      }
    );
  }
  
  /**
   * 执行HTTP请求
   */
  async request<T = any>(requestConfig: HttpRequestConfig): Promise<HttpResponse<T>> {
    const { method, url, headers, params, data, timeout } = requestConfig;
    
    const axiosConfig: AxiosRequestConfig = {
      method,
      url,
      headers: headers || {},
      params,
      data,
      timeout: timeout || this.config.timeout
    };
    
    let lastError: Error | undefined;
    
    // 重试逻辑
    for (let attempt = 0; attempt <= this.config.retryAttempts; attempt++) {
      try {
        const response = await this.axiosInstance.request(axiosConfig);
        
        // 检查HTTP状态码
        if (response.status >= 400) {
          throw new HttpError(
            `HTTP ${response.status}: ${response.statusText}`,
            response.status,
            response.data,
            url,
            method
          );
        }
        
        return {
          data: response.data,
          status: response.status,
          statusText: response.statusText,
          headers: response.headers as Record<string, string>
        };
      } catch (error) {
        lastError = error as Error;
        
        // 如果是最后一次尝试，或者不是网络错误，直接抛出
        if (attempt === this.config.retryAttempts || !this.isRetryableError(error)) {
          break;
        }
        
        // 等待后重试
        await this.delay(this.config.retryDelay * Math.pow(2, attempt));
      }
    }
    
    // 处理最终错误
    if (lastError) {
      if (axios.isAxiosError(lastError)) {
        if (lastError.code === 'ECONNABORTED') {
          throw HttpError.timeout('Request timeout', url, method);
        } else if (lastError.response) {
          throw new HttpError(
            lastError.message,
            lastError.response.status,
            lastError.response.data,
            url,
            method,
            lastError
          );
        } else {
          throw HttpError.networkError(lastError.message, lastError);
        }
      } else {
        throw new DataApiError('Request failed', 'REQUEST_ERROR', undefined, undefined, lastError);
      }
    }
    
    throw new DataApiError('Unknown error occurred', 'UNKNOWN_ERROR');
  }
  
  /**
   * GET请求
   */
  async get<T = any>(url: string, params?: Record<string, any>, headers?: Record<string, string>): Promise<HttpResponse<T>> {
    return this.request<T>({
      method: 'GET',
      url,
      params,
      headers: headers || {}
    });
  }
  
  /**
   * POST请求
   */
  async post<T = any>(url: string, data?: any, headers?: Record<string, string>): Promise<HttpResponse<T>> {
    return this.request<T>({
      method: 'POST',
      url,
      data,
      headers: headers || {}
    });
  }
  
  /**
   * PUT请求
   */
  async put<T = any>(url: string, data?: any, headers?: Record<string, string>): Promise<HttpResponse<T>> {
    return this.request<T>({
      method: 'PUT',
      url,
      data,
      headers: headers || {}
    });
  }
  
  /**
   * DELETE请求
   */
  async delete<T = any>(url: string, headers?: Record<string, string>): Promise<HttpResponse<T>> {
    return this.request<T>({
      method: 'DELETE',
      url,
      headers: headers || {}
    });
  }
  
  /**
   * HEAD请求
   */
  async head(url: string, headers?: Record<string, string>): Promise<HttpResponse<void>> {
    return this.request<void>({
      method: 'HEAD',
      url,
      headers: headers || {}
    });
  }
  
  /**
   * 构建URL
   */
  buildUrl(path: string, params?: Record<string, any>): string {
    let url = path.startsWith('/') ? path : `/${path}`;
    
    if (params && Object.keys(params).length > 0) {
      const searchParams = new URLSearchParams();
      Object.entries(params).forEach(([key, value]) => {
        if (value !== undefined && value !== null) {
          searchParams.append(key, String(value));
        }
      });
      url += `?${searchParams.toString()}`;
    }
    
    return url;
  }
  
  /**
   * 检查是否为可重试的错误
   */
  private isRetryableError(error: any): boolean {
    if (axios.isAxiosError(error)) {
      // 网络错误或超时
      if (!error.response) {
        return true;
      }
      
      // 5xx服务器错误
      if (error.response.status >= 500) {
        return true;
      }
      
      // 429 Too Many Requests
      if (error.response.status === 429) {
        return true;
      }
    }
    
    return false;
  }
  
  /**
   * 延迟函数
   */
  private delay(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }
  
  /**
   * 获取Axios实例
   */
  getAxiosInstance(): AxiosInstance {
    return this.axiosInstance;
  }
}