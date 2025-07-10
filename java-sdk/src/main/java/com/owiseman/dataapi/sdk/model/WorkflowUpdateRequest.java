package com.owiseman.dataapi.sdk.model;

/**
 * 工作流更新请求
 */
public class WorkflowUpdateRequest {
    private String id;
    private String name;
    private String description;
    private String definition;
    private String projectId;
    private String userId;
    private String updateAt;
    
    public WorkflowUpdateRequest() {}
    
    public WorkflowUpdateRequest(String id, String name, String description, String definition, 
                               String projectId, String userId, String updateAt) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.definition = definition;
        this.projectId = projectId;
        this.userId = userId;
        this.updateAt = updateAt;
    }
    
    // Getters and Setters
    public String getId() {
        return id;
    }
    
    public void setId(String id) {
        this.id = id;
    }
    
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
    
    public String getUpdateAt() {
        return updateAt;
    }
    
    public void setUpdateAt(String updateAt) {
        this.updateAt = updateAt;
    }
}