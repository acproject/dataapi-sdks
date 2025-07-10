#include "dataapi/client/DatabaseClient.h"
#include "dataapi/Types.h"
#include "dataapi/exceptions/DataApiException.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>

namespace dataapi {
namespace client {

DatabaseClient::DatabaseClient(std::shared_ptr<http::HttpClient> httpClient) : httpClient(httpClient) {
}

PageResult<DatabaseInfo> DatabaseClient::list(int page, int size, const std::string& projectId) {
    std::ostringstream oss;
    oss << "/databases?page=" << page << "&size=" << size;
    
    auto response = httpClient->get(oss.str());
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to list databases");
    }
    
    Json json = Json::parse(response.body);
    PageResult<DatabaseInfo> result;
    
    result.content.clear();
    for (const auto& item : json["content"]) {
        DatabaseInfo database;
        from_json(item, database);
        result.content.push_back(database);
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

DatabaseInfo DatabaseClient::getInfo(const std::string& databaseId) {
    auto response = httpClient->get("/databases/" + databaseId);
    if (response.statusCode == 404) {
        throw std::runtime_error("Database not found: " + databaseId);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get database");
    }
    
    Json json = Json::parse(response.body);
    DatabaseInfo database;
    from_json(json, database);
    return database;
}

DatabaseConnectionResult DatabaseClient::testConnection(const DatabaseConfig& config) {
    Json configJson;
    to_json(configJson, config);
    auto response = httpClient->post("/databases/test-connection", configJson);
    if (response.statusCode == 404) {
        throw std::runtime_error("Database configuration test failed");
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to test database connection");
    }
    
    Json json = Json::parse(response.body);
    DatabaseConnectionResult result;
    from_json(json, result);
    return result;
}

QueryResult DatabaseClient::executeQuery(const std::string& databaseId, const std::string& sql, const Parameters& params) {
    Json requestJson;
    requestJson["sql"] = sql;
    
    auto response = httpClient->post("/databases/" + databaseId + "/execute", requestJson);
    if (response.statusCode == 404) {
        throw std::runtime_error("Database not found: " + databaseId);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to execute SQL");
    }
    
    Json json = Json::parse(response.body);
    QueryResult result;
    from_json(json, result);
    return result;
}

} // namespace client
} // namespace dataapi