"""Client configuration for DataAPI SDK."""

from typing import Optional
from pydantic import BaseModel, Field, HttpUrl


class ClientConfig(BaseModel):
    """Configuration for DataAPI client.
    
    Args:
        base_url: Base URL for the DataAPI service
        timeout: Request timeout in seconds
        max_retries: Maximum number of retry attempts
        retry_delay: Delay between retries in seconds
        user_agent: User agent string for requests
        verify_ssl: Whether to verify SSL certificates
    """
    
    base_url: HttpUrl = Field(
        default="https://api.dataapi.com",
        description="Base URL for the DataAPI service"
    )
    timeout: float = Field(
        default=30.0,
        gt=0,
        description="Request timeout in seconds"
    )
    max_retries: int = Field(
        default=3,
        ge=0,
        description="Maximum number of retry attempts"
    )
    retry_delay: float = Field(
        default=1.0,
        ge=0,
        description="Delay between retries in seconds"
    )
    user_agent: str = Field(
        default="DataAPI-Python-SDK/1.0.0",
        description="User agent string for requests"
    )
    verify_ssl: bool = Field(
        default=True,
        description="Whether to verify SSL certificates"
    )
    
    class Config:
        """Pydantic configuration."""
        frozen = True
        extra = "forbid"
        
    def model_dump_headers(self) -> dict[str, str]:
        """Get headers for HTTP requests.
        
        Returns:
            Dictionary of headers
        """
        return {
            "User-Agent": self.user_agent,
            "Accept": "application/json",
            "Content-Type": "application/json",
        }