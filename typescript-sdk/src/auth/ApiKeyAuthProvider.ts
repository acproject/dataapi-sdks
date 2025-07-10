import { AuthenticationProvider } from './AuthenticationProvider';
import { AuthenticationType, TokenResponse } from '../types';

/**
 * API Key认证提供者
 */
export class ApiKeyAuthProvider implements AuthenticationProvider {
  private apiKey: string;
  private headerName: string;
  
  constructor(apiKey: string, headerName: string = 'X-API-Key') {
    this.apiKey = apiKey;
    this.headerName = headerName;
  }
  
  getAuthenticationType(): AuthenticationType {
    return AuthenticationType.API_KEY;
  }
  
  async getAccessToken(): Promise<string> {
    return this.apiKey;
  }
  
  async refreshToken(): Promise<TokenResponse> {
    // API Key通常不需要刷新
    return {
      accessToken: this.apiKey
    };
  }
  
  async isTokenValid(): Promise<boolean> {
    return this.apiKey.length > 0;
  }
  
  clearAuthentication(): void {
    this.apiKey = '';
  }
  
  async getAuthenticationHeaders(): Promise<Record<string, string>> {
    return {
      [this.headerName]: this.apiKey
    };
  }
  
  /**
   * 更新API Key
   */
  updateApiKey(apiKey: string): void {
    this.apiKey = apiKey;
  }
  
  /**
   * 获取头名称
   */
  getHeaderName(): string {
    return this.headerName;
  }
}