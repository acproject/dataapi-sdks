#pragma once

#include "CommonTypes.h"

namespace dataapi {

// Forward declarations
struct SysUser;
struct UserCreateRequest;
struct UserUpdateRequest;
struct UserRole;
struct UserPermission;
struct UserPreferences;
struct UserActivity;
struct LoginHistory;
struct UserSession;
struct UserStatistics;

// System User
struct SysUser {
    std::string id;
    std::string username;
    std::string email;
    std::string status;
    Timestamp createdAt;
    Timestamp lastLoginAt;
};

// Login Response
struct LoginResponse {
    std::string token;
    std::string refreshToken;
    SysUser user;
    Timestamp expiresAt;
};

// User Create Request
struct UserCreateRequest {
    std::string username;
    std::string email;
    std::string password;
    std::optional<std::string> displayName;
    Json profile;
};

// User Update Request
struct UserUpdateRequest {
    std::optional<std::string> username;
    std::optional<std::string> email;
    std::optional<std::string> displayName;
    std::optional<std::string> password;
    Json profile;
};

// User Role
struct UserRole {
    std::string id;
    std::string name;
    std::vector<std::string> permissions;
};

// User Permission
struct UserPermission {
    std::string resource;
    std::string action;
    bool allowed;
};

// User Preferences
struct UserPreferences {
    std::string language;
    std::string theme;
    Json settings;
};

// User Activity
struct UserActivity {
    std::string id;
    std::string action;
    Timestamp timestamp;
    Json details;
};

// Login History
struct LoginHistory {
    std::string id;
    Timestamp loginTime;
    std::string ipAddress;
    std::string userAgent;
};

// User Session
struct UserSession {
    std::string id;
    std::string token;
    Timestamp expiresAt;
    bool active;
};

// User Statistics
struct UserStatistics {
    int totalLogins;
    Timestamp lastActivity;
    Json details;
};

/**
 * 用户结构
 */
struct User {
    std::string id;
    std::string username;
    std::string email;
    std::optional<std::string> displayName;
    std::string createTime;
    std::string updateTime;
    std::string status;
};

// JSON序列化/反序列化函数声明
void to_json(Json& j, const SysUser& u);
void from_json(const Json& j, SysUser& u);

void to_json(Json& j, const User& u);
void from_json(const Json& j, User& u);

void to_json(Json& j, const UserCreateRequest& u);
void from_json(const Json& j, UserCreateRequest& u);

void to_json(Json& j, const UserUpdateRequest& u);
void from_json(const Json& j, UserUpdateRequest& u);

} // namespace dataapi