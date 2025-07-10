#include <dataapi/DataApiClient.h>
#include <dataapi/auth/AuthenticationProvider.h>
#include <dataapi/error/DataApiError.h>
#include <dataapi/Types.h>
#include <iostream>
#include <vector>

using namespace dataapi;
using namespace dataapi::auth;
using namespace dataapi::error;

int main() {
    try {
        // 创建API密钥认证提供者
        auto authProvider = std::make_shared<auth::ApiKeyAuthProvider>("your-api-key-here");
        
        // 创建开发环境客户端
        auto client = DataApiClient::createForDevelopment("your-api-key-here");
        
        std::cout << "=== DataAPI C++ SDK 基础示例 ===" << std::endl;
        
        // 测试连接
        std::cout << "\n1. 测试连接..." << std::endl;
        if (client->testConnection()) {
            std::cout << "✓ 连接成功!" << std::endl;
        } else {
            std::cout << "✗ 连接失败!" << std::endl;
            return 1;
        }
        
        // 获取API版本信息
        std::cout << "\n2. 获取API版本信息..." << std::endl;
        auto version = client->getVersion();
        std::cout << "API版本: " << version.version << std::endl;
        std::cout << "构建时间: " << version.buildTime << std::endl;
        std::cout << "Git提交: " << version.gitCommit << std::endl;
        
        // 获取健康状态
        std::cout << "\n3. 获取健康状态..." << std::endl;
        auto health = client->getHealth();
        std::cout << "状态: " << health.status << std::endl;
        std::cout << "消息: " << health.message << std::endl;
        
        // 获取当前用户信息
        std::cout << "\n4. 获取当前用户信息..." << std::endl;
        auto userClient = client->getUserClient();
        auto currentUser = userClient->getCurrentUser();
        std::cout << "用户ID: " << currentUser.id << std::endl;
        std::cout << "用户名: " << currentUser.username << std::endl;
        std::cout << "邮箱: " << currentUser.email << std::endl;
        if (currentUser.displayName) {
            std::cout << "显示名称: " << *currentUser.displayName << std::endl;
        }
        
        // 列出项目
        std::cout << "\n5. 列出项目..." << std::endl;
        auto projectClient = client->getProjectClient();
        auto projects = projectClient->listProjects();
        std::cout << "项目数量: " << projects.size() << std::endl;
        
        for (const auto& project : projects) {
            std::cout << "  - " << project.name;
            if (project.description) {
                std::cout << " (" << *project.description << ")";
            }
            std::cout << std::endl;
        }
        
        // 列出工作流
        std::cout << "\n6. 列出工作流..." << std::endl;
        auto workflowClient = client->getWorkflowClient();
        auto workflows = workflowClient->listWorkflows();
        std::cout << "工作流数量: " << workflows.size() << std::endl;
        
        for (const auto& workflow : workflows) {
            std::cout << "  - " << workflow.name;
            if (workflow.description) {
                std::cout << " (" << *workflow.description << ")";
            }
            std::cout << std::endl;
        }
        
        // 列出数据库
        std::cout << "\n7. 列出数据库..." << std::endl;
        auto databaseClient = client->getDatabaseClient();
        auto databases = databaseClient->listDatabases();
        std::cout << "数据库数量: " << databases.size() << std::endl;
        
        for (const auto& db : databases) {
            std::cout << "  - " << db.name << " (" << db.type << ")" << std::endl;
        }
        
        // 列出AI提供商
        std::cout << "\n8. 列出AI提供商..." << std::endl;
        auto aiClient = client->getAiProviderClient();
        auto providers = aiClient->listProviders();
        std::cout << "AI提供商数量: " << providers.size() << std::endl;
        
        for (const auto& provider : providers) {
            std::cout << "  - " << provider.name << " (" << provider.type << ")" << std::endl;
        }
        
        // 演示错误处理
        std::cout << "\n9. 演示错误处理..." << std::endl;
        try {
            // 尝试获取不存在的工作流
            auto nonExistentWorkflow = workflowClient->getWorkflowById("non-existent-id");
        } catch (const error::NotFoundError& e) {
            std::cout << "✓ 正确捕获了NotFoundError: " << e.what() << std::endl;
        }
        
        // AI文本生成示例
        std::cout << "\n10. AI文本生成示例..." << std::endl;
        if (!providers.empty()) {
            try {
                auto response = aiClient->generateText(
                    providers[0].id,
                    "请用一句话介绍人工智能",
                    Json{}
                );
                std::cout << "AI生成的文本: " << response.output.dump() << std::endl;
            } catch (const error::DataApiError& e) {
                std::cout << "AI文本生成失败: " << e.what() << std::endl;
            }
        } else {
            std::cout << "没有可用的AI提供商" << std::endl;
        }
        
        std::cout << "\n=== 示例完成 ===" << std::endl;
        
    } catch (const error::AuthenticationError& e) {
        std::cerr << "认证错误: " << e.what() << std::endl;
        std::cerr << "请检查您的API密钥是否正确" << std::endl;
        return 1;
    } catch (const error::NetworkError& e) {
        std::cerr << "网络错误: " << e.what() << std::endl;
        std::cerr << "请检查网络连接" << std::endl;
        return 1;
    } catch (const error::DataApiError& e) {
        std::cerr << "API错误: " << e.what() << std::endl;
        std::cerr << "错误代码: " << e.getCode() << std::endl;
        std::cerr << "HTTP状态码: " << e.getStatusCode() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}