# 快速开始指南

本指南将帮助您在5分钟内开始使用DataAPI SDK。我们提供Java、TypeScript和C++三种语言的SDK。

## 前提条件

在开始之前，请确保您有：

1. **DataAPI访问凭证** - 从DataAPI控制台获取API密钥或访问令牌
2. **开发环境** - 根据您选择的语言准备相应的开发环境
3. **网络访问** - 确保能够访问DataAPI服务端点

## 选择您的语言

- [Java](#java-快速开始)
- [TypeScript](#typescript-快速开始)
- [C++](#c-快速开始)

---

## Java 快速开始

### 1. 添加依赖

#### Maven
```xml
<dependency>
    <groupId>com.owiseman.dataapi</groupId>
    <artifactId>dataapi-java-sdk</artifactId>
    <version>1.0.0</version>
</dependency>
```

#### Gradle
```gradle
implementation 'com.owiseman.dataapi:dataapi-java-sdk:1.0.0'
```

### 2. 创建客户端

```java
import com.owiseman.dataapi.sdk.DataApiClient;
import com.owiseman.dataapi.sdk.auth.BearerTokenAuthProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;

public class QuickStart {
    public static void main(String[] args) {
        // 1. 创建认证提供者
        BearerTokenAuthProvider authProvider = new BearerTokenAuthProvider("your-access-token");
        
        // 2. 创建客户端配置
        ClientConfig config = ClientConfig.builder()
            .baseUrl("https://api.dataapi.com")
            .timeout(30000)
            .build();
        
        // 3. 创建DataAPI客户端
        DataApiClient client = new DataApiClient(config, authProvider);
        
        // 4. 测试连接
        try {
            boolean connected = client.testConnection();
            System.out.println("连接状态: " + (connected ? "成功" : "失败"));
        } catch (Exception e) {
            System.err.println("连接失败: " + e.getMessage());
        }
        
        // 5. 关闭客户端
        client.close();
    }
}
```

### 3. 基本操作

```java
// 获取工作流列表
PageResult<SysWorkflow> workflows = client.getWorkflowClient().getWorkflows(0, 10);
System.out.println("工作流数量: " + workflows.getTotalElements());

// 创建新工作流
WorkflowCreateRequest request = new WorkflowCreateRequest(
    "我的工作流",
    "工作流描述",
    "{\"nodes\":[], \"edges\":[]}",
    "project-id",
    "user-id"
);
SysWorkflow newWorkflow = client.getWorkflowClient().createWorkflow(request);
System.out.println("创建的工作流ID: " + newWorkflow.getId());
```

---

## TypeScript 快速开始

### 1. 安装包

```bash
# 使用npm
npm install @owiseman/dataapi-typescript-sdk

# 使用yarn
yarn add @owiseman/dataapi-typescript-sdk
```

### 2. 创建客户端

```typescript
import { 
    DataApiClient, 
    BearerTokenAuthProvider, 
    ClientConfig 
} from '@owiseman/dataapi-typescript-sdk';

async function quickStart() {
    // 1. 创建认证提供者
    const authProvider = new BearerTokenAuthProvider('your-access-token');
    
    // 2. 创建客户端配置
    const config = ClientConfig.builder()
        .baseUrl('https://api.dataapi.com')
        .timeout(30000)
        .build();
    
    // 3. 创建DataAPI客户端
    const client = new DataApiClient(config, authProvider);
    
    // 4. 测试连接
    try {
        const connected = await client.testConnection();
        console.log(`连接状态: ${connected ? '成功' : '失败'}`);
    } catch (error) {
        console.error('连接失败:', error.message);
    }
    
    // 5. 关闭客户端
    await client.close();
}

quickStart().catch(console.error);
```

### 3. 基本操作

```typescript
// 获取工作流列表
const workflows = await client.getWorkflowClient().getWorkflows(0, 10);
console.log(`工作流数量: ${workflows.totalElements}`);

// 创建新工作流
const request: WorkflowCreateRequest = {
    name: '我的工作流',
    description: '工作流描述',
    definition: '{"nodes":[], "edges":[]}',
    projectId: 'project-id',
    userId: 'user-id'
};
const newWorkflow = await client.getWorkflowClient().createWorkflow(request);
console.log(`创建的工作流ID: ${newWorkflow.id}`);
```

---

## C++ 快速开始

### 1. 安装依赖

#### 使用vcpkg (推荐)
```bash
vcpkg install curl nlohmann-json openssl
```

#### 手动安装
- libcurl
- nlohmann/json
- OpenSSL

### 2. 编译SDK

```bash
git clone https://github.com/owiseman/dataapi-client.git
cd dataapi-client/dataapi-sdks/cpp-sdk
mkdir build && cd build
cmake ..
make
```

### 3. 创建客户端

```cpp
#include "dataapi/DataApiClient.h"
#include "dataapi/auth/BearerTokenAuthProvider.h"
#include "dataapi/config/ClientConfig.h"
#include <iostream>
#include <memory>

int main() {
    try {
        // 1. 创建认证提供者
        auto authProvider = std::make_shared<dataapi::BearerTokenAuthProvider>("your-access-token");
        
        // 2. 创建客户端配置
        auto config = dataapi::ClientConfig::builder()
            .baseUrl("https://api.dataapi.com")
            .timeout(30000)
            .build();
        
        // 3. 创建DataAPI客户端
        auto client = std::make_unique<dataapi::DataApiClient>(config, authProvider);
        
        // 4. 测试连接
        bool connected = client->testConnection();
        std::cout << "连接状态: " << (connected ? "成功" : "失败") << std::endl;
        
        // 5. 关闭客户端
        client->close();
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

### 4. CMakeLists.txt配置

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyDataApiApp)

set(CMAKE_CXX_STANDARD 17)

# 查找DataAPI SDK
find_package(DataApiSdk REQUIRED)

# 创建可执行文件
add_executable(my_app main.cpp)

# 链接DataAPI SDK
target_link_libraries(my_app DataApiSdk::DataApiSdk)
```

### 5. 基本操作

```cpp
// 获取工作流列表
auto workflows = client->getWorkflowClient()->getWorkflows(0, 10);
std::cout << "工作流数量: " << workflows.totalElements << std::endl;

// 创建新工作流
dataapi::WorkflowCreateRequest request;
request.name = "我的工作流";
request.description = "工作流描述";
request.definition = "{\"nodes\":[], \"edges\":[]}";
request.projectId = "project-id";
request.userId = "user-id";

auto newWorkflow = client->getWorkflowClient()->createWorkflow(request);
std::cout << "创建的工作流ID: " << newWorkflow.id.value_or("N/A") << std::endl;
```

---

## 认证配置

### Bearer Token认证 (推荐)

```java
// Java
BearerTokenAuthProvider auth = new BearerTokenAuthProvider("your-access-token");
```

```typescript
// TypeScript
const auth = new BearerTokenAuthProvider('your-access-token');
```

```cpp
// C++
auto auth = std::make_shared<BearerTokenAuthProvider>("your-access-token");
```

### API Key认证

```java
// Java
ApiKeyAuthProvider auth = new ApiKeyAuthProvider("your-api-key", "X-API-Key");
```

```typescript
// TypeScript
const auth = new ApiKeyAuthProvider('your-api-key', 'X-API-Key');
```

```cpp
// C++
auto auth = std::make_shared<ApiKeyAuthProvider>("your-api-key", "X-API-Key");
```

### Basic认证

```java
// Java
BasicAuthProvider auth = new BasicAuthProvider("username", "password");
```

```typescript
// TypeScript
const auth = new BasicAuthProvider('username', 'password');
```

```cpp
// C++
auto auth = std::make_shared<BasicAuthProvider>("username", "password");
```

---

## 环境配置

### 开发环境

```java
// Java
ClientConfig config = ClientConfig.development();
```

```typescript
// TypeScript
const config = ClientConfig.development();
```

```cpp
// C++
auto config = ClientConfig::development();
```

### 生产环境

```java
// Java
ClientConfig config = ClientConfig.production();
```

```typescript
// TypeScript
const config = ClientConfig.production();
```

```cpp
// C++
auto config = ClientConfig::production();
```

### 自定义配置

```java
// Java
ClientConfig config = ClientConfig.builder()
    .baseUrl("https://your-api.com")
    .timeout(60000)
    .retryAttempts(3)
    .enableLogging(true)
    .build();
```

```typescript
// TypeScript
const config = ClientConfig.builder()
    .baseUrl('https://your-api.com')
    .timeout(60000)
    .retryAttempts(3)
    .enableLogging(true)
    .build();
```

```cpp
// C++
auto config = ClientConfig::builder()
    .baseUrl("https://your-api.com")
    .timeout(60000)
    .retryAttempts(3)
    .enableLogging(true)
    .build();
```

---

## 错误处理

### Java

```java
try {
    SysWorkflow workflow = client.getWorkflowClient().getWorkflow("workflow-id");
} catch (AuthenticationException e) {
    System.err.println("认证失败: " + e.getMessage());
} catch (HttpException e) {
    System.err.println("HTTP错误: " + e.getStatusCode() + " - " + e.getMessage());
} catch (DataApiException e) {
    System.err.println("API错误: " + e.getCode() + " - " + e.getMessage());
}
```

### TypeScript

```typescript
try {
    const workflow = await client.getWorkflowClient().getWorkflow('workflow-id');
} catch (error) {
    if (error instanceof AuthenticationError) {
        console.error('认证失败:', error.message);
    } else if (error instanceof HttpError) {
        console.error('HTTP错误:', error.statusCode, '-', error.message);
    } else if (error instanceof DataApiError) {
        console.error('API错误:', error.code, '-', error.message);
    }
}
```

### C++

```cpp
try {
    auto workflow = client->getWorkflowClient()->getWorkflow("workflow-id");
} catch (const dataapi::AuthenticationException& e) {
    std::cerr << "认证失败: " << e.what() << std::endl;
} catch (const dataapi::HttpException& e) {
    std::cerr << "HTTP错误: " << e.getStatusCode() << " - " << e.what() << std::endl;
} catch (const dataapi::DataApiException& e) {
    std::cerr << "API错误: " << e.getCode() << " - " << e.what() << std::endl;
}
```

---

## 下一步

现在您已经成功设置了DataAPI SDK，可以继续学习：

1. **[API参考文档](api-reference/)** - 了解所有可用的API
2. **[使用指南](guides/)** - 深入了解各种功能
3. **[示例代码](examples/)** - 查看更多实际使用示例
4. **[最佳实践](guides/best-practices.md)** - 学习如何高效使用SDK

## 获取帮助

如果您遇到问题：

- 查看 [常见问题解答](faq.md)
- 提交 [GitHub Issue](https://github.com/owiseman/dataapi-client/issues)
- 发送邮件至 [support@owiseman.com](mailto:support@owiseman.com)