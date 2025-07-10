package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;

/**
 * 项目客户端
 */
public class ProjectClient extends BaseClient {
    
    public ProjectClient(ClientConfig config, AuthenticationProvider authProvider) {
        super(config, authProvider);
    }
    
    // TODO: 实现项目相关的API方法
    // 例如：创建项目、获取项目列表、更新项目等
    
    /**
     * 获取项目信息
     */
    public Object getProject(String projectId) throws DataApiException {
        // TODO: 实现获取项目信息的逻辑
        throw new UnsupportedOperationException("待实现");
    }
}