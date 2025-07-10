#include "dataapi/types/AiTypes.h"
#include <chrono>
#include <cstdint>

namespace dataapi {

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

// AiProviderConfig JSON serialization
void to_json(Json& j, const AiProviderConfig& a) {
    j = Json{
        {"apiKey", a.apiKey},
        {"endpoint", a.endpoint},
        {"settings", a.settings}
    };
}

void from_json(const Json& j, AiProviderConfig& a) {
    j.at("apiKey").get_to(a.apiKey);
    j.at("endpoint").get_to(a.endpoint);
    j.at("settings").get_to(a.settings);
}

// AiServiceRequest JSON serialization
void to_json(Json& j, const AiServiceRequest& a) {
    j = Json{
        {"prompt", a.prompt},
        {"model", a.model},
        {"parameters", a.parameters}
    };
}

void from_json(const Json& j, AiServiceRequest& a) {
    j.at("prompt").get_to(a.prompt);
    j.at("model").get_to(a.model);
    j.at("parameters").get_to(a.parameters);
}

// AiServiceResponse JSON serialization
void to_json(Json& j, const AiServiceResponse& a) {
    j = Json{
        {"response", a.response},
        {"metadata", a.metadata},
        {"requestId", a.requestId}
    };
}

void from_json(const Json& j, AiServiceResponse& a) {
    j.at("response").get_to(a.response);
    j.at("metadata").get_to(a.metadata);
    j.at("requestId").get_to(a.requestId);
}

} // namespace dataapi