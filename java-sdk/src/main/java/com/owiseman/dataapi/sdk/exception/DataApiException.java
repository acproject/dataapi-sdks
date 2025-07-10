package com.owiseman.dataapi.sdk.exception;

/**
 * DataAPI异常基类
 */
public class DataApiException extends Exception {
    private final int statusCode;
    private final String responseBody;
    
    public DataApiException(String message) {
        super(message);
        this.statusCode = -1;
        this.responseBody = null;
    }
    
    public DataApiException(String message, Throwable cause) {
        super(message, cause);
        this.statusCode = -1;
        this.responseBody = null;
    }
    
    public DataApiException(String message, int statusCode, String responseBody) {
        super(message + " (HTTP " + statusCode + ")");
        this.statusCode = statusCode;
        this.responseBody = responseBody;
    }
    
    public int getStatusCode() {
        return statusCode;
    }
    
    public String getResponseBody() {
        return responseBody;
    }
    
    public boolean hasHttpStatus() {
        return statusCode > 0;
    }
}