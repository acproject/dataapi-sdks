/**
 * 客户端配置类
 */

export interface ClientConfig {
  /** API基础URL */
  baseUrl: string;
  
  /** 请求超时时间（毫秒） */
  timeout: number;
  
  /** 重试次数 */
  retryAttempts: number;
  
  /** 重试延迟（毫秒） */
  retryDelay: number;
  
  /** 默认请求头 */
  defaultHeaders: Record<string, string>;
  
  /** 是否启用日志 */
  enableLogging: boolean;
  
  /** 用户代理 */
  userAgent: string;
  
  /** 是否验证SSL证书 */
  validateSSL: boolean;
  
  /** 代理配置 */
  proxy?: {
    host: string;
    port: number;
    auth?: {
      username: string;
      password: string;
    };
  };
}

/**
 * 客户端配置构建器
 */
export class ClientConfigBuilder {
  private config: Partial<ClientConfig> = {};
  
  constructor() {
    // 设置默认值
    this.config = {
      baseUrl: 'http://localhost:8080',
      timeout: 30000,
      retryAttempts: 3,
      retryDelay: 1000,
      defaultHeaders: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
      },
      enableLogging: false,
      userAgent: 'DataAPI-TypeScript-SDK/1.0.0',
      validateSSL: true
    };
  }
  
  baseUrl(url: string): ClientConfigBuilder {
    this.config.baseUrl = url;
    return this;
  }
  
  timeout(ms: number): ClientConfigBuilder {
    this.config.timeout = ms;
    return this;
  }
  
  retryAttempts(attempts: number): ClientConfigBuilder {
    this.config.retryAttempts = attempts;
    return this;
  }
  
  retryDelay(delay: number): ClientConfigBuilder {
    this.config.retryDelay = delay;
    return this;
  }
  
  defaultHeaders(headers: Record<string, string>): ClientConfigBuilder {
    this.config.defaultHeaders = { ...this.config.defaultHeaders, ...headers };
    return this;
  }
  
  enableLogging(enable: boolean): ClientConfigBuilder {
    this.config.enableLogging = enable;
    return this;
  }
  
  userAgent(agent: string): ClientConfigBuilder {
    this.config.userAgent = agent;
    return this;
  }
  
  validateSSL(validate: boolean): ClientConfigBuilder {
    this.config.validateSSL = validate;
    return this;
  }
  
  proxy(proxyConfig: ClientConfig['proxy']): ClientConfigBuilder {
    this.config.proxy = proxyConfig;
    return this;
  }
  
  build(): ClientConfig {
    return this.config as ClientConfig;
  }
  
  /**
   * 创建默认配置
   */
  static default(): ClientConfig {
    return new ClientConfigBuilder().build();
  }
  
  /**
   * 创建开发环境配置
   */
  static development(): ClientConfig {
    return new ClientConfigBuilder()
      .baseUrl('http://localhost:8080')
      .enableLogging(true)
      .validateSSL(false)
      .build();
  }
  
  /**
   * 创建生产环境配置
   */
  static production(baseUrl: string): ClientConfig {
    return new ClientConfigBuilder()
      .baseUrl(baseUrl)
      .enableLogging(false)
      .validateSSL(true)
      .timeout(60000)
      .retryAttempts(5)
      .build();
  }
}