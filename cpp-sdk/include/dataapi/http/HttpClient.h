#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "../Types.h"
#include "../ClientConfig.h"
#include "../auth/AuthenticationProvider.h"

namespace dataapi {
namespace http {

/**
 * HTTP响应结构
 */
struct HttpResponse {
    int statusCode;
    std::string body;
    Headers headers;
    std::string errorMessage;
    
    HttpResponse() : statusCode(0) {}
    
    bool isSuccess() const {
        return statusCode >= 200 && statusCode < 300;
    }
};

/**
 * HTTP客户端类
 */
class HttpClient {
private:
    ClientConfig config;
    std::shared_ptr<auth::AuthenticationProvider> authProvider;
    
    // 内部实现细节
    void* curlHandle; // CURL句柄
    
    /**
     * 初始化CURL
     */
    void initializeCurl();
    
    /**
     * 清理CURL
     */
    void cleanupCurl();
    
    /**
     * 设置通用请求选项
     */
    void setCommonOptions();
    
    /**
     * 构建完整URL
     */
    std::string buildUrl(const std::string& endpoint) const;
    
    /**
     * 准备请求头部
     */
    Headers prepareHeaders(const Headers& customHeaders = {}) const;
    
    /**
     * 执行HTTP请求的内部方法
     */
    HttpResponse executeRequest(const HttpRequestConfig& config);
    
    /**
     * 处理重试逻辑
     */
    HttpResponse executeWithRetry(const HttpRequestConfig& config);
    
    /**
     * 记录请求日志
     */
    void logRequest(const HttpRequestConfig& config) const;
    
    /**
     * 记录响应日志
     */
    void logResponse(const HttpResponse& response) const;
    
public:
    /**
     * 构造函数
     * @param config 客户端配置
     * @param authProvider 认证提供者
     */
    HttpClient(const ClientConfig& config, std::shared_ptr<auth::AuthenticationProvider> authProvider);
    
    /**
     * 析构函数
     */
    ~HttpClient();
    
    /**
     * 禁用拷贝构造和赋值
     */
    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    HttpClient(HttpClient&& other) noexcept;
    HttpClient& operator=(HttpClient&& other) noexcept;
    
    /**
     * 执行HTTP请求
     * @param config 请求配置
     * @return HTTP响应
     */
    HttpResponse request(const HttpRequestConfig& config);
    
    /**
     * 执行GET请求
     * @param endpoint API端点
     * @param params 查询参数
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse get(const std::string& endpoint, 
                    const Parameters& params = {},
                    const Headers& headers = {});
    
    /**
     * 执行POST请求
     * @param endpoint API端点
     * @param data 请求体数据
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse post(const std::string& endpoint,
                     const Json& data = {},
                     const Headers& headers = {});
    
    /**
     * 执行PUT请求
     * @param endpoint API端点
     * @param data 请求体数据
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse put(const std::string& endpoint,
                    const Json& data = {},
                    const Headers& headers = {});
    
    /**
     * 执行DELETE请求
     * @param endpoint API端点
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse del(const std::string& endpoint,
                    const Headers& headers = {});
    
    /**
     * 执行HEAD请求
     * @param endpoint API端点
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse head(const std::string& endpoint,
                     const Headers& headers = {});
    
    /**
     * 执行PATCH请求
     * @param endpoint API端点
     * @param data 请求体数据
     * @param headers 自定义头部
     * @return HTTP响应
     */
    HttpResponse patch(const std::string& endpoint,
                      const Json& data = {},
                      const Headers& headers = {});
    
    /**
     * 测试连接
     * @return 是否连接成功
     */
    bool testConnection();
    
    /**
     * 获取配置
     */
    const ClientConfig& getConfig() const {
        return config;
    }
    
    /**
     * 获取认证提供者
     */
    std::shared_ptr<auth::AuthenticationProvider> getAuthProvider() const {
        return authProvider;
    }
    
    /**
     * 更新配置
     */
    void updateConfig(const ClientConfig& newConfig);
    
    /**
     * 更新认证提供者
     */
    void updateAuthProvider(std::shared_ptr<auth::AuthenticationProvider> newAuthProvider);
    
    /**
     * 设置超时时间
     */
    void setTimeout(int timeoutMs);
    
    /**
     * 启用/禁用SSL验证
     */
    void setVerifySSL(bool verify);
    
    /**
     * 设置代理
     */
    void setProxy(const std::string& proxyUrl);
    
    /**
     * 获取最后一次请求的统计信息
     */
    struct RequestStats {
        double totalTime;
        double connectTime;
        double downloadTime;
        long downloadSize;
        long uploadSize;
    };
    
    RequestStats getLastRequestStats() const;
};

/**
 * HTTP客户端工厂类
 */
class HttpClientFactory {
public:
    /**
     * 创建HTTP客户端
     * @param config 客户端配置
     * @param authProvider 认证提供者
     * @return HTTP客户端实例
     */
    static std::unique_ptr<HttpClient> create(
        const ClientConfig& config,
        std::shared_ptr<auth::AuthenticationProvider> authProvider
    );
    
    /**
     * 创建默认HTTP客户端
     * @param baseUrl 基础URL
     * @param authProvider 认证提供者
     * @return HTTP客户端实例
     */
    static std::unique_ptr<HttpClient> createDefault(
        const std::string& baseUrl,
        std::shared_ptr<auth::AuthenticationProvider> authProvider
    );
};

} // namespace http
} // namespace dataapi