package com.owiseman.dataapi.sdk;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.client.*;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * DataAPI客户端主类
 * 提供对DataAPI后端服务的统一访问接口
 */
public class DataApiClient {
    private static final Logger logger = LoggerFactory.getLogger(DataApiClient.class);
    
    private final ClientConfig config;
    private final AuthenticationProvider authProvider;
    
    // 各种服务客户端
    private final WorkflowClient workflowClient;
    private final ProjectClient projectClient;
    private final DatabaseClient databaseClient;
    private final AiProviderClient aiProviderClient;
    private final UserClient userClient;
    
    /**
     * 构造函数
     * @param config 客户端配置
     * @param authProvider 认证提供者
     */
    public DataApiClient(ClientConfig config, AuthenticationProvider authProvider) {
        this.config = config;
        this.authProvider = authProvider;
        
        // 初始化各种客户端
        this.workflowClient = new WorkflowClient(config, authProvider);
        this.projectClient = new ProjectClient(config, authProvider);
        this.databaseClient = new DatabaseClient(config, authProvider);
        this.aiProviderClient = new AiProviderClient(config, authProvider);
        this.userClient = new UserClient(config, authProvider);
        
        logger.info("DataAPI客户端初始化完成，服务端地址: {}", config.getBaseUrl());
    }
    
    /**
     * 获取工作流客户端
     */
    public WorkflowClient workflows() {
        return workflowClient;
    }
    
    /**
     * 获取项目客户端
     */
    public ProjectClient projects() {
        return projectClient;
    }
    
    /**
     * 获取数据库客户端
     */
    public DatabaseClient databases() {
        return databaseClient;
    }
    
    /**
     * 获取AI提供者客户端
     */
    public AiProviderClient aiProviders() {
        return aiProviderClient;
    }
    
    /**
     * 获取用户客户端
     */
    public UserClient users() {
        return userClient;
    }
    
    /**
     * 测试连接
     */
    public boolean testConnection() {
        try {
            // 尝试获取用户信息来测试连接
            userClient.getCurrentUser();
            return true;
        } catch (DataApiException e) {
            logger.error("连接测试失败", e);
            return false;
        }
    }
    
    /**
     * 关闭客户端，释放资源
     */
    public void close() {
        logger.info("关闭DataAPI客户端");
        // 这里可以添加资源清理逻辑
    }
    
    /**
     * 获取客户端配置
     */
    public ClientConfig getConfig() {
        return config;
    }
}