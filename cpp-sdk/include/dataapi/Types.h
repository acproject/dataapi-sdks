#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <functional>
#include <chrono>
#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>

namespace dataapi {

// 基础类型定义
using Json = nlohmann::json;
using Headers = std::map<std::string, std::string>;
using Parameters = std::map<std::string, std::string>;
using Timestamp = std::chrono::system_clock::time_point;

// Forward declarations for AI types
struct AiProvider;
struct AiProviderConfig;
struct AiProviderTestResult;
struct AiProviderCreateRequest;
struct AiProviderUpdateRequest;
struct AiServiceRequest;
struct AiServiceResponse;
struct AiStreamResponse;
struct AiModel;
struct AiInvocationHistory;
struct AiUsageStatistics;
struct AiQuotaInfo;
struct TextGenerationOptions;

// Forward declarations for Project types
struct SysProject;
struct ProjectCreateRequest;
struct ProjectUpdateRequest;
struct ProjectMember;
struct ProjectMemberRequest;
struct ProjectPermission;
struct ProjectConfig;
struct ProjectStatistics;
struct ProjectActivity;
struct ImportResult;

// Forward declarations for User types
struct SysUser;
struct UserCreateRequest;
struct UserUpdateRequest;
struct UserRole;
struct UserPermission;
struct UserPreferences;
struct UserActivity;
struct LoginHistory;
struct UserSession;
struct UserStatistics;

// Forward declarations for Database types
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

// Forward declarations for Workflow types
struct WorkflowExecutionResult;
struct WorkflowExecutionStatus;
struct WorkflowValidationResult;
struct WorkflowStatistics;
struct WorkflowVersion;
struct WorkflowVersionRequest;

/**
 * HTTP方法枚举
 */
enum class HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    PATCH
};

/**
 * 认证类型枚举
 */
enum class AuthenticationType {
    BEARER_TOKEN,
    API_KEY,
    BASIC_AUTH,
    OAUTH2
};

/**
 * 工作流状态枚举
 */
enum class WorkflowStatus {
    DRAFT,
    ACTIVE,
    INACTIVE,
    ARCHIVED
};

/**
 * 执行状态枚举
 */
enum class ExecutionStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

/**
 * API响应结构
 */
template<typename T>
struct ApiResponse {
    bool success;
    std::optional<T> data;
    std::optional<std::string> message;
    std::optional<std::string> code;
    std::optional<std::string> timestamp;
};

/**
 * 分页结果结构
 */
template<typename T>
struct PageResult {
    std::vector<T> content;
    int pageNumber;
    int pageSize;
    long totalElements;
    int totalPages;
    bool first;
    bool last;
    bool empty;
};

/**
 * HTTP请求配置
 */
struct HttpRequestConfig {
    HttpMethod method;
    std::string url;
    Headers headers;
    Parameters params;
    Json data;
    int timeout = 30000; // 默认30秒
};

/**
 * HTTP响应结构
 */
// HttpResponse is defined in dataapi/http/HttpClient.h

/**
 * Token响应结构
 */
struct TokenResponse {
    std::string accessToken;
    std::optional<std::string> refreshToken;
    std::optional<int> expiresIn;
    std::optional<std::string> tokenType;
};

/**
 * API版本信息
 */
struct ApiVersion {
    std::string version;
    std::string buildTime;
    std::string gitCommit;
};

/**
 * 健康状态
 */
struct HealthStatus {
    std::string status;
    std::string message;
    std::map<std::string, std::string> details;
};

// AI Provider structure
struct AiProvider {
    std::string id;
    std::string name;
    std::string type;
    std::string status;
    Json config;
    Timestamp createdAt;
    Timestamp updatedAt;
};

// AI Provider Config
struct AiProviderConfig {
    std::string apiKey;
    std::string endpoint;
    Json settings;
};

// AI Provider Test Result
struct AiProviderTestResult {
    bool success;
    std::string message;
    Json details;
};

// AI Provider Create Request
struct AiProviderCreateRequest {
    std::string name;
    std::string type;
    AiProviderConfig config;
};

