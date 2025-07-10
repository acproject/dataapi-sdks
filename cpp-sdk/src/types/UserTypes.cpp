#include "dataapi/types/UserTypes.h"
#include <chrono>
#include <cstdint>

namespace dataapi {

// SysUser JSON serialization
void to_json(Json& j, const SysUser& u) {
    j = Json{
        {"id", u.id},
        {"username", u.username},
        {"email", u.email},
        {"status", u.status},
        {"createdAt", std::chrono::duration_cast<std::chrono::milliseconds>(u.createdAt.time_since_epoch()).count()},
        {"lastLoginAt", std::chrono::duration_cast<std::chrono::milliseconds>(u.lastLoginAt.time_since_epoch()).count()}
    };
}

void from_json(const Json& j, SysUser& u) {
    j.at("id").get_to(u.id);
    j.at("username").get_to(u.username);
    j.at("email").get_to(u.email);
    j.at("status").get_to(u.status);
    
    if (j.contains("createdAt")) {
        auto createdAtMs = j.at("createdAt").get<int64_t>();
        u.createdAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(createdAtMs));
    }
    
    if (j.contains("lastLoginAt")) {
        auto lastLoginAtMs = j.at("lastLoginAt").get<int64_t>();
        u.lastLoginAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(lastLoginAtMs));
    }
}

// User JSON serialization
void to_json(Json& j, const User& u) {
    j = Json{
        {"id", u.id},
        {"username", u.username},
        {"email", u.email},
        {"createTime", u.createTime},
        {"updateTime", u.updateTime},
        {"status", u.status}
    };
    if (u.displayName) j["displayName"] = *u.displayName;
}

void from_json(const Json& j, User& u) {
    j.at("id").get_to(u.id);
    j.at("username").get_to(u.username);
    j.at("email").get_to(u.email);
    j.at("createTime").get_to(u.createTime);
    j.at("updateTime").get_to(u.updateTime);
    j.at("status").get_to(u.status);
    
    if (j.contains("displayName") && !j["displayName"].is_null()) {
        u.displayName = j["displayName"].get<std::string>();
    }
}

// UserCreateRequest JSON serialization
void to_json(Json& j, const UserCreateRequest& u) {
    j = Json{
        {"username", u.username},
        {"email", u.email},
        {"password", u.password},
        {"profile", u.profile}
    };
}

void from_json(const Json& j, UserCreateRequest& u) {
    j.at("username").get_to(u.username);
    j.at("email").get_to(u.email);
    j.at("password").get_to(u.password);
    j.at("profile").get_to(u.profile);
}

// UserUpdateRequest JSON serialization
void to_json(Json& j, const UserUpdateRequest& u) {
    j = Json{{"profile", u.profile}};
    if (u.username) j["username"] = *u.username;
    if (u.email) j["email"] = *u.email;
    if (u.displayName) j["displayName"] = *u.displayName;
    if (u.password) j["password"] = *u.password;
}

void from_json(const Json& j, UserUpdateRequest& u) {
    j.at("profile").get_to(u.profile);
    
    if (j.contains("username") && !j["username"].is_null()) {
        u.username = j["username"].get<std::string>();
    }
    if (j.contains("email") && !j["email"].is_null()) {
        u.email = j["email"].get<std::string>();
    }
    if (j.contains("displayName") && !j["displayName"].is_null()) {
        u.displayName = j["displayName"].get<std::string>();
    }
    if (j.contains("password") && !j["password"].is_null()) {
        u.password = j["password"].get<std::string>();
    }
}

} // namespace dataapi