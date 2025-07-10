#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Types.h"
#include "../http/HttpClient.h"

namespace dataapi {
namespace client {

/**
 * 项目客户端类
 * 提供项目相关的API操作
 */
class ProjectClient {
private:
    std::shared_ptr<http::HttpClient> httpClient;
    
public:
    /**
     * 构造函数
     * @param httpClient HTTP客户端
     */
    explicit ProjectClient(std::shared_ptr<http::HttpClient> httpClient);
    
    /**
     * 析构函数
     */
    ~ProjectClient() = default;
    
    /**
     * 禁用拷贝构造和赋值
     */
    ProjectClient(const ProjectClient&) = delete;
    ProjectClient& operator=(const ProjectClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    ProjectClient(ProjectClient&& other) noexcept = default;
    ProjectClient& operator=(ProjectClient&& other) noexcept = default;
    
    /**
     * 获取项目列表
     * @param page 页码（从1开始）
     * @param size 每页大小
     * @param userId 用户ID（可选）
     * @return 项目列表
     */
    PageResult<SysProject> list(int page = 1, int size = 20, 
                               const std::string& userId = "");
    
    /**
     * 根据ID获取项目详情
     * @param id 项目ID
     * @return 项目详情
     */
    SysProject getById(const std::string& id);
    
    /**
     * 创建新项目
     * @param request 项目创建请求
     * @return 创建的项目
     */
    SysProject create(const ProjectCreateRequest& request);
    
    /**
     * 更新项目
     * @param id 项目ID
     * @param request 项目更新请求
     * @return 更新后的项目
     */
    SysProject update(const std::string& id, const ProjectUpdateRequest& request);
    
    /**
     * 删除项目
     * @param id 项目ID
     */
    void deleteProject(const std::string& id);
    
    /**
     * 获取项目成员列表
     * @param projectId 项目ID
     * @param page 页码
     * @param size 每页大小
     * @return 成员列表
     */
    PageResult<ProjectMember> getMembers(const std::string& projectId, 
                                        int page = 1, int size = 20);
    
    /**
     * 添加项目成员
     * @param projectId 项目ID
     * @param request 成员添加请求
     * @return 添加的成员
     */
    ProjectMember addMember(const std::string& projectId, 
                           const ProjectMemberRequest& request);
    
    /**
     * 更新项目成员角色
     * @param projectId 项目ID
     * @param userId 用户ID
     * @param role 新角色
     * @return 更新后的成员
     */
    ProjectMember updateMemberRole(const std::string& projectId, 
                                  const std::string& userId,
                                  const std::string& role);
    
    /**
     * 移除项目成员
     * @param projectId 项目ID
     * @param userId 用户ID
     */
    void removeMember(const std::string& projectId, const std::string& userId);
    
    /**
     * 获取项目权限列表
     * @param projectId 项目ID
     * @return 权限列表
     */
    std::vector<ProjectPermission> getPermissions(const std::string& projectId);
    
    /**
     * 设置项目权限
     * @param projectId 项目ID
     * @param permissions 权限列表
     */
    void setPermissions(const std::string& projectId, 
                       const std::vector<ProjectPermission>& permissions);
    
    /**
     * 检查用户在项目中的权限
     * @param projectId 项目ID
     * @param userId 用户ID
     * @param permission 权限名称
     * @return 是否有权限
     */
    bool hasPermission(const std::string& projectId, 
                      const std::string& userId,
                      const std::string& permission);
    
    /**
     * 获取项目配置
     * @param projectId 项目ID
     * @return 项目配置
     */
    ProjectConfig getConfig(const std::string& projectId);
    
    /**
     * 更新项目配置
     * @param projectId 项目ID
     * @param config 新配置
     * @return 更新后的配置
     */
    ProjectConfig updateConfig(const std::string& projectId, 
                              const ProjectConfig& config);
    
    /**
     * 获取项目统计信息
     * @param projectId 项目ID
     * @return 统计信息
     */
    ProjectStatistics getStatistics(const std::string& projectId);
    
    /**
     * 获取项目活动日志
     * @param projectId 项目ID
     * @param page 页码
     * @param size 每页大小
     * @param startTime 开始时间（可选）
     * @param endTime 结束时间（可选）
     * @return 活动日志列表
     */
    PageResult<ProjectActivity> getActivities(const std::string& projectId,
                                             int page = 1, int size = 20,
                                             const std::string& startTime = "",
                                             const std::string& endTime = "");
    
    /**
     * 归档项目
     * @param projectId 项目ID
     */
    void archive(const std::string& projectId);
    
    /**
     * 恢复归档的项目
     * @param projectId 项目ID
     */
    void unarchive(const std::string& projectId);
    
    /**
     * 导出项目数据
     * @param projectId 项目ID
     * @param format 导出格式（json, csv等）
     * @return 导出的数据
     */
    std::string exportData(const std::string& projectId, 
                          const std::string& format = "json");
    
    /**
     * 导入项目数据
     * @param projectId 项目ID
     * @param data 导入的数据
     * @param format 数据格式
     * @return 导入结果
     */
    ImportResult importData(const std::string& projectId,
                           const std::string& data,
                           const std::string& format = "json");
    
    /**
     * 复制项目
     * @param sourceProjectId 源项目ID
     * @param name 新项目名称
     * @param description 新项目描述
     * @param copyMembers 是否复制成员
     * @return 复制的项目
     */
    SysProject clone(const std::string& sourceProjectId,
                    const std::string& name,
                    const std::string& description = "",
                    bool copyMembers = false);
    
    /**
     * 获取项目模板列表
     * @param page 页码
     * @param size 每页大小
     * @return 模板列表
     */
    PageResult<ProjectTemplate> getTemplates(int page = 1, int size = 20);
    
    /**
     * 从模板创建项目
     * @param templateId 模板ID
     * @param request 项目创建请求
     * @return 创建的项目
     */
    SysProject createFromTemplate(const std::string& templateId,
                                 const ProjectCreateRequest& request);
};

} // namespace client
} // namespace dataapi