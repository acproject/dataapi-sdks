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

PageResult<AiProvider> AiProviderClient::listProviders(int page, int size) {
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
    result.size = json["size"].get<int>();
    result.number = json["number"].get<int>();
    result.first = json["first"].get<bool>();
    result.last = json["last"].get<bool>();
    
    return result;
}

AiProvider AiProviderClient::getProvider(const std::string& providerId) {
    auto response = httpClient->get("/ai-providers/" + providerId);
    if (response.statusCode == 404) {
        throw std::runtime_error("AI provider not found: " + providerId);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to get AI provider");
    }
    
    Json json = Json::parse(response.body);
    AiProvider provider;
    from_json(json, provider);
    return provider;
}

AiProviderTestResult AiProviderClient::testProvider(const std::string& providerId) {
    Json emptyJson;
    auto response = httpClient->post("/ai-providers/" + providerId + "/test", emptyJson);
    if (response.statusCode == 404) {
        throw std::runtime_error("AI provider not found: " + providerId);
    }
    if (response.statusCode != 200) {
        throw std::runtime_error("Failed to test AI provider");
    }
    
    Json json = Json::parse(response.body);
    AiProviderTestResult result;
    from_json(json, result);
    return result;
}

AiInvokeResponse AiProviderClient::invokeAi(const std::string& providerId, const AiInvokeRequest& request) {
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
    AiInvokeResponse result;
    from_json(responseJson, result);
    return result;
}

} // namespace client
} // namespace dataapi