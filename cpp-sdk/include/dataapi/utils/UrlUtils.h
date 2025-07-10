#pragma once

#include <string>
#include <map>

namespace dataapi {
namespace utils {

// URL参数类型定义
using Parameters = std::map<std::string, std::string>;

/**
 * URL工具类
 */
class UrlUtils {
public:
    /**
     * URL编码
     * @param value 要编码的字符串
     * @return 编码后的字符串
     */
    static std::string encode(const std::string& value);
    
    /**
     * URL解码
     * @param value 要解码的字符串
     * @return 解码后的字符串
     */
    static std::string decode(const std::string& value);
    
    /**
     * 构建查询字符串
     * @param params 参数映射
     * @return 查询字符串
     */
    static std::string buildQueryString(const Parameters& params);
    
    /**
     * 解析查询字符串
     * @param queryString 查询字符串
     * @return 参数映射
     */
    static Parameters parseQueryString(const std::string& queryString);
    
    /**
     * 连接URL路径
     * @param base 基础路径
     * @param path 要连接的路径
     * @return 连接后的路径
     */
    static std::string joinPath(const std::string& base, const std::string& path);
    
    /**
     * 验证URL是否有效
     * @param url 要验证的URL
     * @return 是否为有效URL
     */
    static bool isValidUrl(const std::string& url);
};

} // namespace utils
} // namespace dataapi