"""Workflow service for DataAPI SDK."""

from typing import Any, Dict, List, Optional

from ..client import HTTPClient
from ..types import (
    PaginatedResponse,
    QueryOptions,
    Workflow,
    WorkflowExecution,
    WorkflowStatus,
    WorkflowStep,
)


class WorkflowService:
    """Service for managing workflows.
    
    Args:
        client: HTTP client instance
    """
    
    def __init__(self, client: HTTPClient) -> None:
        self.client = client
    
    # Workflow operations
    async def list_workflows_async(
        self, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List all workflows.
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflows
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/workflows",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Workflow objects
        if data and hasattr(data, 'data'):
            data.data = [Workflow(**item) for item in data.data]
        return data
    
    def list_workflows(self, options: Optional[QueryOptions] = None) -> PaginatedResponse:
        """List all workflows (sync).
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflows
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/workflows",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to Workflow objects
        if data and hasattr(data, 'data'):
            data.data = [Workflow(**item) for item in data.data]
        return data
    
    async def get_workflow_async(self, workflow_id: str) -> Workflow:
        """Get a workflow by ID.
        
        Args:
            workflow_id: Workflow identifier
            
        Returns:
            Workflow object
        """
        return await self.client.get_async(
            f"/workflows/{workflow_id}",
            response_model=Workflow,
        )
    
    def get_workflow(self, workflow_id: str) -> Workflow:
        """Get a workflow by ID (sync).
        
        Args:
            workflow_id: Workflow identifier
            
        Returns:
            Workflow object
        """
        return self.client.get(
            f"/workflows/{workflow_id}",
            response_model=Workflow,
        )
    
    async def create_workflow_async(
        self,
        name: str,
        steps: List[WorkflowStep],
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Create a new workflow.
        
        Args:
            name: Workflow name
            steps: List of workflow steps
            description: Optional workflow description
            metadata: Optional metadata
            
        Returns:
            Created workflow object
        """
        data = {
            "name": name,
            "steps": [step.dict() for step in steps],
            "description": description,
            "metadata": metadata or {},
        }
        return await self.client.post_async(
            "/workflows",
            json_data=data,
            response_model=Workflow,
        )
    
    def create_workflow(
        self,
        name: str,
        steps: List[WorkflowStep],
        description: Optional[str] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Create a new workflow (sync).
        
        Args:
            name: Workflow name
            steps: List of workflow steps
            description: Optional workflow description
            metadata: Optional metadata
            
        Returns:
            Created workflow object
        """
        data = {
            "name": name,
            "steps": [step.dict() for step in steps],
            "description": description,
            "metadata": metadata or {},
        }
        return self.client.post(
            "/workflows",
            json_data=data,
            response_model=Workflow,
        )
    
    async def update_workflow_async(
        self,
        workflow_id: str,
        name: Optional[str] = None,
        steps: Optional[List[WorkflowStep]] = None,
        description: Optional[str] = None,
        is_active: Optional[bool] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Update a workflow.
        
        Args:
            workflow_id: Workflow identifier
            name: New workflow name
            steps: New workflow steps
            description: New workflow description
            is_active: Whether workflow is active
            metadata: New metadata
            
        Returns:
            Updated workflow object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if steps is not None:
            data["steps"] = [step.dict() for step in steps]
        if description is not None:
            data["description"] = description
        if is_active is not None:
            data["is_active"] = is_active
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.patch_async(
            f"/workflows/{workflow_id}",
            json_data=data,
            response_model=Workflow,
        )
    
    def update_workflow(
        self,
        workflow_id: str,
        name: Optional[str] = None,
        steps: Optional[List[WorkflowStep]] = None,
        description: Optional[str] = None,
        is_active: Optional[bool] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Update a workflow (sync).
        
        Args:
            workflow_id: Workflow identifier
            name: New workflow name
            steps: New workflow steps
            description: New workflow description
            is_active: Whether workflow is active
            metadata: New metadata
            
        Returns:
            Updated workflow object
        """
        data = {}
        if name is not None:
            data["name"] = name
        if steps is not None:
            data["steps"] = [step.dict() for step in steps]
        if description is not None:
            data["description"] = description
        if is_active is not None:
            data["is_active"] = is_active
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.patch(
            f"/workflows/{workflow_id}",
            json_data=data,
            response_model=Workflow,
        )
    
    async def delete_workflow_async(self, workflow_id: str) -> None:
        """Delete a workflow.
        
        Args:
            workflow_id: Workflow identifier
        """
        await self.client.delete_async(f"/workflows/{workflow_id}")
    
    def delete_workflow(self, workflow_id: str) -> None:
        """Delete a workflow (sync).
        
        Args:
            workflow_id: Workflow identifier
        """
        self.client.delete(f"/workflows/{workflow_id}")
    
    # Workflow execution operations
    async def execute_workflow_async(
        self,
        workflow_id: str,
        input_data: Optional[Dict[str, Any]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> WorkflowExecution:
        """Execute a workflow.
        
        Args:
            workflow_id: Workflow identifier
            input_data: Input data for workflow execution
            metadata: Optional metadata
            
        Returns:
            Workflow execution object
        """
        data = {
            "input_data": input_data or {},
            "metadata": metadata or {},
        }
        return await self.client.post_async(
            f"/workflows/{workflow_id}/execute",
            json_data=data,
            response_model=WorkflowExecution,
        )
    
    def execute_workflow(
        self,
        workflow_id: str,
        input_data: Optional[Dict[str, Any]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> WorkflowExecution:
        """Execute a workflow (sync).
        
        Args:
            workflow_id: Workflow identifier
            input_data: Input data for workflow execution
            metadata: Optional metadata
            
        Returns:
            Workflow execution object
        """
        data = {
            "input_data": input_data or {},
            "metadata": metadata or {},
        }
        return self.client.post(
            f"/workflows/{workflow_id}/execute",
            json_data=data,
            response_model=WorkflowExecution,
        )
    
    async def list_executions_async(
        self,
        workflow_id: Optional[str] = None,
        status: Optional[WorkflowStatus] = None,
        options: Optional[QueryOptions] = None,
    ) -> PaginatedResponse:
        """List workflow executions.
        
        Args:
            workflow_id: Filter by workflow ID
            status: Filter by execution status
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflow executions
        """
        params = {}
        if workflow_id:
            params["workflow_id"] = workflow_id
        if status:
            params["status"] = status.value
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/workflow-executions",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to WorkflowExecution objects
        if data and hasattr(data, 'data'):
            data.data = [WorkflowExecution(**item) for item in data.data]
        return data
    
    def list_executions(
        self,
        workflow_id: Optional[str] = None,
        status: Optional[WorkflowStatus] = None,
        options: Optional[QueryOptions] = None,
    ) -> PaginatedResponse:
        """List workflow executions (sync).
        
        Args:
            workflow_id: Filter by workflow ID
            status: Filter by execution status
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflow executions
        """
        params = {}
        if workflow_id:
            params["workflow_id"] = workflow_id
        if status:
            params["status"] = status.value
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/workflow-executions",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to WorkflowExecution objects
        if data and hasattr(data, 'data'):
            data.data = [WorkflowExecution(**item) for item in data.data]
        return data
    
    async def get_execution_async(self, execution_id: str) -> WorkflowExecution:
        """Get a workflow execution by ID.
        
        Args:
            execution_id: Execution identifier
            
        Returns:
            Workflow execution object
        """
        return await self.client.get_async(
            f"/workflow-executions/{execution_id}",
            response_model=WorkflowExecution,
        )
    
    def get_execution(self, execution_id: str) -> WorkflowExecution:
        """Get a workflow execution by ID (sync).
        
        Args:
            execution_id: Execution identifier
            
        Returns:
            Workflow execution object
        """
        return self.client.get(
            f"/workflow-executions/{execution_id}",
            response_model=WorkflowExecution,
        )
    
    async def cancel_execution_async(self, execution_id: str) -> WorkflowExecution:
        """Cancel a workflow execution.
        
        Args:
            execution_id: Execution identifier
            
        Returns:
            Updated workflow execution object
        """
        return await self.client.post_async(
            f"/workflow-executions/{execution_id}/cancel",
            response_model=WorkflowExecution,
        )
    
    def cancel_execution(self, execution_id: str) -> WorkflowExecution:
        """Cancel a workflow execution (sync).
        
        Args:
            execution_id: Execution identifier
            
        Returns:
            Updated workflow execution object
        """
        return self.client.post(
            f"/workflow-executions/{execution_id}/cancel",
            response_model=WorkflowExecution,
        )
    
    async def retry_execution_async(
        self,
        execution_id: str,
        from_step: Optional[str] = None,
    ) -> WorkflowExecution:
        """Retry a failed workflow execution.
        
        Args:
            execution_id: Execution identifier
            from_step: Step ID to retry from (optional)
            
        Returns:
            New workflow execution object
        """
        data = {}
        if from_step:
            data["from_step"] = from_step
        
        return await self.client.post_async(
            f"/workflow-executions/{execution_id}/retry",
            json_data=data,
            response_model=WorkflowExecution,
        )
    
    def retry_execution(
        self,
        execution_id: str,
        from_step: Optional[str] = None,
    ) -> WorkflowExecution:
        """Retry a failed workflow execution (sync).
        
        Args:
            execution_id: Execution identifier
            from_step: Step ID to retry from (optional)
            
        Returns:
            New workflow execution object
        """
        data = {}
        if from_step:
            data["from_step"] = from_step
        
        return self.client.post(
            f"/workflow-executions/{execution_id}/retry",
            json_data=data,
            response_model=WorkflowExecution,
        )
    
    async def get_execution_logs_async(
        self,
        execution_id: str,
        step_id: Optional[str] = None,
        options: Optional[QueryOptions] = None,
    ) -> Dict[str, Any]:
        """Get execution logs.
        
        Args:
            execution_id: Execution identifier
            step_id: Filter by step ID (optional)
            options: Query options for filtering and pagination
            
        Returns:
            Execution logs
        """
        params = {}
        if step_id:
            params["step_id"] = step_id
        if options:
            params.update(options.dict(exclude_none=True))
        
        return await self.client.get_async(
            f"/workflow-executions/{execution_id}/logs",
            params=params,
        )
    
    def get_execution_logs(
        self,
        execution_id: str,
        step_id: Optional[str] = None,
        options: Optional[QueryOptions] = None,
    ) -> Dict[str, Any]:
        """Get execution logs (sync).
        
        Args:
            execution_id: Execution identifier
            step_id: Filter by step ID (optional)
            options: Query options for filtering and pagination
            
        Returns:
            Execution logs
        """
        params = {}
        if step_id:
            params["step_id"] = step_id
        if options:
            params.update(options.dict(exclude_none=True))
        
        return self.client.get(
            f"/workflow-executions/{execution_id}/logs",
            params=params,
        )
    
    # Workflow templates
    async def list_templates_async(
        self, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List workflow templates.
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflow templates
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        return await self.client.get_async(
            "/workflow-templates",
            params=params,
            response_model=PaginatedResponse,
        )
    
    def list_templates(self, options: Optional[QueryOptions] = None) -> PaginatedResponse:
        """List workflow templates (sync).
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of workflow templates
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        return self.client.get(
            "/workflow-templates",
            params=params,
            response_model=PaginatedResponse,
        )
    
    async def create_from_template_async(
        self,
        template_id: str,
        name: str,
        parameters: Optional[Dict[str, Any]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Create a workflow from a template.
        
        Args:
            template_id: Template identifier
            name: Workflow name
            parameters: Template parameters
            metadata: Optional metadata
            
        Returns:
            Created workflow object
        """
        data = {
            "name": name,
            "parameters": parameters or {},
            "metadata": metadata or {},
        }
        return await self.client.post_async(
            f"/workflow-templates/{template_id}/create",
            json_data=data,
            response_model=Workflow,
        )
    
    def create_from_template(
        self,
        template_id: str,
        name: str,
        parameters: Optional[Dict[str, Any]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Workflow:
        """Create a workflow from a template (sync).
        
        Args:
            template_id: Template identifier
            name: Workflow name
            parameters: Template parameters
            metadata: Optional metadata
            
        Returns:
            Created workflow object
        """
        data = {
            "name": name,
            "parameters": parameters or {},
            "metadata": metadata or {},
        }
        return self.client.post(
            f"/workflow-templates/{template_id}/create",
            json_data=data,
            response_model=Workflow,
        )
    
    # Workflow validation
    async def validate_workflow_async(
        self, workflow_definition: Dict[str, Any]
    ) -> Dict[str, Any]:
        """Validate a workflow definition.
        
        Args:
            workflow_definition: Workflow definition to validate
            
        Returns:
            Validation result
        """
        return await self.client.post_async(
            "/workflows/validate",
            json_data=workflow_definition,
        )
    
    def validate_workflow(self, workflow_definition: Dict[str, Any]) -> Dict[str, Any]:
        """Validate a workflow definition (sync).
        
        Args:
            workflow_definition: Workflow definition to validate
            
        Returns:
            Validation result
        """
        return self.client.post(
            "/workflows/validate",
            json_data=workflow_definition,
        )