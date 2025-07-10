package com.owiseman.dataapi.sdk.model;

/**
 * 工作流执行请求
 */
public class WorkflowExecutionRequest {
    private String workflowId;
    private String projectId;
    private String userId;
    private String workflowDefinition;
    private Object initialData;
    private Integer timeoutMinutes;
    
    public WorkflowExecutionRequest() {}
    
    public WorkflowExecutionRequest(String workflowId, String projectId, String userId, 
                                  String workflowDefinition, Object initialData, Integer timeoutMinutes) {
        this.workflowId = workflowId;
        this.projectId = projectId;
        this.userId = userId;
        this.workflowDefinition = workflowDefinition;
        this.initialData = initialData;
        this.timeoutMinutes = timeoutMinutes;
    }
    
    // Getters and Setters
    public String getWorkflowId() {
        return workflowId;
    }
    
    public void setWorkflowId(String workflowId) {
        this.workflowId = workflowId;
    }
    
    public String getProjectId() {
        return projectId;
    }
    
    public void setProjectId(String projectId) {
        this.projectId = projectId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public void setUserId(String userId) {
        this.userId = userId;
    }
    
    public String getWorkflowDefinition() {
        return workflowDefinition;
    }
    
    public void setWorkflowDefinition(String workflowDefinition) {
        this.workflowDefinition = workflowDefinition;
    }
    
    public Object getInitialData() {
        return initialData;
    }
    
    public void setInitialData(Object initialData) {
        this.initialData = initialData;
    }
    
    public Integer getTimeoutMinutes() {
        return timeoutMinutes;
    }
    
    public void setTimeoutMinutes(Integer timeoutMinutes) {
        this.timeoutMinutes = timeoutMinutes;
    }
}