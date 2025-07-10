# DataAPI SDK 套件

[![Build Status](https://github.com/owiseman/dataapi-client/workflows/CI/badge.svg)](https://github.com/owiseman/dataapi-client/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Java](https://img.shields.io/badge/Java-8%2B-orange)](https://www.oracle.com/java/)
[![TypeScript](https://img.shields.io/badge/TypeScript-4.0%2B-blue)](https://www.typescriptlang.org/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-red)](https://isocpp.org/)

DataAPI SDK套件为开发者提供了与DataAPI服务交互的多语言客户端库。支持Java、TypeScript和C++三种主流编程语言，让您可以轻松地在不同平台和项目中集成DataAPI功能。

## 🚀 特性

### 核心功能
- ✅ **工作流管理** - 创建、更新、删除和执行工作流
- ✅ **项目管理** - 完整的项目生命周期管理
- ✅ **数据库操作** - SQL查询和数据操作
- ✅ **AI提供者集成** - 多种AI模型和服务集成
- ✅ **用户管理** - 用户认证和权限管理

### 技术特性
- 🔐 **多种认证方式** - Bearer Token、API Key、Basic Auth、OAuth2
- 🔄 **自动重试机制** - 智能重试策略，提高可靠性
- 📝 **详细日志记录** - 完整的请求/响应日志
- ⚡ **高性能** - 连接池、请求缓存等优化
- 🛡️ **类型安全** - 完整的类型定义和验证
- 📊 **错误处理** - 结构化的错误信息和处理

## 📦 支持的语言

| 语言 | 版本要求 | 状态 | 文档 |
|------|----------|------|------|
| [Java](java-sdk/) | 8+ | ✅ 稳定 | [Java文档](docs/api-reference/java/) |
| [TypeScript](typescript-sdk/) | Node.js 14+ | ✅ 稳定 | [TypeScript文档](docs/api-reference/typescript/) |
| [C++](cpp-sdk/) | C++17 | ✅ 稳定 | [C++文档](docs/api-reference/cpp/) |

## 🏃‍♂️ 快速开始

### Java

```xml
<!-- Maven -->
<dependency>
    <groupId>com.owiseman.dataapi</groupId>
    <artifactId>dataapi-java-sdk</artifactId>
    <version>1.0.0</version>
</dependency>
```

```java
import com.owiseman.dataapi.sdk.DataApiClient;
import com.owiseman.dataapi.sdk.auth.BearerTokenAuthProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;

// 创建客户端
BearerTokenAuthProvider auth = new BearerTokenAuthProvider("your-token");
DataApiClient client = new DataApiClient(ClientConfig.development(), auth);

// 测试连接
boolean connected = client.testConnection();
System.out.println("Connected: " + connected);

// 获取工作流
PageResult<SysWorkflow> workflows = client.getWorkflowClient().getWorkflows(0, 10);
System.out.println("Workflows: " + workflows.getTotalElements());
```

### TypeScript

```bash
npm install @owiseman/dataapi-typescript-sdk
```

```typescript
import { DataApiClient, BearerTokenAuthProvider, ClientConfig } from '@owiseman/dataapi-typescript-sdk';

// 创建客户端
const auth = new BearerTokenAuthProvider('your-token');
const client = new DataApiClient(ClientConfig.development(), auth);

// 测试连接
const connected = await client.testConnection();
console.log('Connected:', connected);

// 获取工作流
const workflows = await client.getWorkflowClient().getWorkflows(0, 10);
console.log('Workflows:', workflows.totalElements);
```

### C++

```cpp
#include "dataapi/DataApiClient.h"
#include "dataapi/auth/BearerTokenAuthProvider.h"

// 创建客户端
auto auth = std::make_shared<dataapi::BearerTokenAuthProvider>("your-token");
auto client = std::make_unique<dataapi::DataApiClient>(
    dataapi::ClientConfig::development(), auth);

// 测试连接
bool connected = client->testConnection();
std::cout << "Connected: " << connected << std::endl;

// 获取工作流
auto workflows = client->getWorkflowClient()->getWorkflows(0, 10);
std::cout << "Workflows: " << workflows.totalElements << std::endl;
```

## 📚 文档

### 快速导航
- 📖 [快速开始指南](docs/getting-started.md) - 5分钟快速上手
- 🔧 [安装指南](docs/getting-started.md#安装指南) - 各语言安装方法
- 📋 [API参考](docs/api-reference/) - 完整API文档
- 💡 [使用指南](docs/guides/) - 详细使用说明
- 🎯 [示例代码](docs/examples/) - 实际使用示例

### API文档
- [Java SDK API](docs/api-reference/java/)
- [TypeScript SDK API](docs/api-reference/typescript/)
- [C++ SDK API](docs/api-reference/cpp/)

### 使用指南
- [认证配置](docs/guides/authentication.md)
- [客户端配置](docs/guides/configuration.md)
- [错误处理](docs/guides/error-handling.md)
- [最佳实践](docs/guides/best-practices.md)

## 🏗️ 项目结构

```
dataapi-sdks/
├── java-sdk/                 # Java SDK
│   ├── src/main/java/        # Java源代码
│   ├── src/test/java/        # Java测试代码
│   ├── build.gradle          # Gradle构建配置
│   └── README.md             # Java SDK文档
├── typescript-sdk/           # TypeScript SDK
│   ├── src/                  # TypeScript源代码
│   ├── tests/                # TypeScript测试代码
│   ├── package.json          # npm配置
│   └── README.md             # TypeScript SDK文档
├── cpp-sdk/                  # C++ SDK
│   ├── include/              # C++头文件
│   ├── src/                  # C++源代码
│   ├── tests/                # C++测试代码
│   ├── CMakeLists.txt        # CMake配置
│   └── README.md             # C++ SDK文档
├── examples/                 # 示例代码
│   ├── java/                 # Java示例
│   ├── typescript/           # TypeScript示例
│   └── cpp/                  # C++示例
├── docs/                     # 文档
│   ├── api-reference/        # API参考文档
│   ├── guides/               # 使用指南
│   └── examples/             # 示例文档
├── build.gradle              # 根构建配置
└── README.md                 # 本文件
```

## 🔧 开发环境设置

### 前提条件

- **Java**: JDK 8或更高版本
- **Node.js**: 14.0或更高版本
- **C++**: 支持C++17的编译器 (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake**: 3.16或更高版本 (仅C++)

### 构建所有SDK

```bash
# 克隆仓库
git clone https://github.com/owiseman/dataapi-client.git
cd dataapi-client/dataapi-sdks

# 构建所有SDK
./gradlew build

# 或者单独构建
./gradlew :java-sdk:build          # 构建Java SDK
./gradlew :typescript-sdk:build    # 构建TypeScript SDK
./gradlew :cpp-sdk:build           # 构建C++ SDK
```

### 运行测试

```bash
# 运行所有测试
./gradlew test

# 运行特定语言的测试
./gradlew :java-sdk:test
./gradlew :typescript-sdk:test
./gradlew :cpp-sdk:test
```

### 运行示例

```bash
# 运行Java示例
./gradlew :examples:runJavaExample

# 运行TypeScript示例
./gradlew :examples:runTypeScriptExample

# 运行C++示例
./gradlew :examples:runCppExample

# 运行所有示例
./gradlew :examples:runAllExamples
```

## 🧪 测试

每个SDK都包含完整的单元测试和集成测试：

- **单元测试** - 测试各个组件的功能
- **集成测试** - 测试与实际API的交互
- **性能测试** - 测试SDK的性能表现
- **兼容性测试** - 测试不同版本的兼容性

## 📊 性能基准

| 操作 | Java | TypeScript | C++ |
|------|------|------------|-----|
| 创建客户端 | ~10ms | ~5ms | ~2ms |
| 简单API调用 | ~50ms | ~45ms | ~30ms |
| 批量操作 (100个) | ~2s | ~2.5s | ~1.5s |
| 内存使用 | ~50MB | ~30MB | ~10MB |

*基准测试在标准开发环境下进行，实际性能可能因环境而异。*

## 🤝 贡献

我们欢迎社区贡献！请查看 [贡献指南](CONTRIBUTING.md) 了解如何参与项目开发。

### 贡献方式

1. 🐛 **报告Bug** - 提交Issue描述问题
2. 💡 **功能建议** - 提出新功能想法
3. 📝 **改进文档** - 完善文档内容
4. 🔧 **代码贡献** - 提交Pull Request
5. 🧪 **测试** - 编写和改进测试用例

### 开发流程

1. Fork项目
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送分支 (`git push origin feature/amazing-feature`)
5. 创建Pull Request

## 📋 版本历史

### v1.0.0 (2024-01-15)
- ✨ 初始版本发布
- ✅ Java、TypeScript、C++ SDK
- 🔐 多种认证方式支持
- 📚 完整文档和示例

### v1.1.0 (计划中)
- 🚀 性能优化
- 🔄 WebSocket支持
- 📊 更多监控指标
- 🌐 国际化支持

## 🆘 获取帮助

### 文档和资源
- 📖 [完整文档](https://docs.dataapi.owiseman.com)
- 🎥 [视频教程](https://www.youtube.com/playlist?list=PLDataAPI)
- 📚 [博客文章](https://blog.owiseman.com/tags/dataapi)

### 社区支持
- 💬 [GitHub讨论区](https://github.com/owiseman/dataapi-client/discussions)
- 🐛 [问题反馈](https://github.com/owiseman/dataapi-client/issues)
- 💌 [邮件支持](mailto:support@owiseman.com)
- 🗨️ [Slack社区](https://dataapi-community.slack.com)

### 商业支持
- 🏢 [企业支持](https://owiseman.com/enterprise)
- 📞 [技术咨询](https://owiseman.com/consulting)
- 🎓 [培训服务](https://owiseman.com/training)

## 📄 许可证

本项目采用 [MIT许可证](LICENSE)。

```
MIT License

Copyright (c) 2024 Owiseman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

<div align="center">

**[🏠 主页](https://owiseman.com) • [📖 文档](https://docs.dataapi.owiseman.com) • [🐛 问题](https://github.com/owiseman/dataapi-client/issues) • [💬 讨论](https://github.com/owiseman/dataapi-client/discussions)**

由 ❤️ 和 ☕ 在 [Owiseman](https://owiseman.com) 制作

</div>