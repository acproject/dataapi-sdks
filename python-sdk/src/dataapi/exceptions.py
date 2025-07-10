"""Exception classes for DataAPI SDK."""

from typing import Any, Dict, Optional


class DataAPIError(Exception):
    """Base exception for all DataAPI errors.
    
    Args:
        message: Error message
        status_code: HTTP status code if applicable
        error_code: DataAPI specific error code
        details: Additional error details
    """
    
    def __init__(
        self,
        message: str,
        status_code: Optional[int] = None,
        error_code: Optional[str] = None,
        details: Optional[Dict[str, Any]] = None,
    ) -> None:
        super().__init__(message)
        self.message = message
        self.status_code = status_code
        self.error_code = error_code
        self.details = details or {}
    
    def __str__(self) -> str:
        """String representation of the error."""
        parts = [self.message]
        if self.status_code:
            parts.append(f"Status: {self.status_code}")
        if self.error_code:
            parts.append(f"Code: {self.error_code}")
        return " | ".join(parts)
    
    def __repr__(self) -> str:
        """Detailed representation of the error."""
        return (
            f"{self.__class__.__name__}("
            f"message={self.message!r}, "
            f"status_code={self.status_code}, "
            f"error_code={self.error_code!r}, "
            f"details={self.details!r}"
            f")"
        )
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert error to dictionary.
        
        Returns:
            Dictionary representation of the error
        """
        return {
            "type": self.__class__.__name__,
            "message": self.message,
            "status_code": self.status_code,
            "error_code": self.error_code,
            "details": self.details,
        }


class AuthenticationError(DataAPIError):
    """Raised when authentication fails.
    
    This typically occurs when:
    - API key is invalid or missing
    - OAuth2 token is expired or invalid
    - Authentication credentials are malformed
    """
    pass


class AuthorizationError(DataAPIError):
    """Raised when authorization fails.
    
    This typically occurs when:
    - User doesn't have permission to access a resource
    - API key doesn't have required scopes
    - Resource access is restricted
    """
    pass


class ValidationError(DataAPIError):
    """Raised when request validation fails.
    
    This typically occurs when:
    - Required fields are missing
    - Field values are invalid
    - Request format is incorrect
    """
    pass


class NotFoundError(DataAPIError):
    """Raised when a requested resource is not found.
    
    This typically occurs when:
    - Database, table, or record doesn't exist
    - Workflow or execution is not found
    - User or project doesn't exist
    """
    pass


class ConflictError(DataAPIError):
    """Raised when a request conflicts with current state.
    
    This typically occurs when:
    - Resource already exists
    - Concurrent modification detected
    - Business rule violation
    """
    pass


class RateLimitError(DataAPIError):
    """Raised when rate limit is exceeded.
    
    This typically occurs when:
    - Too many requests in a time window
    - API quota exceeded
    - Throttling is active
    """
    
    def __init__(
        self,
        message: str,
        status_code: Optional[int] = None,
        error_code: Optional[str] = None,
        details: Optional[Dict[str, Any]] = None,
        retry_after: Optional[int] = None,
    ) -> None:
        super().__init__(message, status_code, error_code, details)
        self.retry_after = retry_after


class ServerError(DataAPIError):
    """Raised when server encounters an internal error.
    
    This typically occurs when:
    - Internal server error (5xx status codes)
    - Service is temporarily unavailable
    - Database connection issues
    """
    pass


class TimeoutError(DataAPIError):
    """Raised when a request times out.
    
    This typically occurs when:
    - Request takes longer than configured timeout
    - Server is slow to respond
    - Network connectivity issues
    """
    pass


class ConnectionError(DataAPIError):
    """Raised when connection to the API fails.
    
    This typically occurs when:
    - Network connectivity issues
    - DNS resolution fails
    - SSL/TLS handshake fails
    """
    pass


def create_error_from_response(
    status_code: int,
    response_data: Optional[Dict[str, Any]] = None,
) -> DataAPIError:
    """Create appropriate error from HTTP response.
    
    Args:
        status_code: HTTP status code
        response_data: Response data from API
        
    Returns:
        Appropriate DataAPIError subclass
    """
    response_data = response_data or {}
    message = response_data.get("message", f"HTTP {status_code} error")
    error_code = response_data.get("error_code")
    details = response_data.get("details", {})
    
    if status_code == 401:
        return AuthenticationError(message, status_code, error_code, details)
    elif status_code == 403:
        return AuthorizationError(message, status_code, error_code, details)
    elif status_code == 404:
        return NotFoundError(message, status_code, error_code, details)
    elif status_code == 409:
        return ConflictError(message, status_code, error_code, details)
    elif status_code == 422:
        return ValidationError(message, status_code, error_code, details)
    elif status_code == 429:
        retry_after = response_data.get("retry_after")
        return RateLimitError(message, status_code, error_code, details, retry_after)
    elif 500 <= status_code < 600:
        return ServerError(message, status_code, error_code, details)
    else:
        return DataAPIError(message, status_code, error_code, details)