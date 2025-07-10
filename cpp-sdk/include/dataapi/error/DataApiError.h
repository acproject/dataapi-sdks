#pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include "../Types.h"

namespace dataapi {
namespace error {

/**
 * DataAPI基础异常类
 */
class DataApiError : public std::runtime_error {
protected:
    std::string code_;
    int statusCode_;
    std::string requestId_;
    Json responseBody_;
    std::shared_ptr<std::exception> cause_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param code 错误代码
     * @param statusCode HTTP状态码
     * @param requestId 请求ID
     * @param responseBody 响应体
     * @param cause 原因异常
     */
    DataApiError(const std::string& message,
                const std::string& code = "",
                int statusCode = 0,
                const std::string& requestId = "",
                const Json& responseBody = {},
                std::shared_ptr<std::exception> cause = nullptr);
    
    /**
     * 析构函数
     */
    virtual ~DataApiError() = default;
    
    /**
     * 获取错误代码
     */
    const std::string& getCode() const {
        return code_;
    }
    
    /**
     * 获取HTTP状态码
     */
    int getStatusCode() const {
        return statusCode_;
    }
    
    /**
     * 获取请求ID
     */
    const std::string& getRequestId() const {
        return requestId_;
    }
    
    /**
     * 获取响应体
     */
    const Json& getResponseBody() const {
        return responseBody_;
    }
    
    /**
     * 获取原因异常
     */
    std::shared_ptr<std::exception> getCause() const {
        return cause_;
    }
    
    /**
     * 转换为JSON格式
     */
    virtual Json toJson() const;
    
    /**
     * 转换为字符串
     */
    virtual std::string toString() const;
    
    /**
     * 是否为客户端错误（4xx）
     */
    bool isClientError() const {
        return statusCode_ >= 400 && statusCode_ < 500;
    }
    
    /**
     * 是否为服务器错误（5xx）
     */
    bool isServerError() const {
        return statusCode_ >= 500 && statusCode_ < 600;
    }
    
    /**
     * 是否为网络错误
     */
    bool isNetworkError() const {
        return statusCode_ == 0;
    }
    
    /**
     * 是否可重试
     */
    virtual bool isRetryable() const;
};

/**
 * 验证错误类
 */
class ValidationError : public DataApiError {
private:
    std::string field_;
    std::vector<std::string> validationRules_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param field 字段名
     * @param validationRules 验证规则
     * @param code 错误代码
     */
    ValidationError(const std::string& message,
                   const std::string& field = "",
                   const std::vector<std::string>& validationRules = {},
                   const std::string& code = "VALIDATION_ERROR");
    
    /**
     * 获取字段名
     */
    const std::string& getField() const {
        return field_;
    }
    
    /**
     * 获取验证规则
     */
    const std::vector<std::string>& getValidationRules() const {
        return validationRules_;
    }
    
    /**
     * 转换为JSON格式
     */
    Json toJson() const override;
    
    /**
     * 转换为字符串
     */
    std::string toString() const override;
};

/**
 * HTTP错误类
 */
class HttpError : public DataApiError {
private:
    std::string method_;
    std::string url_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param statusCode HTTP状态码
     * @param method HTTP方法
     * @param url 请求URL
     * @param requestId 请求ID
     * @param responseBody 响应体
     */
    HttpError(const std::string& message,
             int statusCode,
             const std::string& method = "",
             const std::string& url = "",
             const std::string& requestId = "",
             const Json& responseBody = {});
    
    /**
     * 获取HTTP方法
     */
    const std::string& getMethod() const {
        return method_;
    }
    
    /**
     * 获取请求URL
     */
    const std::string& getUrl() const {
        return url_;
    }
    
    /**
     * 转换为JSON格式
     */
    Json toJson() const override;
    
    /**
     * 转换为字符串
     */
    std::string toString() const override;
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override;
};

/**
 * 认证错误类
 */
class AuthenticationError : public DataApiError {
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param code 错误代码
     */
    AuthenticationError(const std::string& message,
                       const std::string& code = "AUTHENTICATION_ERROR");
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return false; // 认证错误通常不可重试
    }
};

/**
 * 授权错误类
 */
class AuthorizationError : public DataApiError {
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param code 错误代码
     */
    AuthorizationError(const std::string& message,
                      const std::string& code = "AUTHORIZATION_ERROR");
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return false; // 授权错误通常不可重试
    }
};

/**
 * 资源未找到错误类
 */
class NotFoundError : public DataApiError {
private:
    std::string resourceType_;
    std::string resourceId_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param resourceType 资源类型
     * @param resourceId 资源ID
     */
    NotFoundError(const std::string& message,
                 const std::string& resourceType = "",
                 const std::string& resourceId = "");
    
    /**
     * 获取资源类型
     */
    const std::string& getResourceType() const {
        return resourceType_;
    }
    
    /**
     * 获取资源ID
     */
    const std::string& getResourceId() const {
        return resourceId_;
    }
    
    /**
     * 转换为JSON格式
     */
    Json toJson() const override;
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return false; // 资源未找到错误通常不可重试
    }
};

/**
 * 冲突错误类
 */
class ConflictError : public DataApiError {
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param code 错误代码
     */
    ConflictError(const std::string& message,
                 const std::string& code = "CONFLICT_ERROR");
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return false; // 冲突错误通常不可重试
    }
};

/**
 * 速率限制错误类
 */
class RateLimitError : public DataApiError {
private:
    int retryAfter_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param retryAfter 重试等待时间（秒）
     */
    RateLimitError(const std::string& message, int retryAfter = 0);
    
    /**
     * 获取重试等待时间
     */
    int getRetryAfter() const {
        return retryAfter_;
    }
    
    /**
     * 转换为JSON格式
     */
    Json toJson() const override;
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return true; // 速率限制错误可以重试
    }
};

/**
 * 超时错误类
 */
class TimeoutError : public DataApiError {
private:
    int timeoutMs_;
    
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param timeoutMs 超时时间（毫秒）
     */
    TimeoutError(const std::string& message, int timeoutMs = 0);
    
    /**
     * 获取超时时间
     */
    int getTimeoutMs() const {
        return timeoutMs_;
    }
    
    /**
     * 转换为JSON格式
     */
    Json toJson() const override;
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return true; // 超时错误可以重试
    }
};

/**
 * 网络错误类
 */
class NetworkError : public DataApiError {
public:
    /**
     * 构造函数
     * @param message 错误消息
     * @param cause 原因异常
     */
    NetworkError(const std::string& message,
                std::shared_ptr<std::exception> cause = nullptr);
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return true; // 网络错误可以重试
    }
};

/**
 * 服务不可用错误类
 */
class ServiceUnavailableError : public DataApiError {
public:
    /**
     * 构造函数
     * @param message 错误消息
     */
    ServiceUnavailableError(const std::string& message);
    
    /**
     * 是否可重试
     */
    bool isRetryable() const override {
        return true; // 服务不可用错误可以重试
    }
};

} // namespace error
} // namespace dataapi