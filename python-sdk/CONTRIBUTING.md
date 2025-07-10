# Contributing to DataAPI Python SDK

Thank you for your interest in contributing to the DataAPI Python SDK! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Contributing Guidelines](#contributing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Documentation](#documentation)
- [Release Process](#release-process)
- [Community](#community)

## Code of Conduct

This project adheres to a [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## Getting Started

### Prerequisites

- Python 3.9 or higher
- [UV](https://github.com/astral-sh/uv) package manager
- Git
- Docker (optional, for containerized development)

### Fork and Clone

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/dataapi-client.git
   cd dataapi-client/dataapi-sdks/python-sdk
   ```

3. Add the upstream repository:
   ```bash
   git remote add upstream https://github.com/ORIGINAL_OWNER/dataapi-client.git
   ```

## Development Setup

### Using UV (Recommended)

1. Install dependencies:
   ```bash
   uv sync
   ```

2. Activate the virtual environment:
   ```bash
   source .venv/bin/activate  # On Windows: .venv\Scripts\activate
   ```

3. Install pre-commit hooks:
   ```bash
   uv run pre-commit install
   ```

### Using Docker

1. Build the development container:
   ```bash
   docker-compose build dataapi-sdk-dev
   ```

2. Start the development environment:
   ```bash
   docker-compose run --rm dataapi-sdk-dev
   ```

### Verify Installation

```bash
# Run tests
uv run pytest

# Run linting
uv run ruff check src tests examples

# Run type checking
uv run mypy src/dataapi
```

## Contributing Guidelines

### Types of Contributions

We welcome several types of contributions:

- **Bug Reports**: Report issues you encounter
- **Feature Requests**: Suggest new features or improvements
- **Code Contributions**: Submit bug fixes or new features
- **Documentation**: Improve or add documentation
- **Tests**: Add or improve test coverage
- **Examples**: Provide usage examples

### Before You Start

1. **Check existing issues**: Look for existing issues or discussions
2. **Create an issue**: For significant changes, create an issue first to discuss
3. **Get feedback**: Engage with maintainers and community members

### Branch Strategy

- `main`: Stable release branch
- `develop`: Development branch for upcoming features
- `feature/feature-name`: Feature development branches
- `bugfix/issue-number`: Bug fix branches
- `hotfix/issue-number`: Critical fixes for production

### Commit Guidelines

We follow [Conventional Commits](https://www.conventionalcommits.org/):

```
type(scope): description

[optional body]

[optional footer]
```

#### Types
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks
- `perf`: Performance improvements
- `ci`: CI/CD changes

#### Examples
```
feat(auth): add OAuth2 authentication support

fix(client): handle connection timeout errors

docs(readme): update installation instructions

test(database): add integration tests for table operations
```

## Pull Request Process

### Before Submitting

1. **Update your branch**:
   ```bash
   git fetch upstream
   git rebase upstream/develop
   ```

2. **Run the full test suite**:
   ```bash
   make test-all
   ```

3. **Check code quality**:
   ```bash
   make lint
   make type-check
   ```

4. **Update documentation** if needed

5. **Update CHANGELOG.md** with your changes

### Submitting the PR

1. **Create a descriptive title**:
   - Use conventional commit format
   - Be specific and concise

2. **Write a comprehensive description**:
   - Explain what changes you made and why
   - Reference related issues
   - Include screenshots for UI changes
   - List any breaking changes

3. **Use the PR template**:
   ```markdown
   ## Description
   Brief description of changes

   ## Type of Change
   - [ ] Bug fix
   - [ ] New feature
   - [ ] Breaking change
   - [ ] Documentation update

   ## Testing
   - [ ] Tests pass locally
   - [ ] Added new tests for changes
   - [ ] Updated existing tests

   ## Checklist
   - [ ] Code follows style guidelines
   - [ ] Self-review completed
   - [ ] Documentation updated
   - [ ] CHANGELOG.md updated
   ```

### Review Process

1. **Automated checks**: CI/CD pipeline runs automatically
2. **Code review**: Maintainers and community members review
3. **Address feedback**: Make requested changes
4. **Approval**: At least one maintainer approval required
5. **Merge**: Maintainers will merge when ready

## Coding Standards

### Python Style

- **PEP 8**: Follow Python style guidelines
- **Line length**: 88 characters (Black default)
- **Imports**: Use isort for import organization
- **Type hints**: Required for all public APIs
- **Docstrings**: Google style docstrings

### Code Quality Tools

- **Black**: Code formatting
- **Ruff**: Linting and code analysis
- **isort**: Import sorting
- **mypy**: Static type checking
- **bandit**: Security analysis

### Example Code Style

```python
from typing import Optional, Dict, Any
from dataapi.types import Database

class DatabaseService:
    """Service for managing database operations.
    
    This service provides methods for creating, reading, updating,
    and deleting databases through the DataAPI.
    
    Args:
        client: The HTTP client instance.
        base_path: The base API path for database operations.
    """
    
    def __init__(self, client: HTTPClient, base_path: str = "/databases") -> None:
        self._client = client
        self._base_path = base_path
    
    async def get_database(
        self, 
        database_id: str, 
        *, 
        include_tables: bool = False
    ) -> Database:
        """Get a database by ID.
        
        Args:
            database_id: The unique identifier of the database.
            include_tables: Whether to include table information.
            
        Returns:
            The database object.
            
        Raises:
            NotFoundError: If the database doesn't exist.
            AuthenticationError: If authentication fails.
        """
        params: Dict[str, Any] = {}
        if include_tables:
            params["include_tables"] = "true"
            
        response = await self._client.get(
            f"{self._base_path}/{database_id}",
            params=params,
            response_model=Database,
        )
        return response
```

## Testing

### Test Structure

```
tests/
├── unit/                 # Unit tests
├── integration/          # Integration tests
├── performance/          # Performance tests
├── fixtures/            # Test fixtures
├── conftest.py          # Pytest configuration
└── mock-server/         # Mock API server
```

### Writing Tests

1. **Use pytest**: All tests use pytest framework
2. **Test markers**: Use appropriate markers (`@pytest.mark.unit`, etc.)
3. **Async tests**: Use `pytest-asyncio` for async test functions
4. **Mocking**: Use `pytest-mock` and `respx` for HTTP mocking
5. **Fixtures**: Create reusable fixtures in `conftest.py`

### Test Examples

```python
import pytest
from dataapi import DataAPIClient
from dataapi.exceptions import NotFoundError

@pytest.mark.unit
def test_client_initialization():
    """Test client initialization with API key."""
    client = DataAPIClient(api_key="test-key")
    assert client.config.base_url == "https://api.dataapi.com"

@pytest.mark.asyncio
async def test_get_database_not_found(mock_client):
    """Test database retrieval with non-existent ID."""
    with pytest.raises(NotFoundError):
        await mock_client.databases.get_database("non-existent")

@pytest.mark.integration
@pytest.mark.network
async def test_real_api_connection():
    """Test connection to real API (requires credentials)."""
    # This test requires real API credentials
    pass
```

### Running Tests

```bash
# All tests
uv run pytest

# Unit tests only
uv run pytest -m unit

# With coverage
uv run pytest --cov=src/dataapi

# Specific test file
uv run pytest tests/test_client.py

# Verbose output
uv run pytest -v
```

## Documentation

### Types of Documentation

1. **API Documentation**: Auto-generated from docstrings
2. **User Guide**: Getting started and tutorials
3. **Examples**: Practical usage examples
4. **Contributing Guide**: This document
5. **Changelog**: Version history and changes

### Writing Documentation

1. **Docstrings**: Use Google style for all public APIs
2. **Markdown**: Use Markdown for guides and documentation
3. **Examples**: Include practical, runnable examples
4. **Links**: Use relative links for internal references

### Building Documentation

```bash
# Build HTML documentation
uv run sphinx-build -b html docs docs/_build/html

# Serve documentation locally
uv run python -m http.server 8000 --directory docs/_build/html

# Check for broken links
uv run sphinx-build -b linkcheck docs docs/_build/linkcheck
```

## Release Process

### Version Numbering

We follow [Semantic Versioning](https://semver.org/):
- **MAJOR**: Incompatible API changes
- **MINOR**: Backwards-compatible functionality additions
- **PATCH**: Backwards-compatible bug fixes

### Release Steps

1. **Update version** in `pyproject.toml` and `src/dataapi/__init__.py`
2. **Update CHANGELOG.md** with release notes
3. **Create release PR** to `main` branch
4. **Tag release** after merge
5. **GitHub Actions** automatically publishes to PyPI

### Pre-release Testing

```bash
# Run full test suite
make test-all

# Build package
uv build

# Test package installation
pip install dist/*.whl
```

## Community

### Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and general discussion
- **Documentation**: Check the official documentation first

### Communication Guidelines

- **Be respectful**: Treat all community members with respect
- **Be constructive**: Provide helpful feedback and suggestions
- **Be patient**: Maintainers and contributors volunteer their time
- **Search first**: Check existing issues and discussions

### Recognition

We recognize contributors in several ways:
- **Contributors list**: Listed in README.md
- **Release notes**: Mentioned in CHANGELOG.md
- **GitHub**: Automatic contribution tracking

## Questions?

If you have questions about contributing, please:

1. Check this document and the README
2. Search existing issues and discussions
3. Create a new discussion or issue
4. Reach out to maintainers

Thank you for contributing to the DataAPI Python SDK! 🎉