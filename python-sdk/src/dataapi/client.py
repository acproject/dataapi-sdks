"""HTTP client for DataAPI SDK."""

import asyncio
import json
from typing import Any, Dict, List, Optional, Union
from urllib.parse import urljoin

import httpx
from pydantic import ValidationError

from .auth import AuthProvider
from .config import ClientConfig
from .exceptions import (
    AuthenticationError,
    DataAPIError,
    NetworkError,
    RateLimitError,
    ServerError,
    ValidationError as DataAPIValidationError,
    create_error_from_response,
)
from .types import BaseDataAPIModel


class HTTPClient:
    """HTTP client for making requests to DataAPI.
    
    Args:
        config: Client configuration
        auth: Authentication provider
    """
    
    def __init__(self, config: ClientConfig, auth: AuthProvider) -> None:
        self.config = config
        self.auth = auth
        self._client: Optional[httpx.AsyncClient] = None
        self._sync_client: Optional[httpx.Client] = None
    
    def _get_headers(self) -> Dict[str, str]:
        """Get headers for requests."""
        headers = self.config.get_headers()
        auth_headers = self.auth.get_headers()
        headers.update(auth_headers)
        return headers
    
    def _build_url(self, endpoint: str) -> str:
        """Build full URL from endpoint."""
        return urljoin(self.config.base_url, endpoint.lstrip("/"))
    
    def _handle_response(self, response: httpx.Response) -> Any:
        """Handle HTTP response and extract data."""
        if response.is_success:
            if response.headers.get("content-type", "").startswith("application/json"):
                try:
                    return response.json()
                except json.JSONDecodeError as e:
                    raise DataAPIError(f"Invalid JSON response: {e}") from e
            return response.text
        
        # Handle error responses
        error = create_error_from_response(response)
        raise error
    
    async def _get_async_client(self) -> httpx.AsyncClient:
        """Get or create async HTTP client."""
        if self._client is None:
            self._client = httpx.AsyncClient(
                timeout=self.config.timeout,
                verify=self.config.verify_ssl,
                follow_redirects=True,
            )
        return self._client
    
    def _get_sync_client(self) -> httpx.Client:
        """Get or create sync HTTP client."""
        if self._sync_client is None:
            self._sync_client = httpx.Client(
                timeout=self.config.timeout,
                verify=self.config.verify_ssl,
                follow_redirects=True,
            )
        return self._sync_client
    
    async def _make_request_async(
        self,
        method: str,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
        json_data: Optional[Dict[str, Any]] = None,
        data: Optional[Dict[str, Any]] = None,
        files: Optional[Dict[str, Any]] = None,
        retry_count: int = 0,
    ) -> Any:
        """Make async HTTP request with retry logic."""
        client = await self._get_async_client()
        url = self._build_url(endpoint)
        headers = self._get_headers()
        
        try:
            response = await client.request(
                method=method,
                url=url,
                params=params,
                json=json_data,
                data=data,
                files=files,
                headers=headers,
            )
            return self._handle_response(response)
        
        except httpx.TimeoutException as e:
            if retry_count < self.config.max_retries:
                await asyncio.sleep(self.config.retry_delay * (2 ** retry_count))
                return await self._make_request_async(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise NetworkError(f"Request timeout: {e}") from e
        
        except httpx.NetworkError as e:
            if retry_count < self.config.max_retries:
                await asyncio.sleep(self.config.retry_delay * (2 ** retry_count))
                return await self._make_request_async(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise NetworkError(f"Network error: {e}") from e
        
        except RateLimitError:
            if retry_count < self.config.max_retries:
                # Exponential backoff for rate limiting
                await asyncio.sleep(self.config.retry_delay * (2 ** retry_count))
                return await self._make_request_async(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise
    
    def _make_request_sync(
        self,
        method: str,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
        json_data: Optional[Dict[str, Any]] = None,
        data: Optional[Dict[str, Any]] = None,
        files: Optional[Dict[str, Any]] = None,
        retry_count: int = 0,
    ) -> Any:
        """Make sync HTTP request with retry logic."""
        client = self._get_sync_client()
        url = self._build_url(endpoint)
        headers = self._get_headers()
        
        try:
            response = client.request(
                method=method,
                url=url,
                params=params,
                json=json_data,
                data=data,
                files=files,
                headers=headers,
            )
            return self._handle_response(response)
        
        except httpx.TimeoutException as e:
            if retry_count < self.config.max_retries:
                import time
                time.sleep(self.config.retry_delay * (2 ** retry_count))
                return self._make_request_sync(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise NetworkError(f"Request timeout: {e}") from e
        
        except httpx.NetworkError as e:
            if retry_count < self.config.max_retries:
                import time
                time.sleep(self.config.retry_delay * (2 ** retry_count))
                return self._make_request_sync(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise NetworkError(f"Network error: {e}") from e
        
        except RateLimitError:
            if retry_count < self.config.max_retries:
                import time
                time.sleep(self.config.retry_delay * (2 ** retry_count))
                return self._make_request_sync(
                    method, endpoint, params, json_data, data, files, retry_count + 1
                )
            raise
    
    # Async methods
    async def get_async(
        self,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make async GET request."""
        data = await self._make_request_async("GET", endpoint, params=params)
        if response_model and data:
            try:
                if isinstance(data, list):
                    return [response_model(**item) for item in data]
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    async def post_async(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        data: Optional[Dict[str, Any]] = None,
        files: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make async POST request."""
        response_data = await self._make_request_async(
            "POST", endpoint, json_data=json_data, data=data, files=files
        )
        if response_model and response_data:
            try:
                return response_model(**response_data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return response_data
    
    async def put_async(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make async PUT request."""
        data = await self._make_request_async("PUT", endpoint, json_data=json_data)
        if response_model and data:
            try:
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    async def patch_async(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make async PATCH request."""
        data = await self._make_request_async("PATCH", endpoint, json_data=json_data)
        if response_model and data:
            try:
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    async def delete_async(
        self,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
    ) -> Any:
        """Make async DELETE request."""
        return await self._make_request_async("DELETE", endpoint, params=params)
    
    # Sync methods
    def get(
        self,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make sync GET request."""
        data = self._make_request_sync("GET", endpoint, params=params)
        if response_model and data:
            try:
                if isinstance(data, list):
                    return [response_model(**item) for item in data]
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    def post(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        data: Optional[Dict[str, Any]] = None,
        files: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make sync POST request."""
        response_data = self._make_request_sync(
            "POST", endpoint, json_data=json_data, data=data, files=files
        )
        if response_model and response_data:
            try:
                return response_model(**response_data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return response_data
    
    def put(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make sync PUT request."""
        data = self._make_request_sync("PUT", endpoint, json_data=json_data)
        if response_model and data:
            try:
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    def patch(
        self,
        endpoint: str,
        json_data: Optional[Dict[str, Any]] = None,
        response_model: Optional[type] = None,
    ) -> Any:
        """Make sync PATCH request."""
        data = self._make_request_sync("PATCH", endpoint, json_data=json_data)
        if response_model and data:
            try:
                return response_model(**data)
            except (ValidationError, TypeError) as e:
                raise DataAPIValidationError(f"Response validation failed: {e}") from e
        return data
    
    def delete(
        self,
        endpoint: str,
        params: Optional[Dict[str, Any]] = None,
    ) -> Any:
        """Make sync DELETE request."""
        return self._make_request_sync("DELETE", endpoint, params=params)
    
    async def close_async(self) -> None:
        """Close async HTTP client."""
        if self._client:
            await self._client.aclose()
            self._client = None
    
    def close(self) -> None:
        """Close sync HTTP client."""
        if self._sync_client:
            self._sync_client.close()
            self._sync_client = None
    
    async def __aenter__(self):
        """Async context manager entry."""
        return self
    
    async def __aexit__(self, exc_type, exc_val, exc_tb):
        """Async context manager exit."""
        await self.close_async()
    
    def __enter__(self):
        """Sync context manager entry."""
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        """Sync context manager exit."""
        self.close()