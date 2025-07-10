package com.owiseman.dataapi.sdk.examples;

import com.owiseman.dataapi.sdk.DataApiClient;
import com.owiseman.dataapi.sdk.auth.BearerTokenAuthProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.model.SysWorkflow;
import com.owiseman.dataapi.sdk.model.WorkflowCreateRequest;
import com.owiseman.dataapi.sdk.model.PageResult;
import com.owiseman.dataapi.sdk.exception.DataApiException;

/**
 * Java SDK基础示例
 */
public class BasicExample {
    
    public static void main(String[] args) {
        try {
            // 创建认证提供者
            BearerTokenAuthProvider authProvider = new BearerTokenAuthProvider("your-access-token");
            
            // 创建客户端配置
            ClientConfig config = ClientConfig.development();
            
            // 创建DataAPI客户端
            DataApiClient client = new DataApiClient(config, authProvider);
            
            System.out.println("=== DataAPI Java SDK 示例 ===");
            
            // 测试连接
            System.out.println("测试连接...");
            boolean connected = client.testConnection();
            System.out.println("连接状态: " + (connected ? "成功" : "失败"));
            
            if (!connected) {
                System.out.println("无法连接到服务器，请检查配置");
                return;
            }
            
            // 获取API版本
            try {
                String version = client.getVersion();
                System.out.println("API版本: " + version);
            } catch (Exception e) {
                System.out.println("获取版本信息失败: " + e.getMessage());
            }
            
            // 工作流操作示例
            System.out.println("\n=== 工作流操作示例 ===");
            
            // 获取工作流列表
            try {
                PageResult<SysWorkflow> workflows = client.getWorkflowClient().getWorkflows(0, 10);
                System.out.println("工作流总数: " + workflows.getTotalElements());
                
                for (SysWorkflow workflow : workflows.getContent()) {
                    System.out.println("- " + workflow.getName() + " (ID: " + workflow.getId() + ")");
                }
            } catch (DataApiException e) {
                System.out.println("获取工作流列表失败: " + e.getMessage());
            }
            
            // 创建工作流示例
            try {
                WorkflowCreateRequest createRequest = new WorkflowCreateRequest(
                    "示例工作流",
                    "这是一个Java SDK创建的示例工作流",
                    "{\"nodes\":[],\"edges\":[]}",
                    "project-1",
                    "user-1"
                );
                
                SysWorkflow newWorkflow = client.getWorkflowClient().createWorkflow(createRequest);
                System.out.println("创建工作流成功: " + newWorkflow.getName() + " (ID: " + newWorkflow.getId() + ")");
                
                // 删除刚创建的工作流
                client.getWorkflowClient().deleteWorkflow(newWorkflow.getId());
                System.out.println("删除工作流成功");
                
            } catch (DataApiException e) {
                System.out.println("工作流操作失败: " + e.getMessage());
            }
            
            // 数据库操作示例
            System.out.println("\n=== 数据库操作示例 ===");
            
            try {
                // 测试数据库连接
                boolean dbConnected = client.getDatabaseClient().testConnection();
                System.out.println("数据库连接状态: " + (dbConnected ? "成功" : "失败"));
                
                if (dbConnected) {
                    // 获取表列表
                    // Object tables = client.getDatabaseClient().getTables();
                    // System.out.println("数据库表: " + tables);
                }
            } catch (Exception e) {
                System.out.println("数据库操作失败: " + e.getMessage());
            }
            
            // 用户操作示例
            System.out.println("\n=== 用户操作示例 ===");
            
            try {
                // Object currentUser = client.getUserClient().getCurrentUser();
                // System.out.println("当前用户: " + currentUser);
            } catch (Exception e) {
                System.out.println("获取用户信息失败: " + e.getMessage());
            }
            
            // 关闭客户端
            client.close();
            System.out.println("\n客户端已关闭");
            
        } catch (Exception e) {
            System.err.println("示例执行失败: " + e.getMessage());
            e.printStackTrace();
        }
    }
}