#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "dataapi/DataApiClient.h"
#include "dataapi/auth/AuthenticationProvider.h"
#include "dataapi/error/DataApiError.h"

using namespace dataapi;
using namespace dataapi::auth;
using namespace dataapi::client;
using namespace dataapi::error;

int main() {
    try {
        // 创建API密钥认证提供者
        auto authProvider = std::make_shared<ApiKeyAuthProvider>("your-api-key-here");
        
        // 创建开发环境客户端
        auto client = DataApiClient::createForDevelopment("your-api-key-here");
        
        std::cout << "DataAPI C++ SDK Basic Example" << std::endl;
        std::cout << "==============================" << std::endl;
        
        // 测试连接
        std::cout << "\n1. Testing connection..." << std::endl;
        if (client->testConnection()) {
            std::cout << "✓ Connection successful!" << std::endl;
        } else {
            std::cout << "✗ Connection failed!" << std::endl;
            return 1;
        }
        
        // 获取API版本
        std::cout << "\n2. Getting API version..." << std::endl;
        try {
            auto version = client->getVersion();
            std::cout << "✓ API Version: " << version.version << std::endl;
            std::cout << "  Build: " << version.build << std::endl;
            std::cout << "  Environment: " << version.environment << std::endl;
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get version: " << e.what() << std::endl;
        }
        
        // 获取健康状态
        std::cout << "\n3. Checking health status..." << std::endl;
        try {
            auto health = client->getHealth();
            std::cout << "✓ Health Status: " << health.status << std::endl;
            std::cout << "  Uptime: " << health.uptime << " seconds" << std::endl;
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get health: " << e.what() << std::endl;
        }
        
        // 获取当前用户信息
        std::cout << "\n4. Getting current user..." << std::endl;
        try {
            auto& userClient = client->getUserClient();
            auto user = userClient.getCurrentUser();
            std::cout << "✓ Current User: " << user.username << std::endl;
            std::cout << "  Email: " << user.email << std::endl;
            std::cout << "  Role: " << user.role << std::endl;
        } catch (const AuthenticationError& e) {
            std::cout << "✗ Authentication failed: " << e.what() << std::endl;
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get user: " << e.what() << std::endl;
        }
        
        // 获取项目列表
        std::cout << "\n5. Getting projects..." << std::endl;
        try {
            auto& projectClient = client->getProjectClient();
            auto projects = projectClient.list(1, 5); // 获取前5个项目
            std::cout << "✓ Found " << projects.total << " projects:" << std::endl;
            
            for (const auto& project : projects.items) {
                std::cout << "  - " << project.name << " (ID: " << project.id << ")" << std::endl;
                std::cout << "    Description: " << project.description << std::endl;
                std::cout << "    Created: " << project.createdAt << std::endl;
            }
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get projects: " << e.what() << std::endl;
        }
        
        // 获取工作流列表
        std::cout << "\n6. Getting workflows..." << std::endl;
        try {
            auto& workflowClient = client->getWorkflowClient();
            auto workflows = workflowClient.list(1, 5); // 获取前5个工作流
            std::cout << "✓ Found " << workflows.total << " workflows:" << std::endl;
            
            for (const auto& workflow : workflows.items) {
                std::cout << "  - " << workflow.name << " (ID: " << workflow.id << ")" << std::endl;
                std::cout << "    Status: " << workflow.status << std::endl;
                std::cout << "    Project: " << workflow.projectId << std::endl;
            }
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get workflows: " << e.what() << std::endl;
        }
        
        // 获取数据库列表
        std::cout << "\n7. Getting databases..." << std::endl;
        try {
            auto& databaseClient = client->getDatabaseClient();
            auto databases = databaseClient.list(1, 5); // 获取前5个数据库
            std::cout << "✓ Found " << databases.total << " databases:" << std::endl;
            
            for (const auto& database : databases.items) {
                std::cout << "  - " << database.name << " (" << database.type << ")" << std::endl;
                std::cout << "    Host: " << database.host << ":" << database.port << std::endl;
                std::cout << "    Status: " << database.status << std::endl;
            }
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get databases: " << e.what() << std::endl;
        }
        
        // 获取AI提供者列表
        std::cout << "\n8. Getting AI providers..." << std::endl;
        try {
            auto& aiClient = client->getAiProviderClient();
            auto providers = aiClient.list(1, 5); // 获取前5个AI提供者
            std::cout << "✓ Found " << providers.total << " AI providers:" << std::endl;
            
            for (const auto& provider : providers.items) {
                std::cout << "  - " << provider.name << " (" << provider.type << ")" << std::endl;
                std::cout << "    Status: " << provider.status << std::endl;
                std::cout << "    Models: " << provider.modelCount << std::endl;
            }
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to get AI providers: " << e.what() << std::endl;
        }
        
        // 演示错误处理
        std::cout << "\n9. Demonstrating error handling..." << std::endl;
        try {
            auto& projectClient = client->getProjectClient();
            // 尝试获取不存在的项目
            auto project = projectClient.getById("non-existent-id");
        } catch (const NotFoundError& e) {
            std::cout << "✓ Caught NotFoundError: " << e.what() << std::endl;
            std::cout << "  Resource Type: " << e.getResourceType() << std::endl;
            std::cout << "  Resource ID: " << e.getResourceId() << std::endl;
        } catch (const DataApiError& e) {
            std::cout << "✓ Caught DataApiError: " << e.what() << std::endl;
            std::cout << "  Error Code: " << e.getCode() << std::endl;
            std::cout << "  Status Code: " << e.getStatusCode() << std::endl;
        }
        
        // 演示文本生成（如果有AI提供者）
        std::cout << "\n10. Demonstrating AI text generation..." << std::endl;
        try {
            auto& aiClient = client->getAiProviderClient();
            auto providers = aiClient.list(1, 1);
            
            if (!providers.items.empty()) {
                const auto& provider = providers.items[0];
                std::cout << "Using AI provider: " << provider.name << std::endl;
                
                TextGenerationOptions options;
                options.maxTokens = 100;
                options.temperature = 0.7;
                
                auto result = aiClient.generateText(
                    provider.id,
                    "Hello, how are you today?",
                    options
                );
                
                std::cout << "✓ Generated text: " << result.text << std::endl;
                std::cout << "  Tokens used: " << result.tokensUsed << std::endl;
            } else {
                std::cout << "No AI providers available for text generation." << std::endl;
            }
        } catch (const DataApiError& e) {
            std::cout << "✗ Failed to generate text: " << e.what() << std::endl;
        }
        
        std::cout << "\n==============================" << std::endl;
        std::cout << "Example completed successfully!" << std::endl;
        
    } catch (const DataApiError& e) {
        std::cerr << "DataAPI Error: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getCode() << std::endl;
        std::cerr << "Status Code: " << e.getStatusCode() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}