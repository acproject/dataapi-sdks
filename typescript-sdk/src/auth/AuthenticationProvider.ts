import { AuthenticationType, TokenResponse } from '../types';

/**
 * 认证提供者接口
 */
export interface AuthenticationProvider {
  /**
   * 获取认证类型
   */
  getAuthenticationType(): AuthenticationType;
  
  /**
   * 获取访问令牌
   */
  getAccessToken(): Promise<string>;
  
  /**
   * 刷新访问令牌
   */
  refreshToken(): Promise<TokenResponse>;
  
  /**
   * 验证令牌是否有效
   */
  isTokenValid(): Promise<boolean>;
  
  /**
   * 清除认证信息
   */
  clearAuthentication(): void;
  
  /**
   * 获取认证头
   */
  getAuthenticationHeaders(): Promise<Record<string, string>>;
}