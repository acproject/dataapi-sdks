#pragma once

#include "CommonTypes.h"

namespace dataapi {

// Forward declarations
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

// JSON序列化/反序列化函数声明
void to_json(Json& j, const AiProvider& a);
void from_json(const Json& j, AiProvider& a);

void to_json(Json& j, const AiProviderTestResult& a);
void from_json(const Json& j, AiProviderTestResult& a);

void to_json(Json& j, const AiInvokeRequest& a);
void from_json(const Json& j, AiInvokeRequest& a);

void to_json(Json& j, const AiInvokeResponse& a);
void from_json(const Json& j, AiInvokeResponse& a);

void to_json(Json& j, const AiProviderConfig& a);
void from_json(const Json& j, AiProviderConfig& a);

void to_json(Json& j, const AiServiceRequest& a);
void from_json(const Json& j, AiServiceRequest& a);

void to_json(Json& j, const AiServiceResponse& a);
void from_json(const Json& j, AiServiceResponse& a);

} // namespace dataapi