#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Types.h"
#include "../http/HttpClient.h"

namespace dataapi {
namespace client {

/**
 * AI提供者客户端类
 * 提供AI服务相关的API操作
 */
class AiProviderClient {
private:
    std::shared_ptr<http::HttpClient> httpClient;
    
public:
    /**
     * 构造函数
     * @param httpClient HTTP客户端
     */
    explicit AiProviderClient(std::shared_ptr<http::HttpClient> httpClient);
    
    /**
     * 析构函数
     */
    ~AiProviderClient() = default;
    
    /**
     * 禁用拷贝构造和赋值
     */
    AiProviderClient(const AiProviderClient&) = delete;
    AiProviderClient& operator=(const AiProviderClient&) = delete;
    
    /**
     * 启用移动构造和赋值
     */
    AiProviderClient(AiProviderClient&& other) noexcept = default;
    AiProviderClient& operator=(AiProviderClient&& other) noexcept = default;
    
    /**
     * 测试AI提供者配置
     * @param config AI提供者配置
     * @return 测试结果
     */
    AiProviderTestResult testConfiguration(const AiProviderConfig& config);
    
    /**
     * 获取AI提供者列表
     * @param page 页码（从1开始）
     * @param size 每页大小
     * @param type 提供者类型（可选）
     * @return AI提供者列表
     */
    PageResult<AiProvider> list(int page = 1, int size = 20,
                               const std::string& type = "");
    
    /**
     * 根据ID获取AI提供者详情
     * @param id AI提供者ID
     * @return AI提供者详情
     */
    AiProvider getById(const std::string& id);
    
    /**
     * 创建AI提供者
     * @param request AI提供者创建请求
     * @return 创建的AI提供者
     */
    AiProvider create(const AiProviderCreateRequest& request);
    
    /**
     * 更新AI提供者
     * @param id AI提供者ID
     * @param request 更新请求
     * @return 更新后的AI提供者
     */
    AiProvider update(const std::string& id, const AiProviderUpdateRequest& request);
    
    /**
     * 删除AI提供者
     * @param id AI提供者ID
     */
    void deleteProvider(const std::string& id);
    
    /**
     * 调用AI服务
     * @param providerId AI提供者ID
     * @param request AI服务请求
     * @return AI服务响应
     */
    AiServiceResponse invoke(const std::string& providerId,
                            const AiServiceRequest& request);
    
    /**
     * 流式调用AI服务
     * @param providerId AI提供者ID
     * @param request AI服务请求
     * @param callback 流式响应回调函数
     */
    void invokeStream(const std::string& providerId,
                     const AiServiceRequest& request,
                     std::function<void(const AiStreamResponse&)> callback);
    
    /**
     * 批量调用AI服务
     * @param providerId AI提供者ID
     * @param requests AI服务请求列表
     * @return AI服务响应列表
     */
    std::vector<AiServiceResponse> invokeBatch(const std::string& providerId,
                                              const std::vector<AiServiceRequest>& requests);
    
    /**
     * 获取AI模型列表
     * @param providerId AI提供者ID
     * @return 模型列表
     */
    std::vector<AiModel> getModels(const std::string& providerId);
    
    /**
     * 获取AI模型详情
     * @param providerId AI提供者ID
     * @param modelId 模型ID
     * @return 模型详情
     */
    AiModel getModel(const std::string& providerId, const std::string& modelId);
    
    /**
     * 获取AI服务调用历史
     * @param providerId AI提供者ID
     * @param page 页码
     * @param size 每页大小
     * @param startTime 开始时间（可选）
     * @param endTime 结束时间（可选）
     * @return 调用历史列表
     */
    PageResult<AiInvocationHistory> getInvocationHistory(
        const std::string& providerId,
        int page = 1, int size = 20,
        const std::string& startTime = "",
        const std::string& endTime = "");
    
    /**
     * 获取AI服务使用统计
     * @param providerId AI提供者ID
     * @param startTime 开始时间
     * @param endTime 结束时间
     * @return 使用统计
     */
    AiUsageStatistics getUsageStatistics(const std::string& providerId,
                                         const std::string& startTime,
                                         const std::string& endTime);
    
