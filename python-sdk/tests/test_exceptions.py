"""Tests for exceptions module."""

import pytest
from unittest.mock import Mock

from dataapi.exceptions import (
    DataAPIError,
    AuthenticationError,
    AuthorizationError,
    NotFoundError,
    ValidationError,
    RateLimitError,
    ServerError,
    NetworkError,
    create_error_from_response
)


class TestDataAPIError:
    """Test cases for base DataAPIError class."""
    
    def test_base_error_creation(self):
        """Test base error creation."""
        error = DataAPIError("Test error message")
        assert str(error) == "Test error message"
        assert error.message == "Test error message"
        assert error.status_code is None
        assert error.response is None
    
    def test_base_error_with_status_code(self):
        """Test base error with status code."""
        error = DataAPIError("Test error", status_code=400)
        assert error.status_code == 400
    
    def test_base_error_with_response(self):
        """Test base error with response object."""
        mock_response = Mock()
        mock_response.status_code = 400
        mock_response.text = "Bad Request"
        
        error = DataAPIError("Test error", response=mock_response)
        assert error.response == mock_response
        assert error.status_code == 400


class TestSpecificErrors:
    """Test cases for specific error classes."""
    
    def test_authentication_error(self):
        """Test AuthenticationError."""
        error = AuthenticationError("Invalid API key")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Invalid API key"
    
    def test_authorization_error(self):
        """Test AuthorizationError."""
        error = AuthorizationError("Insufficient permissions")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Insufficient permissions"
    
    def test_not_found_error(self):
        """Test NotFoundError."""
        error = NotFoundError("Resource not found")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Resource not found"
    
    def test_validation_error(self):
        """Test ValidationError."""
        error = ValidationError("Invalid input data")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Invalid input data"
    
    def test_rate_limit_error(self):
        """Test RateLimitError."""
        error = RateLimitError("Rate limit exceeded")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Rate limit exceeded"
    
    def test_server_error(self):
        """Test ServerError."""
        error = ServerError("Internal server error")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Internal server error"
    
    def test_network_error(self):
        """Test NetworkError."""
        error = NetworkError("Connection timeout")
        assert isinstance(error, DataAPIError)
        assert str(error) == "Connection timeout"


class TestCreateErrorFromResponse:
    """Test cases for create_error_from_response function."""
    
    def test_create_authentication_error(self):
        """Test creating AuthenticationError from response."""
        mock_response = Mock()
        mock_response.status_code = 401
        mock_response.json.return_value = {"error": "Invalid API key"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, AuthenticationError)
        assert "Invalid API key" in str(error)
        assert error.status_code == 401
    
    def test_create_authorization_error(self):
        """Test creating AuthorizationError from response."""
        mock_response = Mock()
        mock_response.status_code = 403
        mock_response.json.return_value = {"error": "Insufficient permissions"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, AuthorizationError)
        assert "Insufficient permissions" in str(error)
        assert error.status_code == 403
    
    def test_create_not_found_error(self):
        """Test creating NotFoundError from response."""
        mock_response = Mock()
        mock_response.status_code = 404
        mock_response.json.return_value = {"error": "Resource not found"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, NotFoundError)
        assert "Resource not found" in str(error)
        assert error.status_code == 404
    
    def test_create_validation_error(self):
        """Test creating ValidationError from response."""
        mock_response = Mock()
        mock_response.status_code = 422
        mock_response.json.return_value = {
            "error": "Validation failed",
            "details": ["Name is required", "Email format is invalid"]
        }
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ValidationError)
        assert "Validation failed" in str(error)
        assert error.status_code == 422
    
    def test_create_rate_limit_error(self):
        """Test creating RateLimitError from response."""
        mock_response = Mock()
        mock_response.status_code = 429
        mock_response.json.return_value = {"error": "Rate limit exceeded"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, RateLimitError)
        assert "Rate limit exceeded" in str(error)
        assert error.status_code == 429
    
    def test_create_server_error_500(self):
        """Test creating ServerError from 500 response."""
        mock_response = Mock()
        mock_response.status_code = 500
        mock_response.json.return_value = {"error": "Internal server error"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ServerError)
        assert "Internal server error" in str(error)
        assert error.status_code == 500
    
    def test_create_server_error_502(self):
        """Test creating ServerError from 502 response."""
        mock_response = Mock()
        mock_response.status_code = 502
        mock_response.json.return_value = {"error": "Bad gateway"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ServerError)
        assert "Bad gateway" in str(error)
        assert error.status_code == 502
    
    def test_create_validation_error_400(self):
        """Test creating ValidationError from 400 response."""
        mock_response = Mock()
        mock_response.status_code = 400
        mock_response.json.return_value = {"error": "Bad request"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ValidationError)
        assert "Bad request" in str(error)
        assert error.status_code == 400
    
    def test_create_error_with_message_field(self):
        """Test creating error with 'message' field in response."""
        mock_response = Mock()
        mock_response.status_code = 404
        mock_response.json.return_value = {"message": "Not found"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, NotFoundError)
        assert "Not found" in str(error)
    
    def test_create_error_with_detail_field(self):
        """Test creating error with 'detail' field in response."""
        mock_response = Mock()
        mock_response.status_code = 400
        mock_response.json.return_value = {"detail": "Invalid input"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ValidationError)
        assert "Invalid input" in str(error)
    
    def test_create_error_no_json_response(self):
        """Test creating error when response has no JSON."""
        mock_response = Mock()
        mock_response.status_code = 500
        mock_response.json.side_effect = ValueError("No JSON")
        mock_response.text = "Internal Server Error"
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ServerError)
        assert "Internal Server Error" in str(error)
    
    def test_create_error_empty_response(self):
        """Test creating error with empty response."""
        mock_response = Mock()
        mock_response.status_code = 500
        mock_response.json.return_value = {}
        mock_response.text = ""
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ServerError)
        assert "HTTP 500" in str(error)
    
    def test_create_error_unknown_status_code(self):
        """Test creating error with unknown status code."""
        mock_response = Mock()
        mock_response.status_code = 418  # I'm a teapot
        mock_response.json.return_value = {"error": "I'm a teapot"}
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, DataAPIError)
        assert "I'm a teapot" in str(error)
        assert error.status_code == 418
    
    def test_create_error_with_details(self):
        """Test creating error with detailed validation errors."""
        mock_response = Mock()
        mock_response.status_code = 422
        mock_response.json.return_value = {
            "error": "Validation failed",
            "details": [
                {"field": "email", "message": "Invalid email format"},
                {"field": "age", "message": "Must be a positive integer"}
            ]
        }
        
        error = create_error_from_response(mock_response)
        assert isinstance(error, ValidationError)
        error_str = str(error)
        assert "Validation failed" in error_str
        assert "email" in error_str
        assert "age" in error_str
    
    def test_error_inheritance(self):
        """Test that all specific errors inherit from DataAPIError."""
        errors = [
            AuthenticationError("test"),
            AuthorizationError("test"),
            NotFoundError("test"),
            ValidationError("test"),
            RateLimitError("test"),
            ServerError("test"),
            NetworkError("test")
        ]
        
        for error in errors:
            assert isinstance(error, DataAPIError)
            assert isinstance(error, Exception)