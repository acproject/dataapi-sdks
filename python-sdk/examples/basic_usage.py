#!/usr/bin/env python3
"""Basic usage examples for DataAPI Python SDK."""

import asyncio
from typing import List

from dataapi import (
    DataAPIClient,
    ColumnDefinition,
    ColumnType,
    QueryOptions,
    SortField,
    SortOrder,
    Filter,
    FilterOperator,
    WorkflowStep,
)


async def database_examples():
    """Examples of database operations."""
    print("\n=== Database Examples ===")
    
    # Initialize client
    async with DataAPIClient(api_key="your-api-key") as client:
        # Create a database
        print("Creating database...")
        database = await client.databases.create_database_async(
            name="example_db",
            description="Example database for testing",
            metadata={"project": "demo", "environment": "development"}
        )
        print(f"Created database: {database.name} (ID: {database.id})")
        
        # Create a table schema
        schema = [
            ColumnDefinition(
                name="id",
                type=ColumnType.INTEGER,
                primary_key=True,
                nullable=False
            ),
            ColumnDefinition(
                name="name",
                type=ColumnType.STRING,
                nullable=False
            ),
            ColumnDefinition(
                name="email",
                type=ColumnType.STRING,
                unique=True,
                nullable=False
            ),
            ColumnDefinition(
                name="age",
                type=ColumnType.INTEGER,
                nullable=True
            ),
            ColumnDefinition(
                name="created_at",
                type=ColumnType.DATETIME,
                nullable=False
            ),
        ]
        
        # Create a table
        print("Creating table...")
        table = await client.databases.create_table_async(
            database_id=database.id,
            name="users",
            schema=schema,
            description="User information table"
        )
        print(f"Created table: {table.name} (ID: {table.id})")
        
        # Insert records
        print("Inserting records...")
        users_data = [
            {
                "id": 1,
                "name": "Alice Johnson",
                "email": "alice@example.com",
                "age": 30,
                "created_at": "2024-01-15T10:00:00Z"
            },
            {
                "id": 2,
                "name": "Bob Smith",
                "email": "bob@example.com",
                "age": 25,
                "created_at": "2024-01-16T11:00:00Z"
            },
            {
                "id": 3,
                "name": "Carol Davis",
                "email": "carol@example.com",
                "age": 35,
                "created_at": "2024-01-17T12:00:00Z"
            }
        ]
        
        for user_data in users_data:
            record = await client.databases.create_record_async(
                database_id=database.id,
                table_id=table.id,
                data=user_data
            )
            print(f"Inserted record: {record.data['name']}")
        
        # Query records with filtering and sorting
        print("\nQuerying records...")
        query_options = QueryOptions(
            page=1,
            per_page=10,
            sort=[SortField(field="age", order=SortOrder.DESC)],
            filters=[
                Filter(
                    field="age",
                    operator=FilterOperator.GTE,
                    value=25
                )
            ]
        )
        
        records_response = await client.databases.list_records_async(
            database_id=database.id,
            table_id=table.id,
            options=query_options
        )
        
        print(f"Found {len(records_response.data)} records:")
        for record in records_response.data:
            user = record.data
            print(f"  - {user['name']} (age: {user['age']})")
        
        # Update a record
        print("\nUpdating record...")
        if records_response.data:
            first_record = records_response.data[0]
            updated_record = await client.databases.update_record_async(
                database_id=database.id,
                table_id=table.id,
                record_id=first_record.id,
                data={**first_record.data, "age": 31}
            )
            print(f"Updated {updated_record.data['name']}'s age to {updated_record.data['age']}")
        
        # List databases
        print("\nListing all databases...")
        databases_response = await client.databases.list_databases_async()
        for db in databases_response.data:
            print(f"  - {db.name} (created: {db.created_at})")


async def ai_examples():
    """Examples of AI operations."""
    print("\n=== AI Examples ===")
    
    async with DataAPIClient(api_key="your-api-key") as client:
        # List available AI models
        print("Listing AI models...")
        models_response = await client.ai.list_models_async()
        print(f"Available models: {len(models_response.data)}")
        for model in models_response.data[:3]:  # Show first 3
            print(f"  - {model.name} ({model.provider})")
        
        if models_response.data:
            model = models_response.data[0]
            
            # Generate text
            print(f"\nGenerating text with {model.name}...")
            response = await client.ai.generate_text_async(
                model_id=model.id,
                prompt="Write a short poem about artificial intelligence.",
                max_tokens=100,
                temperature=0.7
            )
            print(f"Generated text:\n{response.response}")
            print(f"Tokens used: {response.tokens_used}")
            
            # Chat completion
            print(f"\nChat completion with {model.name}...")
            messages = [
                {"role": "system", "content": "You are a helpful assistant."},
                {"role": "user", "content": "What are the benefits of using APIs?"}
            ]
            
            chat_response = await client.ai.chat_completion_async(
                model_id=model.id,
                messages=messages,
                max_tokens=150,
                temperature=0.5
            )
            print(f"Chat response:\n{chat_response.response}")
            
            # Create embeddings
            print(f"\nCreating embeddings...")
            texts = [
                "DataAPI is a powerful platform for data management.",
                "Machine learning models can process large datasets.",
                "APIs enable seamless integration between systems."
            ]
            
            embeddings_response = await client.ai.create_embeddings_async(
                model_id=model.id,
                texts=texts
            )
            print(f"Created embeddings for {len(texts)} texts")
            print(f"Embedding dimensions: {len(embeddings_response.get('embeddings', [{}])[0].get('values', []))}")
        
        # Get AI usage statistics
        print("\nGetting AI usage statistics...")
        usage = await client.ai.get_usage_async()
        print(f"Total requests: {usage.get('total_requests', 0)}")
        print(f"Total tokens: {usage.get('total_tokens', 0)}")


