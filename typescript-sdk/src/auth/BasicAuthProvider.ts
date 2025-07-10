import { AuthenticationProvider } from './AuthenticationProvider';
import { AuthenticationType, TokenResponse } from '../types';

/**
 * 基础认证提供者
 */
export class BasicAuthProvider implements AuthenticationProvider {
  private username: string;
  private password: string;
  
  constructor(username: string, password: string) {
    this.username = username;
    this.password = password;
  }
  
  getAuthenticationType(): AuthenticationType {
    return AuthenticationType.BASIC_AUTH;
  }
  
  async getAccessToken(): Promise<string> {
    // 对于Basic Auth，返回base64编码的凭据
    return Buffer.from(`${this.username}:${this.password}`).toString('base64');
  }
  
  async refreshToken(): Promise<TokenResponse> {
    // Basic Auth通常不需要刷新
    const token = await this.getAccessToken();
    return {
      accessToken: token
    };
  }
  
  async isTokenValid(): Promise<boolean> {
    return this.username.length > 0 && this.password.length > 0;
  }
  
  clearAuthentication(): void {
    this.username = '';
    this.password = '';
  }
  
  async getAuthenticationHeaders(): Promise<Record<string, string>> {
    const token = await this.getAccessToken();
    return {
      'Authorization': `Basic ${token}`
    };
  }
  
  /**
   * 更新凭据
   */
  updateCredentials(username: string, password: string): void {
    this.username = username;
    this.password = password;
  }
}