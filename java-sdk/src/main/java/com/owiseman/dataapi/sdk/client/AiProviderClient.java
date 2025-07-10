package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;

/**
 * AI提供者客户端
 */
public class AiProviderClient extends BaseClient {
    
    public AiProviderClient(ClientConfig config, AuthenticationProvider authProvider) {
        super(config, authProvider);
    }
    
    // TODO: 实现AI提供者相关的API方法
    // 例如：调用AI模型、获取AI配置等
    
    /**
     * 调用AI模型
     */
    public Object invokeAi(String configId, Object input) throws DataApiException {
        // TODO: 实现AI调用的逻辑
        throw new UnsupportedOperationException("待实现");
    }
}