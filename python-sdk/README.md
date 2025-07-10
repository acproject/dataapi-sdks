# DataAPI Python SDK

A Python client library for the DataAPI service, providing easy access to data management, AI services, and workflow automation.

## Features

- **Authentication**: Support for API key and OAuth2 authentication
- **Data Management**: CRUD operations for databases, tables, and records
- **AI Services**: Integration with various AI providers and models
- **Workflow Management**: Create and execute complex data workflows
- **Type Safety**: Full type hints and Pydantic models
- **Async Support**: Built on httpx for high-performance async operations

## Installation

### Using UV (Recommended)

```bash
uv add dataapi-sdk
```

### Using pip

```bash
pip install dataapi-sdk
```

## Quick Start

```python
import asyncio
from dataapi import DataAPIClient, APIKeyAuth

async def main():
    # Initialize the client
    auth = APIKeyAuth(api_key="your-api-key")
    client = DataAPIClient(auth=auth, base_url="https://api.dataapi.com")
    
    # List databases
    databases = await client.databases.list()
    print(f"Found {len(databases)} databases")
    
    # Create a new database
    new_db = await client.databases.create(
        name="my_database",
        description="My test database"
    )
    print(f"Created database: {new_db.id}")
    
    # Close the client
    await client.close()

if __name__ == "__main__":
    asyncio.run(main())
```

## Authentication

### API Key Authentication

```python
from dataapi import DataAPIClient, APIKeyAuth

auth = APIKeyAuth(api_key="your-api-key")
client = DataAPIClient(auth=auth)
```

### OAuth2 Authentication

```python
from dataapi import DataAPIClient, OAuth2Auth

auth = OAuth2Auth(
    client_id="your-client-id",
    client_secret="your-client-secret",
    token_url="https://auth.dataapi.com/oauth/token"
)
client = DataAPIClient(auth=auth)
```

## Configuration

```python
from dataapi import DataAPIClient, ClientConfig

config = ClientConfig(
    base_url="https://api.dataapi.com",
    timeout=30.0,
    max_retries=3,
    retry_delay=1.0
)

client = DataAPIClient(auth=auth, config=config)
```

## API Reference

### Databases

```python
# List all databases
databases = await client.databases.list()

# Get a specific database
database = await client.databases.get(database_id="db_123")

# Create a new database
new_database = await client.databases.create(
    name="my_database",
    description="Database description"
)

# Update a database
updated_database = await client.databases.update(
    database_id="db_123",
    name="updated_name"
)

# Delete a database
await client.databases.delete(database_id="db_123")
```

### Tables

```python
# List tables in a database
tables = await client.tables.list(database_id="db_123")

# Create a new table
new_table = await client.tables.create(
    database_id="db_123",
    name="users",
    schema={
        "id": {"type": "integer", "primary_key": True},
        "name": {"type": "string", "required": True},
        "email": {"type": "string", "unique": True}
    }
)
```

### AI Services

```python
# List available AI models
models = await client.ai.models.list()

# Generate text
response = await client.ai.generate(
    model="gpt-4",
    prompt="Write a Python function to calculate fibonacci numbers",
    max_tokens=500
)

# Analyze data
analysis = await client.ai.analyze(
    data=my_dataset,
    analysis_type="statistical_summary"
)
```

### Workflows

```python
# Create a workflow
workflow = await client.workflows.create(
    name="Data Processing Pipeline",
    description="Process and analyze customer data",
    steps=[
        {"type": "extract", "source": "database", "table": "customers"},
        {"type": "transform", "operations": ["clean", "normalize"]},
        {"type": "analyze", "model": "statistical_summary"},
        {"type": "load", "destination": "results_table"}
    ]
)

# Execute a workflow
execution = await client.workflows.execute(workflow_id=workflow.id)

# Monitor execution status
status = await client.workflows.get_execution_status(execution_id=execution.id)
```

## Error Handling

```python
from dataapi.exceptions import (
    DataAPIError,
    AuthenticationError,
    ValidationError,
    NotFoundError,
    RateLimitError
)

try:
    database = await client.databases.get("invalid_id")
except NotFoundError:
    print("Database not found")
except AuthenticationError:
    print("Authentication failed")
except ValidationError as e:
    print(f"Validation error: {e.message}")
except DataAPIError as e:
    print(f"API error: {e.message}")
```

## Development

### Setup Development Environment

```bash
# Clone the repository
git clone https://github.com/owiseman/dataapi-client.git
cd dataapi-client/dataapi-sdks/python-sdk

# Install UV if not already installed
curl -LsSf https://astral.sh/uv/install.sh | sh

# Create virtual environment and install dependencies
uv sync

# Activate the virtual environment
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
```

### Running Tests

```bash
# Run all tests
uv run pytest

# Run tests with coverage
uv run pytest --cov=src --cov-report=html

# Run only unit tests
uv run pytest tests/unit/

# Run only integration tests
uv run pytest tests/integration/
```

### Code Quality

```bash
# Format code
uv run black src tests
uv run isort src tests

# Lint code
uv run ruff check src tests

# Type checking
uv run mypy src
```

## Contributing

We welcome contributions! Please see our [Contributing Guide](../CONTRIBUTING.md) for details.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

- Documentation: [https://dataapi-sdk.readthedocs.io](https://dataapi-sdk.readthedocs.io)
- Issues: [GitHub Issues](https://github.com/owiseman/dataapi-client/issues)
- Discussions: [GitHub Discussions](https://github.com/owiseman/dataapi-client/discussions)