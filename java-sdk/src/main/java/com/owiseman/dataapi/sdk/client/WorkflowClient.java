package com.owiseman.dataapi.sdk.client;

import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;
import com.owiseman.dataapi.sdk.model.*;
import com.owiseman.dataapi.sdk.http.HttpClient;
import com.fasterxml.jackson.core.type.TypeReference;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 工作流客户端
 */
public class WorkflowClient extends BaseClient {
    
    public WorkflowClient(ClientConfig config, AuthenticationProvider authProvider) {
        super(config, authProvider);
    }
    
    /**
     * 获取所有工作流
     */
    public List<SysWorkflow> getAllWorkflows(String projectId, String userId) throws DataApiException {
        Map<String, String> params = new HashMap<>();
        params.put("projectId", projectId);
        params.put("userId", userId);
        
        return httpClient.get("/api/workflows/findAll", params, new TypeReference<List<SysWorkflow>>() {});
    }
    
    /**
     * 分页获取工作流
     */
    public PageResult<SysWorkflow> getWorkflowsPage(String projectId, String userId, int page, int size) throws DataApiException {
        Map<String, String> params = new HashMap<>();
        params.put("projectId", projectId);
        params.put("userId", userId);
        params.put("page", String.valueOf(page));
        params.put("size", String.valueOf(size));
        
        return httpClient.get("/api/workflows/page", params, new TypeReference<PageResult<SysWorkflow>>() {});
    }
    
    /**
     * 根据ID获取工作流
     */
    public SysWorkflow getWorkflowById(String id) throws DataApiException {
        return httpClient.get("/api/workflows/" + id, null, SysWorkflow.class);
    }
    
    /**
     * 创建工作流
     */
    public SysWorkflow createWorkflow(WorkflowCreateRequest request) throws DataApiException {
        return httpClient.post("/api/workflows/create", request, SysWorkflow.class);
    }
    
    /**
     * 更新工作流
     */
    public SysWorkflow updateWorkflow(WorkflowUpdateRequest request) throws DataApiException {
        return httpClient.put("/api/workflows/update", request, SysWorkflow.class);
    }
    
    /**
     * 删除工作流
     */
    public void deleteWorkflow(String id) throws DataApiException {
        httpClient.delete("/api/workflows/" + id);
    }
    
    /**
     * 检查工作流是否存在
     */
    public boolean workflowExists(String id) throws DataApiException {
        try {
            httpClient.head("/api/workflows/" + id);
            return true;
        } catch (DataApiException e) {
            if (e.getStatusCode() == 404) {
                return false;
            }
            throw e;
        }
    }
    
    /**
     * 执行工作流
     */
    public WorkflowExecutionResult executeWorkflow(WorkflowExecutionRequest request) throws DataApiException {
        return httpClient.post("/api/workflow/execute", request, WorkflowExecutionResult.class);
    }
    
    /**
     * 获取工作流执行状态
     */
    public WorkflowExecutionStatus getExecutionStatus(String taskId) throws DataApiException {
        return httpClient.get("/api/workflow/status/" + taskId, null, WorkflowExecutionStatus.class);
    }
    
    /**
     * 停止工作流执行
     */
    public void stopExecution(String taskId) throws DataApiException {
        httpClient.post("/api/workflow/stop/" + taskId, null, Void.class);
    }
    
    /**
     * 获取工作流执行日志
     */
    public List<WorkflowExecutionLog> getExecutionLogs(String taskId) throws DataApiException {
        return httpClient.get("/api/workflow/logs/" + taskId, null, new TypeReference<List<WorkflowExecutionLog>>() {});
    }
}