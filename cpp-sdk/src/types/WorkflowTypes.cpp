#include "dataapi/types/WorkflowTypes.h"
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

} // namespace dataapi