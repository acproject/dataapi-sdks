#include "dataapi/utils/UrlUtils.h"
#include <sstream>
#include <iomanip>
#include <cctype>

namespace dataapi {
namespace utils {

std::string UrlUtils::encode(const std::string& value) {
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;
    
    for (char c : value) {
        // 保留字母、数字和一些安全字符
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << std::uppercase;
            encoded << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
            encoded << std::nouppercase;
        }
    }
    
    return encoded.str();
}

std::string UrlUtils::decode(const std::string& value) {
    std::string decoded;
    
    for (size_t i = 0; i < value.length(); ++i) {
        if (value[i] == '%' && i + 2 < value.length()) {
            // 解码百分号编码的字符
            std::string hex = value.substr(i + 1, 2);
            char decodedChar = static_cast<char>(std::stoi(hex, nullptr, 16));
            decoded += decodedChar;
            i += 2; // 跳过接下来的两个字符
        } else if (value[i] == '+') {
            // 将+替换为空格
            decoded += ' ';
        } else {
            decoded += value[i];
        }
    }
    
    return decoded;
}

std::string UrlUtils::buildQueryString(const Parameters& params) {
    if (params.empty()) {
        return "";
    }
    
    std::ostringstream oss;
    bool first = true;
    
    for (const auto& param : params) {
        if (!first) {
            oss << "&";
        }
        oss << encode(param.first) << "=" << encode(param.second);
        first = false;
    }
    
    return oss.str();
}

Parameters UrlUtils::parseQueryString(const std::string& queryString) {
    Parameters params;
    
    if (queryString.empty()) {
        return params;
    }
    
    std::string query = queryString;
    // 移除开头的?字符（如果存在）
    if (query[0] == '?') {
        query = query.substr(1);
    }
    
    std::istringstream iss(query);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t equalPos = pair.find('=');
        if (equalPos != std::string::npos) {
            std::string key = decode(pair.substr(0, equalPos));
            std::string value = decode(pair.substr(equalPos + 1));
            params[key] = value;
        }
    }
    
    return params;
}

std::string UrlUtils::joinPath(const std::string& base, const std::string& path) {
    std::string result = base;
    
    // 确保base以/结尾
    if (!result.empty() && result.back() != '/') {
        result += '/';
    }
    
    // 移除path开头的/
    std::string cleanPath = path;
    if (!cleanPath.empty() && cleanPath.front() == '/') {
        cleanPath = cleanPath.substr(1);
    }
    
    return result + cleanPath;
}

bool UrlUtils::isValidUrl(const std::string& url) {
    // 简单的URL验证
    return url.find("http://") == 0 || url.find("https://") == 0;
}

} // namespace utils
} // namespace dataapi