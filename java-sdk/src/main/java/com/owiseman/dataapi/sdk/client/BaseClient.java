package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.http.HttpClient;

/**
 * 基础客户端类
 */
public abstract class BaseClient {
    protected final ClientConfig config;
    protected final AuthenticationProvider authProvider;
    protected final HttpClient httpClient;
    
    public BaseClient(ClientConfig config, AuthenticationProvider authProvider) {
        this.config = config;
        this.authProvider = authProvider;
        this.httpClient = new HttpClient(config, authProvider);
    }
    
    /**
     * 获取客户端配置
     */
    public ClientConfig getConfig() {
        return config;
    }
    
    /**
     * 获取认证提供者
     */
    public AuthenticationProvider getAuthProvider() {
        return authProvider;
    }
}