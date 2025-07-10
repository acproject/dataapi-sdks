"""AI service for DataAPI SDK."""

from typing import Any, Dict, List, Optional

from ..client import HTTPClient
from ..types import (
    AIModel,
    AIProvider,
    AIResponse,
    PaginatedResponse,
    QueryOptions,
)


class AIService:
    """Service for AI operations.
    
    Args:
        client: HTTP client instance
    """
    
    def __init__(self, client: HTTPClient) -> None:
        self.client = client
    
    # Model operations
    async def list_models_async(
        self, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List available AI models.
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI models
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/ai/models",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIModel objects
        if data and hasattr(data, 'data'):
            data.data = [AIModel(**item) for item in data.data]
        return data
    
    def list_models(self, options: Optional[QueryOptions] = None) -> PaginatedResponse:
        """List available AI models (sync).
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI models
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/ai/models",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIModel objects
        if data and hasattr(data, 'data'):
            data.data = [AIModel(**item) for item in data.data]
        return data
    
    async def get_model_async(self, model_id: str) -> AIModel:
        """Get an AI model by ID.
        
        Args:
            model_id: Model identifier
            
        Returns:
            AI model object
        """
        return await self.client.get_async(
            f"/ai/models/{model_id}",
            response_model=AIModel,
        )
    
    def get_model(self, model_id: str) -> AIModel:
        """Get an AI model by ID (sync).
        
        Args:
            model_id: Model identifier
            
        Returns:
            AI model object
        """
        return self.client.get(
            f"/ai/models/{model_id}",
            response_model=AIModel,
        )
    
    async def list_models_by_provider_async(
        self, provider: AIProvider, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List AI models by provider.
        
        Args:
            provider: AI provider
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI models
        """
        params = {"provider": provider.value}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/ai/models",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIModel objects
        if data and hasattr(data, 'data'):
            data.data = [AIModel(**item) for item in data.data]
        return data
    
    def list_models_by_provider(
        self, provider: AIProvider, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List AI models by provider (sync).
        
        Args:
            provider: AI provider
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI models
        """
        params = {"provider": provider.value}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/ai/models",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIModel objects
        if data and hasattr(data, 'data'):
            data.data = [AIModel(**item) for item in data.data]
        return data
    
    # Text generation
    async def generate_text_async(
        self,
        model_id: str,
        prompt: str,
        max_tokens: Optional[int] = None,
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        stop_sequences: Optional[List[str]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> AIResponse:
        """Generate text using an AI model.
        
        Args:
            model_id: Model identifier
            prompt: Input prompt
            max_tokens: Maximum tokens to generate
            temperature: Sampling temperature (0.0 to 1.0)
            top_p: Top-p sampling parameter
            stop_sequences: List of stop sequences
            metadata: Additional metadata
            
        Returns:
            AI response object
        """
        data = {
            "model_id": model_id,
            "prompt": prompt,
        }
        
        if max_tokens is not None:
            data["max_tokens"] = max_tokens
        if temperature is not None:
            data["temperature"] = temperature
        if top_p is not None:
            data["top_p"] = top_p
        if stop_sequences is not None:
            data["stop_sequences"] = stop_sequences
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.post_async(
            "/ai/generate/text",
            json_data=data,
            response_model=AIResponse,
        )
    
    def generate_text(
        self,
        model_id: str,
        prompt: str,
        max_tokens: Optional[int] = None,
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        stop_sequences: Optional[List[str]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> AIResponse:
        """Generate text using an AI model (sync).
        
        Args:
            model_id: Model identifier
            prompt: Input prompt
            max_tokens: Maximum tokens to generate
            temperature: Sampling temperature (0.0 to 1.0)
            top_p: Top-p sampling parameter
            stop_sequences: List of stop sequences
            metadata: Additional metadata
            
        Returns:
            AI response object
        """
        data = {
            "model_id": model_id,
            "prompt": prompt,
        }
        
        if max_tokens is not None:
            data["max_tokens"] = max_tokens
        if temperature is not None:
            data["temperature"] = temperature
        if top_p is not None:
            data["top_p"] = top_p
        if stop_sequences is not None:
            data["stop_sequences"] = stop_sequences
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.post(
            "/ai/generate/text",
            json_data=data,
            response_model=AIResponse,
        )
    
    # Chat completion
    async def chat_completion_async(
        self,
        model_id: str,
        messages: List[Dict[str, str]],
        max_tokens: Optional[int] = None,
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        stop_sequences: Optional[List[str]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> AIResponse:
        """Generate chat completion using an AI model.
        
        Args:
            model_id: Model identifier
            messages: List of chat messages with 'role' and 'content' keys
            max_tokens: Maximum tokens to generate
            temperature: Sampling temperature (0.0 to 1.0)
            top_p: Top-p sampling parameter
            stop_sequences: List of stop sequences
            metadata: Additional metadata
            
        Returns:
            AI response object
        """
        data = {
            "model_id": model_id,
            "messages": messages,
        }
        
        if max_tokens is not None:
            data["max_tokens"] = max_tokens
        if temperature is not None:
            data["temperature"] = temperature
        if top_p is not None:
            data["top_p"] = top_p
        if stop_sequences is not None:
            data["stop_sequences"] = stop_sequences
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.post_async(
            "/ai/chat/completions",
            json_data=data,
            response_model=AIResponse,
        )
    
    def chat_completion(
        self,
        model_id: str,
        messages: List[Dict[str, str]],
        max_tokens: Optional[int] = None,
        temperature: Optional[float] = None,
        top_p: Optional[float] = None,
        stop_sequences: Optional[List[str]] = None,
        metadata: Optional[Dict[str, Any]] = None,
    ) -> AIResponse:
        """Generate chat completion using an AI model (sync).
        
        Args:
            model_id: Model identifier
            messages: List of chat messages with 'role' and 'content' keys
            max_tokens: Maximum tokens to generate
            temperature: Sampling temperature (0.0 to 1.0)
            top_p: Top-p sampling parameter
            stop_sequences: List of stop sequences
            metadata: Additional metadata
            
        Returns:
            AI response object
        """
        data = {
            "model_id": model_id,
            "messages": messages,
        }
        
        if max_tokens is not None:
            data["max_tokens"] = max_tokens
        if temperature is not None:
            data["temperature"] = temperature
        if top_p is not None:
            data["top_p"] = top_p
        if stop_sequences is not None:
            data["stop_sequences"] = stop_sequences
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.post(
            "/ai/chat/completions",
            json_data=data,
            response_model=AIResponse,
        )
    
    # Embeddings
    async def create_embeddings_async(
        self,
        model_id: str,
        texts: List[str],
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, Any]:
        """Create embeddings for texts.
        
        Args:
            model_id: Model identifier
            texts: List of texts to embed
            metadata: Additional metadata
            
        Returns:
            Embeddings response
        """
        data = {
            "model_id": model_id,
            "texts": texts,
        }
        
        if metadata is not None:
            data["metadata"] = metadata
        
        return await self.client.post_async(
            "/ai/embeddings",
            json_data=data,
        )
    
    def create_embeddings(
        self,
        model_id: str,
        texts: List[str],
        metadata: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, Any]:
        """Create embeddings for texts (sync).
        
        Args:
            model_id: Model identifier
            texts: List of texts to embed
            metadata: Additional metadata
            
        Returns:
            Embeddings response
        """
        data = {
            "model_id": model_id,
            "texts": texts,
        }
        
        if metadata is not None:
            data["metadata"] = metadata
        
        return self.client.post(
            "/ai/embeddings",
            json_data=data,
        )
    
    # Response history
    async def list_responses_async(
        self, options: Optional[QueryOptions] = None
    ) -> PaginatedResponse:
        """List AI response history.
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI responses
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = await self.client.get_async(
            "/ai/responses",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIResponse objects
        if data and hasattr(data, 'data'):
            data.data = [AIResponse(**item) for item in data.data]
        return data
    
    def list_responses(self, options: Optional[QueryOptions] = None) -> PaginatedResponse:
        """List AI response history (sync).
        
        Args:
            options: Query options for filtering and pagination
            
        Returns:
            Paginated list of AI responses
        """
        params = {}
        if options:
            params.update(options.dict(exclude_none=True))
        
        data = self.client.get(
            "/ai/responses",
            params=params,
            response_model=PaginatedResponse,
        )
        # Convert data items to AIResponse objects
        if data and hasattr(data, 'data'):
            data.data = [AIResponse(**item) for item in data.data]
        return data
    
    async def get_response_async(self, response_id: str) -> AIResponse:
        """Get an AI response by ID.
        
        Args:
            response_id: Response identifier
            
        Returns:
            AI response object
        """
        return await self.client.get_async(
            f"/ai/responses/{response_id}",
            response_model=AIResponse,
        )
    
    def get_response(self, response_id: str) -> AIResponse:
        """Get an AI response by ID (sync).
        
        Args:
            response_id: Response identifier
            
        Returns:
            AI response object
        """
        return self.client.get(
            f"/ai/responses/{response_id}",
            response_model=AIResponse,
        )
    
    async def delete_response_async(self, response_id: str) -> None:
        """Delete an AI response.
        
        Args:
            response_id: Response identifier
        """
        await self.client.delete_async(f"/ai/responses/{response_id}")
    
    def delete_response(self, response_id: str) -> None:
        """Delete an AI response (sync).
        
        Args:
            response_id: Response identifier
        """
        self.client.delete(f"/ai/responses/{response_id}")
    
    # Batch operations
    async def batch_generate_text_async(
        self,
        requests: List[Dict[str, Any]],
        metadata: Optional[Dict[str, Any]] = None,
    ) -> List[AIResponse]:
        """Generate text for multiple prompts in batch.
        
        Args:
            requests: List of generation requests
            metadata: Additional metadata
            
        Returns:
            List of AI response objects
        """
        data = {
            "requests": requests,
        }
        
        if metadata is not None:
            data["metadata"] = metadata
        
        response_data = await self.client.post_async(
            "/ai/batch/generate/text",
            json_data=data,
        )
        
        return [AIResponse(**item) for item in response_data.get("responses", [])]
    
    def batch_generate_text(
        self,
        requests: List[Dict[str, Any]],
        metadata: Optional[Dict[str, Any]] = None,
    ) -> List[AIResponse]:
        """Generate text for multiple prompts in batch (sync).
        
        Args:
            requests: List of generation requests
            metadata: Additional metadata
            
        Returns:
            List of AI response objects
        """
        data = {
            "requests": requests,
        }
        
        if metadata is not None:
            data["metadata"] = metadata
        
        response_data = self.client.post(
            "/ai/batch/generate/text",
            json_data=data,
        )
        
        return [AIResponse(**item) for item in response_data.get("responses", [])]
    
    # Usage and billing
    async def get_usage_async(
        self,
        start_date: Optional[str] = None,
        end_date: Optional[str] = None,
        model_id: Optional[str] = None,
    ) -> Dict[str, Any]:
        """Get AI usage statistics.
        
        Args:
            start_date: Start date (ISO format)
            end_date: End date (ISO format)
            model_id: Filter by model ID
            
        Returns:
            Usage statistics
        """
        params = {}
        if start_date:
            params["start_date"] = start_date
        if end_date:
            params["end_date"] = end_date
        if model_id:
            params["model_id"] = model_id
        
        return await self.client.get_async("/ai/usage", params=params)
    
    def get_usage(
        self,
        start_date: Optional[str] = None,
        end_date: Optional[str] = None,
        model_id: Optional[str] = None,
    ) -> Dict[str, Any]:
        """Get AI usage statistics (sync).
        
        Args:
            start_date: Start date (ISO format)
            end_date: End date (ISO format)
            model_id: Filter by model ID
            
        Returns:
            Usage statistics
        """
        params = {}
        if start_date:
            params["start_date"] = start_date
        if end_date:
            params["end_date"] = end_date
        if model_id:
            params["model_id"] = model_id
        
        return self.client.get("/ai/usage", params=params)