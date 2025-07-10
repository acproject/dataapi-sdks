"""Type definitions for DataAPI SDK."""

from datetime import datetime
from enum import Enum
from typing import Any, Dict, List, Optional, Union

from pydantic import BaseModel, Field


class BaseDataAPIModel(BaseModel):
    """Base model for all DataAPI types."""
    
    class Config:
        """Pydantic configuration."""
        extra = "forbid"
        use_enum_values = True
        json_encoders = {
            datetime: lambda v: v.isoformat(),
        }


# Database Types
class Database(BaseDataAPIModel):
    """Database model.
    
    Args:
        id: Unique database identifier
        name: Database name
        description: Optional database description
        created_at: Creation timestamp
        updated_at: Last update timestamp
        owner_id: ID of the database owner
        metadata: Additional metadata
    """
    
    id: str
    name: str
    description: Optional[str] = None
    created_at: datetime
    updated_at: datetime
    owner_id: str
    metadata: Dict[str, Any] = Field(default_factory=dict)


class ColumnType(str, Enum):
    """Supported column types."""
    STRING = "string"
    INTEGER = "integer"
    FLOAT = "float"
    BOOLEAN = "boolean"
    DATETIME = "datetime"
    JSON = "json"
    TEXT = "text"
    BINARY = "binary"


class ColumnDefinition(BaseDataAPIModel):
    """Column definition for table schema.
    
    Args:
        name: Column name
        type: Column data type
        nullable: Whether column can be null
        primary_key: Whether column is primary key
        unique: Whether column values must be unique
        default: Default value for column
        description: Optional column description
    """
    
    name: str
    type: ColumnType
    nullable: bool = True
    primary_key: bool = False
    unique: bool = False
    default: Optional[Any] = None
    description: Optional[str] = None


class Table(BaseDataAPIModel):
    """Table model.
    
    Args:
        id: Unique table identifier
        name: Table name
        database_id: ID of the parent database
        schema: Table schema definition
        description: Optional table description
        created_at: Creation timestamp
        updated_at: Last update timestamp
        row_count: Number of rows in table
        metadata: Additional metadata
    """
    
    id: str
    name: str
    database_id: str
    schema: List[ColumnDefinition]
    description: Optional[str] = None
    created_at: datetime
    updated_at: datetime
    row_count: int = 0
    metadata: Dict[str, Any] = Field(default_factory=dict)


class Record(BaseDataAPIModel):
    """Record model.
    
    Args:
        id: Unique record identifier
        table_id: ID of the parent table
        data: Record data as key-value pairs
        created_at: Creation timestamp
        updated_at: Last update timestamp
        version: Record version for optimistic locking
    """
    
    id: str
    table_id: str
    data: Dict[str, Any]
    created_at: datetime
    updated_at: datetime
    version: int = 1


# User and Project Types
class UserRole(str, Enum):
    """User roles."""
    ADMIN = "admin"
    DEVELOPER = "developer"
    VIEWER = "viewer"


class User(BaseDataAPIModel):
    """User model.
    
    Args:
        id: Unique user identifier
        email: User email address
        name: User display name
        role: User role
        created_at: Creation timestamp
        updated_at: Last update timestamp
        is_active: Whether user is active
        metadata: Additional metadata
    """
    
    id: str
    email: str
    name: str
    role: UserRole
    created_at: datetime
    updated_at: datetime
    is_active: bool = True
    metadata: Dict[str, Any] = Field(default_factory=dict)


class Project(BaseDataAPIModel):
    """Project model.
    
    Args:
        id: Unique project identifier
        name: Project name
        description: Optional project description
        owner_id: ID of the project owner
        created_at: Creation timestamp
        updated_at: Last update timestamp
        is_active: Whether project is active
        settings: Project settings
        metadata: Additional metadata
    """
    
    id: str
    name: str
    description: Optional[str] = None
    owner_id: str
    created_at: datetime
    updated_at: datetime
    is_active: bool = True
    settings: Dict[str, Any] = Field(default_factory=dict)
    metadata: Dict[str, Any] = Field(default_factory=dict)


# AI Types
class AIProvider(str, Enum):
    """Supported AI providers."""
    OPENAI = "openai"
    ANTHROPIC = "anthropic"
    GOOGLE = "google"
    AZURE = "azure"
    HUGGINGFACE = "huggingface"


class AIModel(BaseDataAPIModel):
    """AI model definition.
    
    Args:
        id: Unique model identifier
        name: Model name
        provider: AI provider
        description: Optional model description
        capabilities: List of model capabilities
        max_tokens: Maximum tokens supported
        cost_per_token: Cost per token
        is_available: Whether model is available
        metadata: Additional metadata
    """
    
    id: str
    name: str
    provider: AIProvider
    description: Optional[str] = None
    capabilities: List[str] = Field(default_factory=list)
    max_tokens: Optional[int] = None
    cost_per_token: Optional[float] = None
    is_available: bool = True
    metadata: Dict[str, Any] = Field(default_factory=dict)