async def workflow_examples():
    """Examples of workflow operations."""
    print("\n=== Workflow Examples ===")
    
    async with DataAPIClient(api_key="your-api-key") as client:
        # Define workflow steps
        steps = [
            WorkflowStep(
                id="step1",
                type="data_extraction",
                name="Extract Data",
                description="Extract data from source",
                config={
                    "source_type": "database",
                    "query": "SELECT * FROM users WHERE active = true"
                }
            ),
            WorkflowStep(
                id="step2",
                type="data_transformation",
                name="Transform Data",
                description="Clean and transform the data",
                config={
                    "transformations": [
                        {"type": "normalize_email", "field": "email"},
                        {"type": "format_phone", "field": "phone"}
                    ]
                },
                depends_on=["step1"]
            ),
            WorkflowStep(
                id="step3",
                type="ai_processing",
                name="AI Analysis",
                description="Analyze data with AI",
                config={
                    "model_id": "gpt-4",
                    "analysis_type": "sentiment",
                    "field": "feedback"
                },
                depends_on=["step2"]
            ),
            WorkflowStep(
                id="step4",
                type="data_export",
                name="Export Results",
                description="Export processed data",
                config={
                    "destination_type": "s3",
                    "bucket": "processed-data",
                    "format": "json"
                },
                depends_on=["step3"]
            )
        ]
        
        # Create workflow
        print("Creating workflow...")
        workflow = await client.workflows.create_workflow_async(
            name="Data Processing Pipeline",
            steps=steps,
            description="Complete data processing pipeline with AI analysis",
            metadata={"version": "1.0", "environment": "production"}
        )
        print(f"Created workflow: {workflow.name} (ID: {workflow.id})")
        
        # Execute workflow
        print("\nExecuting workflow...")
        execution = await client.workflows.execute_workflow_async(
            workflow_id=workflow.id,
            input_data={
                "source_database": "user_feedback_db",
                "date_range": "2024-01-01 to 2024-01-31"
            },
            metadata={"triggered_by": "scheduled_job"}
        )
        print(f"Started execution: {execution.id} (status: {execution.status})")
        
        # Monitor execution
        print("\nMonitoring execution...")
        import time
        for i in range(5):  # Check status 5 times
            current_execution = await client.workflows.get_execution_async(execution.id)
            print(f"  Status: {current_execution.status}")
            
            if current_execution.status in ["completed", "failed", "cancelled"]:
                break
            
            await asyncio.sleep(2)  # Wait 2 seconds
        
        # Get execution logs
        print("\nGetting execution logs...")
        logs = await client.workflows.get_execution_logs_async(execution.id)
        print(f"Log entries: {len(logs.get('logs', []))}")
        
        # List all workflows
        print("\nListing workflows...")
        workflows_response = await client.workflows.list_workflows_async()
        for wf in workflows_response.data:
            print(f"  - {wf.name} (steps: {len(wf.steps)})")
        
        # List workflow templates
        print("\nListing workflow templates...")
        templates_response = await client.workflows.list_templates_async()
        print(f"Available templates: {len(templates_response.data)}")
        for template in templates_response.data[:3]:  # Show first 3
            print(f"  - {template.get('name', 'Unnamed')}")


async def error_handling_examples():
    """Examples of error handling."""
    print("\n=== Error Handling Examples ===")
    
    from dataapi import (
        AuthenticationError,
        NotFoundError,
        RateLimitError,
        ValidationError,
        NetworkError
    )
    
    async with DataAPIClient(api_key="invalid-key") as client:
        try:
            # This will raise an AuthenticationError
            await client.databases.list_databases_async()
        except AuthenticationError as e:
            print(f"Authentication failed: {e}")
        
        try:
            # This will raise a NotFoundError
            await client.databases.get_database_async("non-existent-id")
        except NotFoundError as e:
            print(f"Resource not found: {e}")
        
        try:
            # This will raise a ValidationError
            await client.databases.create_database_async(
                name="",  # Empty name should cause validation error
                description="Test database"
            )
        except ValidationError as e:
            print(f"Validation failed: {e}")


def sync_examples():
    """Examples using synchronous methods."""
    print("\n=== Synchronous Examples ===")
    
    # Using sync methods
    with DataAPIClient(api_key="your-api-key") as client:
        # Check API health
        health = client.health_check()
        print(f"API Status: {health.get('status', 'unknown')}")
        
        # Get user info
        user_info = client.get_user_info()
        print(f"User: {user_info.get('name', 'Unknown')}")
        
        # Get API info
        api_info = client.get_api_info()
        print(f"API Version: {api_info.get('version', 'unknown')}")
        
        # List databases (sync)
        databases = client.databases.list_databases()
        print(f"Total databases: {len(databases.data)}")


async def main():
    """Run all examples."""
    print("DataAPI Python SDK Examples")
    print("============================")
    
    # Note: Replace "your-api-key" with your actual API key
    print("\n⚠️  Remember to replace 'your-api-key' with your actual API key!")
    
    try:
        await database_examples()
        await ai_examples()
        await workflow_examples()
        await error_handling_examples()
        sync_examples()
        
        print("\n✅ All examples completed successfully!")
        
    except Exception as e:
        print(f"\n❌ Error running examples: {e}")
        print("Make sure you have a valid API key and internet connection.")


if __name__ == "__main__":
    # Run the examples
    asyncio.run(main())