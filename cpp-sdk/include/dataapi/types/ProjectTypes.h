#pragma once

#include "CommonTypes.h"

namespace dataapi {

// Forward declarations
struct SysProject;
struct ProjectCreateRequest;
struct ProjectUpdateRequest;
struct ProjectMember;
struct ProjectMemberRequest;
struct ProjectPermission;
struct ProjectConfig;
struct ProjectStatistics;
struct ProjectActivity;
struct ImportResult;

// System Project
struct SysProject {
    std::string id;
    std::string name;
    std::string description;
    std::string status;
    std::string ownerId;
    Timestamp createdAt;
    Timestamp updatedAt;
};

// Project Create Request
struct ProjectCreateRequest {
    std::string name;
    std::optional<std::string> description;
    std::string userId;
    Json settings;
};

// Project Update Request
struct ProjectUpdateRequest {
    std::optional<std::string> name;
    std::optional<std::string> description;
    Json settings;
};

// Project Member
struct ProjectMember {
    std::string userId;
    std::string role;
    Timestamp joinedAt;
};

// Project Member Request
struct ProjectMemberRequest {
    std::string userId;
    std::string role;
};

// Project Permission
struct ProjectPermission {
    std::string action;
    bool allowed;
};

// Project Config
struct ProjectConfig {
    Json settings;
    Json permissions;
};

// Project Statistics
struct ProjectStatistics {
    int totalWorkflows;
    int totalExecutions;
    int totalMembers;
    Json details;
};

// Project Activity
struct ProjectActivity {
    std::string id;
    std::string action;
    std::string userId;
    Timestamp timestamp;
    Json details;
};

// Project Template
struct ProjectTemplate {
    std::string id;
    std::string name;
    std::string description;
    std::string type;
    Json content;
    Json styles;
    Json pages;
    std::string preview;
    bool isDefault;
    bool isPublic;
    std::vector<std::string> tags;
    std::string templateKey;
    std::string userId;
    Timestamp createdAt;
    Timestamp updatedAt;
};

// Import Result
struct ImportResult {
    bool success;
    std::string message;
    Json details;
};

/**
 * 项目结构
 */
struct Project {
    std::string id;
    std::string name;
    std::optional<std::string> description;
    std::string createTime;
    std::string updateTime;
    std::string userId;
    std::string status;
};

// JSON序列化/反序列化函数声明
void to_json(Json& j, const SysProject& p);
void from_json(const Json& j, SysProject& p);

void to_json(Json& j, const Project& p);
void from_json(const Json& j, Project& p);

void to_json(Json& j, const ProjectCreateRequest& p);
void from_json(const Json& j, ProjectCreateRequest& p);

void to_json(Json& j, const ProjectUpdateRequest& p);
void from_json(const Json& j, ProjectUpdateRequest& p);

} // namespace dataapi