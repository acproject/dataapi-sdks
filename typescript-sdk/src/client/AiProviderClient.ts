import { HttpClient } from '../http/HttpClient';
import { AiInvokeRequest, AiInvokeResponse } from '../types';
import { DataApiError } from '../errors';

/**
 * AI提供者客户端
 */
export class AiProviderClient {
  private httpClient: HttpClient;
  
  constructor(httpClient: HttpClient) {
    this.httpClient = httpClient;
  }
  
  /**
   * 调用AI模型
   */
  async invoke(request: AiInvokeRequest): Promise<AiInvokeResponse> {
    try {
      const response = await this.httpClient.post<AiInvokeResponse>('/ai/invoke', request);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to invoke AI', 'INVOKE_AI_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取AI配置列表
   */
  async getConfigs(): Promise<Record<string, any>[]> {
    try {
      const response = await this.httpClient.get<Record<string, any>[]>('/ai/configs');
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get AI configs', 'GET_AI_CONFIGS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 根据ID获取AI配置
   */
  async getConfig(configId: string): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.get<Record<string, any>>(`/ai/configs/${configId}`);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get AI config ${configId}`, 'GET_AI_CONFIG_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 创建AI配置
   */
  async createConfig(config: Record<string, any>): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.post<Record<string, any>>('/ai/configs', config);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to create AI config', 'CREATE_AI_CONFIG_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 更新AI配置
   */
  async updateConfig(configId: string, config: Record<string, any>): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.put<Record<string, any>>(`/ai/configs/${configId}`, config);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to update AI config ${configId}`, 'UPDATE_AI_CONFIG_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 删除AI配置
   */
  async deleteConfig(configId: string): Promise<void> {
    try {
      await this.httpClient.delete(`/ai/configs/${configId}`);
    } catch (error) {
      throw new DataApiError(`Failed to delete AI config ${configId}`, 'DELETE_AI_CONFIG_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 测试AI配置
   */
  async testConfig(configId: string, testInput?: any): Promise<{ success: boolean; message?: string }> {
    try {
      const response = await this.httpClient.post<{ success: boolean; message?: string }>(
        `/ai/configs/${configId}/test`,
        { input: testInput }
      );
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to test AI config ${configId}`, 'TEST_AI_CONFIG_ERROR', undefined, undefined, error as Error);
    }
  }
}