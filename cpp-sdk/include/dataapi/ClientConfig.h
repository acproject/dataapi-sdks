#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace dataapi {

/**
 * 客户端配置类
 */
class ClientConfig {
public:
    std::string baseUrl;
    int timeout = 30000; // 默认30秒超时
    bool enableLogging = false;
    bool enableRetry = true;
    int maxRetries = 3;
    int retryDelay = 1000; // 重试延迟（毫秒）
    std::string version = "1.0.0";
    std::unordered_map<std::string, std::string> defaultHeaders;
    std::string userAgent;
    bool verifySSL = true;
    std::string proxyUrl;
    int connectionPoolSize = 10;
    
    /**
     * 默认构造函数
     */
    ClientConfig() = default;
    
    /**
     * 构造函数
     * @param baseUrl API基础URL
     */
    explicit ClientConfig(const std::string& baseUrl) : baseUrl(baseUrl) {
        userAgent = "DataAPI-CPP-SDK/" + version;
    }
    
    /**
     * 拷贝构造函数
     */
    ClientConfig(const ClientConfig& other) = default;
    
    /**
     * 移动构造函数
     */
    ClientConfig(ClientConfig&& other) noexcept = default;
    
    /**
     * 拷贝赋值操作符
     */
    ClientConfig& operator=(const ClientConfig& other) = default;
    
    /**
     * 移动赋值操作符
     */
    ClientConfig& operator=(ClientConfig&& other) noexcept = default;
    
    /**
     * 析构函数
     */
    ~ClientConfig() = default;
    
    /**
     * 验证配置
     * @return 配置是否有效
     */
    bool isValid() const {
        return !baseUrl.empty() && timeout > 0 && maxRetries >= 0;
    }
    
    /**
     * 设置默认头部
     * @param key 头部键
     * @param value 头部值
     */
    void setDefaultHeader(const std::string& key, const std::string& value) {
        defaultHeaders[key] = value;
    }
    
    /**
     * 获取默认头部
     * @param key 头部键
     * @return 头部值，如果不存在则返回空字符串
     */
    std::string getDefaultHeader(const std::string& key) const {
        auto it = defaultHeaders.find(key);
        return it != defaultHeaders.end() ? it->second : "";
    }
    
    /**
     * 移除默认头部
     * @param key 头部键
     */
    void removeDefaultHeader(const std::string& key) {
        defaultHeaders.erase(key);
    }
};

/**
 * 客户端配置构建器
 */
class ClientConfigBuilder {
public:
    /**
     * 创建默认配置
     * @return 默认配置
     */
    static ClientConfig defaultConfig() {
        ClientConfig config;
        config.baseUrl = "http://localhost:8080/api";
        config.timeout = 30000;
        config.enableLogging = false;
        config.enableRetry = true;
        config.maxRetries = 3;
        config.retryDelay = 1000;
        config.verifySSL = true;
        config.connectionPoolSize = 10;
        config.userAgent = "DataAPI-CPP-SDK/1.0.0";
        return config;
    }
    
    /**
     * 创建开发环境配置
     * @return 开发环境配置
     */
    static ClientConfig development() {
        ClientConfig config = defaultConfig();
        config.baseUrl = "http://localhost:8080/api";
        config.enableLogging = true;
        config.verifySSL = false;
        return config;
    }
    
    /**
     * 创建生产环境配置
     * @param baseUrl API基础URL
     * @return 生产环境配置
     */
    static ClientConfig production(const std::string& baseUrl) {
        ClientConfig config = defaultConfig();
        config.baseUrl = baseUrl;
        config.enableLogging = false;
        config.verifySSL = true;
        return config;
    }
    
    /**
     * 创建测试环境配置
     * @return 测试环境配置
     */
    static ClientConfig testing() {
        ClientConfig config = defaultConfig();
        config.baseUrl = "http://localhost:8080/api";
        config.enableLogging = true;
        config.verifySSL = false;
        config.timeout = 10000; // 测试环境使用较短的超时时间
        return config;
    }
};

} // namespace dataapi