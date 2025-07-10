#include "dataapi/client/UserClient.h"
#include "dataapi/Types.h"
#include "dataapi/exceptions/DataApiException.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>

namespace dataapi {
namespace client {

UserClient::UserClient(std::shared_ptr<http::HttpClient> httpClient) : httpClient(httpClient) {
}

PageResult<SysUser> UserClient::list(int page, int size, const std::string& search, const std::string& role) {
    std::ostringstream oss;
    oss << "/users?page=" << page << "&size=" << size;
    
    auto response = httpClient->get(oss.str());
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to list users");
    }
    
    Json json = Json::parse(response.body);
    PageResult<SysUser> result;
    
    result.content.clear();
    for (const auto& item : json["content"]) {
        SysUser user;
        from_json(item, user);
        result.content.push_back(user);
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

SysUser UserClient::getById(const std::string& id) {
    auto response = httpClient->get("/users/" + id);
    if (response.statusCode == 404) {
        throw std::runtime_error("User not found: " + id);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get user");
    }
    
    Json json = Json::parse(response.body);
    SysUser user;
    from_json(json, user);
    return user;
}

SysUser UserClient::getCurrentUser() {
    auto response = httpClient->get("/users/me");
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get current user");
    }
    
    Json json = Json::parse(response.body);
    SysUser user;
    from_json(json, user);
    return user;
}

SysUser UserClient::create(const UserCreateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->post("/users", json);
    if (response.statusCode != 201) {
        throw std::runtime_error("Failed to create user");
    }
    
    Json responseJson = Json::parse(response.body);
    SysUser user;
    from_json(responseJson, user);
    return user;
}

SysUser UserClient::update(const std::string& id, const UserUpdateRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->put("/users/" + id, json);
    if (response.statusCode == 404) {
        throw std::runtime_error("User not found: " + id);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to update user");
    }
    
    Json responseJson = Json::parse(response.body);
    SysUser user;
    from_json(responseJson, user);
    return user;
}

void UserClient::deleteUser(const std::string& id) {
    auto response = httpClient->del("/users/" + id);
    if (response.statusCode == 404) {
        throw std::runtime_error("User not found: " + id);
    }
    if (response.statusCode != 204) {
        throw std::runtime_error("Failed to delete user");
    }
}

} // namespace client
} // namespace dataapi