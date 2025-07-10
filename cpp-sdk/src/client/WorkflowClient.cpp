#include "dataapi/client/WorkflowClient.h"
#include "dataapi/error/DataApiError.h"
#include <sstream>

namespace dataapi {
namespace client {

WorkflowClient::WorkflowClient(std::shared_ptr<http::HttpClient> httpClient) : httpClient(httpClient) {
}

PageResult<SysWorkflow> WorkflowClient::list(int page, int size, const std::string& projectId, const std::string& userId) {
    std::ostringstream oss;
    oss << "/workflows?page=" << page << "&size=" << size;
    
    auto response = httpClient->get(oss.str());
    if (response.statusCode != 200) {
        throw error::HttpError("Failed to list workflows", response.statusCode);
    }
    
    auto json = Json::parse(response.body);
    PageResult<SysWorkflow> result;
    
    result.content.clear();
    for (const auto& item : json["content"]) {
        SysWorkflow workflow;
        from_json(item, workflow);
        result.content.push_back(workflow);
    }
    
    result.totalElements = json["totalElements"].get<long>();
    result.totalPages = json["totalPages"].get<int>();
    result.pageSize = json["size"].get<int>();
    result.pageNumber = json["number"].get<int>();
    result.first = json["first"].get<bool>();
    result.last = json["last"].get<bool>();
    result.empty = json.value("empty", false);
    
    return result;
}

SysWorkflow WorkflowClient::getById(const std::string& id) {
    auto response = httpClient->get("/workflows/" + id);
    if (response.statusCode == 404) {
        throw error::NotFoundError("Workflow not found: " + id);
    }
    if (response.statusCode != 200) {
        throw error::HttpError("Failed to get workflow", response.statusCode);
    }
    
    auto json = Json::parse(response.body);
    SysWorkflow workflow;
    from_json(json, workflow);
    return workflow;
}

SysWorkflow WorkflowClient::create(const WorkflowCreateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->post("/workflows", json.dump());
    if (response.statusCode != 201) {
        throw error::HttpError("Failed to create workflow", response.statusCode);
    }
    
    auto responseJson = Json::parse(response.body);
    SysWorkflow workflow;
    from_json(responseJson, workflow);
    return workflow;
}

SysWorkflow WorkflowClient::update(const std::string& id, const WorkflowUpdateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->put("/workflows/" + id, json.dump());
    if (response.statusCode == 404) {
        throw error::NotFoundError("Workflow not found: " + id);
    }
    if (response.statusCode != 200) {
        throw error::HttpError("Failed to update workflow", response.statusCode);
    }
    
    auto responseJson = Json::parse(response.body);
    SysWorkflow workflow;
    from_json(responseJson, workflow);
    return workflow;
}

void WorkflowClient::deleteWorkflow(const std::string& id) {
    auto response = httpClient->del("/workflows/" + id);
    if (response.statusCode == 404) {
        throw error::NotFoundError("Workflow not found: " + id);
    }
    if (response.statusCode != 204) {
        throw error::HttpError("Failed to delete workflow", response.statusCode);
    }
}

WorkflowExecutionResult WorkflowClient::execute(const std::string& id, const Json& input) {
    Json json = input;
    
    auto response = httpClient->post("/workflows/" + id + "/execute", json.dump());
    if (response.statusCode == 404) {
        throw error::NotFoundError("Workflow not found: " + id);
    }
    if (response.statusCode != 200) {
        throw error::HttpError("Failed to execute workflow", response.statusCode);
    }
    
    auto responseJson = Json::parse(response.body);
    WorkflowExecutionResult result;
    from_json(responseJson, result);
    return result;
}

} // namespace client
} // namespace dataapi