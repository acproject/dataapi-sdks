# DataAPI C++ SDK

DataAPI的C++客户端SDK，提供了完整的API访问功能。

## 特性

- 🚀 现代C++17设计
- 🔐 多种认证方式支持（Bearer Token、API Key、Basic Auth）
- 🌐 完整的HTTP客户端功能
- 📊 工作流管理
- 🗄️ 数据库操作
- 🤖 AI服务集成
- 👥 用户和项目管理
- 🛡️ 完善的错误处理
- 📦 CMake构建支持

## 依赖要求

- C++17或更高版本
- CMake 3.16+
- libcurl
- OpenSSL
- nlohmann/json

### macOS安装依赖

```bash
brew install curl openssl nlohmann-json
```

### Ubuntu/Debian安装依赖

```bash
sudo apt-get install libcurl4-openssl-dev libssl-dev nlohmann-json3-dev
```

### CentOS/RHEL安装依赖

```bash
sudo yum install libcurl-devel openssl-devel
# nlohmann-json需要从源码编译或使用第三方仓库
```

## 构建

### 基本构建

```bash
mkdir build
cd build
cmake ..
make
```

### 构建选项

```bash
# 构建示例程序
cmake -DBUILD_EXAMPLES=ON ..

# 构建测试
cmake -DBUILD_TESTING=ON ..

# 构建共享库
cmake -DBUILD_SHARED_LIBS=ON ..

# 启用调试模式
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

## 安装

```bash
sudo make install
```

## 快速开始

### 基本使用

```cpp
#include <dataapi/DataApiClient.h>
#include <dataapi/auth/AuthenticationProvider.h>
#include <iostream>

using namespace dataapi;

int main() {
    try {
        // 创建客户端
        auto client = DataApiClient::createForDevelopment("your-api-key");
        
        // 测试连接
        if (client->testConnection()) {
            std::cout << "连接成功!" << std::endl;
        }
        
        // 获取API版本
        auto version = client->getVersion();
        std::cout << "API版本: " << version.version << std::endl;
        
        // 获取当前用户信息
        auto userClient = client->getUserClient();
        auto currentUser = userClient->getCurrentUser();
        std::cout << "当前用户: " << currentUser.username << std::endl;
        
        // 列出项目
        auto projectClient = client->getProjectClient();
        auto projects = projectClient->listProjects();
        std::cout << "项目数量: " << projects.size() << std::endl;
        
    } catch (const DataApiError& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### 工作流操作

```cpp
#include <dataapi/DataApiClient.h>

using namespace dataapi;

int main() {
    auto client = DataApiClient::createForDevelopment("your-api-key");
    auto workflowClient = client->getWorkflowClient();
    
    try {
        // 创建工作流
        WorkflowCreateRequest request;
        request.name = "测试工作流";
        request.description = "这是一个测试工作流";
        request.definition = R"({
            "nodes": [],
            "edges": []
        })";
        request.projectId = "project-id";
        request.userId = "user-id";
        
        auto workflow = workflowClient->createWorkflow(request);
        std::cout << "工作流已创建: " << workflow.id.value() << std::endl;
        
        // 执行工作流
        WorkflowExecutionRequest execRequest;
        execRequest.workflowId = workflow.id.value();
        execRequest.projectId = "project-id";
        execRequest.userId = "user-id";
        execRequest.workflowDefinition = request.definition;
        
        auto executionId = workflowClient->executeWorkflow(execRequest);
        std::cout << "工作流执行ID: " << executionId << std::endl;
        
    } catch (const DataApiError& e) {
        std::cerr << "工作流操作失败: " << e.what() << std::endl;
    }
    
    return 0;
}
```

### 数据库操作

```cpp
#include <dataapi/DataApiClient.h>

using namespace dataapi;

int main() {
    auto client = DataApiClient::createForDevelopment("your-api-key");
    auto dbClient = client->getDatabaseClient();
    
    try {
        // 执行SQL查询
        std::string sql = "SELECT * FROM users LIMIT 10";
        auto result = dbClient->executeQuery("database-id", sql);
        
        if (result.success) {
            std::cout << "查询成功，返回 " << result.data->size() << " 行数据" << std::endl;
            
            // 打印列名
            if (result.columns) {
                for (const auto& column : *result.columns) {
                    std::cout << column << "\t";
                }
                std::cout << std::endl;
            }
            
            // 打印数据
            if (result.data) {
                for (const auto& row : *result.data) {
                    std::cout << row.dump() << std::endl;
                }
            }
        }
        
    } catch (const DataApiError& e) {
        std::cerr << "数据库操作失败: " << e.what() << std::endl;
    }
    
    return 0;
}
```

### AI服务调用

```cpp
#include <dataapi/DataApiClient.h>

using namespace dataapi;

int main() {
    auto client = DataApiClient::createForDevelopment("your-api-key");
    auto aiClient = client->getAiProviderClient();
    
    try {
        // 文本生成
        auto response = aiClient->generateText(
            "provider-id",
            "请写一首关于春天的诗",
            Json{}
        );
        
        std::cout << "AI生成的文本: " << response.output.dump() << std::endl;
        
        // 流式文本生成
        aiClient->generateTextStream(
            "provider-id",
            "请解释什么是机器学习",
            Json{},
            [](const std::string& chunk) {
                std::cout << chunk << std::flush;
            }
        );
        
    } catch (const DataApiError& e) {
        std::cerr << "AI服务调用失败: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## 错误处理

SDK提供了完善的错误处理机制：

```cpp
#include <dataapi/DataApiError.h>

try {
    // API调用
    auto result = client->someOperation();
} catch (const AuthenticationError& e) {
    std::cerr << "认证失败: " << e.what() << std::endl;
} catch (const NotFoundError& e) {
    std::cerr << "资源未找到: " << e.what() << std::endl;
} catch (const RateLimitError& e) {
    std::cerr << "请求频率限制: " << e.what() << std::endl;
    // 可以重试
    if (e.isRetryable()) {
        // 等待后重试
    }
} catch (const DataApiError& e) {
    std::cerr << "API错误: " << e.what() << std::endl;
    std::cerr << "错误代码: " << e.getCode() << std::endl;
    std::cerr << "HTTP状态码: " << e.getStatusCode() << std::endl;
}
```

## 配置选项

### 客户端配置

```cpp
#include <dataapi/ClientConfig.h>

// 自定义配置
ClientConfig config;
config.baseUrl = "https://api.example.com";
config.timeout = 60000; // 60秒
config.enableLogging = true;
config.maxRetries = 3;
config.retryDelay = 1000; // 1秒

// 自定义认证
auto auth = std::make_shared<auth::BearerTokenAuthProvider>("your-token");

// 创建客户端
auto client = std::make_unique<DataApiClient>(config, auth);
```

### 环境配置

```cpp
// 开发环境
auto devClient = DataApiClient::createForDevelopment("dev-api-key");

// 生产环境
auto prodClient = DataApiClient::createForProduction("prod-api-key");
```

## CMake集成

在你的项目中使用DataAPI SDK：

```cmake
find_package(DataApiSDK REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app DataApi::SDK)
```

## 许可证

MIT License

## 支持

如有问题或建议，请提交Issue或联系开发团队。