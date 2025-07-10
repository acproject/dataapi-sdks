# 贡献指南

感谢您对DataAPI SDK项目的关注！我们欢迎各种形式的贡献，包括但不限于：

- 🐛 报告Bug
- 💡 提出新功能建议
- 📝 改进文档
- 🔧 提交代码修复或新功能
- 🧪 编写测试用例
- 🌐 翻译文档

## 开始之前

在开始贡献之前，请确保您已经：

1. 阅读了项目的 [README](README.md)
2. 查看了现有的 [Issues](https://github.com/owiseman/dataapi-client/issues)
3. 了解了项目的 [行为准则](#行为准则)

## 开发环境设置

### 前提条件

- **Java**: JDK 8或更高版本
- **Node.js**: 14.0或更高版本
- **C++**: 支持C++17的编译器
- **CMake**: 3.16或更高版本
- **Git**: 用于版本控制

### 克隆项目

```bash
git clone https://github.com/owiseman/dataapi-client.git
cd dataapi-client/dataapi-sdks
```

### 环境检查

```bash
# 检查开发环境
./gradlew checkEnvironment

# 设置开发环境
./gradlew setupDev
```

### 构建项目

```bash
# 构建所有SDK
./gradlew buildAll

# 运行测试
./gradlew testAll
```

## 贡献流程

### 1. 创建Issue

在开始编码之前，请先创建一个Issue来描述您要解决的问题或要添加的功能。这有助于：

- 避免重复工作
- 获得社区反馈
- 确保您的贡献符合项目方向

### 2. Fork项目

1. 在GitHub上Fork本项目
2. 克隆您的Fork到本地
3. 添加原项目为上游仓库

```bash
git remote add upstream https://github.com/owiseman/dataapi-client.git
```

### 3. 创建分支

为您的贡献创建一个新分支：

```bash
# 功能分支
git checkout -b feature/your-feature-name

# Bug修复分支
git checkout -b fix/issue-number-description

# 文档分支
git checkout -b docs/improvement-description
```

### 4. 进行开发

#### 代码规范

**Java代码规范**
- 遵循 [Google Java Style Guide](https://google.github.io/styleguide/javaguide.html)
- 使用4个空格缩进
- 类名使用PascalCase
- 方法名和变量名使用camelCase
- 常量使用UPPER_SNAKE_CASE

**TypeScript代码规范**
- 遵循 [TypeScript官方风格指南](https://www.typescriptlang.org/docs/handbook/declaration-files/do-s-and-don-ts.html)
- 使用2个空格缩进
- 使用严格的TypeScript配置
- 优先使用接口而不是类型别名

**C++代码规范**
- 遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- 使用4个空格缩进
- 类名使用PascalCase
- 函数名和变量名使用camelCase
- 常量使用kConstantName格式

#### 提交规范

我们使用 [Conventional Commits](https://www.conventionalcommits.org/) 规范：

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

**类型 (type)**:
- `feat`: 新功能
- `fix`: Bug修复
- `docs`: 文档更新
- `style`: 代码格式化（不影响功能）
- `refactor`: 代码重构
- `test`: 测试相关
- `chore`: 构建过程或辅助工具的变动

**示例**:
```
feat(java): add retry mechanism for HTTP requests

fix(typescript): resolve memory leak in connection pool

docs: update getting started guide

test(cpp): add unit tests for authentication providers
```

### 5. 测试

在提交之前，请确保：

```bash
# 运行所有测试
./gradlew testAll

# 运行代码质量检查
./gradlew checkQuality

# 运行示例
./gradlew runExamples
```

#### 测试要求

- **单元测试**: 新功能必须包含单元测试
- **集成测试**: 重要功能需要集成测试
- **测试覆盖率**: 保持80%以上的代码覆盖率
- **性能测试**: 性能相关的改动需要性能测试

### 6. 提交Pull Request

1. 推送您的分支到您的Fork
2. 在GitHub上创建Pull Request
3. 填写PR模板中的所有必要信息
4. 等待代码审查

#### PR标题格式

```
<type>(<scope>): <description>
```

#### PR描述模板

```markdown
## 变更类型
- [ ] Bug修复
- [ ] 新功能
- [ ] 文档更新
- [ ] 性能优化
- [ ] 代码重构
- [ ] 其他

## 变更描述
简要描述您的变更内容...

## 相关Issue
- Closes #123
- Related to #456

## 测试
- [ ] 单元测试通过
- [ ] 集成测试通过
- [ ] 手动测试完成

## 检查清单
- [ ] 代码遵循项目规范
- [ ] 添加了必要的测试
- [ ] 更新了相关文档
- [ ] 没有引入破坏性变更
- [ ] 通过了所有CI检查
```

## 代码审查

### 审查标准

- **功能正确性**: 代码是否正确实现了预期功能
- **代码质量**: 代码是否清晰、可维护
- **性能**: 是否有性能问题
- **安全性**: 是否存在安全漏洞
- **测试覆盖**: 是否有足够的测试
- **文档**: 是否需要更新文档

### 审查流程

1. 自动化检查（CI/CD）
2. 代码审查（至少一个维护者）
3. 测试验证
4. 合并到主分支

## 发布流程

### 版本号规范

我们使用 [Semantic Versioning](https://semver.org/)：

- **MAJOR**: 不兼容的API变更
- **MINOR**: 向后兼容的功能新增
- **PATCH**: 向后兼容的Bug修复

### 发布步骤

1. 更新版本号
2. 更新CHANGELOG
3. 创建Release Tag
4. 发布到包管理器
5. 更新文档

## 社区

### 沟通渠道

- **GitHub Issues**: 报告Bug和功能请求
- **GitHub Discussions**: 一般讨论和问答
- **Slack**: 实时交流 ([加入链接](https://dataapi-community.slack.com))
- **邮件**: support@owiseman.com

### 会议

- **每周开发会议**: 周三 UTC 14:00
- **月度社区会议**: 每月第一个周五 UTC 15:00

## 行为准则

### 我们的承诺

为了营造一个开放和友好的环境，我们承诺：

- 使用友好和包容的语言
- 尊重不同的观点和经验
- 优雅地接受建设性批评
- 关注对社区最有利的事情
- 对其他社区成员表示同理心

### 不可接受的行为

- 使用性别化语言或图像，以及不受欢迎的性关注或性骚扰
- 恶意评论、人身攻击或政治攻击
- 公开或私下骚扰
- 未经明确许可发布他人的私人信息
- 在专业环境中可能被认为不合适的其他行为

### 执行

如果您遇到不当行为，请联系项目维护者：support@owiseman.com

## 认可贡献者

我们使用 [All Contributors](https://allcontributors.org/) 规范来认可所有类型的贡献。

### 贡献类型

- 💻 代码
- 📖 文档
- 🐛 Bug报告
- 💡 想法和规划
- 🤔 答疑解惑
- 🚇 基础设施
- 🚧 维护
- 👀 代码审查
- 🧪 测试
- 🌍 翻译

## 许可证

通过贡献代码，您同意您的贡献将在 [MIT许可证](LICENSE) 下授权。

## 常见问题

### Q: 我是新手，可以贡献吗？
A: 当然可以！我们欢迎所有级别的贡献者。可以从标记为 `good first issue` 的Issue开始。

### Q: 我发现了Bug，但不知道如何修复？
A: 请创建一个Issue详细描述Bug，包括重现步骤。即使您不能修复，报告Bug也是很有价值的贡献。

### Q: 我想添加新功能，应该怎么做？
A: 首先创建一个Issue讨论您的想法，获得社区反馈后再开始实现。

### Q: 代码审查需要多长时间？
A: 通常在1-3个工作日内会有初步反馈。复杂的PR可能需要更长时间。

### Q: 我的PR被拒绝了，怎么办？
A: 不要气馁！查看反馈意见，进行相应修改，或者在Issue中讨论不同的实现方案。

---

感谢您的贡献！🎉