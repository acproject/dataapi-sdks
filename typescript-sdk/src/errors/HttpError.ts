import { DataApiError } from './DataApiError';

/**
 * HTTP错误类
 */
export class HttpError extends DataApiError {
  public readonly url?: string;
  public readonly method?: string;
  
  constructor(
    message: string,
    statusCode: number,
    responseBody?: any,
    url?: string,
    method?: string,
    cause?: Error
  ) {
    super(message, 'HTTP_ERROR', statusCode, responseBody, cause);
    this.name = 'HttpError';
    this.url = url ?? '';
    this.method = method ?? '';
  }
  
  /**
   * 创建网络错误
   */
  static networkError(message: string = 'Network error occurred', cause?: Error): HttpError {
    return new HttpError(message, 0, undefined, undefined, undefined, cause);
  }
  
  /**
   * 创建超时错误
   */
  static timeout(message: string = 'Request timeout', url?: string, method?: string): HttpError {
    return new HttpError(message, 408, undefined, url, method);
  }
  
  /**
   * 创建404错误
   */
  static notFound(message: string = 'Resource not found', url?: string, method?: string): HttpError {
    return new HttpError(message, 404, undefined, url, method);
  }
  
  /**
   * 创建500错误
   */
  static internalServerError(message: string = 'Internal server error', responseBody?: any, url?: string, method?: string): HttpError {
    return new HttpError(message, 500, responseBody, url, method);
  }
  
  /**
   * 创建503错误
   */
  static serviceUnavailable(message: string = 'Service unavailable', url?: string, method?: string): HttpError {
    return new HttpError(message, 503, undefined, url, method);
  }
  
  /**
   * 获取错误详情
   */
  getDetails(): Record<string, any> {
    return {
      ...super.getDetails(),
      url: this.url,
      method: this.method
    };
  }
  
  /**
   * 转换为JSON格式
   */
  override toJSON(): Record<string, any> {
    return {
      ...super.toJSON(),
      url: this.url,
      method: this.method
    };
  }
}