// AI Provider Update Request
struct AiProviderUpdateRequest {
    std::string name;
    AiProviderConfig config;
};

// AI Service Request
struct AiServiceRequest {
    std::string prompt;
    std::string model;
    Json parameters;
};

// AI Service Response
struct AiServiceResponse {
    std::string response;
    Json metadata;
    std::string requestId;
};

// AI Stream Response
struct AiStreamResponse {
    std::string chunk;
    bool isComplete;
    Json metadata;
};

// AI Model
struct AiModel {
    std::string id;
    std::string name;
    std::string provider;
    Json capabilities;
};

// AI Invocation History
struct AiInvocationHistory {
    std::string id;
    std::string prompt;
    std::string response;
    Timestamp timestamp;
    Json metadata;
};

// AI Usage Statistics
struct AiUsageStatistics {
    int totalRequests;
    int totalTokens;
    double totalCost;
    Json breakdown;
};

// AI Quota Info
struct AiQuotaInfo {
    int remainingRequests;
    int remainingTokens;
    Timestamp resetTime;
};

// AI Quota Settings
struct AiQuotaSettings {
    int maxRequests;
    int maxTokens;
    std::string period;
    Json restrictions;
};

// Text Generation Options
struct TextGenerationOptions {
    double temperature = 0.7;
    int maxTokens = 1000;
    double topP = 1.0;
    Json additionalParams;
};

// Text Generation Result
struct TextGenerationResult {
    std::string text;
    std::string model;
    Json usage;
    Json metadata;
};

// Chat Completion Options
struct ChatCompletionOptions {
    double temperature = 0.7;
    int maxTokens = 1000;
    double topP = 1.0;
    bool stream = false;
    Json additionalParams;
};

// Image Generation Options
struct ImageGenerationOptions {
    std::string size = "1024x1024";
    std::string quality = "standard";
    int n = 1;
    Json additionalParams;
};

// Speech to Text Options
struct SpeechToTextOptions {
    std::string language;
    std::string model;
    double temperature = 0.0;
    Json additionalParams;
};

// Text to Speech Options
struct TextToSpeechOptions {
    std::string voice;
    std::string model;
    double speed = 1.0;
    Json additionalParams;
};

// Embedding Options
struct EmbeddingOptions {
    std::string model;
    int dimensions = 0;
    Json additionalParams;
};

// Embedding Result
struct EmbeddingResult {
    std::vector<std::vector<double>> embeddings;
    std::string model;
    Json usage;
    Json metadata;
};

// Image Analysis Options
struct ImageAnalysisOptions {
    std::string model;
    int maxTokens = 300;
    Json additionalParams;
};

// Image Analysis Result
struct ImageAnalysisResult {
    std::string description;
    std::vector<Json> objects;
    std::string model;
    Json metadata;
};

// Chat Completion Result
struct ChatCompletionResult {
    std::string content;
    std::string role;
    std::string finishReason;
    Json usage;
    Json metadata;
};

// Image Generation Result
struct ImageGenerationResult {
    std::vector<std::string> urls;
    std::string model;
    Json metadata;
};

// Speech to Text Result
struct SpeechToTextResult {
    std::string text;
    std::string language;
    Json metadata;
};

// Text to Speech Result
struct TextToSpeechResult {
    std::string audioUrl;
    std::string format;
    Json metadata;
};

// Sentiment Analysis Options
struct SentimentAnalysisOptions {
    std::string language;
    std::string model;
    Json additionalParams;
};

// Sentiment Analysis Result
struct SentimentAnalysisResult {
    std::string sentiment;
    double confidence;
    Json details;
    Json metadata;
};

// Entity Recognition Options
struct EntityRecognitionOptions {
    std::string language;
    std::string model;
    std::vector<std::string> entityTypes;
    Json additionalParams;
};

// Entity Recognition Result
struct EntityRecognitionResult {
    std::vector<Json> entities;
    Json metadata;
};

// Text Classification Options
struct TextClassificationOptions {
    std::string model;
    std::vector<std::string> labels;
    Json additionalParams;
};