    /**
     * 获取AI服务配额信息
     * @param providerId AI提供者ID
     * @return 配额信息
     */
    AiQuotaInfo getQuotaInfo(const std::string& providerId);
    
    /**
     * 设置AI服务配额
     * @param providerId AI提供者ID
     * @param quota 配额设置
     */
    void setQuota(const std::string& providerId, const AiQuotaSettings& quota);
    
    /**
     * 文本生成
     * @param providerId AI提供者ID
     * @param prompt 提示文本
     * @param options 生成选项
     * @return 生成结果
     */
    TextGenerationResult generateText(const std::string& providerId,
                                     const std::string& prompt,
                                     const TextGenerationOptions& options = {});
    
    /**
     * 流式文本生成
     * @param providerId AI提供者ID
     * @param prompt 提示文本
     * @param callback 流式响应回调
     * @param options 生成选项
     */
    void generateTextStream(const std::string& providerId,
                           const std::string& prompt,
                           std::function<void(const std::string&)> callback,
                           const TextGenerationOptions& options = {});
    
    /**
     * 图像生成
     * @param providerId AI提供者ID
     * @param prompt 提示文本
     * @param options 生成选项
     * @return 生成结果
     */
    ImageGenerationResult generateImage(const std::string& providerId,
                                       const std::string& prompt,
                                       const ImageGenerationOptions& options = {});
    
    /**
     * 图像分析
     * @param providerId AI提供者ID
     * @param imageData 图像数据（base64编码）
     * @param prompt 分析提示（可选）
     * @param options 分析选项
     * @return 分析结果
     */
    ImageAnalysisResult analyzeImage(const std::string& providerId,
                                    const std::string& imageData,
                                    const std::string& prompt = "",
                                    const ImageAnalysisOptions& options = {});
    
    /**
     * 语音转文本
     * @param providerId AI提供者ID
     * @param audioData 音频数据（base64编码）
     * @param options 转换选项
     * @return 转换结果
     */
    SpeechToTextResult speechToText(const std::string& providerId,
                                   const std::string& audioData,
                                   const SpeechToTextOptions& options = {});
    
    /**
     * 文本转语音
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param options 转换选项
     * @return 转换结果
     */
    TextToSpeechResult textToSpeech(const std::string& providerId,
                                   const std::string& text,
                                   const TextToSpeechOptions& options = {});
    
    /**
     * 文本嵌入
     * @param providerId AI提供者ID
     * @param texts 文本列表
     * @param options 嵌入选项
     * @return 嵌入结果
     */
    EmbeddingResult getEmbeddings(const std::string& providerId,
                                 const std::vector<std::string>& texts,
                                 const EmbeddingOptions& options = {});
    
    /**
     * 文本分类
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param categories 分类类别
     * @param options 分类选项
     * @return 分类结果
     */
    ClassificationResult classifyText(const std::string& providerId,
                                     const std::string& text,
                                     const std::vector<std::string>& categories,
                                     const TextClassificationOptions& options = {});
    
    /**
     * 情感分析
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param options 分析选项
     * @return 分析结果
     */
    SentimentAnalysisResult analyzeSentiment(const std::string& providerId,
                                            const std::string& text,
                                            const SentimentAnalysisOptions& options = {});
    
    /**
     * 实体识别
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param options 识别选项
     * @return 识别结果
     */
    EntityRecognitionResult recognizeEntities(const std::string& providerId,
                                             const std::string& text,
                                             const EntityRecognitionOptions& options = {});
    
    /**
     * 文本摘要
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param options 摘要选项
     * @return 摘要结果
     */
    SummarizationResult summarizeText(const std::string& providerId,
                                     const std::string& text,
                                     const TextSummarizationOptions& options = {});
    
    /**
     * 文本翻译
     * @param providerId AI提供者ID
     * @param text 文本内容
     * @param targetLanguage 目标语言
     * @param sourceLanguage 源语言（可选，自动检测）
     * @param options 翻译选项
     * @return 翻译结果
     */
    TranslationResult translateText(const std::string& providerId,
                                   const std::string& text,
                                   const std::string& targetLanguage,
                                   const std::string& sourceLanguage = "",
                                   const TranslationOptions& options = {});
};

} // namespace client
} // namespace dataapi