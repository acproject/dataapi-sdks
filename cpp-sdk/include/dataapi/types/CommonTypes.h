#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <functional>
#include <chrono>
#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>

namespace dataapi {

// 基础类型定义
using Json = nlohmann::json;
using Headers = std::map<std::string, std::string>;
using Parameters = std::map<std::string, std::string>;
using Timestamp = std::chrono::system_clock::time_point;

/**
 * HTTP方法枚举
 */
enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    PATCH
};

/**
 * 认证类型枚举
 */
enum class AuthenticationType {
    BEARER_TOKEN,
    API_KEY,
    BASIC_AUTH,
    OAUTH2
};

/**
 * API响应结构
 */
template<typename T>
struct ApiResponse {
    bool success;
    std::optional<T> data;
    std::optional<std::string> message;
    std::optional<std::string> code;
    std::optional<std::string> timestamp;
};

/**
 * 分页结果结构
 */
template<typename T>
struct PageResult {
    std::vector<T> content;
    int pageNumber;
    int pageSize;
    long totalElements;
    int totalPages;
    bool first;
    bool last;
    bool empty;
};

/**
 * HTTP请求配置
 */
struct HttpRequestConfig {
    HttpMethod method;
    std::string url;
    Headers headers;
    Parameters params;
    Json data;
    int timeout = 30000; // 默认30秒
};

/**
 * Token响应结构
 */
struct TokenResponse {
    std::string accessToken;
    std::optional<std::string> refreshToken;
    std::optional<int> expiresIn;
    std::optional<std::string> tokenType;
};

/**
 * API版本信息
 */
struct ApiVersion {
    std::string version;
    std::string buildTime;
    std::string gitCommit;
};

/**
 * 健康状态
 */
struct HealthStatus {
    std::string status;
    std::string message;
    std::map<std::string, std::string> details;
};

/**
 * 错误响应结构
 */
struct ErrorResponse {
    std::string code;
    std::string message;
    std::optional<Json> details;
    std::optional<std::string> timestamp;
};

// API Key
struct ApiKey {
    std::string id;
    std::string name;
    std::string key;
    std::string userId;
    std::vector<std::string> permissions;
    Timestamp createdAt;
    Timestamp expiresAt;
    bool active;
};

// API Key Create Request
struct ApiKeyCreateRequest {
    std::string name;
    std::vector<std::string> permissions;
    std::optional<Timestamp> expiresAt;
};

// API Key Update Request
struct ApiKeyUpdateRequest {
    std::string name;
    std::vector<std::string> permissions;
    std::optional<Timestamp> expiresAt;
    bool active;
};

// Audit Log
struct AuditLog {
    std::string id;
    std::string userId;
    std::string action;
    std::string resource;
    Json details;
    Timestamp timestamp;
    std::string ipAddress;
    std::string userAgent;
};

// System Settings
struct SystemSettings {
    std::string key;
    Json value;
    std::string description;
    Timestamp updatedAt;
};

// System Settings Update Request
struct SystemSettingsUpdateRequest {
    std::string key;
    Json value;
    std::string description;
};

// Notification
struct Notification {
    std::string id;
    std::string userId;
    std::string title;
    std::string message;
    std::string type;
    bool read;
    Timestamp createdAt;
};

// Notification Create Request
struct NotificationCreateRequest {
    std::string userId;
    std::string title;
    std::string message;
    std::string type;
};

// Two Factor Auth Setup
struct TwoFactorAuthSetup {
    std::string secret;
    std::string qrCodeUrl;
    std::vector<std::string> backupCodes;
    bool enabled;
};

// Two Factor Auth Verify Request
struct TwoFactorAuthVerifyRequest {
    std::string code;
    std::string secret;
};

// Password Reset Request
struct PasswordResetRequest {
    std::string email;
    std::string newPassword;
    std::string resetToken;
};

// Password Change Request
struct PasswordChangeRequest {
    std::string currentPassword;
    std::string newPassword;
};

// Login Request
struct LoginRequest {
    std::string username;
    std::string password;
    std::optional<std::string> twoFactorCode;
};

// PageResult JSON serialization templates
template<typename T>
void to_json(Json& j, const PageResult<T>& p) {
    j = Json{
        {"content", p.content},
        {"pageNumber", p.pageNumber},
        {"pageSize", p.pageSize},
        {"totalElements", p.totalElements},
        {"totalPages", p.totalPages},
        {"first", p.first},
        {"last", p.last},
        {"empty", p.empty}
    };
}

template<typename T>
void from_json(const Json& j, PageResult<T>& p) {
    j.at("content").get_to(p.content);
    j.at("pageNumber").get_to(p.pageNumber);
    j.at("pageSize").get_to(p.pageSize);
    j.at("totalElements").get_to(p.totalElements);
    j.at("totalPages").get_to(p.totalPages);
    j.at("first").get_to(p.first);
    j.at("last").get_to(p.last);
    j.at("empty").get_to(p.empty);
}

} // namespace dataapi