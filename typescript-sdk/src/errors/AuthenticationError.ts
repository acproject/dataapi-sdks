import { DataApiError } from './DataApiError';

/**
 * 认证错误类
 */
export class AuthenticationError extends DataApiError {
  constructor(
    message: string,
    cause?: Error,
    statusCode?: number,
    responseBody?: any
  ) {
    super(message, 'AUTHENTICATION_ERROR', statusCode, responseBody, cause);
    this.name = 'AuthenticationError';
  }
  
  /**
   * 创建token过期错误
   */
  static tokenExpired(message: string = 'Access token has expired'): AuthenticationError {
    return new AuthenticationError(message, undefined, 401);
  }
  
  /**
   * 创建token无效错误
   */
  static invalidToken(message: string = 'Invalid access token'): AuthenticationError {
    return new AuthenticationError(message, undefined, 401);
  }
  
  /**
   * 创建认证失败错误
   */
  static authenticationFailed(message: string = 'Authentication failed'): AuthenticationError {
    return new AuthenticationError(message, undefined, 401);
  }
  
  /**
   * 创建权限不足错误
   */
  static insufficientPermissions(message: string = 'Insufficient permissions'): AuthenticationError {
    return new AuthenticationError(message, undefined, 403);
  }
}