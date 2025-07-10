#include "dataapi/types/ProjectTypes.h"
#include <chrono>
#include <cstdint>

namespace dataapi {

// SysProject JSON serialization
void to_json(Json& j, const SysProject& p) {
    j = Json{
        {"id", p.id},
        {"name", p.name},
        {"description", p.description},
        {"status", p.status},
        {"ownerId", p.ownerId},
        {"createdAt", std::chrono::duration_cast<std::chrono::milliseconds>(p.createdAt.time_since_epoch()).count()},
        {"updatedAt", std::chrono::duration_cast<std::chrono::milliseconds>(p.updatedAt.time_since_epoch()).count()}
    };
}

void from_json(const Json& j, SysProject& p) {
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    j.at("description").get_to(p.description);
    j.at("status").get_to(p.status);
    j.at("ownerId").get_to(p.ownerId);
    
    if (j.contains("createdAt")) {
        auto createdAtMs = j.at("createdAt").get<int64_t>();
        p.createdAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(createdAtMs));
    }
    
    if (j.contains("updatedAt")) {
        auto updatedAtMs = j.at("updatedAt").get<int64_t>();
        p.updatedAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(updatedAtMs));
    }
}

// Project JSON serialization
void to_json(Json& j, const Project& p) {
    j = Json{
        {"id", p.id},
        {"name", p.name},
        {"createTime", p.createTime},
        {"updateTime", p.updateTime},
        {"userId", p.userId},
        {"status", p.status}
    };
    if (p.description) j["description"] = *p.description;
}

void from_json(const Json& j, Project& p) {
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    j.at("createTime").get_to(p.createTime);
    j.at("updateTime").get_to(p.updateTime);
    j.at("userId").get_to(p.userId);
    j.at("status").get_to(p.status);
    
    if (j.contains("description") && !j["description"].is_null()) {
        p.description = j["description"].get<std::string>();
    }
}

// ProjectCreateRequest JSON serialization
void to_json(Json& j, const ProjectCreateRequest& p) {
    j = Json{
        {"name", p.name},
        {"userId", p.userId},
        {"settings", p.settings}
    };
    if (p.description) j["description"] = *p.description;
}

void from_json(const Json& j, ProjectCreateRequest& p) {
    j.at("name").get_to(p.name);
    j.at("userId").get_to(p.userId);
    j.at("settings").get_to(p.settings);
    
    if (j.contains("description") && !j["description"].is_null()) {
        p.description = j["description"].get<std::string>();
    }
}

// ProjectUpdateRequest JSON serialization
void to_json(Json& j, const ProjectUpdateRequest& p) {
    j = Json{{"settings", p.settings}};
    if (p.name) j["name"] = *p.name;
    if (p.description) j["description"] = *p.description;
}

void from_json(const Json& j, ProjectUpdateRequest& p) {
    j.at("settings").get_to(p.settings);
    
    if (j.contains("name") && !j["name"].is_null()) {
        p.name = j["name"].get<std::string>();
    }
    if (j.contains("description") && !j["description"].is_null()) {
        p.description = j["description"].get<std::string>();
    }
}

} // namespace dataapi