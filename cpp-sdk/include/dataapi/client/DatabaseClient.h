#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Types.h"
#include "../http/HttpClient.h"

namespace dataapi {
namespace client {

/**
 * 数据库客户端类
 * 提供数据库相关的API操作
 */
class DatabaseClient {
private:
    std::shared_ptr<http::HttpClient> httpClient;
    
public:
    /**
     * 构造函数
     * @param httpClient HTTP客户端
     */
    explicit DatabaseClient(std::shared_ptr<http::HttpClient> httpClient);
    
    /**
     * 析构函数
     */
    ~DatabaseClient() = default;
    
    /**
     * 禁用拷贝构造和赋值
     */
    DatabaseClient(const DatabaseClient&) = delete;
    DatabaseClient& operator=(const DatabaseClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    DatabaseClient(DatabaseClient&& other) noexcept = default;
    DatabaseClient& operator=(DatabaseClient&& other) noexcept = default;
    
    /**
     * 测试数据库连接
     * @param config 数据库配置
     * @return 连接测试结果
     */
    DatabaseConnectionResult testConnection(const DatabaseConfig& config);
    
    /**
     * 获取数据库信息
     * @param databaseId 数据库ID
     * @return 数据库信息
     */
    DatabaseInfo getInfo(const std::string& databaseId);
    
    /**
     * 获取数据库列表
     * @param page 页码（从1开始）
     * @param size 每页大小
     * @param projectId 项目ID（可选）
     * @return 数据库列表
     */
    PageResult<DatabaseInfo> list(int page = 1, int size = 20,
                                 const std::string& projectId = "");
    
    /**
     * 创建数据库连接
     * @param request 数据库创建请求
     * @return 创建的数据库信息
     */
    DatabaseInfo create(const DatabaseCreateRequest& request);
    
    /**
     * 更新数据库配置
     * @param databaseId 数据库ID
     * @param request 更新请求
     * @return 更新后的数据库信息
     */
    DatabaseInfo update(const std::string& databaseId, 
                       const DatabaseUpdateRequest& request);
    
    /**
     * 删除数据库连接
     * @param databaseId 数据库ID
     */
    void deleteDatabase(const std::string& databaseId);
    
    /**
     * 获取数据库表列表
     * @param databaseId 数据库ID
     * @param schema 模式名（可选）
     * @return 表列表
     */
    std::vector<TableInfo> getTables(const std::string& databaseId,
                                    const std::string& schema = "");
    
    /**
     * 获取表结构信息
     * @param databaseId 数据库ID
     * @param tableName 表名
     * @param schema 模式名（可选）
     * @return 表结构信息
     */
    TableSchema getTableSchema(const std::string& databaseId,
                              const std::string& tableName,
                              const std::string& schema = "");
    
    /**
     * 获取表数据预览
     * @param databaseId 数据库ID
     * @param tableName 表名
     * @param limit 限制行数
     * @param schema 模式名（可选）
     * @return 表数据
     */
    QueryResult getTablePreview(const std::string& databaseId,
                               const std::string& tableName,
                               int limit = 100,
                               const std::string& schema = "");
    
    /**
     * 执行SQL查询
     * @param databaseId 数据库ID
     * @param sql SQL语句
     * @param params 参数（可选）
     * @return 查询结果
     */
    QueryResult executeQuery(const std::string& databaseId,
                            const std::string& sql,
                            const Parameters& params = {});
    
    /**
     * 执行SQL更新操作
     * @param databaseId 数据库ID
     * @param sql SQL语句
     * @param params 参数（可选）
     * @return 更新结果
     */
    UpdateResult executeUpdate(const std::string& databaseId,
                              const std::string& sql,
                              const Parameters& params = {});
    
    /**
     * 批量执行SQL操作
     * @param databaseId 数据库ID
     * @param sqls SQL语句列表
     * @return 批量执行结果
     */
    BatchResult executeBatch(const std::string& databaseId,
                            const std::vector<std::string>& sqls);
    
    /**
     * 执行存储过程
     * @param databaseId 数据库ID
     * @param procedureName 存储过程名
     * @param params 参数
     * @return 执行结果
     */
    QueryResult executeProcedure(const std::string& databaseId,
                                const std::string& procedureName,
                                const Parameters& params = {});
    
    /**
     * 开始事务
     * @param databaseId 数据库ID
     * @return 事务ID
     */
    std::string beginTransaction(const std::string& databaseId);
    
    /**
     * 提交事务
     * @param databaseId 数据库ID
     * @param transactionId 事务ID
     */
    void commitTransaction(const std::string& databaseId,
                          const std::string& transactionId);
    
    /**
     * 回滚事务
     * @param databaseId 数据库ID
     * @param transactionId 事务ID
     */
    void rollbackTransaction(const std::string& databaseId,
                            const std::string& transactionId);
    
    /**
     * 在事务中执行SQL
     * @param databaseId 数据库ID
     * @param transactionId 事务ID
     * @param sql SQL语句
     * @param params 参数（可选）
     * @return 执行结果
     */
    QueryResult executeInTransaction(const std::string& databaseId,
                                   const std::string& transactionId,
                                   const std::string& sql,
                                   const Parameters& params = {});
    
    /**
     * 获取数据库统计信息
     * @param databaseId 数据库ID
     * @return 统计信息
     */
    DatabaseStatistics getStatistics(const std::string& databaseId);
    
    /**
     * 获取查询执行计划
     * @param databaseId 数据库ID
     * @param sql SQL语句
     * @return 执行计划
     */
    QueryPlan getQueryPlan(const std::string& databaseId,
                          const std::string& sql);
    
    /**
     * 获取数据库性能指标
     * @param databaseId 数据库ID
     * @param startTime 开始时间
     * @param endTime 结束时间
     * @return 性能指标
     */
    DatabaseMetrics getMetrics(const std::string& databaseId,
                              const std::string& startTime,
                              const std::string& endTime);
    
    /**
     * 导出查询结果
     * @param databaseId 数据库ID
     * @param sql SQL语句
     * @param format 导出格式（csv, json, excel等）
     * @param params 参数（可选）
     * @return 导出的数据
     */
    std::string exportQueryResult(const std::string& databaseId,
                                 const std::string& sql,
                                 const std::string& format = "csv",
                                 const Parameters& params = {});
    
    /**
     * 导入数据到表
     * @param databaseId 数据库ID
     * @param tableName 表名
     * @param data 数据
     * @param format 数据格式
     * @param options 导入选项
     * @return 导入结果
     */
    ImportResult importData(const std::string& databaseId,
                           const std::string& tableName,
                           const std::string& data,
                           const std::string& format = "csv",
                           const ImportOptions& options = {});
    
    /**
     * 创建数据库备份
     * @param databaseId 数据库ID
     * @param backupName 备份名称
     * @return 备份信息
     */
    BackupInfo createBackup(const std::string& databaseId,
                           const std::string& backupName);
    
    /**
     * 恢复数据库备份
     * @param databaseId 数据库ID
     * @param backupId 备份ID
     * @return 恢复结果
     */
    RestoreResult restoreBackup(const std::string& databaseId,
                               const std::string& backupId);
    
    /**
     * 获取备份列表
     * @param databaseId 数据库ID
     * @param page 页码
     * @param size 每页大小
     * @return 备份列表
     */
    PageResult<BackupInfo> getBackups(const std::string& databaseId,
                                     int page = 1, int size = 20);
    
    /**
     * 删除备份
     * @param databaseId 数据库ID
     * @param backupId 备份ID
     */
    void deleteBackup(const std::string& databaseId,
                     const std::string& backupId);
};

} // namespace client
} // namespace dataapi