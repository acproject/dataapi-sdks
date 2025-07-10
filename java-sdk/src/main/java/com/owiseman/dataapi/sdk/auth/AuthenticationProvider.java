package com.owiseman.dataapi.sdk.auth;

import com.owiseman.dataapi.sdk.exception.AuthenticationException;

/**
 * 认证提供者接口
 */
public interface AuthenticationProvider {
    
    /**
     * 获取访问令牌
     * @return 访问令牌
     * @throws AuthenticationException 认证异常
     */
    String getAccessToken() throws AuthenticationException;
    
    /**
     * 刷新访问令牌
     * @return 新的访问令牌
     * @throws AuthenticationException 认证异常
     */
    String refreshToken() throws AuthenticationException;
    
    /**
     * 检查令牌是否有效
     * @return true如果令牌有效，否则false
     */
    boolean isTokenValid();
    
    /**
     * 清除认证信息
     */
    void clearAuthentication();
    
    /**
     * 获取认证类型
     * @return 认证类型
     */
    AuthenticationType getAuthenticationType();
    
    /**
     * 认证类型枚举
     */
    enum AuthenticationType {
        BEARER_TOKEN,
        API_KEY,
        BASIC_AUTH,
        OAUTH2
    }
}