// Text Classification Result
struct TextClassificationResult {
    std::vector<Json> classifications;
    Json metadata;
};

// Classification Result (alias for TextClassificationResult)
using ClassificationResult = TextClassificationResult;

// Text Summarization Options
struct TextSummarizationOptions {
    int maxLength = 150;
    int minLength = 30;
    std::string model;
    Json additionalParams;
};

// Text Summarization Result
struct TextSummarizationResult {
    std::string summary;
    std::string model;
    Json metadata;
};

// Summarization Result (alias for TextSummarizationResult)
using SummarizationResult = TextSummarizationResult;

// Translation Options
struct TranslationOptions {
    std::string sourceLanguage;
    std::string targetLanguage;
    std::string model;
    Json additionalParams;
};

// Translation Result
struct TranslationResult {
    std::string translatedText;
    std::string sourceLanguage;
    std::string targetLanguage;
    Json metadata;
};

// API Key
struct ApiKey {
    std::string id;
    std::string name;
    std::string key;
    std::string userId;
    std::vector<std::string> permissions;
    Timestamp createdAt;
    Timestamp expiresAt;
    bool active;
};

// API Key Create Request
struct ApiKeyCreateRequest {
    std::string name;
    std::vector<std::string> permissions;
    std::optional<Timestamp> expiresAt;
};

// API Key Update Request
struct ApiKeyUpdateRequest {
    std::string name;
    std::vector<std::string> permissions;
    std::optional<Timestamp> expiresAt;
    bool active;
};

// Audit Log
struct AuditLog {
    std::string id;
    std::string userId;
    std::string action;
    std::string resource;
    Json details;
    Timestamp timestamp;
    std::string ipAddress;
    std::string userAgent;
};

// System Settings
struct SystemSettings {
    std::string key;
    Json value;
    std::string description;
    Timestamp updatedAt;
};

// System Settings Update Request
struct SystemSettingsUpdateRequest {
    std::string key;
    Json value;
    std::string description;
};

// Notification
struct Notification {
    std::string id;
    std::string userId;
    std::string title;
    std::string message;
    std::string type;
    bool read;
    Timestamp createdAt;
};

// Notification Create Request
struct NotificationCreateRequest {
    std::string userId;
    std::string title;
    std::string message;
    std::string type;
};

// Two Factor Auth Setup
struct TwoFactorAuthSetup {
    std::string secret;
    std::string qrCodeUrl;
    std::vector<std::string> backupCodes;
    bool enabled;
};

// Two Factor Auth Verify Request
struct TwoFactorAuthVerifyRequest {
    std::string code;
    std::string secret;
};

// Password Reset Request
struct PasswordResetRequest {
    std::string email;
    std::string newPassword;
    std::string resetToken;
};

// Password Change Request
struct PasswordChangeRequest {
    std::string currentPassword;
    std::string newPassword;
};

// Login Request
struct LoginRequest {
    std::string username;
    std::string password;
    std::optional<std::string> twoFactorCode;
};

// System User
struct SysUser {
    std::string id;
    std::string username;
    std::string email;
    std::string status;
    Timestamp createdAt;
    Timestamp lastLoginAt;
};

// Login Response
struct LoginResponse {
    std::string token;
    std::string refreshToken;
    SysUser user;
    Timestamp expiresAt;
};

// System Project
struct SysProject {
    std::string id;
    std::string name;
    std::string description;
    std::string status;
    std::string ownerId;
    Timestamp createdAt;
    Timestamp updatedAt;
};

// Project Create Request
struct ProjectCreateRequest {
    std::string name;
    std::string description;
    Json settings;
};

// Project Update Request
struct ProjectUpdateRequest {
    std::string name;
    std::string description;
    Json settings;
};

// Project Member
struct ProjectMember {
    std::string userId;
    std::string role;
    Timestamp joinedAt;
};

// Project Member Request
struct ProjectMemberRequest {
    std::string userId;
    std::string role;
};

