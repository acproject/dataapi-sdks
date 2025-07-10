#pragma once

#include <memory>
#include <string>
#include "ClientConfig.h"
#include "http/HttpClient.h"
#include "auth/AuthenticationProvider.h"
#include "client/WorkflowClient.h"
#include "client/ProjectClient.h"
#include "client/DatabaseClient.h"
#include "client/AiProviderClient.h"
#include "client/UserClient.h"
#include "Types.h"

namespace dataapi {

/**
 * DataAPI C++ SDK主客户端类
 */
class DataApiClient {
public:
    /**
     * 构造函数
     * @param config 客户端配置
     * @param authProvider 认证提供者
     */
    DataApiClient(const ClientConfig& config, std::shared_ptr<auth::AuthenticationProvider> authProvider);
    
    /**
     * 析构函数
     */
    ~DataApiClient();
    
    // 禁用拷贝构造和赋值
    DataApiClient(const DataApiClient&) = delete;
    DataApiClient& operator=(const DataApiClient&) = delete;
    
    // 启用移动构造和赋值
    DataApiClient(DataApiClient&&) = default;
    DataApiClient& operator=(DataApiClient&&) = default;
    
    /**
     * 获取工作流客户端
     */
    client::WorkflowClient& getWorkflowClient();
    
    /**
     * 获取项目客户端
     */
    client::ProjectClient& getProjectClient();
    
    /**
     * 获取数据库客户端
     */
    client::DatabaseClient& getDatabaseClient();
    
    /**
     * 获取AI提供者客户端
     */
    client::AiProviderClient& getAiProviderClient();
    
    /**
     * 获取用户客户端
     */
    client::UserClient& getUserClient();
    
    /**
     * 测试连接
     */
    bool testConnection();
    
    /**
     * 获取API版本
     */
    ApiVersion getVersion();
    
    /**
     * 获取健康状态
     */
    HealthStatus getHealth();
    
    /**
     * 获取配置
     */
    const ClientConfig& getConfig() const;
    
    /**
     * 获取认证提供者
     */
    std::shared_ptr<auth::AuthenticationProvider> getAuthProvider() const;
    
    /**
     * 获取HTTP客户端
     */
    std::shared_ptr<http::HttpClient> getHttpClient();
    
    /**
     * 更新配置
     */
    void updateConfig(const ClientConfig& newConfig);
    
    /**
     * 关闭客户端
     */
    void close();
    
    /**
     * 创建开发环境客户端
     */
    static std::unique_ptr<DataApiClient> createForDevelopment(
        const std::string& apiKey,
        const std::string& baseUrl = "https://dev-api.dataapi.com"
    );
    
    /**
     * 创建生产环境客户端
     */
    static std::unique_ptr<DataApiClient> createForProduction(
        const std::string& apiKey,
        const std::string& baseUrl = "https://api.dataapi.com"
    );
    
private:
    ClientConfig config_;
    std::shared_ptr<auth::AuthenticationProvider> authProvider_;
    std::shared_ptr<http::HttpClient> httpClient_;
    
    // 各种服务客户端
    std::unique_ptr<client::WorkflowClient> workflowClient_;
    std::unique_ptr<client::ProjectClient> projectClient_;
    std::unique_ptr<client::DatabaseClient> databaseClient_;
    std::unique_ptr<client::AiProviderClient> aiProviderClient_;
    std::unique_ptr<client::UserClient> userClient_;
    
    /**
     * 初始化客户端
     */
    void initialize();
};

} // namespace dataapi