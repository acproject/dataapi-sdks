"""Tests for HTTP client module."""

import pytest
import asyncio
from unittest.mock import Mock, patch, AsyncMock
from typing import Dict, Any

import httpx
from pydantic import BaseModel

from dataapi.client import HTTPClient
from dataapi.config import ClientConfig
from dataapi.auth import APIKeyAuth, BearerTokenAuth
from dataapi.exceptions import (
    AuthenticationError,
    NotFoundError,
    RateLimitError,
    ServerError,
    NetworkError
)


class TestModel(BaseModel):
    """Test model for response parsing."""
    id: str
    name: str
    value: int


class TestHTTPClient:
    """Test cases for HTTPClient class."""
    
    def test_client_creation(self):
        """Test HTTP client creation."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        assert client.config == config
        assert client.auth == auth
        assert client._async_client is None
        assert client._sync_client is None
    
    def test_client_creation_without_auth(self):
        """Test HTTP client creation without authentication."""
        config = ClientConfig()
        client = HTTPClient(config=config)
        
        assert client.config == config
        assert client.auth is None
    
    @pytest.mark.asyncio
    async def test_async_client_initialization(self):
        """Test async client initialization."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Access async client to trigger initialization
        async_client = client._get_async_client()
        assert async_client is not None
        assert isinstance(async_client, httpx.AsyncClient)
        
        # Cleanup
        await client.close_async()
    
    def test_sync_client_initialization(self):
        """Test sync client initialization."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Access sync client to trigger initialization
        sync_client = client._get_sync_client()
        assert sync_client is not None
        assert isinstance(sync_client, httpx.Client)
        
        # Cleanup
        client.close()
    
    @pytest.mark.asyncio
    async def test_async_get_request(self):
        """Test async GET request."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {"id": "123", "name": "test", "value": 42}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            response = await client.request_async(
                method="GET",
                endpoint="/test",
                response_model=TestModel
            )
            
            assert isinstance(response, TestModel)
            assert response.id == "123"
            assert response.name == "test"
            assert response.value == 42
            
            # Verify the request was made correctly
            mock_async_client.get.assert_called_once()
            call_args = mock_async_client.get.call_args
            assert "/test" in call_args[0][0]  # URL contains endpoint
        
        await client.close_async()
    
    def test_sync_get_request(self):
        """Test sync GET request."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {"id": "123", "name": "test", "value": 42}
        
        with patch.object(client, '_get_sync_client') as mock_get_client:
            mock_sync_client = Mock()
            mock_sync_client.get.return_value = mock_response
            mock_get_client.return_value = mock_sync_client
            
            response = client.request(
                method="GET",
                endpoint="/test",
                response_model=TestModel
            )
            
            assert isinstance(response, TestModel)
            assert response.id == "123"
            assert response.name == "test"
            assert response.value == 42
            
            # Verify the request was made correctly
            mock_sync_client.get.assert_called_once()
        
        client.close()
    
    @pytest.mark.asyncio
    async def test_async_post_request_with_data(self):
        """Test async POST request with data."""
        config = ClientConfig()
        auth = BearerTokenAuth(token="test-token")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 201
        mock_response.json.return_value = {"id": "456", "name": "created", "value": 100}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.post.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            request_data = {"name": "new_item", "value": 100}
            response = await client.request_async(
                method="POST",
                endpoint="/items",
                data=request_data,
                response_model=TestModel
            )
            
            assert isinstance(response, TestModel)
            assert response.id == "456"
            assert response.name == "created"
            
            # Verify the request was made with correct data
            mock_async_client.post.assert_called_once()
            call_kwargs = mock_async_client.post.call_args[1]
            assert "json" in call_kwargs
            assert call_kwargs["json"] == request_data
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_with_params(self):
        """Test async request with query parameters."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = [{"id": "1", "name": "item1", "value": 10}]
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            params = {"page": 1, "per_page": 10, "filter": "active"}
            response = await client.request_async(
                method="GET",
                endpoint="/items",
                params=params
            )
            
            assert response == [{"id": "1", "name": "item1", "value": 10}]
            
            # Verify the request was made with correct params
            mock_async_client.get.assert_called_once()
            call_kwargs = mock_async_client.get.call_args[1]
            assert "params" in call_kwargs
            assert call_kwargs["params"] == params
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_authentication_error(self):
        """Test async request with authentication error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="invalid-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock 401 response
        mock_response = Mock()
        mock_response.status_code = 401
        mock_response.json.return_value = {"error": "Invalid API key"}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(AuthenticationError, match="Invalid API key"):
                await client.request_async(
                    method="GET",
                    endpoint="/protected"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_not_found_error(self):
        """Test async request with not found error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock 404 response
        mock_response = Mock()
        mock_response.status_code = 404
        mock_response.json.return_value = {"error": "Resource not found"}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(NotFoundError, match="Resource not found"):
                await client.request_async(
                    method="GET",
                    endpoint="/nonexistent"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_rate_limit_error(self):
        """Test async request with rate limit error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock 429 response
        mock_response = Mock()
        mock_response.status_code = 429
        mock_response.json.return_value = {"error": "Rate limit exceeded"}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(RateLimitError, match="Rate limit exceeded"):
                await client.request_async(
                    method="GET",
                    endpoint="/api/data"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_server_error(self):
        """Test async request with server error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock 500 response
        mock_response = Mock()
        mock_response.status_code = 500
        mock_response.json.return_value = {"error": "Internal server error"}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.return_value = mock_response
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(ServerError, match="Internal server error"):
                await client.request_async(
                    method="GET",
                    endpoint="/api/data"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_network_error(self):
        """Test async request with network error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.side_effect = httpx.ConnectError("Connection failed")
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(NetworkError, match="Connection failed"):
                await client.request_async(
                    method="GET",
                    endpoint="/api/data"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_timeout_error(self):
        """Test async request with timeout error."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            mock_async_client.get.side_effect = httpx.TimeoutException("Request timeout")
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(NetworkError, match="Request timeout"):
                await client.request_async(
                    method="GET",
                    endpoint="/api/data"
                )
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_retry_on_timeout(self):
        """Test async request retry on timeout."""
        config = ClientConfig(max_retries=2, retry_delay=0.1)
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock successful response after retries
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {"id": "123", "name": "test", "value": 42}
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            # First two calls timeout, third succeeds
            mock_async_client.get.side_effect = [
                httpx.TimeoutException("Timeout 1"),
                httpx.TimeoutException("Timeout 2"),
                mock_response
            ]
            mock_get_client.return_value = mock_async_client
            
            response = await client.request_async(
                method="GET",
                endpoint="/test",
                response_model=TestModel
            )
            
            assert isinstance(response, TestModel)
            assert response.id == "123"
            
            # Verify 3 attempts were made (initial + 2 retries)
            assert mock_async_client.get.call_count == 3
        
        await client.close_async()
    
    @pytest.mark.asyncio
    async def test_async_request_max_retries_exceeded(self):
        """Test async request when max retries exceeded."""
        config = ClientConfig(max_retries=1, retry_delay=0.1)
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        with patch.object(client, '_get_async_client') as mock_get_client:
            mock_async_client = AsyncMock()
            # All calls timeout
            mock_async_client.get.side_effect = httpx.TimeoutException("Persistent timeout")
            mock_get_client.return_value = mock_async_client
            
            with pytest.raises(NetworkError, match="Persistent timeout"):
                await client.request_async(
                    method="GET",
                    endpoint="/test"
                )
            
            # Verify 2 attempts were made (initial + 1 retry)
            assert mock_async_client.get.call_count == 2
        
        await client.close_async()
    
    def test_auth_headers_included(self):
        """Test that authentication headers are included in requests."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {"success": True}
        
        with patch.object(client, '_get_sync_client') as mock_get_client:
            mock_sync_client = Mock()
            mock_sync_client.get.return_value = mock_response
            mock_get_client.return_value = mock_sync_client
            
            client.request(method="GET", endpoint="/test")
            
            # Verify headers include authentication
            call_kwargs = mock_sync_client.get.call_args[1]
            assert "headers" in call_kwargs
            headers = call_kwargs["headers"]
            assert "X-API-Key" in headers
            assert headers["X-API-Key"] == "test-key"
        
        client.close()
    
    @pytest.mark.asyncio
    async def test_context_manager_async(self):
        """Test async context manager usage."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        
        async with HTTPClient(config=config, auth=auth) as client:
            assert client is not None
            # Client should be usable within context
            assert client.config == config
    
    def test_context_manager_sync(self):
        """Test sync context manager usage."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        
        with HTTPClient(config=config, auth=auth) as client:
            assert client is not None
            # Client should be usable within context
            assert client.config == config
    
    def test_request_without_response_model(self):
        """Test request without response model returns raw JSON."""
        config = ClientConfig()
        auth = APIKeyAuth(api_key="test-key")
        client = HTTPClient(config=config, auth=auth)
        
        # Mock response
        mock_response = Mock()
        mock_response.status_code = 200
        mock_response.json.return_value = {"raw": "data", "count": 5}
        
        with patch.object(client, '_get_sync_client') as mock_get_client:
            mock_sync_client = Mock()
            mock_sync_client.get.return_value = mock_response
            mock_get_client.return_value = mock_sync_client
            
            response = client.request(method="GET", endpoint="/raw")
            
            assert response == {"raw": "data", "count": 5}
        
        client.close()