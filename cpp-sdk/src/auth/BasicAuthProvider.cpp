#include "dataapi/auth/AuthenticationProvider.h"
#include <string>
#include <algorithm>

namespace dataapi {
namespace auth {

// Base64编码表
static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// Base64编码函数
static std::string base64_encode(const std::string& input) {
    std::string encoded;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (encoded.size() % 4) {
        encoded.push_back('=');
    }
    return encoded;
}

// 实现BasicAuthProvider的getAuthHeaders方法
std::unordered_map<std::string, std::string> BasicAuthProvider::getAuthHeaders() const {
    std::string credentials = username + ":" + password;
    std::string encoded = base64_encode(credentials);
    return {{"Authorization", "Basic " + encoded}};
}

} // namespace auth
} // namespace dataapi