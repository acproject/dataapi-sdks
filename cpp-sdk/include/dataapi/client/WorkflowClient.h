#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Types.h"
#include "../http/HttpClient.h"

namespace dataapi {
namespace client {

/**
 * 工作流客户端类
 * 提供工作流相关的API操作
 */
class WorkflowClient {
private:
    std::shared_ptr<http::HttpClient> httpClient;
    
public:
    /**
     * 构造函数
     * @param httpClient HTTP客户端
     */
    explicit WorkflowClient(std::shared_ptr<http::HttpClient> httpClient);
    
    /**
     * 析构函数
     */
    ~WorkflowClient() = default;
    
    /**
     * 禁用拷贝构造和赋值
     */
    WorkflowClient(const WorkflowClient&) = delete;
    WorkflowClient& operator=(const WorkflowClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    WorkflowClient(WorkflowClient&& other) noexcept = default;
    WorkflowClient& operator=(WorkflowClient&& other) noexcept = default;
    
    /**
     * 获取工作流列表
     * @param page 页码（从1开始）
     * @param size 每页大小
     * @param projectId 项目ID（可选）
     * @param userId 用户ID（可选）
     * @return 工作流列表
     */
    PageResult<SysWorkflow> list(int page = 1, int size = 20, 
                                const std::string& projectId = "",
                                const std::string& userId = "");
    
    /**
     * 根据ID获取工作流详情
     * @param id 工作流ID
     * @return 工作流详情
     */
    SysWorkflow getById(const std::string& id);
    
    /**
     * 创建新工作流
     * @param request 工作流创建请求
     * @return 创建的工作流
     */
    SysWorkflow create(const WorkflowCreateRequest& request);
    
    /**
     * 更新工作流
     * @param id 工作流ID
     * @param request 工作流更新请求
     * @return 更新后的工作流
     */
    SysWorkflow update(const std::string& id, const WorkflowUpdateRequest& request);
    
    /**
     * 删除工作流
     * @param id 工作流ID
     */
    void deleteWorkflow(const std::string& id);
    
    /**
     * 执行工作流
     * @param id 工作流ID
     * @param input 输入参数
     * @return 执行结果
     */
    WorkflowExecutionResult execute(const std::string& id, const Json& input = {});
    
    /**
     * 异步执行工作流
     * @param id 工作流ID
     * @param input 输入参数
     * @return 执行ID
     */
    std::string executeAsync(const std::string& id, const Json& input = {});
    
    /**
     * 获取工作流执行状态
     * @param executionId 执行ID
     * @return 执行状态
     */
    WorkflowExecutionStatus getExecutionStatus(const std::string& executionId);
    
    /**
     * 获取工作流执行结果
     * @param executionId 执行ID
     * @return 执行结果
     */
    WorkflowExecutionResult getExecutionResult(const std::string& executionId);
    
    /**
     * 停止工作流执行
     * @param executionId 执行ID
     */
    void stopExecution(const std::string& executionId);
    
    /**
     * 获取工作流执行历史
     * @param workflowId 工作流ID
     * @param page 页码
     * @param size 每页大小
     * @return 执行历史列表
     */
    PageResult<WorkflowExecutionResult> getExecutionHistory(
        const std::string& workflowId, int page = 1, int size = 20);
    
    /**
     * 验证工作流定义
     * @param definition 工作流定义
     * @return 验证结果
     */
    WorkflowValidationResult validate(const Json& definition);
    
    /**
     * 导出工作流
     * @param id 工作流ID
     * @return 工作流定义JSON
     */
    Json exportWorkflow(const std::string& id);
    
    /**
     * 导入工作流
     * @param definition 工作流定义
     * @param name 工作流名称
     * @param description 工作流描述
     * @return 导入的工作流
     */
    SysWorkflow importWorkflow(const Json& definition, 
                              const std::string& name,
                              const std::string& description = "");
    
    /**
     * 复制工作流
     * @param id 源工作流ID
     * @param name 新工作流名称
     * @param description 新工作流描述
     * @return 复制的工作流
     */
    SysWorkflow clone(const std::string& id, 
                     const std::string& name,
                     const std::string& description = "");
    
    /**
     * 获取工作流统计信息
     * @param id 工作流ID
     * @return 统计信息
     */
    WorkflowStatistics getStatistics(const std::string& id);
    
    /**
     * 获取工作流版本列表
     * @param workflowId 工作流ID
     * @param page 页码
     * @param size 每页大小
     * @return 版本列表
     */
    PageResult<WorkflowVersion> getVersions(const std::string& workflowId, 
                                           int page = 1, int size = 20);
    
    /**
     * 创建工作流版本
     * @param workflowId 工作流ID
     * @param version 版本信息
     * @return 创建的版本
     */
    WorkflowVersion createVersion(const std::string& workflowId, 
                                 const WorkflowVersionRequest& version);
    
    /**
     * 发布工作流版本
     * @param workflowId 工作流ID
     * @param versionId 版本ID
     */
    void publishVersion(const std::string& workflowId, const std::string& versionId);
    
    /**
     * 回滚到指定版本
     * @param workflowId 工作流ID
     * @param versionId 版本ID
     */
    void rollbackToVersion(const std::string& workflowId, const std::string& versionId);
};

} // namespace client
} // namespace dataapi