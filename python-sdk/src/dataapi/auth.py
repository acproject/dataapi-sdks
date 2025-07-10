"""Authentication providers for DataAPI SDK."""

import time
from abc import ABC, abstractmethod
from typing import Dict, Optional

import httpx
from pydantic import BaseModel, Field, SecretStr

from .exceptions import AuthenticationError


class AuthProvider(ABC):
    """Abstract base class for authentication providers."""
    
    @abstractmethod
    async def get_headers(self) -> Dict[str, str]:
        """Get authentication headers for requests.
        
        Returns:
            Dictionary of headers to include in requests
            
        Raises:
            AuthenticationError: If authentication fails
        """
        pass
    
    @abstractmethod
    async def refresh_if_needed(self) -> None:
        """Refresh authentication if needed.
        
        Raises:
            AuthenticationError: If refresh fails
        """
        pass


class APIKeyAuth(AuthProvider):
    """API Key authentication provider.
    
    Args:
        api_key: The API key for authentication
        header_name: Name of the header to use for the API key
    """
    
    def __init__(self, api_key: str, header_name: str = "X-API-Key") -> None:
        self.api_key = SecretStr(api_key)
        self.header_name = header_name
    
    async def get_headers(self) -> Dict[str, str]:
        """Get API key headers.
        
        Returns:
            Dictionary containing the API key header
        """
        return {self.header_name: self.api_key.get_secret_value()}
    
    async def refresh_if_needed(self) -> None:
        """API keys don't need refreshing."""
        pass


class BearerTokenAuth(AuthProvider):
    """Bearer token authentication provider.
    
    Args:
        token: The bearer token for authentication
    """
    
    def __init__(self, token: str) -> None:
        self.token = SecretStr(token)
    
    async def get_headers(self) -> Dict[str, str]:
        """Get bearer token headers.
        
        Returns:
            Dictionary containing the authorization header
        """
        return {"Authorization": f"Bearer {self.token.get_secret_value()}"}
    
    async def refresh_if_needed(self) -> None:
        """Bearer tokens don't need refreshing."""
        pass


class OAuth2Token(BaseModel):
    """OAuth2 token model.
    
    Args:
        access_token: The access token
        token_type: Type of token (usually 'Bearer')
        expires_in: Token lifetime in seconds
        refresh_token: Optional refresh token
        scope: Token scope
    """
    
    access_token: SecretStr
    token_type: str = "Bearer"
    expires_in: Optional[int] = None
    refresh_token: Optional[SecretStr] = None
    scope: Optional[str] = None
    
    # Internal fields
    _issued_at: float = Field(default_factory=time.time, alias="issued_at")
    
    class Config:
        """Pydantic configuration."""
        allow_population_by_field_name = True
    
    @property
    def is_expired(self) -> bool:
        """Check if token is expired.
        
        Returns:
            True if token is expired, False otherwise
        """
        if self.expires_in is None:
            return False
        
        # Add 60 second buffer to avoid using tokens that expire very soon
        expiry_time = self._issued_at + self.expires_in - 60
        return time.time() >= expiry_time


class OAuth2Auth(AuthProvider):
    """OAuth2 authentication provider.
    
    Args:
        client_id: OAuth2 client ID
        client_secret: OAuth2 client secret
        token_url: URL to obtain tokens
        scope: Optional scope for the token
        token: Optional existing token
    """
    
    def __init__(
        self,
        client_id: str,
        client_secret: str,
        token_url: str,
        scope: Optional[str] = None,
        token: Optional[OAuth2Token] = None,
    ) -> None:
        self.client_id = client_id
        self.client_secret = SecretStr(client_secret)
        self.token_url = token_url
        self.scope = scope
        self._token = token
        self._http_client = httpx.AsyncClient()
    
    async def get_headers(self) -> Dict[str, str]:
        """Get OAuth2 headers.
        
        Returns:
            Dictionary containing the authorization header
            
        Raises:
            AuthenticationError: If no valid token is available
        """
        await self.refresh_if_needed()
        
        if self._token is None:
            raise AuthenticationError("No valid OAuth2 token available")
        
        return {
            "Authorization": f"{self._token.token_type} {self._token.access_token.get_secret_value()}"
        }
    
    async def refresh_if_needed(self) -> None:
        """Refresh token if needed.
        
        Raises:
            AuthenticationError: If token refresh fails
        """
        if self._token is None or self._token.is_expired:
            await self._refresh_token()
    
    async def _refresh_token(self) -> None:
        """Refresh the OAuth2 token.
        
        Raises:
            AuthenticationError: If token refresh fails
        """
        data = {
            "grant_type": "client_credentials",
            "client_id": self.client_id,
            "client_secret": self.client_secret.get_secret_value(),
        }
        
        if self.scope:
            data["scope"] = self.scope
        
        # If we have a refresh token, use it
        if self._token and self._token.refresh_token:
            data["grant_type"] = "refresh_token"
            data["refresh_token"] = self._token.refresh_token.get_secret_value()
        
        try:
            response = await self._http_client.post(
                self.token_url,
                data=data,
                headers={"Content-Type": "application/x-www-form-urlencoded"},
            )
            response.raise_for_status()
            
            token_data = response.json()
            self._token = OAuth2Token(**token_data)
            
        except httpx.HTTPStatusError as e:
            error_msg = f"OAuth2 token refresh failed: {e.response.status_code}"
            try:
                error_data = e.response.json()
                if "error_description" in error_data:
                    error_msg += f" - {error_data['error_description']}"
                elif "error" in error_data:
                    error_msg += f" - {error_data['error']}"
            except Exception:
                pass
            
            raise AuthenticationError(error_msg) from e
        
        except Exception as e:
            raise AuthenticationError(f"OAuth2 token refresh failed: {str(e)}") from e
    
    async def close(self) -> None:
        """Close the HTTP client."""
        await self._http_client.aclose()
    
    def __del__(self) -> None:
        """Cleanup when object is destroyed."""
        # Note: This is not guaranteed to be called, but provides cleanup
        # in case the user forgets to call close()
        try:
            import asyncio
            loop = asyncio.get_event_loop()
            if loop.is_running():
                loop.create_task(self._http_client.aclose())
        except Exception:
            pass