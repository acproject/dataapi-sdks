"""Tests for configuration module."""

import pytest
from dataapi.config import ClientConfig


class TestClientConfig:
    """Test cases for ClientConfig class."""
    
    def test_default_config(self):
        """Test default configuration values."""
        config = ClientConfig()
        
        assert config.base_url == "https://api.dataapi.com"
        assert config.timeout == 30.0
        assert config.max_retries == 3
        assert config.retry_delay == 1.0
        assert config.user_agent.startswith("DataAPI-Python-SDK/")
        assert config.verify_ssl is True
    
    def test_custom_config(self):
        """Test custom configuration values."""
        config = ClientConfig(
            base_url="https://custom.api.com",
            timeout=60.0,
            max_retries=5,
            retry_delay=2.0,
            user_agent="Custom-Agent/1.0",
            verify_ssl=False
        )
        
        assert config.base_url == "https://custom.api.com"
        assert config.timeout == 60.0
        assert config.max_retries == 5
        assert config.retry_delay == 2.0
        assert config.user_agent == "Custom-Agent/1.0"
        assert config.verify_ssl is False
    
    def test_get_headers(self):
        """Test header generation."""
        config = ClientConfig(user_agent="Test-Agent/1.0")
        headers = config.get_headers()
        
        assert "User-Agent" in headers
        assert headers["User-Agent"] == "Test-Agent/1.0"
        assert "Accept" in headers
        assert headers["Accept"] == "application/json"
        assert "Content-Type" in headers
        assert headers["Content-Type"] == "application/json"
    
    def test_config_validation(self):
        """Test configuration validation."""
        # Test negative timeout
        with pytest.raises(ValueError):
            ClientConfig(timeout=-1.0)
        
        # Test negative max_retries
        with pytest.raises(ValueError):
            ClientConfig(max_retries=-1)
        
        # Test negative retry_delay
        with pytest.raises(ValueError):
            ClientConfig(retry_delay=-1.0)
        
        # Test invalid base_url
        with pytest.raises(ValueError):
            ClientConfig(base_url="not-a-url")
    
    def test_config_immutability(self):
        """Test that config is immutable after creation."""
        config = ClientConfig()
        
        # Should not be able to modify after creation
        with pytest.raises(AttributeError):
            config.base_url = "https://new.url.com"
    
    def test_config_serialization(self):
        """Test config serialization."""
        config = ClientConfig(
            base_url="https://test.api.com",
            timeout=45.0,
            max_retries=2
        )
        
        # Test dict conversion
        config_dict = config.model_dump()
        assert config_dict["base_url"] == "https://test.api.com"
        assert config_dict["timeout"] == 45.0
        assert config_dict["max_retries"] == 2
        
        # Test recreation from dict
        new_config = ClientConfig(**config_dict)
        assert new_config.base_url == config.base_url
        assert new_config.timeout == config.timeout
        assert new_config.max_retries == config.max_retries