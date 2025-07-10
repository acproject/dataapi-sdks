#include "dataapi/client/AiProviderClient.h"
#include "dataapi/Types.h"
#include "dataapi/exceptions/DataApiException.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>

namespace dataapi {
namespace client {

AiProviderClient::AiProviderClient(std::shared_ptr<http::HttpClient> httpClient) : httpClient(httpClient) {
}

PageResult<AiProvider> AiProviderClient::list(int page, int size, const std::string& type) {
    std::ostringstream oss;
    oss << "/ai-providers?page=" << page << "&size=" << size;
    
    auto response = httpClient->get(oss.str());
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to list AI providers");
    }
    
    Json json = Json::parse(response.body);
    PageResult<AiProvider> result;
    
    result.content.clear();
    for (const auto& item : json["content"]) {
        AiProvider provider;
        from_json(item, provider);
        result.content.push_back(provider);
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

AiProvider AiProviderClient::getById(const std::string& id) {
    auto response = httpClient->get("/ai-providers/" + id);
    if (response.statusCode == 404) {
        throw std::runtime_error("AI provider not found: " + id);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get AI provider");
    }
    
    Json json = Json::parse(response.body);
    AiProvider provider;
    from_json(json, provider);
    return provider;
}

AiProviderTestResult AiProviderClient::testConfiguration(const AiProviderConfig& config) {
    Json configJson;
    to_json(configJson, config);
    auto response = httpClient->post("/ai-providers/test", configJson);
    if (response.statusCode == 404) {
        throw std::runtime_error("AI provider configuration test failed");
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to test AI provider");
    }
    
    Json json = Json::parse(response.body);
    AiProviderTestResult result;
    from_json(json, result);
    return result;
}

AiServiceResponse AiProviderClient::invoke(const std::string& providerId, const AiServiceRequest& request) {
    Json json;
    to_json(json, request);
    
    auto response = httpClient->post("/ai-providers/" + providerId + "/invoke", json);
    if (response.statusCode == 404) {
        throw std::runtime_error("AI provider not found: " + providerId);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to invoke AI");
    }
    
    Json responseJson = Json::parse(response.body);
    AiServiceResponse result;
    from_json(responseJson, result);
    return result;
}

} // namespace client
} // namespace dataapi