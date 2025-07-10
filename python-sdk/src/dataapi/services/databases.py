"""Database service for DataAPI SDK."""

from typing import Any, Dict, List, Optional

from ..client import HTTPClient
from ..types import (
    ColumnDefinition,
    Database,
    PaginatedResponse,
    QueryOptions,
    Record,
    Table,
)


class DatabaseService:
    """Service for managing databases.
    
    Args:
        client: HTTP client instance
    """
    
    def __init__(self, client: HTTPClient) -> None:
        self.client = client
    
    # Database operations
    async def list_databases_async(
        self, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List all databases.
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of databases
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/databases",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Database objects
        if data and hasattr(data, 'data'):
            data.data = [Database(**item) for item in data.data]
        return data
    
    def list_databases(self, options: Optional[QueryOptions] = None) -> PaginatedResponse:
        """List all databases (sync).
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of databases
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/databases",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Database objects
        if data and hasattr(data, 'data'):
            data.data = [Database(**item) for item in data.data]
        return data
    
    async def get_database_async(self, database_id: str) -> Database:
        """Get a database by ID.
        
        Args:
            database_id: Database identifier
            
        Returns:
            Database object
        """
        return await self.client.get_async(
            f"/databases/{database_id}",
            response_model=Database,
        )
    
    def get_database(self, database_id: str) -> Database:
        """Get a database by ID (sync).
        
        Args:
            database_id: Database identifier
            
        Returns:
            Database object
        """
        return self.client.get(
            f"/databases/{database_id}",
            response_model=Database,
        )
    
    async def create_database_async(
        self,
        name: str,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Database:
        """Create a new database.
        
        Args:
            name: Database name
            description: Optional database description
            metadata: Optional metadata
            
        Returns:
            Created database object
        """
        data = {
            "name": name,
            "description": description,
            "metadata": metadata or {},
        }
        return await self.client.post_async(
            "/databases",
            json_data=data,
            response_model=Database,
        )
    
    def create_database(
        self,
        name: str,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Database:
        """Create a new database (sync).
        
        Args:
            name: Database name
            description: Optional database description
            metadata: Optional metadata
            
        Returns:
            Created database object
        """
        data = {
            "name": name,
            "description": description,
            "metadata": metadata or {},
        }
        return self.client.post(
            "/databases",
            json_data=data,
            response_model=Database,
        )
    
    async def update_database_async(
        self,
        database_id: str,
        name: Optional[str] = None,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Database:
        """Update a database.
        
        Args:
            database_id: Database identifier
            name: New database name
            description: New database description
            metadata: New metadata
            
        Returns:
            Updated database object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if description is not None:
            data["description"] = description
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.patch_async(
            f"/databases/{database_id}",
            json_data=data,
            response_model=Database,
        )
    
    def update_database(
        self,
        database_id: str,
        name: Optional[str] = None,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Database:
        """Update a database (sync).
        
        Args:
            database_id: Database identifier
            name: New database name
            description: New database description
            metadata: New metadata
            
        Returns:
            Updated database object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if description is not None:
            data["description"] = description
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.patch(
            f"/databases/{database_id}",
            json_data=data,
            response_model=Database,
        )
    
    async def delete_database_async(self, database_id: str) -> None:
        """Delete a database.
        
        Args:
            database_id: Database identifier
        """
        await self.client.delete_async(f"/databases/{database_id}")
    
    def delete_database(self, database_id: str) -> None:
        """Delete a database (sync).
        
        Args:
            database_id: Database identifier
        """
        self.client.delete(f"/databases/{database_id}")
    
    # Table operations
    async def list_tables_async(
        self, database_id: str, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List tables in a database.
        
        Args:
            database_id: Database identifier
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of tables
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            f"/databases/{database_id}/tables",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Table objects
        if data and hasattr(data, 'data'):
            data.data = [Table(**item) for item in data.data]
        return data
    
    def list_tables(
        self, database_id: str, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List tables in a database (sync).
        
        Args:
            database_id: Database identifier
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of tables
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            f"/databases/{database_id}/tables",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Table objects
        if data and hasattr(data, 'data'):
            data.data = [Table(**item) for item in data.data]
        return data
    
    async def get_table_async(self, database_id: str, table_id: str) -> Table:
        """Get a table by ID.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            
        Returns:
            Table object
        """
        return await self.client.get_async(
            f"/databases/{database_id}/tables/{table_id}",
            response_model=Table,
        )
    
    def get_table(self, database_id: str, table_id: str) -> Table:
        """Get a table by ID (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            
        Returns:
            Table object
        """
        return self.client.get(
            f"/databases/{database_id}/tables/{table_id}",
            response_model=Table,
        )
    
    async def create_table_async(
        self,
        database_id: str,
        name: str,
        schema: List[ColumnDefinition],
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Table:
        """Create a new table.
        
        Args:
            database_id: Database identifier
            name: Table name
            schema: Table schema definition
            description: Optional table description
            metadata: Optional metadata
            
        Returns:
            Created table object
        """
        data = {
            "name": name,
            "schema": [col.dict() for col in schema],
            "description": description,
            "metadata": metadata or {},
        }
        return await self.client.post_async(
            f"/databases/{database_id}/tables",
            json_data=data,
            response_model=Table,
        )
    
    def create_table(
        self,
        database_id: str,
        name: str,
        schema: List[ColumnDefinition],
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Table:
        """Create a new table (sync).
        
        Args:
            database_id: Database identifier
            name: Table name
            schema: Table schema definition
            description: Optional table description
            metadata: Optional metadata
            
        Returns:
            Created table object
        """
        data = {
            "name": name,
            "schema": [col.dict() for col in schema],
            "description": description,
            "metadata": metadata or {},
        }
        return self.client.post(
            f"/databases/{database_id}/tables",
            json_data=data,
            response_model=Table,
        )
    
    async def update_table_async(
        self,
        database_id: str,
        table_id: str,
        name: Optional[str] = None,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Table:
        """Update a table.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            name: New table name
            description: New table description
            metadata: New metadata
            
        Returns:
            Updated table object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if description is not None:
            data["description"] = description
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.patch_async(
            f"/databases/{database_id}/tables/{table_id}",
            json_data=data,
            response_model=Table,
        )
    
    def update_table(
        self,
        database_id: str,
        table_id: str,
        name: Optional[str] = None,
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Table:
        """Update a table (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            name: New table name
            description: New table description
            metadata: New metadata
            
        Returns:
            Updated table object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if description is not None:
            data["description"] = description
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.patch(
            f"/databases/{database_id}/tables/{table_id}",
            json_data=data,
            response_model=Table,
        )
    
    async def delete_table_async(self, database_id: str, table_id: str) -> None:
        """Delete a table.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
        """
        await self.client.delete_async(f"/databases/{database_id}/tables/{table_id}")
    
    def delete_table(self, database_id: str, table_id: str) -> None:
        """Delete a table (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
        """
        self.client.delete(f"/databases/{database_id}/tables/{table_id}")
    
    # Record operations
    async def list_records_async(
        self,
        database_id: str,
        table_id: str,
        options: Optional[QueryOptions] = None,
    ) -> PaginatedResponse:
        """List records in a table.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of records
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            f"/databases/{database_id}/tables/{table_id}/records",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Record objects
        if data and hasattr(data, 'data'):
            data.data = [Record(**item) for item in data.data]
        return data
    
    def list_records(
        self,
        database_id: str,
        table_id: str,
        options: Optional[QueryOptions] = None,
    ) -> PaginatedResponse:
        """List records in a table (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of records
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            f"/databases/{database_id}/tables/{table_id}/records",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Record objects
        if data and hasattr(data, 'data'):
            data.data = [Record(**item) for item in data.data]
        return data
    
    async def get_record_async(
        self, database_id: str, table_id: str, record_id: str
    ) -> Record:
        """Get a record by ID.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
            
        Returns:
            Record object
        """
        return await self.client.get_async(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}",
            response_model=Record,
        )
    
    def get_record(self, database_id: str, table_id: str, record_id: str) -> Record:
        """Get a record by ID (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
            
        Returns:
            Record object
        """
        return self.client.get(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}",
            response_model=Record,
        )
    
    async def create_record_async(
        self,
        database_id: str,
        table_id: str,
        data: Dict[str, Any],
    ) -> Record:
        """Create a new record.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            data: Record data
            
        Returns:
            Created record object
        """
        return await self.client.post_async(
            f"/databases/{database_id}/tables/{table_id}/records",
            json_data={"data": data},
            response_model=Record,
        )
    
    def create_record(
        self,
        database_id: str,
        table_id: str,
        data: Dict[str, Any],
    ) -> Record:
        """Create a new record (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            data: Record data
            
        Returns:
            Created record object
        """
        return self.client.post(
            f"/databases/{database_id}/tables/{table_id}/records",
            json_data={"data": data},
            response_model=Record,
        )
    
    async def update_record_async(
        self,
        database_id: str,
        table_id: str,
        record_id: str,
        data: Dict[str, Any],
        version: Optional[int] = None,
    ) -> Record:
        """Update a record.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
            data: New record data
            version: Record version for optimistic locking
            
        Returns:
            Updated record object
        """
        payload = {"data": data}
        if version is not None:
            payload["version"] = version
        
        return await self.client.patch_async(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}",
            json_data=payload,
            response_model=Record,
        )
    
    def update_record(
        self,
        database_id: str,
        table_id: str,
        record_id: str,
        data: Dict[str, Any],
        version: Optional[int] = None,
    ) -> Record:
        """Update a record (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
            data: New record data
            version: Record version for optimistic locking
            
        Returns:
            Updated record object
        """
        payload = {"data": data}
        if version is not None:
            payload["version"] = version
        
        return self.client.patch(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}",
            json_data=payload,
            response_model=Record,
        )
    
    async def delete_record_async(
        self, database_id: str, table_id: str, record_id: str
    ) -> None:
        """Delete a record.
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
        """
        await self.client.delete_async(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}"
        )
    
    def delete_record(self, database_id: str, table_id: str, record_id: str) -> None:
        """Delete a record (sync).
        
        Args:
            database_id: Database identifier
            table_id: Table identifier
            record_id: Record identifier
        """
        self.client.delete(
            f"/databases/{database_id}/tables/{table_id}/records/{record_id}"
        )