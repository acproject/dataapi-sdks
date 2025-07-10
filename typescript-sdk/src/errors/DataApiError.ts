/**
 * DataAPI基础错误类
 */
export class DataApiError extends Error {
  public readonly code?: string;
  public readonly statusCode?: number;
  public readonly responseBody?: any;
  public readonly timestamp: string;
  
  constructor(
    message: string,
    code?: string,
    statusCode?: number,
    responseBody?: any,
    cause?: Error
  ) {
    super(message);
    this.name = 'DataApiError';
    this.code = code ?? undefined;
    this.statusCode = statusCode ?? undefined;
    this.responseBody = responseBody ?? undefined;
    this.timestamp = new Date().toISOString();
    
    if (cause) {
      (this as any).cause = cause;
    }
    
    // 确保错误堆栈正确显示
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, DataApiError);
    }
  }
  
  /**
   * 检查是否为HTTP错误
   */
  isHttpError(): boolean {
    return this.statusCode !== undefined;
  }
  
  /**
   * 检查是否为客户端错误（4xx）
   */
  isClientError(): boolean {
    return this.statusCode !== undefined && this.statusCode >= 400 && this.statusCode < 500;
  }
  
  /**
   * 检查是否为服务器错误（5xx）
   */
  isServerError(): boolean {
    return this.statusCode !== undefined && this.statusCode >= 500;
  }
  
  /**
   * 获取错误详情
   */
  getDetails(): Record<string, any> {
    return {
      name: this.name,
      message: this.message,
      code: this.code,
      statusCode: this.statusCode,
      responseBody: this.responseBody,
      timestamp: this.timestamp,
      stack: this.stack
    };
  }
  
  /**
   * 转换为JSON格式
   */
  toJSON(): Record<string, any> {
    return {
      name: this.name,
      message: this.message,
      code: this.code,
      statusCode: this.statusCode,
      timestamp: this.timestamp
    };
  }
}