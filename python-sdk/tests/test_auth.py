"""Tests for authentication module."""

import pytest
from unittest.mock import Mock, patch
from datetime import datetime, timedelta

from dataapi.auth import APIKeyAuth, BearerTokenAuth, OAuth2Auth, OAuth2Token
from dataapi.exceptions import AuthenticationError


class TestAPIKeyAuth:
    """Test cases for APIKeyAuth class."""
    
    def test_api_key_auth_creation(self):
        """Test API key authentication creation."""
        auth = APIKeyAuth(api_key="test-api-key")
        assert auth.api_key == "test-api-key"
    
    def test_api_key_auth_headers(self):
        """Test API key authentication headers."""
        auth = APIKeyAuth(api_key="test-api-key")
        headers = auth.get_auth_headers()
        
        assert "X-API-Key" in headers
        assert headers["X-API-Key"] == "test-api-key"
    
    def test_api_key_auth_empty_key(self):
        """Test API key authentication with empty key."""
        with pytest.raises(ValueError, match="API key cannot be empty"):
            APIKeyAuth(api_key="")
    
    def test_api_key_auth_none_key(self):
        """Test API key authentication with None key."""
        with pytest.raises(ValueError, match="API key cannot be empty"):
            APIKeyAuth(api_key=None)


class TestBearerTokenAuth:
    """Test cases for BearerTokenAuth class."""
    
    def test_bearer_token_auth_creation(self):
        """Test bearer token authentication creation."""
        auth = BearerTokenAuth(token="test-bearer-token")
        assert auth.token == "test-bearer-token"
    
    def test_bearer_token_auth_headers(self):
        """Test bearer token authentication headers."""
        auth = BearerTokenAuth(token="test-bearer-token")
        headers = auth.get_auth_headers()
        
        assert "Authorization" in headers
        assert headers["Authorization"] == "Bearer test-bearer-token"
    
    def test_bearer_token_auth_empty_token(self):
        """Test bearer token authentication with empty token."""
        with pytest.raises(ValueError, match="Token cannot be empty"):
            BearerTokenAuth(token="")
    
    def test_bearer_token_auth_none_token(self):
        """Test bearer token authentication with None token."""
        with pytest.raises(ValueError, match="Token cannot be empty"):
            BearerTokenAuth(token=None)


class TestOAuth2Token:
    """Test cases for OAuth2Token class."""
    
    def test_oauth2_token_creation(self):
        """Test OAuth2 token creation."""
        expires_at = datetime.utcnow() + timedelta(hours=1)
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=expires_at,
            refresh_token="refresh-token"
        )
        
        assert token.access_token == "access-token"
        assert token.token_type == "Bearer"
        assert token.expires_at == expires_at
        assert token.refresh_token == "refresh-token"
    
    def test_oauth2_token_is_expired_false(self):
        """Test OAuth2 token expiration check when not expired."""
        expires_at = datetime.utcnow() + timedelta(hours=1)
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=expires_at
        )
        
        assert not token.is_expired()
    
    def test_oauth2_token_is_expired_true(self):
        """Test OAuth2 token expiration check when expired."""
        expires_at = datetime.utcnow() - timedelta(hours=1)
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=expires_at
        )
        
        assert token.is_expired()
    
    def test_oauth2_token_is_expired_buffer(self):
        """Test OAuth2 token expiration check with buffer."""
        # Token expires in 4 minutes (less than 5 minute buffer)
        expires_at = datetime.utcnow() + timedelta(minutes=4)
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=expires_at
        )
        
        assert token.is_expired()  # Should be considered expired due to buffer
    
    def test_oauth2_token_no_expiration(self):
        """Test OAuth2 token without expiration."""
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer"
        )
        
        assert not token.is_expired()


