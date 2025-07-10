import { DataApiClient } from '../../typescript-sdk/src/client/DataApiClient';
import { BearerTokenAuthProvider } from '../../typescript-sdk/src/auth/BearerTokenAuthProvider';
import { ClientConfig } from '../../typescript-sdk/src/config/ClientConfig';
import { WorkflowCreateRequest, SysWorkflow, PageResult } from '../../typescript-sdk/src/types';
import { DataApiError } from '../../typescript-sdk/src/errors/DataApiError';

/**
 * TypeScript SDK基础示例
 */
async function basicExample() {
    try {
        console.log('=== DataAPI TypeScript SDK 示例 ===');
        
        // 创建认证提供者
        const authProvider = new BearerTokenAuthProvider('your-access-token');
        
        // 创建客户端配置
        const config = {
            baseUrl: 'https://api.example.com',
            timeout: 30000,
            enableLogging: true
        };
        
        // 创建DataAPI客户端
        const client = new DataApiClient(config, authProvider);
        
        // 测试连接
        console.log('测试连接...');
        try {
            const connected = await client.testConnection();
            console.log(`连接状态: ${connected ? '成功' : '失败'}`);
            
            if (!connected) {
                console.log('无法连接到服务器，请检查配置');
                return;
            }
        } catch (error) {
            console.log('连接测试失败:', error instanceof Error ? error.message : error);
            return;
        }
        
        // 获取API版本
        try {
            const version = await client.getVersion();
            console.log(`API版本: ${version}`);
        } catch (error) {
            console.log('获取版本信息失败:', error instanceof Error ? error.message : error);
        }
        
        // 获取健康状态
        try {
            const health = await client.getHealth();
            console.log('健康状态:', health);
        } catch (error) {
            console.log('获取健康状态失败:', error instanceof Error ? error.message : error);
        }
        
        // 工作流操作示例
        console.log('\n=== 工作流操作示例 ===');
        
        const workflowClient = client.workflow;
        
        // 获取工作流列表
        try {
            const workflows: PageResult<SysWorkflow> = await workflowClient.getWorkflows(0, 10);
            console.log(`工作流总数: ${workflows.totalElements}`);
            
            workflows.content.forEach(workflow => {
                console.log(`- ${workflow.name} (ID: ${workflow.id})`);
            });
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('获取工作流列表失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('获取工作流列表失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // 创建工作流示例
        try {
            const createRequest: WorkflowCreateRequest = {
                name: '示例工作流',
                description: '这是一个TypeScript SDK创建的示例工作流',
                definition: '{"nodes":[],"edges":[]}',
                projectId: 'project-1',
                userId: 'user-1'
            };
            
            const newWorkflow = await workflowClient.createWorkflow(createRequest);
            console.log(`创建工作流成功: ${newWorkflow.name} (ID: ${newWorkflow.id})`);
            
            // 获取工作流详情
            if (newWorkflow.id) {
                const workflowDetail = await workflowClient.getWorkflow(newWorkflow.id);
                console.log('工作流详情:', workflowDetail.name, '状态:', workflowDetail.status);
                
                // 删除刚创建的工作流
                await workflowClient.deleteWorkflow(newWorkflow.id);
                console.log('删除工作流成功');
            }
            
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('工作流操作失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('工作流操作失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // 数据库操作示例
        console.log('\n=== 数据库操作示例 ===');
        
        const databaseClient = client.database;
        
        try {
            // 测试数据库连接
            const dbConnected = await databaseClient.testConnection();
            console.log(`数据库连接状态: ${dbConnected ? '成功' : '失败'}`);
            
            if (dbConnected) {
                // 获取连接信息
                const connectionInfo = await databaseClient.getConnectionInfo();
                console.log('数据库连接信息:', connectionInfo);
                
                // 获取表列表
                const tables = await databaseClient.getTables();
                console.log('数据库表数量:', tables.length);
            }
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('数据库操作失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('数据库操作失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // 项目操作示例
        console.log('\n=== 项目操作示例 ===');
        
        const projectClient = client.project;
        
        try {
            const projects = await projectClient.getProjects(0, 10);
            console.log(`项目总数: ${projects.totalElements}`);
            
            projects.content.forEach((project: any) => {
                console.log(`- ${project.name} (ID: ${project.id})`);
            });
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('获取项目列表失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('获取项目列表失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // AI提供者操作示例
        console.log('\n=== AI提供者操作示例 ===');
        
        const aiClient = client.aiProvider;
        
        try {
            // 测试AI配置
            const testResult = await aiClient.testAiConfig('config-1');
            console.log('AI配置测试结果:', testResult);
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('AI操作失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('AI操作失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // 用户操作示例
        console.log('\n=== 用户操作示例 ===');
        
        const userClient = client.user;
        
        try {
            const currentUser = await userClient.getCurrentUser();
            console.log('当前用户:', currentUser.username, '邮箱:', currentUser.email);
        } catch (error) {
            if (error instanceof DataApiError) {
                console.log('获取用户信息失败:', error.message, '状态码:', error.statusCode);
            } else {
                console.log('获取用户信息失败:', error instanceof Error ? error.message : error);
            }
        }
        
        // 关闭客户端
        await client.close();
        console.log('\n客户端已关闭');
        
    } catch (error) {
        console.error('示例执行失败:', error instanceof Error ? error.message : error);
        if (error instanceof Error && error.stack) {
            console.error('错误堆栈:', error.stack);
        }
    }
}

// 运行示例
if (require.main === module) {
    basicExample().catch(console.error);
}

export { basicExample };