// Project Permission
struct ProjectPermission {
    std::string action;
    bool allowed;
};

// Project Config
struct ProjectConfig {
    Json settings;
    Json permissions;
};

// Project Statistics
struct ProjectStatistics {
    int totalWorkflows;
    int totalExecutions;
    int totalMembers;
    Json details;
};

// Project Activity
struct ProjectActivity {
    std::string id;
    std::string action;
    std::string userId;
    Timestamp timestamp;
    Json details;
};

// Project Template
struct ProjectTemplate {
    std::string id;
    std::string name;
    std::string description;
    std::string type;
    Json content;
    Json styles;
    Json pages;
    std::string preview;
    bool isDefault;
    bool isPublic;
    std::vector<std::string> tags;
    std::string templateKey;
    std::string userId;
    Timestamp createdAt;
    Timestamp updatedAt;
};

// Import Result
struct ImportResult {
    bool success;
    std::string message;
    Json details;
};

// User Create Request
struct UserCreateRequest {
    std::string username;
    std::string email;
    std::string password;
    Json profile;
};

// User Update Request
struct UserUpdateRequest {
    std::string email;
    Json profile;
};

// User Role
struct UserRole {
    std::string id;
    std::string name;
    std::vector<std::string> permissions;
};

// User Permission
struct UserPermission {
    std::string resource;
    std::string action;
    bool allowed;
};

// User Preferences
struct UserPreferences {
    std::string language;
    std::string theme;
    Json settings;
};

// User Activity
struct UserActivity {
    std::string id;
    std::string action;
    Timestamp timestamp;
    Json details;
};

// Login History
struct LoginHistory {
    std::string id;
    Timestamp loginTime;
    std::string ipAddress;
    std::string userAgent;
};

// User Session
struct UserSession {
    std::string id;
    std::string token;
    Timestamp expiresAt;
    bool active;
};

// User Statistics
struct UserStatistics {
    int totalLogins;
    Timestamp lastActivity;
    Json details;
};

