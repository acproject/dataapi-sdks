#pragma once

#include "../Types.h"
#include <string>
#include <stdexcept>

namespace dataapi {
namespace utils {

/**
 * JSON工具类
 */
class JsonUtils {
public:
    /**
     * 解析JSON字符串
     * @param jsonString JSON字符串
     * @return Json对象
     * @throws ValidationError 如果JSON格式无效
     */
    static Json parseJson(const std::string& jsonString);
    
    /**
     * 将Json对象转换为字符串
     * @param json Json对象
     * @return JSON字符串
     */
    static std::string toJsonString(const Json& json);
    
    /**
     * 将Json对象转换为格式化字符串
     * @param json Json对象
     * @param indent 缩进空格数
     * @return 格式化的JSON字符串
     */
    static std::string toJsonString(const Json& json, int indent);
    
    /**
     * 检查Json对象是否包含指定键
     * @param json Json对象
     * @param key 键名
     * @return 是否包含该键
     */
    static bool hasKey(const Json& json, const std::string& key);
    
    /**
     * 获取Json对象中指定键的值
     * @param json Json对象
     * @param key 键名
     * @return 键对应的值
     * @throws ValidationError 如果键不存在
     */
    static Json getValue(const Json& json, const std::string& key);
    
    /**
     * 获取Json对象中指定键的值，如果不存在则返回默认值
     * @param json Json对象
     * @param key 键名
     * @param defaultValue 默认值
     * @return 键对应的值或默认值
     */
    static Json getValue(const Json& json, const std::string& key, const Json& defaultValue);
};

} // namespace utils
} // namespace dataapi