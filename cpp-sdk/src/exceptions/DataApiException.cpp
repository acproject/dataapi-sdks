#include "dataapi/error/DataApiError.h"
#include <string>
#include <vector>
#include <memory>

namespace dataapi {
namespace error {

DataApiError::DataApiError(const std::string& message,
                          const std::string& code,
                          int statusCode,
                          const std::string& requestId,
                          const Json& responseBody,
                          std::shared_ptr<std::exception> cause)
    : std::runtime_error(message), code_(code), statusCode_(statusCode),
      requestId_(requestId), responseBody_(responseBody), cause_(cause) {
}

HttpError::HttpError(const std::string& message,
                    int statusCode,
                    const std::string& method,
                    const std::string& url,
                    const std::string& requestId,
                    const Json& responseBody)
    : DataApiError(message, "", statusCode, requestId, responseBody),
      method_(method), url_(url) {
}

NotFoundError::NotFoundError(const std::string& message,
                            const std::string& resourceType,
                            const std::string& resourceId)
    : DataApiError(message, "NOT_FOUND_ERROR", 404),
      resourceType_(resourceType), resourceId_(resourceId) {
}

AuthenticationError::AuthenticationError(const std::string& message,
                                        const std::string& code)
    : DataApiError(message, code, 401) {
}

ValidationError::ValidationError(const std::string& message,
                                const std::string& field,
                                const std::vector<std::string>& validationRules,
                                const std::string& code)
    : DataApiError(message, code, 400), field_(field), validationRules_(validationRules) {
}

// DataApiError虚函数实现
Json DataApiError::toJson() const {
    Json json;
    json["message"] = what();
    json["code"] = code_;
    json["statusCode"] = statusCode_;
    json["requestId"] = requestId_;
    if (!responseBody_.is_null()) {
        json["responseBody"] = responseBody_;
    }
    return json;
}

std::string DataApiError::toString() const {
    return toJson().dump();
}

bool DataApiError::isRetryable() const {
    return false; // 默认不可重试
}

// ValidationError虚函数实现
Json ValidationError::toJson() const {
    Json json = DataApiError::toJson();
    json["field"] = field_;
    json["validationRules"] = validationRules_;
    return json;
}

std::string ValidationError::toString() const {
    return toJson().dump();
}

// HttpError虚函数实现
Json HttpError::toJson() const {
    Json json = DataApiError::toJson();
    json["method"] = method_;
    json["url"] = url_;
    return json;
}

std::string HttpError::toString() const {
    return toJson().dump();
}

bool HttpError::isRetryable() const {
    // 5xx错误通常可以重试
    return statusCode_ >= 500 && statusCode_ < 600;
}

// NotFoundError虚函数实现
Json NotFoundError::toJson() const {
    Json json = DataApiError::toJson();
    json["resourceType"] = resourceType_;
    json["resourceId"] = resourceId_;
    return json;
}

} // namespace error
} // namespace dataapi