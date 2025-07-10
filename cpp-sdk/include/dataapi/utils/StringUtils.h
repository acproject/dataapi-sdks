#pragma once

#include <string>
#include <vector>

namespace dataapi {
namespace utils {

/**
 * 字符串工具类
 */
class StringUtils {
public:
    /**
     * 去除字符串首尾空白字符
     * @param str 输入字符串
     * @return 去除空白字符后的字符串
     */
    static std::string trim(const std::string& str);
    
    /**
     * 将字符串转换为小写
     * @param str 输入字符串
     * @return 小写字符串
     */
    static std::string toLower(const std::string& str);
    
    /**
     * 将字符串转换为大写
     * @param str 输入字符串
     * @return 大写字符串
     */
    static std::string toUpper(const std::string& str);
    
    /**
     * 检查字符串是否以指定前缀开始
     * @param str 输入字符串
     * @param prefix 前缀
     * @return 是否以前缀开始
     */
    static bool startsWith(const std::string& str, const std::string& prefix);
    
    /**
     * 检查字符串是否以指定后缀结束
     * @param str 输入字符串
     * @param suffix 后缀
     * @return 是否以后缀结束
     */
    static bool endsWith(const std::string& str, const std::string& suffix);
    
    /**
     * 按分隔符分割字符串
     * @param str 输入字符串
     * @param delimiter 分隔符
     * @return 分割后的字符串数组
     */
    static std::vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * 用分隔符连接字符串数组
     * @param strings 字符串数组
     * @param delimiter 分隔符
     * @return 连接后的字符串
     */
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    /**
     * 替换字符串中的子串
     * @param str 输入字符串
     * @param from 要替换的子串
     * @param to 替换为的子串
     * @return 替换后的字符串
     */
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    
    /**
     * 检查字符串是否为空
     * @param str 输入字符串
     * @return 是否为空
     */
    static bool isEmpty(const std::string& str);
    
    /**
     * 检查字符串是否为空白（空或只包含空白字符）
     * @param str 输入字符串
     * @return 是否为空白
     */
    static bool isBlank(const std::string& str);
};

} // namespace utils
} // namespace dataapi