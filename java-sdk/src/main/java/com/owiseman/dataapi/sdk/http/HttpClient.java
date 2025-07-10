package com.owiseman.dataapi.sdk.http;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.owiseman.dataapi.sdk.auth.AuthenticationProvider;
import com.owiseman.dataapi.sdk.config.ClientConfig;
import com.owiseman.dataapi.sdk.exception.DataApiException;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.*;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.Map;

/**
 * HTTP客户端
 */
public class HttpClient {
    private static final Logger logger = LoggerFactory.getLogger(HttpClient.class);
    
    private final ClientConfig config;
    private final AuthenticationProvider authProvider;
    private final CloseableHttpClient httpClient;
    private final ObjectMapper objectMapper;
    
    public HttpClient(ClientConfig config, AuthenticationProvider authProvider) {
        this.config = config;
        this.authProvider = authProvider;
        this.objectMapper = new ObjectMapper();
        this.httpClient = createHttpClient();
    }
    
    private CloseableHttpClient createHttpClient() {
        RequestConfig requestConfig = RequestConfig.custom()
                .setConnectTimeout((int) config.getConnectTimeout().toMillis())
                .setSocketTimeout((int) config.getReadTimeout().toMillis())
                .build();
        
        return HttpClientBuilder.create()
                .setDefaultRequestConfig(requestConfig)
                .build();
    }
    
    /**
     * GET请求
     */
    public <T> T get(String path, Map<String, String> params, Class<T> responseType) throws DataApiException {
        String url = buildUrl(path, params);
        HttpGet request = new HttpGet(url);
        addHeaders(request);
        
        return executeRequest(request, responseType);
    }
    
    /**
     * GET请求 - 泛型类型
     */
    public <T> T get(String path, Map<String, String> params, TypeReference<T> typeReference) throws DataApiException {
        String url = buildUrl(path, params);
        HttpGet request = new HttpGet(url);
        addHeaders(request);
        
        return executeRequest(request, typeReference);
    }
    
    /**
     * POST请求
     */
    public <T> T post(String path, Object body, Class<T> responseType) throws DataApiException {
        String url = config.getBaseUrl() + path;
        HttpPost request = new HttpPost(url);
        addHeaders(request);
        
        if (body != null) {
            try {
                String jsonBody = objectMapper.writeValueAsString(body);
                request.setEntity(new StringEntity(jsonBody, StandardCharsets.UTF_8));
            } catch (Exception e) {
                throw new DataApiException("序列化请求体失败", e);
            }
        }
        
        return executeRequest(request, responseType);
    }
    
    /**
     * PUT请求
     */
    public <T> T put(String path, Object body, Class<T> responseType) throws DataApiException {
        String url = config.getBaseUrl() + path;
        HttpPut request = new HttpPut(url);
        addHeaders(request);
        
        if (body != null) {
            try {
                String jsonBody = objectMapper.writeValueAsString(body);
                request.setEntity(new StringEntity(jsonBody, StandardCharsets.UTF_8));
            } catch (Exception e) {
                throw new DataApiException("序列化请求体失败", e);
            }
        }
        
        return executeRequest(request, responseType);
    }
    
    /**
     * DELETE请求
     */
    public void delete(String path) throws DataApiException {
        String url = config.getBaseUrl() + path;
        HttpDelete request = new HttpDelete(url);
        addHeaders(request);
        
        executeRequest(request, Void.class);
    }
    
    /**
     * HEAD请求
     */
    public void head(String path) throws DataApiException {
        String url = config.getBaseUrl() + path;
        HttpHead request = new HttpHead(url);
        addHeaders(request);
        
        executeRequest(request, Void.class);
    }
    
    private void addHeaders(HttpRequestBase request) throws DataApiException {
        // 添加默认头部
        config.getDefaultHeaders().forEach(request::addHeader);
        
        // 添加认证头部
        String token = authProvider.getAccessToken();
        if (token != null) {
            request.addHeader("Authorization", "Bearer " + token);
        }
    }
    
    private <T> T executeRequest(HttpRequestBase request, Class<T> responseType) throws DataApiException {
        try {
            if (config.isEnableLogging()) {
                logger.debug("执行请求: {} {}", request.getMethod(), request.getURI());
            }
            
            HttpResponse response = httpClient.execute(request);
            return handleResponse(response, responseType);
        } catch (IOException e) {
            throw new DataApiException("请求执行失败", e);
        }
    }
    
    private <T> T executeRequest(HttpRequestBase request, TypeReference<T> typeReference) throws DataApiException {
        try {
            if (config.isEnableLogging()) {
                logger.debug("执行请求: {} {}", request.getMethod(), request.getURI());
            }
            
            HttpResponse response = httpClient.execute(request);
            return handleResponse(response, typeReference);
        } catch (IOException e) {
            throw new DataApiException("请求执行失败", e);
        }
    }
    
    private <T> T handleResponse(HttpResponse response, Class<T> responseType) throws DataApiException {
        int statusCode = response.getStatusLine().getStatusCode();
        
        try {
            HttpEntity entity = response.getEntity();
            String responseBody = entity != null ? EntityUtils.toString(entity, StandardCharsets.UTF_8) : null;
            
            if (config.isEnableLogging()) {
                logger.debug("响应状态: {}, 响应体: {}", statusCode, responseBody);
            }
            
            if (statusCode >= 200 && statusCode < 300) {
                if (responseType == Void.class || responseBody == null || responseBody.isEmpty()) {
                    return null;
                }
                return objectMapper.readValue(responseBody, responseType);
            } else {
                throw new DataApiException("请求失败", statusCode, responseBody);
            }
        } catch (IOException e) {
            throw new DataApiException("解析响应失败", e);
        }
    }
    
    private <T> T handleResponse(HttpResponse response, TypeReference<T> typeReference) throws DataApiException {
        int statusCode = response.getStatusLine().getStatusCode();
        
        try {
            HttpEntity entity = response.getEntity();
            String responseBody = entity != null ? EntityUtils.toString(entity, StandardCharsets.UTF_8) : null;
            
            if (config.isEnableLogging()) {
                logger.debug("响应状态: {}, 响应体: {}", statusCode, responseBody);
            }
            
            if (statusCode >= 200 && statusCode < 300) {
                if (responseBody == null || responseBody.isEmpty()) {
                    return null;
                }
                return objectMapper.readValue(responseBody, typeReference);
            } else {
                throw new DataApiException("请求失败", statusCode, responseBody);
            }
        } catch (IOException e) {
            throw new DataApiException("解析响应失败", e);
        }
    }
    
    private String buildUrl(String path, Map<String, String> params) {
        StringBuilder url = new StringBuilder(config.getBaseUrl() + path);
        
        if (params != null && !params.isEmpty()) {
            url.append("?");
            params.forEach((key, value) -> {
                try {
                    url.append(URLEncoder.encode(key, StandardCharsets.UTF_8.toString()))
                       .append("=")
                       .append(URLEncoder.encode(value, StandardCharsets.UTF_8.toString()))
                       .append("&");
                } catch (Exception e) {
                    logger.warn("URL编码失败: {}={}", key, value);
                }
            });
            // 移除最后的&
            url.setLength(url.length() - 1);
        }
        
        return url.toString();
    }
    
    /**
     * 关闭HTTP客户端
     */
    public void close() {
        try {
            httpClient.close();
        } catch (IOException e) {
            logger.warn("关闭HTTP客户端失败", e);
        }
    }
}