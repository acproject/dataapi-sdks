import { HttpClient } from '../http/HttpClient';
import { User, PageResult } from '../types';
import { DataApiError } from '../errors';

/**
 * 用户客户端
 */
export class UserClient {
  private httpClient: HttpClient;
  
  constructor(httpClient: HttpClient) {
    this.httpClient = httpClient;
  }
  
  /**
   * 获取当前用户信息
   */
  async getCurrentUser(): Promise<User> {
    try {
      const response = await this.httpClient.get<User>('/users/me');
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get current user', 'GET_CURRENT_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 根据ID获取用户信息
   */
  async getUser(id: string): Promise<User> {
    try {
      const response = await this.httpClient.get<User>(`/users/${id}`);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get user ${id}`, 'GET_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取用户列表
   */
  async getUsers(page: number = 0, size: number = 20): Promise<PageResult<User>> {
    try {
      const response = await this.httpClient.get<PageResult<User>>('/users', { page, size });
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get users', 'GET_USERS_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 更新当前用户信息
   */
  async updateCurrentUser(user: Partial<User>): Promise<User> {
    try {
      const response = await this.httpClient.put<User>('/users/me', user);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to update current user', 'UPDATE_CURRENT_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 更新用户信息
   */
  async updateUser(id: string, user: Partial<User>): Promise<User> {
    try {
      const response = await this.httpClient.put<User>(`/users/${id}`, user);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to update user ${id}`, 'UPDATE_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 创建用户
   */
  async createUser(user: Omit<User, 'id' | 'createTime' | 'updateTime'>): Promise<User> {
    try {
      const response = await this.httpClient.post<User>('/users', user);
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to create user', 'CREATE_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 删除用户
   */
  async deleteUser(id: string): Promise<void> {
    try {
      await this.httpClient.delete(`/users/${id}`);
    } catch (error) {
      throw new DataApiError(`Failed to delete user ${id}`, 'DELETE_USER_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 修改密码
   */
  async changePassword(oldPassword: string, newPassword: string): Promise<void> {
    try {
      await this.httpClient.post('/users/me/change-password', {
        oldPassword,
        newPassword
      });
    } catch (error) {
      throw new DataApiError('Failed to change password', 'CHANGE_PASSWORD_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 重置密码
   */
  async resetPassword(userId: string, newPassword: string): Promise<void> {
    try {
      await this.httpClient.post(`/users/${userId}/reset-password`, {
        newPassword
      });
    } catch (error) {
      throw new DataApiError(`Failed to reset password for user ${userId}`, 'RESET_PASSWORD_ERROR', undefined, undefined, error as Error);
    }
  }
}