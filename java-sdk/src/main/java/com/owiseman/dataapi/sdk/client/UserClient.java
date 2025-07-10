package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;

/**
 * 用户客户端
 */
public class UserClient extends BaseClient {
    
    public UserClient(ClientConfig config, AuthenticationProvider authProvider) {
        super(config, authProvider);
    }
    
    // TODO: 实现用户相关的API方法
    // 例如：获取用户信息、更新用户信息等
    
    /**
     * 获取当前用户信息
     */
    public Object getCurrentUser() throws DataApiException {
        // TODO: 实现获取当前用户信息的逻辑
        throw new UnsupportedOperationException("待实现");
    }
}