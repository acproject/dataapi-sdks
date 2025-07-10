package com.owiseman.dataapi.sdk.model;

/**
 * 工作流创建请求
 */
public class WorkflowCreateRequest {
    private String name;
    private String description;
    private String definition;
    private String projectId;
    private String userId;
    
    public WorkflowCreateRequest() {}
    
    public WorkflowCreateRequest(String name, String description, String definition, 
                               String projectId, String userId) {
        this.name = name;
        this.description = description;
        this.definition = definition;
        this.projectId = projectId;
        this.userId = userId;
    }
    
    // Getters and Setters
    public String getName() {
        return name;
    }
    
    public void setName(String name) {
        this.name = name;
    }
    
    public String getDescription() {
        return description;
    }
    
    public void setDescription(String description) {
        this.description = description;
    }
    
    public String getDefinition() {
        return definition;
    }
    
    public void setDefinition(String definition) {
        this.definition = definition;
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
}