package com.owiseman.dataapi.sdk.auth;

import com.owiseman.dataapi.sdk.exception.AuthenticationException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

/**
 * Bearer Token认证提供者
 */
public class BearerTokenAuthProvider implements AuthenticationProvider {
    private static final Logger logger = LoggerFactory.getLogger(BearerTokenAuthProvider.class);
    
    private String accessToken;
    private String refreshToken;
    private Instant tokenExpiry;
    private final TokenRefreshCallback refreshCallback;
    
    /**
     * Token刷新回调接口
     */
    public interface TokenRefreshCallback {
        TokenResponse refreshToken(String refreshToken) throws AuthenticationException;
    }
    
    /**
     * Token响应类
     */
    public static class TokenResponse {
        private final String accessToken;
        private final String refreshToken;
        private final long expiresInSeconds;
        
        public TokenResponse(String accessToken, String refreshToken, long expiresInSeconds) {
            this.accessToken = accessToken;
            this.refreshToken = refreshToken;
            this.expiresInSeconds = expiresInSeconds;
        }
        
        public String getAccessToken() { return accessToken; }
        public String getRefreshToken() { return refreshToken; }
        public long getExpiresInSeconds() { return expiresInSeconds; }
    }
    
    /**
     * 构造函数 - 仅使用访问令牌
     */
    public BearerTokenAuthProvider(String accessToken) {
        this(accessToken, null, null, null);
    }
    
    /**
     * 构造函数 - 使用访问令牌和刷新令牌
     */
    public BearerTokenAuthProvider(String accessToken, String refreshToken, 
                                 Long expiresInSeconds, TokenRefreshCallback refreshCallback) {
        this.accessToken = accessToken;
        this.refreshToken = refreshToken;
        this.refreshCallback = refreshCallback;
        
        if (expiresInSeconds != null) {
            this.tokenExpiry = Instant.now().plus(expiresInSeconds, ChronoUnit.SECONDS);
        }
        
        logger.debug("BearerToken认证提供者初始化完成");
    }
    
    @Override
    public String getAccessToken() throws AuthenticationException {
        if (accessToken == null) {
            throw new AuthenticationException("访问令牌未设置");
        }
        
        // 检查令牌是否即将过期（提前5分钟刷新）
        if (tokenExpiry != null && Instant.now().plus(5, ChronoUnit.MINUTES).isAfter(tokenExpiry)) {
            logger.debug("访问令牌即将过期，尝试刷新");
            return refreshToken();
        }
        
        return accessToken;
    }
    
    @Override
    public String refreshToken() throws AuthenticationException {
        if (refreshToken == null || refreshCallback == null) {
            throw new AuthenticationException("无法刷新令牌：缺少刷新令牌或回调函数");
        }
        
        try {
            logger.debug("正在刷新访问令牌");
            TokenResponse response = refreshCallback.refreshToken(refreshToken);
            
            this.accessToken = response.getAccessToken();
            if (response.getRefreshToken() != null) {
                this.refreshToken = response.getRefreshToken();
            }
            
            if (response.getExpiresInSeconds() > 0) {
                this.tokenExpiry = Instant.now().plus(response.getExpiresInSeconds(), ChronoUnit.SECONDS);
            }
            
            logger.debug("访问令牌刷新成功");
            return this.accessToken;
        } catch (Exception e) {
            logger.error("刷新访问令牌失败", e);
            throw new AuthenticationException("刷新访问令牌失败: " + e.getMessage(), e);
        }
    }
    
    @Override
    public boolean isTokenValid() {
        if (accessToken == null) {
            return false;
        }
        
        if (tokenExpiry == null) {
            return true; // 如果没有过期时间，假设令牌有效
        }
        
        return Instant.now().isBefore(tokenExpiry);
    }
    
    @Override
    public void clearAuthentication() {
        logger.debug("清除认证信息");
        this.accessToken = null;
        this.refreshToken = null;
        this.tokenExpiry = null;
    }
    
    @Override
    public AuthenticationType getAuthenticationType() {
        return AuthenticationType.BEARER_TOKEN;
    }
    
    /**
     * 更新访问令牌
     */
    public void updateAccessToken(String accessToken, Long expiresInSeconds) {
        this.accessToken = accessToken;
        if (expiresInSeconds != null) {
            this.tokenExpiry = Instant.now().plus(expiresInSeconds, ChronoUnit.SECONDS);
        }
        logger.debug("访问令牌已更新");
    }
    
    /**
     * 获取令牌过期时间
     */
    public Instant getTokenExpiry() {
        return tokenExpiry;
    }
}