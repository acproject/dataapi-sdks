"""Main DataAPI client for Python SDK."""

from typing import Optional

from .auth import APIKeyAuth, AuthProvider, BearerTokenAuth, OAuth2Auth
from .client import HTTPClient
from .config import ClientConfig
from .services import AIService, DatabaseService, WorkflowService


class DataAPIClient:
    """Main client for DataAPI SDK.
    
    This is the primary interface for interacting with the DataAPI.
    It provides access to all services and handles authentication and configuration.
    
    Args:
        api_key: API key for authentication (optional if using other auth methods)
        base_url: Base URL for the DataAPI (defaults to production URL)
        timeout: Request timeout in seconds
        max_retries: Maximum number of retries for failed requests
        retry_delay: Delay between retries in seconds
        verify_ssl: Whether to verify SSL certificates
        auth_provider: Custom authentication provider (optional)
        
    Example:
        Basic usage with API key:
        
        ```python
        from dataapi import DataAPIClient
        
        client = DataAPIClient(api_key="your-api-key")
        
        # List databases
        databases = await client.databases.list_databases_async()
        
        # Generate text with AI
        response = await client.ai.generate_text_async(
            model_id="gpt-4",
            prompt="Hello, world!"
        )
        ```
        
        Using OAuth2:
        
        ```python
        from dataapi import DataAPIClient, OAuth2Auth
        
        auth = OAuth2Auth(
            client_id="your-client-id",
            client_secret="your-client-secret",
            token_url="https://auth.dataapi.com/oauth/token"
        )
        
        client = DataAPIClient(auth_provider=auth)
        ```
    """
    
    def __init__(
        self,
        api_key: Optional[str] = None,
        base_url: str = "https://api.dataapi.com/v1",
        timeout: float = 30.0,
        max_retries: int = 3,
        retry_delay: float = 1.0,
        verify_ssl: bool = True,
        auth_provider: Optional[AuthProvider] = None,
    ) -> None:
        # Create configuration
        self.config = ClientConfig(
            base_url=base_url,
            timeout=timeout,
            max_retries=max_retries,
            retry_delay=retry_delay,
            verify_ssl=verify_ssl,
        )
        
        # Set up authentication
        if auth_provider:
            self.auth = auth_provider
        elif api_key:
            self.auth = APIKeyAuth(api_key=api_key)
        else:
            raise ValueError(
                "Either 'api_key' or 'auth_provider' must be provided"
            )
        
        # Create HTTP client
        self.http_client = HTTPClient(config=self.config, auth=self.auth)
        
        # Initialize services
        self.databases = DatabaseService(self.http_client)
        self.ai = AIService(self.http_client)
        self.workflows = WorkflowService(self.http_client)
    
    @classmethod
    def with_api_key(
        cls,
        api_key: str,
        base_url: str = "https://api.dataapi.com/v1",
        **kwargs,
    ) -> "DataAPIClient":
        """Create client with API key authentication.
        
        Args:
            api_key: API key for authentication
            base_url: Base URL for the DataAPI
            **kwargs: Additional configuration options
            
        Returns:
            DataAPIClient instance
        """
        return cls(api_key=api_key, base_url=base_url, **kwargs)
    
    @classmethod
    def with_bearer_token(
        cls,
        token: str,
        base_url: str = "https://api.dataapi.com/v1",
        **kwargs,
    ) -> "DataAPIClient":
        """Create client with bearer token authentication.
        
        Args:
            token: Bearer token for authentication
            base_url: Base URL for the DataAPI
            **kwargs: Additional configuration options
            
        Returns:
            DataAPIClient instance
        """
        auth = BearerTokenAuth(token=token)
        return cls(auth_provider=auth, base_url=base_url, **kwargs)
    
    @classmethod
    def with_oauth2(
        cls,
        client_id: str,
        client_secret: str,
        token_url: str,
        scope: Optional[str] = None,
        base_url: str = "https://api.dataapi.com/v1",
        **kwargs,
    ) -> "DataAPIClient":
        """Create client with OAuth2 authentication.
        
        Args:
            client_id: OAuth2 client ID
            client_secret: OAuth2 client secret
            token_url: OAuth2 token endpoint URL
            scope: OAuth2 scope (optional)
            base_url: Base URL for the DataAPI
            **kwargs: Additional configuration options
            
        Returns:
            DataAPIClient instance
        """
        auth = OAuth2Auth(
            client_id=client_id,
            client_secret=client_secret,
            token_url=token_url,
            scope=scope,
        )
        return cls(auth_provider=auth, base_url=base_url, **kwargs)
    
    async def health_check_async(self) -> dict:
        """Check API health status.
        
        Returns:
            Health status information
        """
        return await self.http_client.get_async("/health")
    
    def health_check(self) -> dict:
        """Check API health status (sync).
        
        Returns:
            Health status information
        """
        return self.http_client.get("/health")
    
    async def get_user_info_async(self) -> dict:
        """Get current user information.
        
        Returns:
            User information
        """
        return await self.http_client.get_async("/user")
    
    def get_user_info(self) -> dict:
        """Get current user information (sync).
        
        Returns:
            User information
        """
        return self.http_client.get("/user")
    
    async def get_api_info_async(self) -> dict:
        """Get API information and version.
        
        Returns:
            API information
        """
        return await self.http_client.get_async("/info")
    
    def get_api_info(self) -> dict:
        """Get API information and version (sync).
        
        Returns:
            API information
        """
        return self.http_client.get("/info")
    
    async def close_async(self) -> None:
        """Close the HTTP client and clean up resources."""
        await self.http_client.close_async()
    
    def close(self) -> None:
        """Close the HTTP client and clean up resources (sync)."""
        self.http_client.close()
    
    async def __aenter__(self) -> "DataAPIClient":
        """Async context manager entry."""
        return self
    
    async def __aexit__(self, exc_type, exc_val, exc_tb) -> None:
        """Async context manager exit."""
        await self.close_async()
    
    def __enter__(self) -> "DataAPIClient":
        """Sync context manager entry."""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb) -> None:
        """Sync context manager exit."""
        self.close()
    
    def __repr__(self) -> str:
        """String representation of the client."""
        return f"DataAPIClient(base_url='{self.config.base_url}')"