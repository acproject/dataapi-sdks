/**
 * TypeScript类型定义
 */

// 基础类型
export interface ApiResponse<T = any> {
  success: boolean;
  data?: T;
  message?: string;
  code?: string;
  timestamp?: string;
}

export interface PageResult<T> {
  content: T[];
  pageNumber: number;
  pageSize: number;
  totalElements: number;
  totalPages: number;
  first: boolean;
  last: boolean;
  empty: boolean;
}

// 工作流相关类型
export interface SysWorkflow {
  id?: string;
  name: string;
  description?: string;
  definition: string;
  projectId: string;
  userId: string;
  createTime?: string;
  updateTime?: string;
  status?: WorkflowStatus;
  version?: number;
}

export interface WorkflowCreateRequest {
  name: string;
  description?: string;
  definition: string;
  projectId: string;
  userId: string;
}

export interface WorkflowUpdateRequest {
  id: string;
  name?: string;
  description?: string;
  definition?: string;
  projectId?: string;
  userId?: string;
  updateTime?: string;
}

export interface WorkflowExecutionRequest {
  workflowId: string;
  projectId: string;
  userId: string;
  workflowDefinition: string;
  initialData?: any;
  timeoutMinutes?: number;
}

export enum WorkflowStatus {
  DRAFT = 'DRAFT',
  ACTIVE = 'ACTIVE',
  INACTIVE = 'INACTIVE',
  ARCHIVED = 'ARCHIVED'
}

export enum ExecutionStatus {
  PENDING = 'PENDING',
  RUNNING = 'RUNNING',
  COMPLETED = 'COMPLETED',
  FAILED = 'FAILED',
  CANCELLED = 'CANCELLED'
}

// HTTP相关类型
export interface HttpRequestConfig {
  method: 'GET' | 'POST' | 'PUT' | 'DELETE' | 'HEAD' | 'PATCH';
  url: string;
  headers?: Record<string, string>;
  params?: Record<string, any>;
  data?: any;
  timeout?: number;
}

export interface HttpResponse<T = any> {
  data: T;
  status: number;
  statusText: string;
  headers: Record<string, string>;
}

// 认证相关类型
export enum AuthenticationType {
  BEARER_TOKEN = 'BEARER_TOKEN',
  API_KEY = 'API_KEY',
  BASIC_AUTH = 'BASIC_AUTH',
  OAUTH2 = 'OAUTH2'
}

export interface TokenResponse {
  accessToken: string;
  refreshToken?: string;
  expiresIn?: number;
  tokenType?: string;
}

export interface TokenRefreshCallback {
  (token: string): Promise<TokenResponse>;
}

// 错误类型
export interface ErrorResponse {
  code: string;
  message: string;
  details?: any;
  timestamp?: string;
}

// 项目相关类型
export interface Project {
  id: string;
  name: string;
  description?: string;
  createTime: string;
  updateTime: string;
  userId: string;
  status: string;
}

// 用户相关类型
export interface User {
  id: string;
  username: string;
  email: string;
  displayName?: string;
  createTime: string;
  updateTime: string;
  status: string;
}

// 数据库相关类型
export interface SqlExecutionResult {
  success: boolean;
  rowsAffected?: number;
  data?: any[];
  columns?: string[];
  executionTime?: number;
  message?: string;
}

// AI相关类型
export interface AiInvokeRequest {
  configId: string;
  input: any;
  parameters?: Record<string, any>;
}

export interface AiInvokeResponse {
  output: any;
  usage?: {
    promptTokens?: number;
    completionTokens?: number;
    totalTokens?: number;
  };
  model?: string;
  finishReason?: string;
}