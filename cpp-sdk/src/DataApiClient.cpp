#include "dataapi/DataApiClient.h"
#include "dataapi/exceptions/DataApiException.h"
#include <stdexcept>

using namespace dataapi::exceptions;

namespace dataapi {

DataApiClient::DataApiClient(const ClientConfig& config, std::shared_ptr<auth::AuthenticationProvider> authProvider)
    : config_(config)
    , authProvider_(authProvider)
    , httpClient_(std::make_shared<http::HttpClient>(config, authProvider))
    , workflowClient_(std::make_unique<client::WorkflowClient>(httpClient_))
    , projectClient_(std::make_unique<client::ProjectClient>(httpClient_))
    , databaseClient_(std::make_unique<client::DatabaseClient>(httpClient_))
    , aiProviderClient_(std::make_unique<client::AiProviderClient>(httpClient_))
    , userClient_(std::make_unique<client::UserClient>(httpClient_)) {
    initialize();
}

DataApiClient::~DataApiClient() = default;

bool DataApiClient::testConnection() {
    try {
        auto response = httpClient_->get("/health");
        return response.statusCode == 200;
    } catch (const std::exception&) {
        return false;
    }
}

ApiVersion DataApiClient::getVersion() {
    auto response = httpClient_->get("/version");
    if (response.statusCode != 200) {
        throw HttpError("Failed to get API version", response.statusCode);
    }
    
    ApiVersion version;
    auto json = Json::parse(response.body);
    version.version = json["version"].get<std::string>();
    version.buildTime = json["buildTime"].get<std::string>();
    version.gitCommit = json["gitCommit"].get<std::string>();
    
    return version;
}

HealthStatus DataApiClient::getHealth() {
    auto response = httpClient_->get("/health");
    if (response.statusCode != 200) {
        throw HttpError("Failed to get health status", response.statusCode);
    }
    
    HealthStatus health;
    auto json = Json::parse(response.body);
    health.status = json["status"].get<std::string>();
    health.message = json["message"].get<std::string>();
    if (json.contains("details")) {
        for (auto& [key, value] : json["details"].items()) {
            health.details[key] = value.get<std::string>();
        }
    }
    
    return health;
}

client::WorkflowClient& DataApiClient::getWorkflowClient() {
    return *workflowClient_;
}

client::ProjectClient& DataApiClient::getProjectClient() {
    return *projectClient_;
}

client::DatabaseClient& DataApiClient::getDatabaseClient() {
    return *databaseClient_;
}

client::AiProviderClient& DataApiClient::getAiProviderClient() {
    return *aiProviderClient_;
}

client::UserClient& DataApiClient::getUserClient() {
    return *userClient_;
}

const ClientConfig& DataApiClient::getConfig() const {
    return config_;
}

std::shared_ptr<auth::AuthenticationProvider> DataApiClient::getAuthProvider() const {
    return authProvider_;
}

std::shared_ptr<http::HttpClient> DataApiClient::getHttpClient() {
    return httpClient_;
}

void DataApiClient::updateConfig(const ClientConfig& newConfig) {
    config_ = newConfig;
    httpClient_ = std::make_shared<http::HttpClient>(config_, authProvider_);
    initialize();
}

void DataApiClient::close() {
    // 清理资源
}

void DataApiClient::initialize() {
    // 初始化逻辑
}

std::unique_ptr<DataApiClient> DataApiClient::createForDevelopment(
    const std::string& apiKey,
    const std::string& baseUrl) {
    ClientConfig config(baseUrl);
    auto authProvider = std::make_shared<auth::ApiKeyAuthProvider>(apiKey);
    return std::make_unique<DataApiClient>(config, authProvider);
}

std::unique_ptr<DataApiClient> DataApiClient::createForProduction(
    const std::string& apiKey,
    const std::string& baseUrl) {
    ClientConfig config(baseUrl);
    auto authProvider = std::make_shared<auth::ApiKeyAuthProvider>(apiKey);
    return std::make_unique<DataApiClient>(config, authProvider);
}

} // namespace dataapi