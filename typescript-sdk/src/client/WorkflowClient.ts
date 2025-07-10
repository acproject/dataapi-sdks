import { HttpClient } from '../http/HttpClient';
import { 
  SysWorkflow, 
  WorkflowCreateRequest, 
  WorkflowUpdateRequest, 
  WorkflowExecutionRequest,
  PageResult,
  ExecutionStatus
} from '../types';
import { DataApiError } from '../errors';

/**
 * 工作流客户端
 */
export class WorkflowClient {
  private httpClient: HttpClient;
  
  constructor(httpClient: HttpClient) {
    this.httpClient = httpClient;
  }
  
  /**
   * 获取工作流列表
   */
  async getWorkflows(
    page: number = 0,
    size: number = 20,
    projectId?: string,
    userId?: string
  ): Promise<PageResult<SysWorkflow>> {
    try {
      const params: Record<string, any> = { page, size };
      if (projectId) params['projectId'] = projectId;
      if (userId) params['userId'] = userId;
      
      const response = await this.httpClient.get<PageResult<SysWorkflow>>('/workflows', params);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get workflows', 'GET_WORKFLOWS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 根据ID获取工作流
   */
  async getWorkflow(id: string): Promise<SysWorkflow> {
    try {
      const response = await this.httpClient.get<SysWorkflow>(`/workflows/${id}`);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get workflow ${id}`, 'GET_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 创建工作流
   */
  async createWorkflow(request: WorkflowCreateRequest): Promise<SysWorkflow> {
    try {
      const response = await this.httpClient.post<SysWorkflow>('/workflows', request);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to create workflow', 'CREATE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 更新工作流
   */
  async updateWorkflow(request: WorkflowUpdateRequest): Promise<SysWorkflow> {
    try {
      const response = await this.httpClient.put<SysWorkflow>(`/workflows/${request.id}`, request);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to update workflow ${request.id}`, 'UPDATE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 删除工作流
   */
  async deleteWorkflow(id: string): Promise<void> {
    try {
      await this.httpClient.delete(`/workflows/${id}`);
    } catch (error) {
      throw new DataApiError(`Failed to delete workflow ${id}`, 'DELETE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 检查工作流是否存在
   */
  async workflowExists(id: string): Promise<boolean> {
    try {
      const response = await this.httpClient.head(`/workflows/${id}`);
      return response.status === 200;
    } catch (error) {
      return false;
    }
  }
  
  /**
   * 执行工作流
   */
  async executeWorkflow(request: WorkflowExecutionRequest): Promise<{ executionId: string }> {
    try {
      const response = await this.httpClient.post<{ executionId: string }>('/workflows/execute', request);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to execute workflow', 'EXECUTE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取工作流执行状态
   */
  async getExecutionStatus(executionId: string): Promise<{ status: ExecutionStatus; result?: any; error?: string }> {
    try {
      const response = await this.httpClient.get<{ status: ExecutionStatus; result?: any; error?: string }>(
        `/workflows/executions/${executionId}/status`
      );
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get execution status ${executionId}`, 'GET_EXECUTION_STATUS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 停止工作流执行
   */
  async stopExecution(executionId: string): Promise<void> {
    try {
      await this.httpClient.post(`/workflows/executions/${executionId}/stop`);
    } catch (error) {
      throw new DataApiError(`Failed to stop execution ${executionId}`, 'STOP_EXECUTION_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取工作流执行日志
   */
  async getExecutionLogs(executionId: string, page: number = 0, size: number = 100): Promise<PageResult<string>> {
    try {
      const response = await this.httpClient.get<PageResult<string>>(
        `/workflows/executions/${executionId}/logs`,
        { page, size }
      );
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get execution logs ${executionId}`, 'GET_EXECUTION_LOGS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取工作流执行历史
   */
  async getExecutionHistory(
    workflowId: string,
    page: number = 0,
    size: number = 20
  ): Promise<PageResult<{ executionId: string; status: ExecutionStatus; startTime: string; endTime?: string }>> {
    try {
      const response = await this.httpClient.get<PageResult<{ executionId: string; status: ExecutionStatus; startTime: string; endTime?: string }>>(
        `/workflows/${workflowId}/executions`,
        { page, size }
      );
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get execution history for workflow ${workflowId}`, 'GET_EXECUTION_HISTORY_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 验证工作流定义
   */
  async validateWorkflow(definition: string): Promise<{ valid: boolean; errors?: string[] }> {
    try {
      const response = await this.httpClient.post<{ valid: boolean; errors?: string[] }>(
        '/workflows/validate',
        { definition }
      );
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to validate workflow', 'VALIDATE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 复制工作流
   */
  async duplicateWorkflow(id: string, newName?: string): Promise<SysWorkflow> {
    try {
      const response = await this.httpClient.post<SysWorkflow>(
        `/workflows/${id}/duplicate`,
        newName ? { name: newName } : {}
      );
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to duplicate workflow ${id}`, 'DUPLICATE_WORKFLOW_ERROR', undefined, undefined, error as Error);
    }
  }
}