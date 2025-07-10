# DataAPI SDK 文档

本目录包含DataAPI SDK的完整文档，支持Java、TypeScript和C++三种语言。

## 目录结构

```
docs/
├── README.md                 # 本文件
├── getting-started.md        # 快速开始指南
├── api-reference/           # API参考文档
│   ├── java/               # Java SDK API文档
│   ├── typescript/         # TypeScript SDK API文档
│   └── cpp/                # C++ SDK API文档
├── guides/                 # 使用指南
│   ├── authentication.md  # 认证指南
│   ├── configuration.md   # 配置指南
│   ├── error-handling.md  # 错误处理指南
│   └── best-practices.md  # 最佳实践
└── examples/               # 示例代码文档
    ├── java-examples.md    # Java示例
    ├── typescript-examples.md # TypeScript示例
    └── cpp-examples.md     # C++示例
```

## 快速导航

### 开始使用
- [快速开始指南](getting-started.md) - 5分钟快速上手
- [安装指南](#安装指南) - 各语言SDK安装方法
- [基础示例](#基础示例) - 简单的使用示例

### API文档
- [Java SDK API](api-reference/java/) - Java SDK完整API参考
- [TypeScript SDK API](api-reference/typescript/) - TypeScript SDK完整API参考
- [C++ SDK API](api-reference/cpp/) - C++ SDK完整API参考

### 使用指南
- [认证指南](guides/authentication.md) - 如何配置和使用认证
- [配置指南](guides/configuration.md) - 客户端配置选项
- [错误处理指南](guides/error-handling.md) - 错误处理最佳实践
- [最佳实践](guides/best-practices.md) - 使用SDK的最佳实践

### 示例代码
- [Java示例](examples/java-examples.md) - Java SDK使用示例
- [TypeScript示例](examples/typescript-examples.md) - TypeScript SDK使用示例
- [C++示例](examples/cpp-examples.md) - C++ SDK使用示例

## 安装指南

### Java SDK

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

### TypeScript SDK

#### npm
```bash
npm install @owiseman/dataapi-typescript-sdk
```

#### yarn
```bash
yarn add @owiseman/dataapi-typescript-sdk
```

### C++ SDK

#### 使用CMake
```cmake
find_package(DataApiSdk REQUIRED)
target_link_libraries(your_target DataApiSdk::DataApiSdk)
```

#### 手动编译
```bash
git clone https://github.com/owiseman/dataapi-client.git
cd dataapi-client/dataapi-sdks/cpp-sdk
mkdir build && cd build
cmake ..
make
make install
```

## 基础示例

### Java
```java
import com.owiseman.dataapi.sdk.DataApiClient;
import com.owiseman.dataapi.sdk.auth.BearerTokenAuthProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;

public class Example {
    public static void main(String[] args) {
        // 创建认证提供者
        BearerTokenAuthProvider auth = new BearerTokenAuthProvider("your-token");
        
        // 创建客户端
        DataApiClient client = new DataApiClient(ClientConfig.development(), auth);
        
        // 测试连接
        boolean connected = client.testConnection();
        System.out.println("Connected: " + connected);
        
        // 关闭客户端
        client.close();
    }
}
```

### TypeScript
```typescript
import { DataApiClient, BearerTokenAuthProvider, ClientConfig } from '@owiseman/dataapi-typescript-sdk';

async function example() {
    // 创建认证提供者
    const auth = new BearerTokenAuthProvider('your-token');
    
    // 创建客户端
    const client = new DataApiClient(ClientConfig.development(), auth);
    
    // 测试连接
    const connected = await client.testConnection();
    console.log('Connected:', connected);
    
    // 关闭客户端
    await client.close();
}

example().catch(console.error);
```

### C++
```cpp
#include "dataapi/DataApiClient.h"
#include "dataapi/auth/BearerTokenAuthProvider.h"
#include "dataapi/config/ClientConfig.h"

int main() {
    // 创建认证提供者
    auto auth = std::make_shared<dataapi::BearerTokenAuthProvider>("your-token");
    
    // 创建客户端
    auto client = std::make_unique<dataapi::DataApiClient>(
        dataapi::ClientConfig::development(), auth);
    
    // 测试连接
    bool connected = client->testConnection();
    std::cout << "Connected: " << connected << std::endl;
    
    // 关闭客户端
    client->close();
    
    return 0;
}
```

## 支持的功能

### 核心功能
- ✅ 工作流管理 (创建、更新、删除、执行)
- ✅ 项目管理
- ✅ 数据库操作 (查询、更新)
- ✅ AI提供者集成
- ✅ 用户管理

### 认证方式
- ✅ Bearer Token认证
- ✅ API Key认证
- ✅ Basic认证
- ✅ OAuth2认证 (部分支持)

### 高级功能
- ✅ 自动重试机制
- ✅ 请求/响应拦截器
- ✅ 详细的错误处理
- ✅ 连接池管理
- ✅ 日志记录
- ✅ 配置管理

## 版本兼容性

| SDK版本 | API版本 | Java版本 | Node.js版本 | C++标准 |
|---------|---------|----------|-------------|----------|
| 1.0.x   | v1      | 8+       | 14+         | C++17    |
| 1.1.x   | v1      | 8+       | 16+         | C++17    |
| 2.0.x   | v2      | 11+      | 18+         | C++20    |

## 获取帮助

- 📖 [完整文档](https://docs.dataapi.owiseman.com)
- 🐛 [问题反馈](https://github.com/owiseman/dataapi-client/issues)
- 💬 [讨论区](https://github.com/owiseman/dataapi-client/discussions)
- 📧 [邮件支持](mailto:support@owiseman.com)

## 贡献

欢迎贡献代码！请查看 [贡献指南](../CONTRIBUTING.md) 了解详细信息。

## 许可证

本项目采用 [MIT许可证](../LICENSE)。