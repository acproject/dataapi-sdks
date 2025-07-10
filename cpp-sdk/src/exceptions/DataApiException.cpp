#include "dataapi/error/DataApiError.h"

namespace dataapi {
namespace error {

DataApiError::DataApiError(const std::string& message) : std::runtime_error(message) {
}

HttpError::HttpError(const std::string& message, int statusCode)
    : DataApiError(message), statusCode_(statusCode) {
}

int HttpError::getStatusCode() const {
    return statusCode_;
}

NotFoundError::NotFoundError(const std::string& message)
    : HttpError(message, 404) {
}

AuthenticationError::AuthenticationError(const std::string& message)
    : HttpError(message, 401) {
}

ValidationError::ValidationError(const std::string& message)
    : DataApiError(message) {
}

} // namespace error
} // namespace dataapi