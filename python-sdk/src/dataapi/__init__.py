"""DataAPI Python SDK.

A comprehensive Python SDK for interacting with the DataAPI platform.
Provides easy-to-use interfaces for database management, AI services, and workflow automation.
"""

from .auth import APIKeyAuth, BearerTokenAuth, OAuth2Auth
from .config import ClientConfig
from .dataapi_client import DataAPIClient
from .exceptions import (
    AuthenticationError,
    DataAPIError,
    NetworkError,
    NotFoundError,
    RateLimitError,
    ServerError,
    ValidationError,
)
from .types import (
    AIModel,
    AIProvider,
    AIResponse,
    Database,
    Record,
    Table,
    User,
    Workflow,
    WorkflowExecution,
    WorkflowStatus,
)

__version__ = "1.0.0"
__author__ = "DataAPI Team"
__email__ = "support@dataapi.com"
__description__ = "Python SDK for DataAPI platform"
__url__ = "https://github.com/dataapi/dataapi-sdks"

__all__ = [
    # Main client
    "DataAPIClient",
    # Authentication
    "APIKeyAuth",
    "BearerTokenAuth",
    "OAuth2Auth",
    # Configuration
    "ClientConfig",
    # Exceptions
    "DataAPIError",
    "AuthenticationError",
    "NetworkError",
    "NotFoundError",
    "RateLimitError",
    "ServerError",
    "ValidationError",
    # Types
    "Database",
    "Table",
    "Record",
    "User",
    "AIModel",
    "AIProvider",
    "AIResponse",
    "Workflow",
    "WorkflowExecution",
    "WorkflowStatus",
]