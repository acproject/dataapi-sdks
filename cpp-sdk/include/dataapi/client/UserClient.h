#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Types.h"
#include "../http/HttpClient.h"

namespace dataapi {
namespace client {

/**
 * 用户客户端类
 * 提供用户相关的API操作
 */
class UserClient {
private:
    std::shared_ptr<http::HttpClient> httpClient;
    
public:
    /**
     * 构造函数
     * @param httpClient HTTP客户端
     */
    explicit UserClient(std::shared_ptr<http::HttpClient> httpClient);
    
    /**
     * 析构函数
     */
    ~UserClient() = default;
    
    /**
     * 禁用拷贝构造和赋值
     */
    UserClient(const UserClient&) = delete;
    UserClient& operator=(const UserClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    UserClient(UserClient&& other) noexcept = default;
    UserClient& operator=(UserClient&& other) noexcept = default;
    
    /**
     * 获取当前用户信息
     * @return 用户信息
     */
    SysUser getCurrentUser();
    
    /**
     * 更新当前用户信息
     * @param request 用户更新请求
     * @return 更新后的用户信息
     */
    SysUser updateCurrentUser(const UserUpdateRequest& request);
    
    /**
     * 获取用户列表（管理员权限）
     * @param page 页码（从1开始）
     * @param size 每页大小
     * @param search 搜索关键词（可选）
     * @param role 角色过滤（可选）
     * @return 用户列表
     */
    PageResult<SysUser> list(int page = 1, int size = 20,
                            const std::string& search = "",
                            const std::string& role = "");
    
    /**
     * 根据ID获取用户详情
     * @param id 用户ID
     * @return 用户详情
     */
    SysUser getById(const std::string& id);
    
    /**
     * 创建新用户（管理员权限）
     * @param request 用户创建请求
     * @return 创建的用户
     */
    SysUser create(const UserCreateRequest& request);
    
    /**
     * 更新用户信息（管理员权限）
     * @param id 用户ID
     * @param request 用户更新请求
     * @return 更新后的用户信息
     */
    SysUser update(const std::string& id, const UserUpdateRequest& request);
    
    /**
     * 删除用户（管理员权限）
     * @param id 用户ID
     */
    void deleteUser(const std::string& id);
    
    /**
     * 启用/禁用用户（管理员权限）
     * @param id 用户ID
     * @param enabled 是否启用
     */
    void setUserEnabled(const std::string& id, bool enabled);
    
    /**
     * 重置用户密码（管理员权限）
     * @param id 用户ID
     * @param newPassword 新密码
     */
    void resetPassword(const std::string& id, const std::string& newPassword);
    
    /**
     * 修改当前用户密码
     * @param oldPassword 旧密码
     * @param newPassword 新密码
     */
    void changePassword(const std::string& oldPassword, 
                       const std::string& newPassword);
    
    /**
     * 获取用户角色列表
     * @param userId 用户ID
     * @return 角色列表
     */
    std::vector<UserRole> getUserRoles(const std::string& userId);
    
    /**
     * 设置用户角色
     * @param userId 用户ID
     * @param roleIds 角色ID列表
     */
    void setUserRoles(const std::string& userId, 
                     const std::vector<std::string>& roleIds);
    
    /**
     * 添加用户角色
     * @param userId 用户ID
     * @param roleId 角色ID
     */
    void addUserRole(const std::string& userId, const std::string& roleId);
    
    /**
     * 移除用户角色
     * @param userId 用户ID
     * @param roleId 角色ID
     */
    void removeUserRole(const std::string& userId, const std::string& roleId);
    
    /**
     * 获取用户权限列表
     * @param userId 用户ID
     * @return 权限列表
     */
    std::vector<UserPermission> getUserPermissions(const std::string& userId);
    
    /**
     * 检查用户权限
     * @param userId 用户ID
     * @param permission 权限名称
     * @return 是否有权限
     */
    bool hasPermission(const std::string& userId, const std::string& permission);
    
    /**
     * 获取用户偏好设置
     * @param userId 用户ID（可选，默认当前用户）
     * @return 偏好设置
     */
    UserPreferences getPreferences(const std::string& userId = "");
    
    /**
     * 更新用户偏好设置
     * @param preferences 偏好设置
     * @param userId 用户ID（可选，默认当前用户）
     * @return 更新后的偏好设置
     */
    UserPreferences updatePreferences(const UserPreferences& preferences,
                                     const std::string& userId = "");
    
    /**
     * 获取用户活动日志
     * @param userId 用户ID
     * @param page 页码
     * @param size 每页大小
     * @param startTime 开始时间（可选）
     * @param endTime 结束时间（可选）
     * @return 活动日志列表
     */
    PageResult<UserActivity> getActivities(const std::string& userId,
                                          int page = 1, int size = 20,
                                          const std::string& startTime = "",
                                          const std::string& endTime = "");
    
    /**
     * 获取用户登录历史
     * @param userId 用户ID
     * @param page 页码
     * @param size 每页大小
     * @return 登录历史列表
     */
    PageResult<LoginHistory> getLoginHistory(const std::string& userId,
                                            int page = 1, int size = 20);
    
    /**
     * 获取用户会话列表
     * @param userId 用户ID
     * @return 会话列表
     */
    std::vector<UserSession> getSessions(const std::string& userId);
    
    /**
     * 终止用户会话
     * @param userId 用户ID
     * @param sessionId 会话ID
     */
    void terminateSession(const std::string& userId, const std::string& sessionId);
    
    /**
     * 终止用户所有会话
     * @param userId 用户ID
     */
    void terminateAllSessions(const std::string& userId);
    
    /**
     * 获取用户统计信息
     * @param userId 用户ID
     * @return 统计信息
     */
    UserStatistics getStatistics(const std::string& userId);
    
    /**
     * 获取用户项目列表
     * @param userId 用户ID
     * @param page 页码
     * @param size 每页大小
     * @return 项目列表
     */
    PageResult<SysProject> getUserProjects(const std::string& userId,
                                          int page = 1, int size = 20);
    
    /**
     * 获取用户工作流列表
     * @param userId 用户ID
     * @param page 页码
     * @param size 每页大小
     * @return 工作流列表
     */
    PageResult<SysWorkflow> getUserWorkflows(const std::string& userId,
                                            int page = 1, int size = 20);
    
    /**
     * 上传用户头像
     * @param imageData 图像数据（base64编码）
     * @param userId 用户ID（可选，默认当前用户）
     * @return 头像URL
     */
    std::string uploadAvatar(const std::string& imageData,
                            const std::string& userId = "");
    
    /**
     * 删除用户头像
     * @param userId 用户ID（可选，默认当前用户）
     */
    void deleteAvatar(const std::string& userId = "");
    
    /**
     * 发送邮件验证
     * @param email 邮箱地址
     */
    void sendEmailVerification(const std::string& email);
    
    /**
     * 验证邮箱
     * @param token 验证令牌
     * @return 验证结果
     */
    bool verifyEmail(const std::string& token);
    
    /**
     * 发送密码重置邮件
     * @param email 邮箱地址
     */
    void sendPasswordResetEmail(const std::string& email);
    
    /**
     * 重置密码（通过令牌）
     * @param token 重置令牌
     * @param newPassword 新密码
     * @return 重置结果
     */
    bool resetPasswordWithToken(const std::string& token, 
                               const std::string& newPassword);
    
    /**
     * 启用两步验证
     * @param userId 用户ID（可选，默认当前用户）
     * @return 两步验证设置信息
     */
    TwoFactorAuthSetup enableTwoFactorAuth(const std::string& userId = "");
    
    /**
     * 禁用两步验证
     * @param code 验证码
     * @param userId 用户ID（可选，默认当前用户）
     */
    void disableTwoFactorAuth(const std::string& code, 
                             const std::string& userId = "");
    
    /**
     * 验证两步验证码
     * @param code 验证码
     * @param userId 用户ID（可选，默认当前用户）
     * @return 验证结果
     */
    bool verifyTwoFactorCode(const std::string& code, 
                            const std::string& userId = "");
    
    /**
     * 获取用户API密钥列表
     * @param userId 用户ID（可选，默认当前用户）
     * @return API密钥列表
     */
    std::vector<ApiKey> getApiKeys(const std::string& userId = "");
    
    /**
     * 创建API密钥
     * @param name 密钥名称
     * @param permissions 权限列表
     * @param expiresAt 过期时间（可选）
     * @param userId 用户ID（可选，默认当前用户）
     * @return 创建的API密钥
     */
    ApiKey createApiKey(const std::string& name,
                       const std::vector<std::string>& permissions,
                       const std::string& expiresAt = "",
                       const std::string& userId = "");
    
    /**
     * 删除API密钥
     * @param keyId 密钥ID
     * @param userId 用户ID（可选，默认当前用户）
     */
    void deleteApiKey(const std::string& keyId, const std::string& userId = "");
    
    /**
     * 刷新API密钥
     * @param keyId 密钥ID
     * @param userId 用户ID（可选，默认当前用户）
     * @return 新的API密钥
     */
    ApiKey refreshApiKey(const std::string& keyId, const std::string& userId = "");
};

} // namespace client
} // namespace dataapi