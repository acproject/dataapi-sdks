#pragma once

#include "CommonTypes.h"

namespace dataapi {

// Forward declarations
struct DatabaseConnectionResult;
struct DatabaseConfig;
struct DatabaseInfo;
struct DatabaseCreateRequest;
struct DatabaseUpdateRequest;
struct TableInfo;
struct TableSchema;
struct QueryResult;
struct UpdateResult;
struct BatchResult;

// Database Connection Result
struct DatabaseConnectionResult {
    bool success;
    std::string message;
    std::optional<Json> details;
};

// Database Config
struct DatabaseConfig {
    std::string type;
    std::string host;
    int port;
    std::string database;
    std::string username;
    std::string password;
    Json options;
};

// Database Info
struct DatabaseInfo {
    std::string id;
    std::string name;
    std::string type;
    std::string status;
    Timestamp createdAt;
};

// Database Create Request
struct DatabaseCreateRequest {
    std::string name;
    std::string type;
    DatabaseConfig config;
};

// Database Update Request
struct DatabaseUpdateRequest {
    std::string name;
    DatabaseConfig config;
};

// Table Info
struct TableInfo {
    std::string name;
    std::string schema;
    int rowCount;
    Json metadata;
};

// Table Schema
struct TableSchema {
    std::string tableName;
    std::vector<Json> columns;
    std::vector<Json> indexes;
};

// Query Result
struct QueryResult {
    std::vector<Json> rows;
    std::vector<std::string> columns;
    int totalRows;
    Json metadata;
};

// Update Result
struct UpdateResult {
    int affectedRows;
    Json metadata;
};

// Batch Result
struct BatchResult {
    std::vector<Json> results;
    int successCount;
    int errorCount;
    Json errors;
};

// Backup Info
struct BackupInfo {
    std::string id;
    std::string databaseId;
    std::string name;
    std::string description;
    std::string status;
    std::string filePath;
    long fileSize;
    Timestamp createdAt;
    Json metadata;
};

// Restore Result
struct RestoreResult {
    bool success;
    std::string message;
    std::string backupId;
    Timestamp restoredAt;
    Json details;
};

// Query Plan
struct QueryPlan {
    std::string query;
    Json plan;
    double estimatedCost;
    int estimatedRows;
    Json metadata;
};

// Database Metrics
struct DatabaseMetrics {
    int connectionCount;
    double cpuUsage;
    double memoryUsage;
    double diskUsage;
    int queryCount;
    double averageQueryTime;
    Json details;
};

// Database Statistics
struct DatabaseStatistics {
    int totalTables;
    int totalRows;
    long totalSize;
    int totalConnections;
    double averageQueryTime;
    int queryCount;
    Json performanceMetrics;
    Json details;
};

// Import Options
struct ImportOptions {
    std::string format;
    std::string delimiter;
    bool hasHeader;
    std::string encoding;
    Json mappings;
    Json additionalOptions;
};

/**
 * SQL执行结果
 */
struct SqlExecutionResult {
    bool success;
    std::optional<int> rowsAffected;
    std::optional<std::vector<Json>> data;
    std::optional<std::vector<std::string>> columns;
    std::optional<int> executionTime;
    std::optional<std::string> message;
};

// JSON序列化/反序列化函数声明
void to_json(Json& j, const DatabaseConnectionResult& d);
void from_json(const Json& j, DatabaseConnectionResult& d);

void to_json(Json& j, const DatabaseConfig& d);
void from_json(const Json& j, DatabaseConfig& d);

void to_json(Json& j, const DatabaseInfo& d);
void from_json(const Json& j, DatabaseInfo& d);

void to_json(Json& j, const QueryResult& q);
void from_json(const Json& j, QueryResult& q);

void to_json(Json& j, const SqlExecutionResult& s);
void from_json(const Json& j, SqlExecutionResult& s);

} // namespace dataapi