// Database Connection Result
struct DatabaseConnectionResult {
    bool success;
    std::string message;
    Json details;
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

// Workflow Execution Result
struct WorkflowExecutionResult {
    std::string executionId;
    std::string status;
    Json result;
    Timestamp startTime;
    Timestamp endTime;
    Json metadata;
};

// Workflow Execution Status
struct WorkflowExecutionStatus {
    std::string executionId;
    std::string status;
    double progress;
    Json currentStep;
};

// Workflow Validation Result
struct WorkflowValidationResult {
    bool isValid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

// Workflow Statistics
struct WorkflowStatistics {
    int totalExecutions;
    int successfulExecutions;
    int failedExecutions;
    double averageExecutionTime;
};

// Workflow Version
struct WorkflowVersion {
    std::string id;
    int version;
    Json definition;
    Timestamp createdAt;
};

// Workflow Version Request
struct WorkflowVersionRequest {
    Json definition;
    std::string description;
};

/**
 * 工作流结构
 */
struct SysWorkflow {
    std::optional<std::string> id;
    std::string name;
    std::optional<std::string> description;
    std::string definition;
    std::string projectId;
    std::string userId;
    std::optional<std::string> createTime;
    std::optional<std::string> updateTime;
    std::optional<WorkflowStatus> status;
    std::optional<int> version;
};

/**
 * 工作流创建请求
 */
struct WorkflowCreateRequest {
    std::string name;
    std::optional<std::string> description;
    std::string definition;
    std::string projectId;
    std::string userId;
};

/**
 * 工作流更新请求
 */
struct WorkflowUpdateRequest {
    std::string id;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> definition;
    std::optional<std::string> projectId;
    std::optional<std::string> userId;
    std::optional<std::string> updateTime;
};

/**
 * 工作流执行请求
 */
struct WorkflowExecutionRequest {
    std::string workflowId;
    std::string projectId;
    std::string userId;
    std::string workflowDefinition;
    std::optional<Json> initialData;
    std::optional<int> timeoutMinutes;
};

/**
 * 项目结构
 */
struct Project {
    std::string id;
    std::string name;
    std::optional<std::string> description;
    std::string createTime;
    std::string updateTime;
    std::string userId;
    std::string status;
};

/**
 * 用户结构
 */
struct User {
    std::string id;
    std::string username;
    std::string email;
    std::optional<std::string> displayName;
    std::string createTime;
    std::string updateTime;
    std::string status;
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

/**
 * AI调用请求
 */
struct AiInvokeRequest {
    std::string configId;
    Json input;
    std::optional<Json> parameters;
};

/**
 * AI调用响应
 */
struct AiInvokeResponse {
    Json output;
    std::optional<Json> usage;
    std::optional<std::string> model;
    std::optional<std::string> finishReason;
};

/**
 * 错误响应结构
 */
struct ErrorResponse {
    std::string code;
    std::string message;
    std::optional<Json> details;
    std::optional<std::string> timestamp;
};

// JSON序列化/反序列化辅助函数
void to_json(Json& j, const SysWorkflow& w);
void from_json(const Json& j, SysWorkflow& w);

void to_json(Json& j, const WorkflowCreateRequest& w);
void from_json(const Json& j, WorkflowCreateRequest& w);

void to_json(Json& j, const WorkflowUpdateRequest& w);
void from_json(const Json& j, WorkflowUpdateRequest& w);

void to_json(Json& j, const WorkflowExecutionRequest& w);
void from_json(const Json& j, WorkflowExecutionRequest& w);

void to_json(Json& j, const Project& p);
void from_json(const Json& j, Project& p);

void to_json(Json& j, const User& u);
void from_json(const Json& j, User& u);

void to_json(Json& j, const SqlExecutionResult& s);
void from_json(const Json& j, SqlExecutionResult& s);

void to_json(Json& j, const AiInvokeRequest& a);
void from_json(const Json& j, AiInvokeRequest& a);

void to_json(Json& j, const AiInvokeResponse& a);
void from_json(const Json& j, AiInvokeResponse& a);

void to_json(Json& j, const WorkflowExecutionResult& w);
void from_json(const Json& j, WorkflowExecutionResult& w);

void to_json(Json& j, const SysProject& p);
void from_json(const Json& j, SysProject& p);

void to_json(Json& j, const SysUser& u);
void from_json(const Json& j, SysUser& u);

void to_json(Json& j, const AiProvider& a);
void from_json(const Json& j, AiProvider& a);

void to_json(Json& j, const DatabaseConnectionResult& d);
void from_json(const Json& j, DatabaseConnectionResult& d);

void to_json(Json& j, const AiProviderTestResult& a);
void from_json(const Json& j, AiProviderTestResult& a);

void to_json(Json& j, const UserCreateRequest& u);
void from_json(const Json& j, UserCreateRequest& u);

void to_json(Json& j, const UserUpdateRequest& u);
void from_json(const Json& j, UserUpdateRequest& u);

void to_json(Json& j, const ProjectCreateRequest& p);
void from_json(const Json& j, ProjectCreateRequest& p);

void to_json(Json& j, const ProjectUpdateRequest& p);
void from_json(const Json& j, ProjectUpdateRequest& p);

template<typename T>
void to_json(Json& j, const PageResult<T>& p) {
    j = Json{
        {"content", p.content},
        {"pageNumber", p.pageNumber},
        {"pageSize", p.pageSize},
        {"totalElements", p.totalElements},
        {"totalPages", p.totalPages},
        {"first", p.first},
        {"last", p.last},
        {"empty", p.empty}
    };
}

template<typename T>
void from_json(const Json& j, PageResult<T>& p) {
    j.at("content").get_to(p.content);
    j.at("pageNumber").get_to(p.pageNumber);
    j.at("pageSize").get_to(p.pageSize);
    j.at("totalElements").get_to(p.totalElements);
    j.at("totalPages").get_to(p.totalPages);
    j.at("first").get_to(p.first);
    j.at("last").get_to(p.last);
    j.at("empty").get_to(p.empty);
}

} // namespace dataapi