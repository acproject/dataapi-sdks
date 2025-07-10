/**
 * DataAPI TypeScript SDK
 * 
 * 提供对DataAPI后端服务的TypeScript客户端访问
 */

export { DataApiClient } from './client/DataApiClient';
export { ClientConfig } from './config/ClientConfig';
export { AuthenticationProvider, BearerTokenAuthProvider } from './auth';
export { WorkflowClient } from './client/WorkflowClient';
export { ProjectClient } from './client/ProjectClient';
export { DatabaseClient } from './client/DatabaseClient';
export { AiProviderClient } from './client/AiProviderClient';
export { UserClient } from './client/UserClient';
export { HttpClient } from './http/HttpClient';
export { DataApiError, AuthenticationError } from './errors';
export * from './types';

// 版本信息
export const VERSION = '1.0.0';

// 默认配置
export const DEFAULT_CONFIG = {
  baseUrl: 'http://localhost:8080',
  timeout: 30000,
  retryAttempts: 3,
  retryDelay: 1000
};

/**
 * 创建DataAPI客户端的便捷方法
 */
export function createClient(config: Partial<ClientConfig>, authProvider: AuthenticationProvider): DataApiClient {
  return new DataApiClient(config, authProvider);
}

/**
 * 创建Bearer Token认证提供者的便捷方法
 */
export function createBearerTokenAuth(token: string): BearerTokenAuthProvider {
  return new BearerTokenAuthProvider(token);
}