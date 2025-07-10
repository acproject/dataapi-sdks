#include "dataapi/client/ProjectClient.h"
#include "dataapi/Types.h"
#include "dataapi/exceptions/DataApiException.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>

namespace dataapi {
namespace client {

ProjectClient::ProjectClient(std::shared_ptr<http::HttpClient> httpClient) : httpClient(httpClient) {
}

PageResult<SysProject> ProjectClient::list(int page, int size, const std::string& userId) {
    std::ostringstream oss;
    oss << "/projects?page=" << page << "&size=" << size;
    
    auto response = httpClient->get(oss.str());
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to list projects");
    }
    
    Json json = Json::parse(response.body);
    PageResult<SysProject> result;
    
    result.content.clear();
    for (const auto& item : json["content"]) {
        SysProject project;
        from_json(item, project);
        result.content.push_back(project);
    }
    
    result.totalElements = json["totalElements"].get<long>();
    result.totalPages = json["totalPages"].get<int>();
    result.pageSize = json["size"].get<int>();
    result.pageNumber = json["number"].get<int>();
    result.empty = json.value("empty", false);
    result.first = json["first"].get<bool>();
    result.last = json["last"].get<bool>();
    
    return result;
}

SysProject ProjectClient::getById(const std::string& id) {
    auto response = httpClient->get("/projects/" + id);
    if (response.statusCode == 404) {
        throw std::runtime_error("Project not found: " + id);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get project");
    }
    
    Json json = Json::parse(response.body);
    SysProject project;
    from_json(json, project);
    return project;
}

SysProject ProjectClient::create(const ProjectCreateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->post("/projects", json);
    if (response.statusCode != 201) {
        throw std::runtime_error("Failed to create project");
    }
    
    Json responseJson = Json::parse(response.body);
    SysProject project;
    from_json(responseJson, project);
    return project;
}

SysProject ProjectClient::update(const std::string& id, const ProjectUpdateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->put("/projects/" + id, json);
    if (response.statusCode == 404) {
        throw std::runtime_error("Project not found: " + id);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to update project");
    }
    
    Json responseJson = Json::parse(response.body);
    SysProject project;
    from_json(responseJson, project);
    return project;
}

void ProjectClient::deleteProject(const std::string& id) {
    auto response = httpClient->del("/projects/" + id);
    if (response.statusCode == 404) {
        throw std::runtime_error("Project not found: " + id);
    }
    if (response.statusCode != 204) {
        throw std::runtime_error("Failed to delete project");
    }
}

} // namespace client
} // namespace dataapi