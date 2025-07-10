#include "dataapi/http/HttpClient.h"
#include "dataapi/exceptions/DataApiException.h"
#include "dataapi/ClientConfig.h"
#include "dataapi/Types.h"
#include <curl/curl.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <memory>

namespace dataapi {
namespace http {

// CURL写入回调函数
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// CURL头部回调函数
static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, Headers* headers) {
    size_t totalSize = size * nitems;
    std::string header(buffer, totalSize);
    
    // 查找冒号分隔符
    size_t colonPos = header.find(':');
    if (colonPos != std::string::npos) {
        std::string key = header.substr(0, colonPos);
        std::string value = header.substr(colonPos + 1);
        
        // 去除前后空白字符
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));
        value.erase(value.find_last_not_of(" \t\r\n") + 1);
        
        (*headers)[key] = value;
    }
    
    return totalSize;
}

HttpClient::HttpClient(const ClientConfig& config, std::shared_ptr<auth::AuthenticationProvider> authProvider) 
    : config(config), authProvider(authProvider) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient() {
    curl_global_cleanup();
}

HttpClient::HttpClient(HttpClient&& other) noexcept 
    : config(std::move(other.config)), authProvider(std::move(other.authProvider)) {
}

HttpClient& HttpClient::operator=(HttpClient&& other) noexcept {
    if (this != &other) {
        config = std::move(other.config);
        authProvider = std::move(other.authProvider);
    }
    return *this;
}

HttpResponse HttpClient::get(const std::string& endpoint, const Parameters& params, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::GET;
    config.url = endpoint;
    config.params = params;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::post(const std::string& endpoint, const Json& data, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::POST;
    config.url = endpoint;
    config.data = data;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::put(const std::string& endpoint, const Json& data, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::PUT;
    config.url = endpoint;
    config.data = data;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::del(const std::string& endpoint, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::DELETE;
    config.url = endpoint;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::head(const std::string& endpoint, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::HEAD;
    config.url = endpoint;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::patch(const std::string& endpoint, const Json& data, const Headers& headers) {
    HttpRequestConfig config;
    config.method = HttpMethod::PATCH;
    config.url = endpoint;
    config.data = data;
    config.headers = headers;
    return request(config);
}

HttpResponse HttpClient::request(const HttpRequestConfig& requestConfig) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    HttpResponse response;
    std::string responseBody;
    Headers responseHeaders;
    
    // 构建完整URL
    std::string url = config.baseUrl + requestConfig.url;
    
    // 设置基本选项
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, config.timeout / 1000);
    
    // 准备请求体
    std::string body;
    if (!requestConfig.data.is_null()) {
        body = requestConfig.data.dump();
    }
    
    // 设置HTTP方法
    switch (requestConfig.method) {
        case HttpMethod::GET:
            break; // GET是默认方法
        case HttpMethod::POST:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (!body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
            }
            break;
        case HttpMethod::PUT:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            if (!body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
            }
            break;
        case HttpMethod::DELETE:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case HttpMethod::HEAD:
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            break;
        case HttpMethod::PATCH:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
            if (!body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
            }
            break;
    }
    
    // 设置请求头
    struct curl_slist* headerList = nullptr;
    
    // 添加认证头
    if (authProvider) {
        auto authHeaders = authProvider->getAuthHeaders();
        for (const auto& header : authHeaders) {
            std::string headerStr = header.first + ": " + header.second;
            headerList = curl_slist_append(headerList, headerStr.c_str());
        }
    }
    
    // 添加自定义头
    for (const auto& header : requestConfig.headers) {
        std::string headerStr = header.first + ": " + header.second;
        headerList = curl_slist_append(headerList, headerStr.c_str());
    }
    
    // 添加Content-Type头（如果有body）
    if (!body.empty() && requestConfig.headers.find("Content-Type") == requestConfig.headers.end()) {
        headerList = curl_slist_append(headerList, "Content-Type: application/json");
    }
    
    if (headerList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    
    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_slist_free_all(headerList);
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }
    
    // 获取响应状态码
    long statusCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    
    // 清理
    curl_slist_free_all(headerList);
    curl_easy_cleanup(curl);
    
    // 构建响应对象
    response.statusCode = static_cast<int>(statusCode);
    response.headers = responseHeaders;
    response.body = responseBody;
    
    return response;
}

bool HttpClient::testConnection() {
    try {
        HttpRequestConfig config;
        config.method = HttpMethod::HEAD;
        config.url = "/health";
        auto response = request(config);
        return response.statusCode >= 200 && response.statusCode < 300;
    } catch (...) {
        return false;
    }
}

void HttpClient::updateConfig(const ClientConfig& newConfig) {
    config = newConfig;
}

void HttpClient::updateAuthProvider(std::shared_ptr<auth::AuthenticationProvider> newAuthProvider) {
    authProvider = newAuthProvider;
}

void HttpClient::setTimeout(int timeoutMs) {
    config.timeout = timeoutMs;
}

void HttpClient::setVerifySSL(bool verify) {
    config.verifySSL = verify;
}

void HttpClient::setProxy(const std::string& proxyUrl) {
    config.proxyUrl = proxyUrl;
}

HttpClient::RequestStats HttpClient::getLastRequestStats() const {
    // 这里应该实现获取最后一次请求统计信息的逻辑
    // 目前返回默认值
    return RequestStats{0.0, 0.0, 0.0, 0, 0};
}

} // namespace http
} // namespace dataapi