class TestOAuth2Auth:
    """Test cases for OAuth2Auth class."""
    
    def test_oauth2_auth_creation(self):
        """Test OAuth2 authentication creation."""
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token"
        )
        
        assert auth.client_id == "client-id"
        assert auth.client_secret == "client-secret"
        assert auth.token_url == "https://auth.example.com/token"
        assert auth.token is None
    
    def test_oauth2_auth_with_token(self):
        """Test OAuth2 authentication with existing token."""
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=datetime.utcnow() + timedelta(hours=1)
        )
        
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token",
            token=token
        )
        
        assert auth.token == token
    
    def test_oauth2_auth_headers_with_valid_token(self):
        """Test OAuth2 authentication headers with valid token."""
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=datetime.utcnow() + timedelta(hours=1)
        )
        
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token",
            token=token
        )
        
        headers = auth.get_auth_headers()
        assert "Authorization" in headers
        assert headers["Authorization"] == "Bearer access-token"
    
    def test_oauth2_auth_headers_no_token(self):
        """Test OAuth2 authentication headers without token."""
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token"
        )
        
        with pytest.raises(AuthenticationError, match="No valid OAuth2 token available"):
            auth.get_auth_headers()
    
    def test_oauth2_auth_headers_expired_token_no_refresh(self):
        """Test OAuth2 authentication headers with expired token and no refresh token."""
        token = OAuth2Token(
            access_token="access-token",
            token_type="Bearer",
            expires_at=datetime.utcnow() - timedelta(hours=1)
        )
        
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token",
            token=token
        )
        
        with pytest.raises(AuthenticationError, match="OAuth2 token has expired and no refresh token available"):
            auth.get_auth_headers()
    
    @patch('httpx.post')
    def test_oauth2_refresh_token_success(self, mock_post):
        """Test successful OAuth2 token refresh."""
        # Mock successful refresh response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {
            "access_token": "new-access-token",
            "token_type": "Bearer",
            "expires_in": 3600,
            "refresh_token": "new-refresh-token"
        }
        mock_post.return_value = mock_response
        
        # Create expired token with refresh token
        expired_token = OAuth2Token(
            access_token="old-access-token",
            token_type="Bearer",
            expires_at=datetime.utcnow() - timedelta(hours=1),
            refresh_token="old-refresh-token"
        )
        
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token",
            token=expired_token
        )
        
        # This should trigger token refresh
        headers = auth.get_auth_headers()
        
        # Verify the new token is used
        assert headers["Authorization"] == "Bearer new-access-token"
        assert auth.token.access_token == "new-access-token"
        assert auth.token.refresh_token == "new-refresh-token"
        
        # Verify the refresh request was made
        mock_post.assert_called_once()
        call_args = mock_post.call_args
        assert call_args[0][0] == "https://auth.example.com/token"
        assert call_args[1]["data"]["grant_type"] == "refresh_token"
        assert call_args[1]["data"]["refresh_token"] == "old-refresh-token"
    
    @patch('httpx.post')
    def test_oauth2_refresh_token_failure(self, mock_post):
        """Test failed OAuth2 token refresh."""
        # Mock failed refresh response
        mock_response = Mock()
        mock_response.status_code = 400
        mock_response.json.return_value = {
            "error": "invalid_grant",
            "error_description": "The refresh token is invalid"
        }
        mock_post.return_value = mock_response
        
        # Create expired token with refresh token
        expired_token = OAuth2Token(
            access_token="old-access-token",
            token_type="Bearer",
            expires_at=datetime.utcnow() - timedelta(hours=1),
            refresh_token="old-refresh-token"
        )
        
        auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token",
            token=expired_token
        )
        
        # This should raise an authentication error
        with pytest.raises(AuthenticationError, match="Failed to refresh OAuth2 token"):
            auth.get_auth_headers()
    
    def test_oauth2_auth_empty_client_id(self):
        """Test OAuth2 authentication with empty client ID."""
        with pytest.raises(ValueError, match="Client ID cannot be empty"):
            OAuth2Auth(
                client_id="",
                client_secret="client-secret",
                token_url="https://auth.example.com/token"
            )
    
    def test_oauth2_auth_empty_client_secret(self):
        """Test OAuth2 authentication with empty client secret."""
        with pytest.raises(ValueError, match="Client secret cannot be empty"):
            OAuth2Auth(
                client_id="client-id",
                client_secret="",
                token_url="https://auth.example.com/token"
            )
    
    def test_oauth2_auth_invalid_token_url(self):
        """Test OAuth2 authentication with invalid token URL."""
        with pytest.raises(ValueError, match="Invalid token URL"):
            OAuth2Auth(
                client_id="client-id",
                client_secret="client-secret",
                token_url="not-a-url"
            )