class AIResponse(BaseDataAPIModel):
    """AI response model.
    
    Args:
        id: Unique response identifier
        model_id: ID of the model used
        prompt: Input prompt
        response: Generated response
        tokens_used: Number of tokens consumed
        cost: Cost of the request
        created_at: Creation timestamp
        metadata: Additional metadata
    """
    
    id: str
    model_id: str
    prompt: str
    response: str
    tokens_used: int
    cost: Optional[float] = None
    created_at: datetime
    metadata: Dict[str, Any] = Field(default_factory=dict)


# Workflow Types
class WorkflowStatus(str, Enum):
    """Workflow execution status."""
    PENDING = "pending"
    RUNNING = "running"
    COMPLETED = "completed"
    FAILED = "failed"
    CANCELLED = "cancelled"


class WorkflowStep(BaseDataAPIModel):
    """Workflow step definition.
    
    Args:
        id: Step identifier
        type: Step type
        name: Step name
        description: Optional step description
        config: Step configuration
        depends_on: List of step IDs this step depends on
    """
    
    id: str
    type: str
    name: str
    description: Optional[str] = None
    config: Dict[str, Any] = Field(default_factory=dict)
    depends_on: List[str] = Field(default_factory=list)


class Workflow(BaseDataAPIModel):
    """Workflow model.
    
    Args:
        id: Unique workflow identifier
        name: Workflow name
        description: Optional workflow description
        steps: List of workflow steps
        created_at: Creation timestamp
        updated_at: Last update timestamp
        owner_id: ID of the workflow owner
        is_active: Whether workflow is active
        metadata: Additional metadata
    """
    
    id: str
    name: str
    description: Optional[str] = None
    steps: List[WorkflowStep]
    created_at: datetime
    updated_at: datetime
    owner_id: str
    is_active: bool = True
    metadata: Dict[str, Any] = Field(default_factory=dict)


class WorkflowExecution(BaseDataAPIModel):
    """Workflow execution model.
    
    Args:
        id: Unique execution identifier
        workflow_id: ID of the workflow being executed
        status: Current execution status
        started_at: Execution start timestamp
        completed_at: Execution completion timestamp
        error_message: Error message if execution failed
        results: Execution results
        metadata: Additional metadata
    """
    
    id: str
    workflow_id: str
    status: WorkflowStatus
    started_at: datetime
    completed_at: Optional[datetime] = None
    error_message: Optional[str] = None
    results: Dict[str, Any] = Field(default_factory=dict)
    metadata: Dict[str, Any] = Field(default_factory=dict)


# Request/Response Types
class PageInfo(BaseDataAPIModel):
    """Pagination information.
    
    Args:
        page: Current page number
        per_page: Items per page
        total: Total number of items
        total_pages: Total number of pages
        has_next: Whether there is a next page
        has_prev: Whether there is a previous page
    """
    
    page: int
    per_page: int
    total: int
    total_pages: int
    has_next: bool
    has_prev: bool


class PaginatedResponse(BaseDataAPIModel):
    """Paginated response model.
    
    Args:
        data: List of items
        pagination: Pagination information
    """
    
    data: List[Any]
    pagination: PageInfo


# Query Types
class SortOrder(str, Enum):
    """Sort order options."""
    ASC = "asc"
    DESC = "desc"


class SortField(BaseDataAPIModel):
    """Sort field specification.
    
    Args:
        field: Field name to sort by
        order: Sort order
    """
    
    field: str
    order: SortOrder = SortOrder.ASC


class FilterOperator(str, Enum):
    """Filter operators."""
    EQ = "eq"  # Equal
    NE = "ne"  # Not equal
    GT = "gt"  # Greater than
    GTE = "gte"  # Greater than or equal
    LT = "lt"  # Less than
    LTE = "lte"  # Less than or equal
    IN = "in"  # In list
    NOT_IN = "not_in"  # Not in list
    LIKE = "like"  # Like (string matching)
    ILIKE = "ilike"  # Case-insensitive like
    IS_NULL = "is_null"  # Is null
    IS_NOT_NULL = "is_not_null"  # Is not null


class Filter(BaseDataAPIModel):
    """Filter specification.
    
    Args:
        field: Field name to filter on
        operator: Filter operator
        value: Filter value
    """
    
    field: str
    operator: FilterOperator
    value: Optional[Union[str, int, float, bool, List[Any]]] = None


class QueryOptions(BaseDataAPIModel):
    """Query options for list operations.
    
    Args:
        page: Page number (1-based)
        per_page: Items per page
        sort: List of sort fields
        filters: List of filters
        search: Search query
    """
    
    page: int = Field(default=1, ge=1)
    per_page: int = Field(default=20, ge=1, le=100)
    sort: List[SortField] = Field(default_factory=list)
    filters: List[Filter] = Field(default_factory=list)
    search: Optional[str] = None