package com.owiseman.dataapi.sdk.config;

import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

/**
 * 客户端配置类
 */
public class ClientConfig {
    private String baseUrl;
    private Duration connectTimeout = Duration.ofSeconds(30);
    private Duration readTimeout = Duration.ofSeconds(60);
    private Duration writeTimeout = Duration.ofSeconds(60);
    private int maxRetries = 3;
    private Duration retryDelay = Duration.ofSeconds(1);
    private Map<String, String> defaultHeaders = new HashMap<>();
    private boolean enableLogging = true;
    private String userAgent = "DataAPI-Java-SDK/1.0.0";
    
    public ClientConfig(String baseUrl) {
        this.baseUrl = baseUrl;
        initDefaultHeaders();
    }
    
    private void initDefaultHeaders() {
        defaultHeaders.put("Content-Type", "application/json");
        defaultHeaders.put("Accept", "application/json");
        defaultHeaders.put("User-Agent", userAgent);
    }
    
    // Getters and Setters
    public String getBaseUrl() {
        return baseUrl;
    }
    
    public void setBaseUrl(String baseUrl) {
        this.baseUrl = baseUrl;
    }
    
    public Duration getConnectTimeout() {
        return connectTimeout;
    }
    
    public void setConnectTimeout(Duration connectTimeout) {
        this.connectTimeout = connectTimeout;
    }
    
    public Duration getReadTimeout() {
        return readTimeout;
    }
    
    public void setReadTimeout(Duration readTimeout) {
        this.readTimeout = readTimeout;
    }
    
    public Duration getWriteTimeout() {
        return writeTimeout;
    }
    
    public void setWriteTimeout(Duration writeTimeout) {
        this.writeTimeout = writeTimeout;
    }
    
    public int getMaxRetries() {
        return maxRetries;
    }
    
    public void setMaxRetries(int maxRetries) {
        this.maxRetries = maxRetries;
    }
    
    public Duration getRetryDelay() {
        return retryDelay;
    }
    
    public void setRetryDelay(Duration retryDelay) {
        this.retryDelay = retryDelay;
    }
    
    public Map<String, String> getDefaultHeaders() {
        return new HashMap<>(defaultHeaders);
    }
    
    public void addDefaultHeader(String name, String value) {
        defaultHeaders.put(name, value);
    }
    
    public void removeDefaultHeader(String name) {
        defaultHeaders.remove(name);
    }
    
    public boolean isEnableLogging() {
        return enableLogging;
    }
    
    public void setEnableLogging(boolean enableLogging) {
        this.enableLogging = enableLogging;
    }
    
    public String getUserAgent() {
        return userAgent;
    }
    
    public void setUserAgent(String userAgent) {
        this.userAgent = userAgent;
        defaultHeaders.put("User-Agent", userAgent);
    }
    
    /**
     * 创建默认配置
     */
    public static ClientConfig defaultConfig(String baseUrl) {
        return new ClientConfig(baseUrl);
    }
    
    /**
     * 创建开发环境配置
     */
    public static ClientConfig developmentConfig() {
        ClientConfig config = new ClientConfig("http://localhost:8080");
        config.setEnableLogging(true);
        config.setConnectTimeout(Duration.ofSeconds(10));
        return config;
    }
    
    /**
     * 创建生产环境配置
     */
    public static ClientConfig productionConfig(String baseUrl) {
        ClientConfig config = new ClientConfig(baseUrl);
        config.setEnableLogging(false);
        config.setMaxRetries(5);
        config.setRetryDelay(Duration.ofSeconds(2));
        return config;
    }
}