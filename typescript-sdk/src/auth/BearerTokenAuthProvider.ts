import { AuthenticationProvider } from './AuthenticationProvider';
import { AuthenticationType, TokenResponse, TokenRefreshCallback } from '../types';
import { AuthenticationError } from '../errors';

/**
 * Bearer Token认证提供者
 */
export class BearerTokenAuthProvider implements AuthenticationProvider {
  private token: string;
  private refreshCallback?: TokenRefreshCallback;
  private expiresAt?: Date;
  
  constructor(
    token: string,
    refreshCallback?: TokenRefreshCallback,
    expiresIn?: number
  ) {
    this.token = token;
    this.refreshCallback = refreshCallback;
    
    if (expiresIn) {
      this.expiresAt = new Date(Date.now() + expiresIn * 1000);
    }
  }
  
  getAuthenticationType(): AuthenticationType {
    return AuthenticationType.BEARER_TOKEN;
  }
  
  async getAccessToken(): Promise<string> {
    if (await this.isTokenExpired() && this.refreshCallback) {
      await this.refreshToken();
    }
    return this.token;
  }
  
  async refreshToken(): Promise<TokenResponse> {
    if (!this.refreshCallback) {
      throw new AuthenticationError('No refresh callback provided');
    }
    
    try {
      const response = await this.refreshCallback(this.token);
      this.token = response.accessToken;
      
      if (response.expiresIn) {
        this.expiresAt = new Date(Date.now() + response.expiresIn * 1000);
      }
      
      return response;
    } catch (error) {
      throw new AuthenticationError('Failed to refresh token', error);
    }
  }
  
  async isTokenValid(): Promise<boolean> {
    return !await this.isTokenExpired() && this.token.length > 0;
  }
  
  private async isTokenExpired(): Promise<boolean> {
    if (!this.expiresAt) {
      return false; // 如果没有过期时间，假设token有效
    }
    
    // 提前5分钟刷新token
    const bufferTime = 5 * 60 * 1000; // 5分钟
    return Date.now() >= (this.expiresAt.getTime() - bufferTime);
  }
  
  clearAuthentication(): void {
    this.token = '';
    this.expiresAt = undefined;
  }
  
  async getAuthenticationHeaders(): Promise<Record<string, string>> {
    const token = await this.getAccessToken();
    return {
      'Authorization': `Bearer ${token}`
    };
  }
  
  /**
   * 更新token
   */
  updateToken(token: string, expiresIn?: number): void {
    this.token = token;
    
    if (expiresIn) {
      this.expiresAt = new Date(Date.now() + expiresIn * 1000);
    }
  }
  
  /**
   * 设置刷新回调
   */
  setRefreshCallback(callback: TokenRefreshCallback): void {
    this.refreshCallback = callback;
  }
}