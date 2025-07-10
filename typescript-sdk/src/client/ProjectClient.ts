import { HttpClient } from '../http/HttpClient';
import { Project, PageResult } from '../types';
import { DataApiError } from '../errors';

/**
 * 项目客户端
 */
export class ProjectClient {
  private httpClient: HttpClient;
  
  constructor(httpClient: HttpClient) {
    this.httpClient = httpClient;
  }
  
  /**
   * 获取项目列表
   */
  async getProjects(page: number = 0, size: number = 20): Promise<PageResult<Project>> {
    try {
      const response = await this.httpClient.get<PageResult<Project>>('/projects', { page, size });
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get projects', 'GET_PROJECTS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 根据ID获取项目
   */
  async getProject(id: string): Promise<Project> {
    try {
      const response = await this.httpClient.get<Project>(`/projects/${id}`);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get project ${id}`, 'GET_PROJECT_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 创建项目
   */
  async createProject(project: Omit<Project, 'id' | 'createTime' | 'updateTime'>): Promise<Project> {
    try {
      const response = await this.httpClient.post<Project>('/projects', project);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to create project', 'CREATE_PROJECT_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 更新项目
   */
  async updateProject(id: string, project: Partial<Project>): Promise<Project> {
    try {
      const response = await this.httpClient.put<Project>(`/projects/${id}`, project);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to update project ${id}`, 'UPDATE_PROJECT_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 删除项目
   */
  async deleteProject(id: string): Promise<void> {
    try {
      await this.httpClient.delete(`/projects/${id}`);
    } catch (error) {
      throw new DataApiError(`Failed to delete project ${id}`, 'DELETE_PROJECT_ERROR', undefined, undefined, error as Error);
    }
  }
}