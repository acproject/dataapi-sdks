import { HttpClient } from '../http/HttpClient';
import { SqlExecutionResult } from '../types';
import { DataApiError } from '../errors';

/**
 * 数据库客户端
 */
export class DatabaseClient {
  private httpClient: HttpClient;
  
  constructor(httpClient: HttpClient) {
    this.httpClient = httpClient;
  }
  
  /**
   * 执行SQL语句
   */
  async executeSql(sql: string, parameters?: Record<string, any>): Promise<SqlExecutionResult> {
    try {
      const response = await this.httpClient.post<SqlExecutionResult>('/database/execute', {
        sql,
        parameters
      });
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to execute SQL', 'EXECUTE_SQL_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 执行查询SQL
   */
  async query(sql: string, parameters?: Record<string, any>): Promise<any[]> {
    try {
      const result = await this.executeSql(sql, parameters);
      return result.data || [];
    } catch (error) {
      throw new DataApiError('Failed to execute query', 'QUERY_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 执行更新SQL（INSERT, UPDATE, DELETE）
   */
  async update(sql: string, parameters?: Record<string, any>): Promise<number> {
    try {
      const result = await this.executeSql(sql, parameters);
      return result.rowsAffected || 0;
    } catch (error) {
      throw new DataApiError('Failed to execute update', 'UPDATE_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取数据库连接信息
   */
  async getConnectionInfo(): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.get<Record<string, any>>('/database/connection');
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get connection info', 'GET_CONNECTION_INFO_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 测试数据库连接
   */
  async testConnection(): Promise<boolean> {
    try {
      const response = await this.httpClient.get<{ connected: boolean }>('/database/test');
      return response.data.connected;
    } catch (error) {
      return false;
    }
  }
  
  /**
   * 获取数据库表列表
   */
  async getTables(): Promise<string[]> {
    try {
      const response = await this.httpClient.get<string[]>('/database/tables');
      return response.data;
    } catch (error) {
      throw new DataApiError('Failed to get tables', 'GET_TABLES_ERROR', undefined, undefined, error as Error);
    }
  }
  
  /**
   * 获取表结构
   */
  async getTableSchema(tableName: string): Promise<Record<string, any>> {
    try {
      const response = await this.httpClient.get<Record<string, any>>(`/database/tables/${tableName}/schema`);
      return response.data;
    } catch (error) {
      throw new DataApiError(`Failed to get schema for table ${tableName}`, 'GET_TABLE_SCHEMA_ERROR', undefined, undefined, error as Error);
    }
  }
}