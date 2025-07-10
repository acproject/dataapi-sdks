"""Tests for types module."""

import pytest
from datetime import datetime
from typing import Any, Dict, List

from dataapi.types import (
    BaseDataAPIModel,
    Database,
    ColumnType,
    ColumnDefinition,
    Table,
    Record,
    UserRole,
    User,
    AIProvider,
    AIModel,
    AIResponse,
    WorkflowStatus,
    WorkflowStep,
    Workflow,
    WorkflowExecution,
    PageInfo,
    PaginatedResponse,
    SortOrder,
    SortField,
    FilterOperator,
    Filter,
    QueryOptions
)


class TestBaseDataAPIModel:
    """Test cases for BaseDataAPIModel class."""
    
    def test_base_model_creation(self):
        """Test base model creation."""
        model = BaseDataAPIModel()
        assert isinstance(model, BaseDataAPIModel)
    
    def test_base_model_serialization(self):
        """Test base model serialization."""
        model = BaseDataAPIModel()
        data = model.model_dump()
        assert isinstance(data, dict)


class TestDatabase:
    """Test cases for Database model."""
    
    def test_database_creation(self):
        """Test database creation with required fields."""
        db = Database(
            id="db-123",
            name="test_database",
            description="Test database",
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert db.id == "db-123"
        assert db.name == "test_database"
        assert db.description == "Test database"
        assert isinstance(db.created_at, datetime)
        assert isinstance(db.updated_at, datetime)
    
    def test_database_with_metadata(self):
        """Test database creation with metadata."""
        metadata = {"project": "test", "environment": "dev"}
        db = Database(
            id="db-123",
            name="test_database",
            description="Test database",
            created_at=datetime.now(),
            updated_at=datetime.now(),
            metadata=metadata
        )
        
        assert db.metadata == metadata
    
    def test_database_optional_fields(self):
        """Test database with optional fields."""
        db = Database(
            id="db-123",
            name="test_database",
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert db.description is None
        assert db.metadata is None


class TestColumnDefinition:
    """Test cases for ColumnDefinition model."""
    
    def test_column_definition_basic(self):
        """Test basic column definition."""
        col = ColumnDefinition(
            name="id",
            type=ColumnType.INTEGER
        )
        
        assert col.name == "id"
        assert col.type == ColumnType.INTEGER
        assert col.nullable is True  # Default
        assert col.primary_key is False  # Default
        assert col.unique is False  # Default
    
    def test_column_definition_with_constraints(self):
        """Test column definition with constraints."""
        col = ColumnDefinition(
            name="email",
            type=ColumnType.STRING,
            nullable=False,
            unique=True,
            max_length=255
        )
        
        assert col.name == "email"
        assert col.type == ColumnType.STRING
        assert col.nullable is False
        assert col.unique is True
        assert col.max_length == 255
    
    def test_column_definition_primary_key(self):
        """Test primary key column definition."""
        col = ColumnDefinition(
            name="id",
            type=ColumnType.INTEGER,
            primary_key=True,
            nullable=False
        )
        
        assert col.primary_key is True
        assert col.nullable is False


class TestTable:
    """Test cases for Table model."""
    
    def test_table_creation(self):
        """Test table creation."""
        schema = [
            ColumnDefinition(name="id", type=ColumnType.INTEGER, primary_key=True),
            ColumnDefinition(name="name", type=ColumnType.STRING)
        ]
        
        table = Table(
            id="table-123",
            name="users",
            database_id="db-123",
            schema=schema,
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert table.id == "table-123"
        assert table.name == "users"
        assert table.database_id == "db-123"
        assert len(table.schema) == 2
        assert table.schema[0].name == "id"
        assert table.schema[1].name == "name"
    
    def test_table_with_description(self):
        """Test table with description."""
        table = Table(
            id="table-123",
            name="users",
            database_id="db-123",
            schema=[],
            description="User information table",
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert table.description == "User information table"


class TestRecord:
    """Test cases for Record model."""
    
    def test_record_creation(self):
        """Test record creation."""
        data = {"id": 1, "name": "John Doe", "email": "john@example.com"}
        record = Record(
            id="record-123",
            table_id="table-123",
            data=data,
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert record.id == "record-123"
        assert record.table_id == "table-123"
        assert record.data == data
        assert record.data["name"] == "John Doe"


class TestUser:
    """Test cases for User model."""
    
    def test_user_creation(self):
        """Test user creation."""
        user = User(
            id="user-123",
            username="johndoe",
            email="john@example.com",
            role=UserRole.USER,
            created_at=datetime.now()
        )
        
        assert user.id == "user-123"
        assert user.username == "johndoe"
        assert user.email == "john@example.com"
        assert user.role == UserRole.USER
    
    def test_user_admin_role(self):
        """Test user with admin role."""
        user = User(
            id="user-123",
            username="admin",
            email="admin@example.com",
            role=UserRole.ADMIN,
            created_at=datetime.now()
        )
        
        assert user.role == UserRole.ADMIN


class TestAIModel:
    """Test cases for AIModel model."""
    
    def test_ai_model_creation(self):
        """Test AI model creation."""
        model = AIModel(
            id="model-123",
            name="GPT-4",
            provider=AIProvider.OPENAI,
            description="Advanced language model",
            capabilities=["text_generation", "chat"],
            max_tokens=4096,
            created_at=datetime.now()
        )
        
        assert model.id == "model-123"
        assert model.name == "GPT-4"
        assert model.provider == AIProvider.OPENAI
        assert "text_generation" in model.capabilities
        assert model.max_tokens == 4096
    
    def test_ai_model_different_providers(self):
        """Test AI models with different providers."""
        models = [
            AIModel(
                id="model-1",
                name="GPT-4",
                provider=AIProvider.OPENAI,
                capabilities=["text_generation"],
                created_at=datetime.now()
            ),
            AIModel(
                id="model-2",
                name="Claude",
                provider=AIProvider.ANTHROPIC,
                capabilities=["text_generation"],
                created_at=datetime.now()
            ),
            AIModel(
                id="model-3",
                name="Gemini",
                provider=AIProvider.GOOGLE,
                capabilities=["text_generation"],
                created_at=datetime.now()
            )
        ]
        
        assert models[0].provider == AIProvider.OPENAI
        assert models[1].provider == AIProvider.ANTHROPIC
        assert models[2].provider == AIProvider.GOOGLE


class TestAIResponse:
    """Test cases for AIResponse model."""
    
    def test_ai_response_creation(self):
        """Test AI response creation."""
        response = AIResponse(
            id="response-123",
            model_id="model-123",
            response="This is a generated response.",
            tokens_used=25,
            created_at=datetime.now()
        )
        
        assert response.id == "response-123"
        assert response.model_id == "model-123"
        assert response.response == "This is a generated response."
        assert response.tokens_used == 25
    
    def test_ai_response_with_metadata(self):
        """Test AI response with metadata."""
        metadata = {"temperature": 0.7, "max_tokens": 100}
        response = AIResponse(
            id="response-123",
            model_id="model-123",
            response="Generated text",
            tokens_used=25,
            metadata=metadata,
            created_at=datetime.now()
        )
        
        assert response.metadata == metadata
        assert response.metadata["temperature"] == 0.7


class TestWorkflow:
    """Test cases for Workflow model."""
    
    def test_workflow_creation(self):
        """Test workflow creation."""
        steps = [
            WorkflowStep(
                id="step1",
                type="data_extraction",
                name="Extract Data",
                config={"source": "database"}
            ),
            WorkflowStep(
                id="step2",
                type="data_transformation",
                name="Transform Data",
                config={"format": "json"},
                depends_on=["step1"]
            )
        ]
        
        workflow = Workflow(
            id="workflow-123",
            name="Data Pipeline",
            steps=steps,
            created_at=datetime.now(),
            updated_at=datetime.now()
        )
        
        assert workflow.id == "workflow-123"
        assert workflow.name == "Data Pipeline"
        assert len(workflow.steps) == 2
        assert workflow.steps[0].id == "step1"
        assert workflow.steps[1].depends_on == ["step1"]


class TestWorkflowExecution:
    """Test cases for WorkflowExecution model."""
    
    def test_workflow_execution_creation(self):
        """Test workflow execution creation."""
        execution = WorkflowExecution(
            id="exec-123",
            workflow_id="workflow-123",
            status=WorkflowStatus.RUNNING,
            started_at=datetime.now()
        )
        
        assert execution.id == "exec-123"
        assert execution.workflow_id == "workflow-123"
        assert execution.status == WorkflowStatus.RUNNING
        assert execution.completed_at is None
    
    def test_workflow_execution_completed(self):
        """Test completed workflow execution."""
        now = datetime.now()
        execution = WorkflowExecution(
            id="exec-123",
            workflow_id="workflow-123",
            status=WorkflowStatus.COMPLETED,
            started_at=now,
            completed_at=now
        )
        
        assert execution.status == WorkflowStatus.COMPLETED
        assert execution.completed_at is not None


class TestPaginatedResponse:
    """Test cases for PaginatedResponse model."""
    
    def test_paginated_response_creation(self):
        """Test paginated response creation."""
        data = [{"id": 1, "name": "Item 1"}, {"id": 2, "name": "Item 2"}]
        page_info = PageInfo(
            page=1,
            per_page=10,
            total_pages=5,
            total_items=50,
            has_next=True,
            has_previous=False
        )
        
        response = PaginatedResponse[Dict[str, Any]](
            data=data,
            page_info=page_info
        )
        
        assert len(response.data) == 2
        assert response.page_info.page == 1
        assert response.page_info.total_items == 50
        assert response.page_info.has_next is True


class TestQueryOptions:
    """Test cases for QueryOptions model."""
    
    def test_query_options_basic(self):
        """Test basic query options."""
        options = QueryOptions()
        
        assert options.page == 1
        assert options.per_page == 20
        assert options.sort == []
        assert options.filters == []
    
    def test_query_options_with_sorting(self):
        """Test query options with sorting."""
        sort_fields = [
            SortField(field="name", order=SortOrder.ASC),
            SortField(field="created_at", order=SortOrder.DESC)
        ]
        
        options = QueryOptions(
            page=2,
            per_page=50,
            sort=sort_fields
        )
        
        assert options.page == 2
        assert options.per_page == 50
        assert len(options.sort) == 2
        assert options.sort[0].field == "name"
        assert options.sort[0].order == SortOrder.ASC
    
    def test_query_options_with_filters(self):
        """Test query options with filters."""
        filters = [
            Filter(field="status", operator=FilterOperator.EQ, value="active"),
            Filter(field="age", operator=FilterOperator.GTE, value=18)
        ]
        
        options = QueryOptions(
            filters=filters
        )
        
        assert len(options.filters) == 2
        assert options.filters[0].field == "status"
        assert options.filters[0].operator == FilterOperator.EQ
        assert options.filters[0].value == "active"
    
    def test_query_options_validation(self):
        """Test query options validation."""
        # Test invalid page
        with pytest.raises(ValueError):
            QueryOptions(page=0)
        
        # Test invalid per_page
        with pytest.raises(ValueError):
            QueryOptions(per_page=0)
        
        # Test per_page too large
        with pytest.raises(ValueError):
            QueryOptions(per_page=1001)


class TestEnums:
    """Test cases for enum types."""
    
    def test_column_types(self):
        """Test ColumnType enum values."""
        assert ColumnType.STRING == "string"
        assert ColumnType.INTEGER == "integer"
        assert ColumnType.FLOAT == "float"
        assert ColumnType.BOOLEAN == "boolean"
        assert ColumnType.DATETIME == "datetime"
        assert ColumnType.JSON == "json"
    
    def test_user_roles(self):
        """Test UserRole enum values."""
        assert UserRole.ADMIN == "admin"
        assert UserRole.USER == "user"
        assert UserRole.VIEWER == "viewer"
    
    def test_ai_providers(self):
        """Test AIProvider enum values."""
        assert AIProvider.OPENAI == "openai"
        assert AIProvider.ANTHROPIC == "anthropic"
        assert AIProvider.GOOGLE == "google"
        assert AIProvider.CUSTOM == "custom"
    
    def test_workflow_status(self):
        """Test WorkflowStatus enum values."""
        assert WorkflowStatus.PENDING == "pending"
        assert WorkflowStatus.RUNNING == "running"
        assert WorkflowStatus.COMPLETED == "completed"
        assert WorkflowStatus.FAILED == "failed"
        assert WorkflowStatus.CANCELLED == "cancelled"
    
    def test_sort_order(self):
        """Test SortOrder enum values."""
        assert SortOrder.ASC == "asc"
        assert SortOrder.DESC == "desc"
    
    def test_filter_operators(self):
        """Test FilterOperator enum values."""
        assert FilterOperator.EQ == "eq"
        assert FilterOperator.NE == "ne"
        assert FilterOperator.GT == "gt"
        assert FilterOperator.GTE == "gte"
        assert FilterOperator.LT == "lt"
        assert FilterOperator.LTE == "lte"
        assert FilterOperator.IN == "in"
        assert FilterOperator.NOT_IN == "not_in"
        assert FilterOperator.LIKE == "like"
        assert FilterOperator.NOT_LIKE == "not_like"