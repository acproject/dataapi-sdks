#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "dataapi/DataApiClient.h"
#include "dataapi/auth/BearerTokenAuthProvider.h"
#include "dataapi/config/ClientConfig.h"
#include "dataapi/Types.h"
#include "dataapi/exceptions/DataApiException.h"

using namespace dataapi;

/**
 * C++ SDK基础示例
 */
int main() {
    try {
        std::cout << "=== DataAPI C++ SDK 示例 ===" << std::endl;
        
        // 创建认证提供者
        auto authProvider = std::make_shared<BearerTokenAuthProvider>("your-access-token");
        
        // 创建客户端配置
        auto config = ClientConfig::development();
        
        // 创建DataAPI客户端
        auto client = std::make_unique<DataApiClient>(config, authProvider);
        
        // 测试连接
        std::cout << "测试连接..." << std::endl;
        try {
            bool connected = client->testConnection();
            std::cout << "连接状态: " << (connected ? "成功" : "失败") << std::endl;
            
            if (!connected) {
                std::cout << "无法连接到服务器，请检查配置" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cout << "连接测试失败: " << e.what() << std::endl;
            return 1;
        }
        
        // 获取API版本
        try {
            std::string version = client->getVersion();
            std::cout << "API版本: " << version << std::endl;
        } catch (const std::exception& e) {
            std::cout << "获取版本信息失败: " << e.what() << std::endl;
        }
        
        // 获取健康状态
        try {
            Json health = client->getHealth();
            std::cout << "健康状态: " << health.dump() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "获取健康状态失败: " << e.what() << std::endl;
        }
        
        // 工作流操作示例
        std::cout << "\n=== 工作流操作示例 ===" << std::endl;
        
        auto workflowClient = client->getWorkflowClient();
        
        // 获取工作流列表
        try {
            PageResult<SysWorkflow> workflows = workflowClient->getWorkflows(0, 10);
            std::cout << "工作流总数: " << workflows.totalElements << std::endl;
            
            for (const auto& workflow : workflows.content) {
                std::cout << "- " << workflow.name << " (ID: " << workflow.id.value_or("N/A") << ")" << std::endl;
            }
        } catch (const DataApiException& e) {
            std::cout << "获取工作流列表失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "获取工作流列表失败: " << e.what() << std::endl;
        }
        
        // 创建工作流示例
        try {
            WorkflowCreateRequest createRequest;
            createRequest.name = "示例工作流";
            createRequest.description = "这是一个C++ SDK创建的示例工作流";
            createRequest.definition = "{\"nodes\":[],\"edges\":[]}";
            createRequest.projectId = "project-1";
            createRequest.userId = "user-1";
            
            SysWorkflow newWorkflow = workflowClient->createWorkflow(createRequest);
            std::cout << "创建工作流成功: " << newWorkflow.name << " (ID: " << newWorkflow.id.value_or("N/A") << ")" << std::endl;
            
            // 获取工作流详情
            if (newWorkflow.id.has_value()) {
                SysWorkflow workflowDetail = workflowClient->getWorkflow(newWorkflow.id.value());
                std::cout << "工作流详情: " << workflowDetail.name;
                if (workflowDetail.status.has_value()) {
                    std::cout << " 状态: " << static_cast<int>(workflowDetail.status.value());
                }
                std::cout << std::endl;
                
                // 删除刚创建的工作流
                workflowClient->deleteWorkflow(newWorkflow.id.value());
                std::cout << "删除工作流成功" << std::endl;
            }
            
        } catch (const DataApiException& e) {
            std::cout << "工作流操作失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "工作流操作失败: " << e.what() << std::endl;
        }
        
        // 数据库操作示例
        std::cout << "\n=== 数据库操作示例 ===" << std::endl;
        
        auto databaseClient = client->getDatabaseClient();
        
        try {
            // 测试数据库连接
            bool dbConnected = databaseClient->testConnection();
            std::cout << "数据库连接状态: " << (dbConnected ? "成功" : "失败") << std::endl;
            
            if (dbConnected) {
                // 获取连接信息
                Json connectionInfo = databaseClient->getConnectionInfo();
                std::cout << "数据库连接信息: " << connectionInfo.dump() << std::endl;
                
                // 获取表列表
                std::vector<std::string> tables = databaseClient->getTables();
                std::cout << "数据库表数量: " << tables.size() << std::endl;
                
                // 显示前几个表名
                for (size_t i = 0; i < std::min(tables.size(), size_t(5)); ++i) {
                    std::cout << "  - " << tables[i] << std::endl;
                }
            }
        } catch (const DataApiException& e) {
            std::cout << "数据库操作失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "数据库操作失败: " << e.what() << std::endl;
        }
        
        // 项目操作示例
        std::cout << "\n=== 项目操作示例 ===" << std::endl;
        
        auto projectClient = client->getProjectClient();
        
        try {
            PageResult<Project> projects = projectClient->getProjects(0, 10);
            std::cout << "项目总数: " << projects.totalElements << std::endl;
            
            for (const auto& project : projects.content) {
                std::cout << "- " << project.name << " (ID: " << project.id << ")" << std::endl;
            }
        } catch (const DataApiException& e) {
            std::cout << "获取项目列表失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "获取项目列表失败: " << e.what() << std::endl;
        }
        
        // AI提供者操作示例
        std::cout << "\n=== AI提供者操作示例 ===" << std::endl;
        
        auto aiClient = client->getAiProviderClient();
        
        try {
            // 测试AI配置
            Json testResult = aiClient->testAiConfig("config-1");
            std::cout << "AI配置测试结果: " << testResult.dump() << std::endl;
        } catch (const DataApiException& e) {
            std::cout << "AI操作失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "AI操作失败: " << e.what() << std::endl;
        }
        
        // 用户操作示例
        std::cout << "\n=== 用户操作示例 ===" << std::endl;
        
        auto userClient = client->getUserClient();
        
        try {
            User currentUser = userClient->getCurrentUser();
            std::cout << "当前用户: " << currentUser.username << " 邮箱: " << currentUser.email << std::endl;
        } catch (const DataApiException& e) {
            std::cout << "获取用户信息失败: " << e.what() << " 状态码: " << e.getStatusCode() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "获取用户信息失败: " << e.what() << std::endl;
        }
        
        // 关闭客户端
        client->close();
        std::cout << "\n客户端已关闭" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "示例执行失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}