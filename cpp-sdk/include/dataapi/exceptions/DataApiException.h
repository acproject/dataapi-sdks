#pragma once

#include <stdexcept>
#include <string>

namespace dataapi {
namespace exceptions {

/**
 * DataAPI异常基类
 */
class DataApiException : public std::runtime_error {
public:
    explicit DataApiException(const std::string& message)
        : std::runtime_error(message) {}
    
    explicit DataApiException(const char* message)
        : std::runtime_error(message) {}
};

/**
 * HTTP错误异常
 */
class HttpError : public DataApiException {
private:
    int statusCode_;
    
public:
    HttpError(const std::string& message, int statusCode)
        : DataApiException(message), statusCode_(statusCode) {}
    
    int getStatusCode() const {
        return statusCode_;
    }
};

/**
 * 认证错误异常
 */
class AuthenticationError : public DataApiException {
public:
    explicit AuthenticationError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 授权错误异常
 */
class AuthorizationError : public DataApiException {
public:
    explicit AuthorizationError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 验证错误异常
 */
class ValidationError : public DataApiException {
public:
    explicit ValidationError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 资源未找到异常
 */
class NotFoundError : public DataApiException {
public:
    explicit NotFoundError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 冲突错误异常
 */
class ConflictError : public DataApiException {
public:
    explicit ConflictError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 服务器错误异常
 */
class ServerError : public DataApiException {
public:
    explicit ServerError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 网络错误异常
 */
class NetworkError : public DataApiException {
public:
    explicit NetworkError(const std::string& message)
        : DataApiException(message) {}
};

/**
 * 超时错误异常
 */
class TimeoutError : public DataApiException {
public:
    explicit TimeoutError(const std::string& message)
        : DataApiException(message) {}
};

} // namespace exceptions
} // namespace dataapi