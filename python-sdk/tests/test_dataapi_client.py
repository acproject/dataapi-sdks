"""Tests for DataAPIClient main class."""

import pytest
from unittest.mock import Mock, patch, AsyncMock
from datetime import datetime

from dataapi import DataAPIClient
from dataapi.auth import APIKeyAuth, BearerTokenAuth, OAuth2Auth
from dataapi.config import ClientConfig
from dataapi.services import DatabaseService, AIService, WorkflowService
from dataapi.exceptions import AuthenticationError, ValidationError


class TestDataAPIClient:
    """Test cases for DataAPIClient class."""
    
    def test_client_creation_with_api_key(self):
        """Test client creation with API key."""
        client = DataAPIClient(api_key="test-api-key")
        
        assert isinstance(client.auth, APIKeyAuth)
        assert client.auth.api_key == "test-api-key"
        assert isinstance(client.config, ClientConfig)
        assert isinstance(client.databases, DatabaseService)
        assert isinstance(client.ai, AIService)
        assert isinstance(client.workflows, WorkflowService)
    
    def test_client_creation_with_bearer_token(self):
        """Test client creation with bearer token."""
        client = DataAPIClient(bearer_token="test-bearer-token")
        
        assert isinstance(client.auth, BearerTokenAuth)
        assert client.auth.token == "test-bearer-token"
    
    def test_client_creation_with_oauth2(self):
        """Test client creation with OAuth2."""
        oauth2_auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token"
        )
        
        client = DataAPIClient(oauth2_auth=oauth2_auth)
        
        assert isinstance(client.auth, OAuth2Auth)
        assert client.auth.client_id == "client-id"
    
    def test_client_creation_with_custom_config(self):
        """Test client creation with custom configuration."""
        config = ClientConfig(
            base_url="https://custom.api.com",
            timeout=60.0,
            max_retries=5
        )
        
        client = DataAPIClient(
            api_key="test-key",
            config=config
        )
        
        assert client.config.base_url == "https://custom.api.com"
        assert client.config.timeout == 60.0
        assert client.config.max_retries == 5
    
    def test_client_creation_no_auth(self):
        """Test client creation without authentication raises error."""
        with pytest.raises(ValueError, match="At least one authentication method must be provided"):
            DataAPIClient()
    
    def test_client_creation_multiple_auth(self):
        """Test client creation with multiple auth methods raises error."""
        oauth2_auth = OAuth2Auth(
            client_id="client-id",
            client_secret="client-secret",
            token_url="https://auth.example.com/token"
        )
        
        with pytest.raises(ValueError, match="Only one authentication method can be provided"):
            DataAPIClient(
                api_key="test-key",
                bearer_token="test-token",
                oauth2_auth=oauth2_auth
            )
    
    @pytest.mark.asyncio
    async def test_health_check_async(self):
        """Test async health check."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock successful health check response
        mock_response = {
            "status": "healthy",
            "version": "1.0.0",
            "timestamp": datetime.now().isoformat()
        }
        
        with patch.object(client.http_client, 'request_async', new_callable=AsyncMock) as mock_request:
            mock_request.return_value = mock_response
            
            health = await client.health_check_async()
            
            assert health["status"] == "healthy"
            assert health["version"] == "1.0.0"
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/health"
            )
    
    def test_health_check_sync(self):
        """Test sync health check."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock successful health check response
        mock_response = {
            "status": "healthy",
            "version": "1.0.0",
            "timestamp": datetime.now().isoformat()
        }
        
        with patch.object(client.http_client, 'request') as mock_request:
            mock_request.return_value = mock_response
            
            health = client.health_check()
            
            assert health["status"] == "healthy"
            assert health["version"] == "1.0.0"
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/health"
            )
    
    @pytest.mark.asyncio
    async def test_get_user_info_async(self):
        """Test async get user info."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock user info response
        mock_response = {
            "id": "user-123",
            "username": "testuser",
            "email": "test@example.com",
            "role": "user",
            "created_at": datetime.now().isoformat()
        }
        
        with patch.object(client.http_client, 'request_async', new_callable=AsyncMock) as mock_request:
            mock_request.return_value = mock_response
            
            user_info = await client.get_user_info_async()
            
            assert user_info["id"] == "user-123"
            assert user_info["username"] == "testuser"
            assert user_info["email"] == "test@example.com"
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/user/me"
            )
    
    def test_get_user_info_sync(self):
        """Test sync get user info."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock user info response
        mock_response = {
            "id": "user-123",
            "username": "testuser",
            "email": "test@example.com",
            "role": "user"
        }
        
        with patch.object(client.http_client, 'request') as mock_request:
            mock_request.return_value = mock_response
            
            user_info = client.get_user_info()
            
            assert user_info["id"] == "user-123"
            assert user_info["username"] == "testuser"
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/user/me"
            )
    
    @pytest.mark.asyncio
    async def test_get_api_info_async(self):
        """Test async get API info."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock API info response
        mock_response = {
            "name": "DataAPI",
            "version": "2.1.0",
            "description": "Data management and AI platform API",
            "documentation_url": "https://docs.dataapi.com",
            "support_email": "support@dataapi.com"
        }
        
        with patch.object(client.http_client, 'request_async', new_callable=AsyncMock) as mock_request:
            mock_request.return_value = mock_response
            
            api_info = await client.get_api_info_async()
            
            assert api_info["name"] == "DataAPI"
            assert api_info["version"] == "2.1.0"
            assert "documentation_url" in api_info
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/info"
            )
    
    def test_get_api_info_sync(self):
        """Test sync get API info."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock API info response
        mock_response = {
            "name": "DataAPI",
            "version": "2.1.0",
            "description": "Data management and AI platform API"
        }
        
        with patch.object(client.http_client, 'request') as mock_request:
            mock_request.return_value = mock_response
            
            api_info = client.get_api_info()
            
            assert api_info["name"] == "DataAPI"
            assert api_info["version"] == "2.1.0"
            
            mock_request.assert_called_once_with(
                method="GET",
                endpoint="/info"
            )
    
    @pytest.mark.asyncio
    async def test_async_context_manager(self):
        """Test async context manager usage."""
        async with DataAPIClient(api_key="test-key") as client:
            assert client is not None
            assert isinstance(client.databases, DatabaseService)
            assert isinstance(client.ai, AIService)
            assert isinstance(client.workflows, WorkflowService)
            
            # Test that services are accessible
            assert client.databases.http_client == client.http_client
            assert client.ai.http_client == client.http_client
            assert client.workflows.http_client == client.http_client
    
    def test_sync_context_manager(self):
        """Test sync context manager usage."""
        with DataAPIClient(api_key="test-key") as client:
            assert client is not None
            assert isinstance(client.databases, DatabaseService)
            assert isinstance(client.ai, AIService)
            assert isinstance(client.workflows, WorkflowService)
    
    @pytest.mark.asyncio
    async def test_close_async(self):
        """Test async client cleanup."""
        client = DataAPIClient(api_key="test-key")
        
        with patch.object(client.http_client, 'close_async', new_callable=AsyncMock) as mock_close:
            await client.close_async()
            mock_close.assert_called_once()
    
    def test_close_sync(self):
        """Test sync client cleanup."""
        client = DataAPIClient(api_key="test-key")
        
        with patch.object(client.http_client, 'close') as mock_close:
            client.close()
            mock_close.assert_called_once()
    
    def test_service_initialization(self):
        """Test that services are properly initialized."""
        client = DataAPIClient(api_key="test-key")
        
        # Check that all services share the same HTTP client
        assert client.databases.http_client is client.http_client
        assert client.ai.http_client is client.http_client
        assert client.workflows.http_client is client.http_client
        
        # Check that services are different instances
        assert client.databases is not client.ai
        assert client.ai is not client.workflows
        assert client.workflows is not client.databases
    
    @pytest.mark.asyncio
    async def test_authentication_error_handling(self):
        """Test authentication error handling."""
        client = DataAPIClient(api_key="invalid-key")
        
        with patch.object(client.http_client, 'request_async', new_callable=AsyncMock) as mock_request:
            mock_request.side_effect = AuthenticationError("Invalid API key")
            
            with pytest.raises(AuthenticationError, match="Invalid API key"):
                await client.health_check_async()
    
    def test_validation_error_handling(self):
        """Test validation error handling."""
        client = DataAPIClient(api_key="test-key")
        
        with patch.object(client.http_client, 'request') as mock_request:
            mock_request.side_effect = ValidationError("Invalid request data")
            
            with pytest.raises(ValidationError, match="Invalid request data"):
                client.get_user_info()
    
    def test_client_repr(self):
        """Test client string representation."""
        client = DataAPIClient(api_key="test-key")
        
        repr_str = repr(client)
        assert "DataAPIClient" in repr_str
        assert "APIKeyAuth" in repr_str
    
    def test_client_with_custom_base_url(self):
        """Test client with custom base URL."""
        config = ClientConfig(base_url="https://custom.dataapi.com")
        client = DataAPIClient(api_key="test-key", config=config)
        
        assert client.config.base_url == "https://custom.dataapi.com"
        assert client.http_client.config.base_url == "https://custom.dataapi.com"
    
    def test_client_with_custom_timeout(self):
        """Test client with custom timeout."""
        config = ClientConfig(timeout=120.0)
        client = DataAPIClient(api_key="test-key", config=config)
        
        assert client.config.timeout == 120.0
        assert client.http_client.config.timeout == 120.0
    
    def test_client_with_custom_retries(self):
        """Test client with custom retry settings."""
        config = ClientConfig(max_retries=5, retry_delay=2.0)
        client = DataAPIClient(api_key="test-key", config=config)
        
        assert client.config.max_retries == 5
        assert client.config.retry_delay == 2.0
        assert client.http_client.config.max_retries == 5
        assert client.http_client.config.retry_delay == 2.0
    
    @pytest.mark.asyncio
    async def test_concurrent_requests(self):
        """Test concurrent async requests."""
        client = DataAPIClient(api_key="test-key")
        
        # Mock responses
        health_response = {"status": "healthy"}
        user_response = {"id": "user-123", "username": "test"}
        api_response = {"name": "DataAPI", "version": "1.0.0"}
        
        with patch.object(client.http_client, 'request_async', new_callable=AsyncMock) as mock_request:
            mock_request.side_effect = [health_response, user_response, api_response]
            
            # Make concurrent requests
            import asyncio
            results = await asyncio.gather(
                client.health_check_async(),
                client.get_user_info_async(),
                client.get_api_info_async()
            )
            
            assert results[0]["status"] == "healthy"
            assert results[1]["id"] == "user-123"
            assert results[2]["name"] == "DataAPI"
            
            # Verify all requests were made
            assert mock_request.call_count == 3
    
    def test_client_immutability(self):
        """Test that client configuration is immutable after creation."""
        client = DataAPIClient(api_key="test-key")
        
        # Should not be able to modify config after creation
        with pytest.raises(AttributeError):
            client.config.base_url = "https://new.url.com"
        
        # Should not be able to replace services
        original_databases = client.databases
        with pytest.raises(AttributeError):
            client.databases = "new_service"
        
        assert client.databases is original_databases