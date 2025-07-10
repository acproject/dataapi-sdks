#pragma once

#include "CommonTypes.h"

namespace dataapi {

// Forward declarations
struct WorkflowExecutionResult;
struct WorkflowExecutionStatus;
struct WorkflowValidationResult;
struct WorkflowStatistics;
struct WorkflowVersion;
struct WorkflowVersionRequest;

/**
 * 工作流状态枚举
 */
enum class WorkflowStatus {
    DRAFT,
    ACTIVE,
    INACTIVE,
    ARCHIVED
};

/**
 * 执行状态枚举
 */
enum class ExecutionStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

// Workflow Execution Result
struct WorkflowExecutionResult {
    std::string executionId;
    std::string status;
    Json result;
    Timestamp startTime;
    Timestamp endTime;
    Json metadata;
};

// Workflow Execution Status
struct WorkflowExecutionStatus {
    std::string executionId;
    std::string status;
    double progress;
    Json currentStep;
};

// Workflow Validation Result
struct WorkflowValidationResult {
    bool isValid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

// Workflow Statistics
struct WorkflowStatistics {
    int totalExecutions;
    int successfulExecutions;
    int failedExecutions;
    double averageExecutionTime;
};

// Workflow Version
struct WorkflowVersion {
    std::string id;
    int version;
    Json definition;
    Timestamp createdAt;
};

// Workflow Version Request
struct WorkflowVersionRequest {
    Json definition;
    std::string description;
};

/**
 * 工作流结构
 */
struct SysWorkflow {
    std::optional<std::string> id;
    std::string name;
    std::optional<std::string> description;
    std::string definition;
    std::string projectId;
    std::string userId;
    std::optional<std::string> createTime;
    std::optional<std::string> updateTime;
    std::optional<WorkflowStatus> status;
    std::optional<int> version;
};

/**
 * 工作流创建请求
 */
struct WorkflowCreateRequest {
    std::string name;
    std::optional<std::string> description;
    std::string definition;
    std::string projectId;
    std::string userId;
};

/**
 * 工作流更新请求
 */
struct WorkflowUpdateRequest {
    std::string id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> definition;
    std::optional<std::string> projectId;
    std::optional<std::string> userId;
    std::optional<std::string> updateTime;
};

/**
 * 工作流执行请求
 */
struct WorkflowExecutionRequest {
    std::string workflowId;
    std::string projectId;
    std::string userId;
    std::string workflowDefinition;
    std::optional<Json> initialData;
    std::optional<int> timeoutMinutes;
};

// JSON序列化/反序列化函数声明
void to_json(Json& j, const SysWorkflow& w);
void from_json(const Json& j, SysWorkflow& w);

void to_json(Json& j, const WorkflowCreateRequest& w);
void from_json(const Json& j, WorkflowCreateRequest& w);

void to_json(Json& j, const WorkflowUpdateRequest& w);
void from_json(const Json& j, WorkflowUpdateRequest& w);

void to_json(Json& j, const WorkflowExecutionRequest& w);
void from_json(const Json& j, WorkflowExecutionRequest& w);

void to_json(Json& j, const WorkflowExecutionResult& w);
void from_json(const Json& j, WorkflowExecutionResult& w);

} // namespace dataapi