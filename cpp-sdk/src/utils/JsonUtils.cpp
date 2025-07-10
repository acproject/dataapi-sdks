#include "dataapi/utils/JsonUtils.h"
#include "dataapi/error/DataApiError.h"

namespace dataapi {
namespace utils {

Json JsonUtils::parseJson(const std::string& jsonString) {
    try {
        return Json::parse(jsonString);
    } catch (const Json::parse_error& e) {
        throw error::ValidationError("Invalid JSON: " + std::string(e.what()));
    }
}

std::string JsonUtils::toJsonString(const Json& json) {
    return json.dump();
}

std::string JsonUtils::toJsonString(const Json& json, int indent) {
    return json.dump(indent);
}

bool JsonUtils::hasKey(const Json& json, const std::string& key) {
    return json.contains(key);
}

Json JsonUtils::getValue(const Json& json, const std::string& key) {
    if (!hasKey(json, key)) {
        throw error::ValidationError("Key not found: " + key);
    }
    return json[key];
}

Json JsonUtils::getValue(const Json& json, const std::string& key, const Json& defaultValue) {
    if (hasKey(json, key)) {
        return json[key];
    }
    return defaultValue;
}

} // namespace utils
} // namespace dataapi