#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace dataapi {
namespace auth {

/**
 * 认证类型枚举
 */
enum class AuthenticationType {
    BEARER_TOKEN,
    API_KEY,
    BASIC_AUTH,
    OAUTH2,
    CUSTOM
};

/**
 * 认证提供者基类
 */
class AuthenticationProvider {
public:
    /**
     * 虚析构函数
     */
    virtual ~AuthenticationProvider() = default;
    
    /**
     * 获取认证类型
     * @return 认证类型
     */
    virtual AuthenticationType getType() const = 0;
    
    /**
     * 获取认证头部信息
     * @return 认证头部键值对
     */
    virtual std::unordered_map<std::string, std::string> getAuthHeaders() const = 0;
    
    /**
     * 验证认证信息是否有效
     * @return 是否有效
     */
    virtual bool isValid() const = 0;
    
    /**
     * 刷新认证信息（如果支持）
     * @return 是否刷新成功
     */
    virtual bool refresh() { return false; }
    
    /**
     * 清除认证信息
     */
    virtual void clearAuthentication() = 0;
    
    /**
     * 获取认证信息的字符串表示（用于调试）
     * @return 认证信息字符串
     */
    virtual std::string toString() const = 0;
};

/**
 * Bearer Token认证提供者
 */
class BearerTokenAuthProvider : public AuthenticationProvider {
private:
    std::string token;
    
public:
    /**
     * 构造函数
     * @param token Bearer token
     */
    explicit BearerTokenAuthProvider(const std::string& token) : token(token) {}
    
    /**
     * 获取认证类型
     */
    AuthenticationType getType() const override {
        return AuthenticationType::BEARER_TOKEN;
    }
    
    /**
     * 获取认证头部信息
     */
    std::unordered_map<std::string, std::string> getAuthHeaders() const override {
        return {{"Authorization", "Bearer " + token}};
    }
    
    /**
     * 验证认证信息是否有效
     */
    bool isValid() const override {
        return !token.empty();
    }
    
    /**
     * 清除认证信息
     */
    void clearAuthentication() override {
        token.clear();
    }
    
    /**
     * 获取认证信息的字符串表示
     */
    std::string toString() const override {
        return "BearerTokenAuth[token=***]";
    }
    
    /**
     * 获取token
     */
    const std::string& getToken() const {
        return token;
    }
    
    /**
     * 设置token
     */
    void setToken(const std::string& newToken) {
        token = newToken;
    }
};

/**
 * API Key认证提供者
 */
class ApiKeyAuthProvider : public AuthenticationProvider {
private:
    std::string apiKey;
    std::string headerName;
    
public:
    /**
     * 构造函数
     * @param apiKey API密钥
     * @param headerName 头部名称（默认为"X-API-Key"）
     */
    explicit ApiKeyAuthProvider(const std::string& apiKey, const std::string& headerName = "X-API-Key")
        : apiKey(apiKey), headerName(headerName) {}
    
    /**
     * 获取认证类型
     */
    AuthenticationType getType() const override {
        return AuthenticationType::API_KEY;
    }
    
    /**
     * 获取认证头部信息
     */
    std::unordered_map<std::string, std::string> getAuthHeaders() const override {
        return {{headerName, apiKey}};
    }
    
    /**
     * 验证认证信息是否有效
     */
    bool isValid() const override {
        return !apiKey.empty() && !headerName.empty();
    }
    
    /**
     * 清除认证信息
     */
    void clearAuthentication() override {
        apiKey.clear();
    }
    
    /**
     * 获取认证信息的字符串表示
     */
    std::string toString() const override {
        return "ApiKeyAuth[header=" + headerName + ", key=***]";
    }
    
    /**
     * 获取API密钥
     */
    const std::string& getApiKey() const {
        return apiKey;
    }
    
    /**
     * 获取头部名称
     */
    const std::string& getHeaderName() const {
        return headerName;
    }
};

/**
 * Basic认证提供者
 */
class BasicAuthProvider : public AuthenticationProvider {
private:
    std::string username;
    std::string password;
    
public:
    /**
     * 构造函数
     * @param username 用户名
     * @param password 密码
     */
    BasicAuthProvider(const std::string& username, const std::string& password)
        : username(username), password(password) {}
    
    /**
     * 获取认证类型
     */
    AuthenticationType getType() const override {
        return AuthenticationType::BASIC_AUTH;
    }
    
    /**
     * 获取认证头部信息
     */
    std::unordered_map<std::string, std::string> getAuthHeaders() const override;
    
    /**
     * 验证认证信息是否有效
     */
    bool isValid() const override {
        return !username.empty() && !password.empty();
    }
    
    /**
     * 清除认证信息
     */
    void clearAuthentication() override {
        username.clear();
        password.clear();
    }
    
    /**
     * 获取认证信息的字符串表示
     */
    std::string toString() const override {
        return "BasicAuth[username=" + username + ", password=***]";
    }
    
    /**
     * 获取用户名
     */
    const std::string& getUsername() const {
        return username;
    }
    
    /**
     * 获取密码
     */
    const std::string& getPassword() const {
        return password;
    }
};

/**
 * 自定义认证提供者
 */
class CustomAuthProvider : public AuthenticationProvider {
private:
    std::unordered_map<std::string, std::string> headers;
    
public:
    /**
     * 构造函数
     * @param headers 自定义头部信息
     */
    explicit CustomAuthProvider(const std::unordered_map<std::string, std::string>& headers)
        : headers(headers) {}
    
    /**
     * 获取认证类型
     */
    AuthenticationType getType() const override {
        return AuthenticationType::CUSTOM;
    }
    
    /**
     * 获取认证头部信息
     */
    std::unordered_map<std::string, std::string> getAuthHeaders() const override {
        return headers;
    }
    
    /**
     * 验证认证信息是否有效
     */
    bool isValid() const override {
        return !headers.empty();
    }
    
    /**
     * 清除认证信息
     */
    void clearAuthentication() override {
        headers.clear();
    }
    
    /**
     * 获取认证信息的字符串表示
     */
    std::string toString() const override {
        return "CustomAuth[headers=" + std::to_string(headers.size()) + "]";
    }
    
    /**
     * 添加头部
     */
    void addHeader(const std::string& key, const std::string& value) {
        headers[key] = value;
    }
    
    /**
     * 移除头部
     */
    void removeHeader(const std::string& key) {
        headers.erase(key);
    }
};

} // namespace auth
} // namespace dataapi