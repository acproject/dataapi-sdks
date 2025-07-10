package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;

/**
 * 数据库客户端
 */
public class DatabaseClient extends BaseClient {
    
    public DatabaseClient(ClientConfig config, AuthenticationProvider authProvider) {
        super(config, authProvider);
    }
    
    // TODO: 实现数据库相关的API方法
    // 例如：执行SQL、获取数据库连接信息等
    
    /**
     * 执行SQL语句
     */
    public Object executeSql(String sql) throws DataApiException {
        // TODO: 实现执行SQL的逻辑
        throw new UnsupportedOperationException("待实现");
    }
}