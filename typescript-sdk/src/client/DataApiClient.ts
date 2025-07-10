import { ClientConfig, ClientConfigBuilder } from '../config/ClientConfig';
import { AuthenticationProvider } from '../auth/AuthenticationProvider';
import { HttpClient } from '../http/HttpClient';
import { WorkflowClient } from './WorkflowClient';
import { ProjectClient } from './ProjectClient';
import { DatabaseClient } from './DatabaseClient';
import { AiProviderClient } from './AiProviderClient';
import { UserClient } from './UserClient';
import { DataApiError } from '../errors';

/**
 * DataAPI客户端主类
 */
export class DataApiClient {
  private config: ClientConfig;
  private authProvider: AuthenticationProvider;
  private httpClient: HttpClient;
  
  // 各种服务客户端
  private _workflowClient?: WorkflowClient;
  private _projectClient?: ProjectClient;
  private _databaseClient?: DatabaseClient;
  private _aiProviderClient?: AiProviderClient;
  private _userClient?: UserClient;
  
  constructor(config: Partial<ClientConfig>, authProvider: AuthenticationProvider) {
    // 合并配置
    this.config = {
      ...ClientConfigBuilder.default(),
      ...config
    } as ClientConfig;
    
    this.authProvider = authProvider;
    this.httpClient = new HttpClient(this.config, this.authProvider);
  }
  
  /**
   * 获取工作流客户端
   */
  get workflow(): WorkflowClient {
    if (!this._workflowClient) {
      this._workflowClient = new WorkflowClient(this.httpClient);
    }
    return this._workflowClient;
  }
  
  /**
   * 获取项目客户端
   */
  get project(): ProjectClient {
    if (!this._projectClient) {
      this._projectClient = new ProjectClient(this.httpClient);
    }
    return this._projectClient;
  }
  
  /**
   * 获取数据库客户端
   */
  get database(): DatabaseClient {
    if (!this._databaseClient) {
      this._databaseClient = new DatabaseClient(this.httpClient);
    }
    return this._databaseClient;
  }
  
  /**
   * 获取AI提供者客户端
   */
  get aiProvider(): AiProviderClient {
    if (!this._aiProviderClient) {
      this._aiProviderClient = new AiProviderClient(this.httpClient);
    }
    return this._aiProviderClient;
  }
  
  /**
   * 获取用户客户端
   */
  get user(): UserClient {
    if (!this._userClient) {
      this._userClient = new UserClient(this.httpClient);
    }
    return this._userClient;
  }
  
  /**
   * 测试连接
   */
  async testConnection(): Promise<boolean> {
    try {
      const response = await this.httpClient.get('/health');
      return response.status === 200;
    } catch (error) {
      if (this.config.enableLogging) {
        console.error('Connection test failed:', error);
      }
      return false;
    }
  }
  
  /**
   * 获取API版本信息
   */
  async getVersion(): Promise<string> {
    try {
      const response = await this.httpClient.get<{ version: string }>('/version');
      return response.data.version;
    } catch (error) {
      throw new DataApiError('Failed to get API version', 'VERSION_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取API健康状态
   */
  async getHealth(): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.get<Record<string, any>>('/health');
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get health status', 'HEALTH_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取配置信息
   */
  getConfig(): ClientConfig {
    return { ...this.config };
  }
  
  /**
   * 获取认证提供者
   */
  getAuthProvider(): AuthenticationProvider {
    return this.authProvider;
  }
  
  /**
   * 获取HTTP客户端
   */
  getHttpClient(): HttpClient {
    return this.httpClient;
  }
  
  /**
   * 更新配置
   */
  updateConfig(newConfig: Partial<ClientConfig>): void {
    this.config = { ...this.config, ...newConfig };
    // 重新创建HTTP客户端
    this.httpClient = new HttpClient(this.config, this.authProvider);
    
    // 清除缓存的客户端实例，以便使用新配置
    this._workflowClient = undefined;
    this._projectClient = undefined;
    this._databaseClient = undefined;
    this._aiProviderClient = undefined;
    this._userClient = undefined;
  }
  
  /**
   * 关闭客户端
   */
  close(): void {
    // 清除认证信息
    this.authProvider.clearAuthentication();
    
    // 清除客户端实例
    this._workflowClient = undefined;
    this._projectClient = undefined;
    this._databaseClient = undefined;
    this._aiProviderClient = undefined;
    this._userClient = undefined;
  }
  
  /**
   * 创建客户端的静态方法
   */
  static create(config: Partial<ClientConfig>, authProvider: AuthenticationProvider): DataApiClient {
    return new DataApiClient(config, authProvider);
  }
  
  /**
   * 创建开发环境客户端
   */
  static createDevelopment(authProvider: AuthenticationProvider): DataApiClient {
    return new DataApiClient(ClientConfigBuilder.development(), authProvider);
  }
  
  /**
   * 创建生产环境客户端
   */
  static createProduction(baseUrl: string, authProvider: AuthenticationProvider): DataApiClient {
    return new DataApiClient(ClientConfigBuilder.production(baseUrl), authProvider);
  }
}