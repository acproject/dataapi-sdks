# Multi-stage Dockerfile for DataAPI Python SDK

# Build stage
FROM python:3.11-slim as builder

# Set environment variables
ENV PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install UV
RUN curl -LsSf https://astral.sh/uv/install.sh | sh
ENV PATH="/root/.cargo/bin:$PATH"

# Set work directory
WORKDIR /app

# Copy dependency files
COPY pyproject.toml uv.lock ./

# Install dependencies
RUN uv sync --frozen --no-dev

# Copy source code
COPY src/ src/
COPY README.md LICENSE ./

# Build the package
RUN uv build

# Runtime stage
FROM python:3.11-slim as runtime

# Set environment variables
ENV PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user
RUN groupadd -r dataapi && useradd -r -g dataapi dataapi

# Set work directory
WORKDIR /app

# Copy built package from builder stage
COPY --from=builder /app/dist/*.whl /tmp/

# Install the package
RUN pip install /tmp/*.whl && rm /tmp/*.whl

# Copy examples
COPY examples/ examples/

# Change ownership to non-root user
RUN chown -R dataapi:dataapi /app

# Switch to non-root user
USER dataapi

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD python -c "import dataapi; print('DataAPI SDK is healthy')" || exit 1

# Default command
CMD ["python", "-c", "import dataapi; print(f'DataAPI Python SDK v{dataapi.__version__} is ready!')"]

# Development stage
FROM builder as development

# Install development dependencies
RUN uv sync --frozen

# Install additional development tools
RUN apt-get update && apt-get install -y \
    vim \
    less \
    tree \
    && rm -rf /var/lib/apt/lists/*

# Copy all source files
COPY . .

# Install the package in development mode
RUN uv pip install -e .

# Expose common development ports
EXPOSE 8000 8080 3000

# Set development environment
ENV DATAAPI_ENV=development

# Default command for development
CMD ["bash"]

# Testing stage
FROM development as testing

# Run tests
RUN uv run pytest tests/ -v

# Run linting
RUN uv run ruff check src tests examples

# Run type checking
RUN uv run mypy src/dataapi

# Run security checks
RUN uv run bandit -r src/

# Documentation stage
FROM development as docs

# Install documentation dependencies
RUN uv sync --group docs

# Build documentation
RUN uv run sphinx-build -b html docs docs/_build/html

# Expose documentation port
EXPOSE 8000

# Serve documentation
CMD ["python", "-m", "http.server", "8000", "--directory", "docs/_build/html"]

# Production stage (minimal)
FROM python:3.11-alpine as production

# Set environment variables
ENV PYTHONUNBUFFERED=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

# Install runtime dependencies
RUN apk add --no-cache \
    curl \
    ca-certificates

# Create non-root user
RUN addgroup -S dataapi && adduser -S dataapi -G dataapi

# Set work directory
WORKDIR /app

# Copy built package from builder stage
COPY --from=builder /app/dist/*.whl /tmp/

# Install the package
RUN pip install /tmp/*.whl && rm /tmp/*.whl

# Change ownership to non-root user
RUN chown -R dataapi:dataapi /app

# Switch to non-root user
USER dataapi

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD python -c "import dataapi; print('DataAPI SDK is healthy')" || exit 1

# Default command
CMD ["python", "-c", "import dataapi; print(f'DataAPI Python SDK v{dataapi.__version__} is ready!')"]