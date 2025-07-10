#include "dataapi/Types.h"
#include <chrono>
#include <cstdint>

namespace dataapi {

// WorkflowExecutionResult JSON serialization
void to_json(Json& j, const WorkflowExecutionResult& w) {
    j = Json{
        {"executionId", w.executionId},
        {"status", w.status},
        {"result", w.result},
        {"startTime", std::chrono::duration_cast<std::chrono::milliseconds>(w.startTime.time_since_epoch()).count()},
        {"endTime", std::chrono::duration_cast<std::chrono::milliseconds>(w.endTime.time_since_epoch()).count()},
        {"metadata", w.metadata}
    };
}

void from_json(const Json& j, WorkflowExecutionResult& w) {
    j.at("executionId").get_to(w.executionId);
    j.at("status").get_to(w.status);
    j.at("result").get_to(w.result);
    
    if (j.contains("startTime")) {
        auto startTimeMs = j.at("startTime").get<int64_t>();
        w.startTime = std::chrono::system_clock::time_point(std::chrono::milliseconds(startTimeMs));
    }
    
    if (j.contains("endTime")) {
        auto endTimeMs = j.at("endTime").get<int64_t>();
        w.endTime = std::chrono::system_clock::time_point(std::chrono::milliseconds(endTimeMs));
    }
    
    if (j.contains("metadata")) {
        j.at("metadata").get_to(w.metadata);
    }
}

// SysWorkflow JSON serialization
void to_json(Json& j, const SysWorkflow& w) {
    j = Json{
        {"name", w.name},
        {"definition", w.definition},
        {"projectId", w.projectId},
        {"userId", w.userId}
    };
    if (w.id) j["id"] = *w.id;
    if (w.description) j["description"] = *w.description;
    if (w.createTime) j["createTime"] = *w.createTime;
    if (w.updateTime) j["updateTime"] = *w.updateTime;
    if (w.status) j["status"] = static_cast<int>(*w.status);
    if (w.version) j["version"] = *w.version;
}

void from_json(const Json& j, SysWorkflow& w) {
    j.at("name").get_to(w.name);
    j.at("definition").get_to(w.definition);
    j.at("projectId").get_to(w.projectId);
    j.at("userId").get_to(w.userId);
    
    if (j.contains("id") && !j["id"].is_null()) {
        w.id = j["id"].get<std::string>();
    }
    if (j.contains("description") && !j["description"].is_null()) {
        w.description = j["description"].get<std::string>();
    }
    if (j.contains("createTime") && !j["createTime"].is_null()) {
        w.createTime = j["createTime"].get<std::string>();
    }
    if (j.contains("updateTime") && !j["updateTime"].is_null()) {
        w.updateTime = j["updateTime"].get<std::string>();
    }
    if (j.contains("status") && !j["status"].is_null()) {
        w.status = static_cast<WorkflowStatus>(j["status"].get<int>());
    }
    if (j.contains("version") && !j["version"].is_null()) {
        w.version = j["version"].get<int>();
    }
}

// WorkflowCreateRequest JSON serialization
void to_json(Json& j, const WorkflowCreateRequest& w) {
    j = Json{
        {"name", w.name},
        {"definition", w.definition},
        {"projectId", w.projectId},
        {"userId", w.userId}
    };
    if (w.description) j["description"] = *w.description;
}

void from_json(const Json& j, WorkflowCreateRequest& w) {
    j.at("name").get_to(w.name);
    j.at("definition").get_to(w.definition);
    j.at("projectId").get_to(w.projectId);
    j.at("userId").get_to(w.userId);
    
    if (j.contains("description") && !j["description"].is_null()) {
        w.description = j["description"].get<std::string>();
    }
}

// WorkflowUpdateRequest JSON serialization
void to_json(Json& j, const WorkflowUpdateRequest& w) {
    j = Json{{"id", w.id}};
    if (w.name) j["name"] = *w.name;
    if (w.description) j["description"] = *w.description;
    if (w.definition) j["definition"] = *w.definition;
    if (w.projectId) j["projectId"] = *w.projectId;
    if (w.userId) j["userId"] = *w.userId;
    if (w.updateTime) j["updateTime"] = *w.updateTime;
}

void from_json(const Json& j, WorkflowUpdateRequest& w) {
    j.at("id").get_to(w.id);
    
    if (j.contains("name") && !j["name"].is_null()) {
        w.name = j["name"].get<std::string>();
    }
    if (j.contains("description") && !j["description"].is_null()) {
        w.description = j["description"].get<std::string>();
    }
    if (j.contains("definition") && !j["definition"].is_null()) {
        w.definition = j["definition"].get<std::string>();
    }
    if (j.contains("projectId") && !j["projectId"].is_null()) {
        w.projectId = j["projectId"].get<std::string>();
    }
    if (j.contains("userId") && !j["userId"].is_null()) {
        w.userId = j["userId"].get<std::string>();
    }
    if (j.contains("updateTime") && !j["updateTime"].is_null()) {
        w.updateTime = j["updateTime"].get<std::string>();
    }
}

// WorkflowExecutionRequest JSON serialization
void to_json(Json& j, const WorkflowExecutionRequest& w) {
    j = Json{
        {"workflowId", w.workflowId},
        {"projectId", w.projectId},
        {"userId", w.userId},
        {"workflowDefinition", w.workflowDefinition}
    };
    if (w.initialData) j["initialData"] = *w.initialData;
    if (w.timeoutMinutes) j["timeoutMinutes"] = *w.timeoutMinutes;
}

void from_json(const Json& j, WorkflowExecutionRequest& w) {
    j.at("workflowId").get_to(w.workflowId);
    j.at("projectId").get_to(w.projectId);
    j.at("userId").get_to(w.userId);
    j.at("workflowDefinition").get_to(w.workflowDefinition);
    
    if (j.contains("initialData") && !j["initialData"].is_null()) {
        w.initialData = j["initialData"];
    }
    if (j.contains("timeoutMinutes") && !j["timeoutMinutes"].is_null()) {
        w.timeoutMinutes = j["timeoutMinutes"].get<int>();
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

// SqlExecutionResult JSON serialization
void to_json(Json& j, const SqlExecutionResult& s) {
    j = Json{{"success", s.success}};
    if (s.rowsAffected) j["rowsAffected"] = *s.rowsAffected;
    if (s.data) j["data"] = *s.data;
    if (s.columns) j["columns"] = *s.columns;
    if (s.executionTime) j["executionTime"] = *s.executionTime;
    if (s.message) j["message"] = *s.message;
}

void from_json(const Json& j, SqlExecutionResult& s) {
    j.at("success").get_to(s.success);
    
    if (j.contains("rowsAffected") && !j["rowsAffected"].is_null()) {
        s.rowsAffected = j["rowsAffected"].get<int>();
    }
    if (j.contains("data") && !j["data"].is_null()) {
        s.data = j["data"].get<std::vector<Json>>();
    }
    if (j.contains("columns") && !j["columns"].is_null()) {
        s.columns = j["columns"].get<std::vector<std::string>>();
    }
    if (j.contains("executionTime") && !j["executionTime"].is_null()) {
        s.executionTime = j["executionTime"].get<int>();
    }
    if (j.contains("message") && !j["message"].is_null()) {
        s.message = j["message"].get<std::string>();
    }
}

// AiInvokeRequest JSON serialization
void to_json(Json& j, const AiInvokeRequest& a) {
    j = Json{
        {"configId", a.configId},
        {"input", a.input}
    };
    if (a.parameters) j["parameters"] = *a.parameters;
}

void from_json(const Json& j, AiInvokeRequest& a) {
    j.at("configId").get_to(a.configId);
    j.at("input").get_to(a.input);
    
    if (j.contains("parameters") && !j["parameters"].is_null()) {
        a.parameters = j["parameters"];
    }
}

// AiInvokeResponse JSON serialization
void to_json(Json& j, const AiInvokeResponse& a) {
    j = Json{{"output", a.output}};
    if (a.usage) j["usage"] = *a.usage;
    if (a.model) j["model"] = *a.model;
    if (a.finishReason) j["finishReason"] = *a.finishReason;
}

void from_json(const Json& j, AiInvokeResponse& a) {
    j.at("output").get_to(a.output);
    
    if (j.contains("usage") && !j["usage"].is_null()) {
        a.usage = j["usage"];
    }
    if (j.contains("model") && !j["model"].is_null()) {
        a.model = j["model"].get<std::string>();
    }
    if (j.contains("finishReason") && !j["finishReason"].is_null()) {
        a.finishReason = j["finishReason"].get<std::string>();
    }
}

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



// AiProvider JSON serialization
void to_json(Json& j, const AiProvider& a) {
    j = Json{
        {"id", a.id},
        {"name", a.name},
        {"type", a.type},
        {"status", a.status},
        {"config", a.config},
        {"createdAt", std::chrono::duration_cast<std::chrono::milliseconds>(a.createdAt.time_since_epoch()).count()},
        {"updatedAt", std::chrono::duration_cast<std::chrono::milliseconds>(a.updatedAt.time_since_epoch()).count()}
    };
}

void from_json(const Json& j, AiProvider& a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    j.at("type").get_to(a.type);
    j.at("status").get_to(a.status);
    j.at("config").get_to(a.config);
    
    if (j.contains("createdAt")) {
        auto createdAtMs = j.at("createdAt").get<int64_t>();
        a.createdAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(createdAtMs));
    }
    
    if (j.contains("updatedAt")) {
        auto updatedAtMs = j.at("updatedAt").get<int64_t>();
        a.updatedAt = std::chrono::system_clock::time_point(std::chrono::milliseconds(updatedAtMs));
    }
}

// DatabaseConnectionResult JSON serialization
void to_json(Json& j, const DatabaseConnectionResult& d) {
    j = Json{
        {"success", d.success},
        {"message", d.message}
    };
    if (d.details) j["details"] = *d.details;
}

void from_json(const Json& j, DatabaseConnectionResult& d) {
    j.at("success").get_to(d.success);
    j.at("message").get_to(d.message);
    
    if (j.contains("details") && !j["details"].is_null()) {
        d.details = j["details"];
    }
}

// AiProviderTestResult JSON serialization
void to_json(Json& j, const AiProviderTestResult& a) {
    j = Json{
        {"success", a.success},
        {"message", a.message},
        {"details", a.details}
    };
}

void from_json(const Json& j, AiProviderTestResult& a) {
    j.at("success").get_to(a.success);
    j.at("message").get_to(a.message);
    j.at("details").get_to(a.details);
}

// UserCreateRequest JSON serialization
void to_json(Json& j, const UserCreateRequest& u) {
    j = Json{
        {"username", u.username},
        {"email", u.email},
        {"password", u.password}
    };
    if (u.displayName) j["displayName"] = *u.displayName;
}

void from_json(const Json& j, UserCreateRequest& u) {
    j.at("username").get_to(u.username);
    j.at("email").get_to(u.email);
    j.at("password").get_to(u.password);
    
    if (j.contains("displayName") && !j["displayName"].is_null()) {
        u.displayName = j["displayName"].get<std::string>();
    }
}

// UserUpdateRequest JSON serialization
void to_json(Json& j, const UserUpdateRequest& u) {
    j = Json{};
    if (u.username) j["username"] = *u.username;
    if (u.email) j["email"] = *u.email;
    if (u.displayName) j["displayName"] = *u.displayName;
    if (u.password) j["password"] = *u.password;
}

void from_json(const Json& j, UserUpdateRequest& u) {
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

// ProjectCreateRequest JSON serialization
void to_json(Json& j, const ProjectCreateRequest& p) {
    j = Json{
        {"name", p.name},
        {"userId", p.userId}
    };
    if (p.description) j["description"] = *p.description;
}

void from_json(const Json& j, ProjectCreateRequest& p) {
    j.at("name").get_to(p.name);
    j.at("userId").get_to(p.userId);
    
    if (j.contains("description") && !j["description"].is_null()) {
        p.description = j["description"].get<std::string>();
    }
}

// ProjectUpdateRequest JSON serialization
void to_json(Json& j, const ProjectUpdateRequest& p) {
    j = Json{};
    if (p.name) j["name"] = *p.name;
    if (p.description) j["description"] = *p.description;
}

void from_json(const Json& j, ProjectUpdateRequest& p) {
    if (j.contains("name") && !j["name"].is_null()) {
        p.name = j["name"].get<std::string>();
    }
    if (j.contains("description") && !j["description"].is_null()) {
        p.description = j["description"].get<std::string>();
    }
}

} // namespace dataapi