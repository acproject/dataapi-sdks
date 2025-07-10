package com.owiseman.dataapi.sdk.exception;

/**
 * 认证异常
 */
public class AuthenticationException extends DataApiException {
    
    public AuthenticationException(String message) {
        super(message);
    }
    
    public AuthenticationException(String message, Throwable cause) {
        super(message, cause);
    }
    
    public AuthenticationException(String message, int statusCode, String responseBody) {
        super(message, statusCode, responseBody);
    }
}