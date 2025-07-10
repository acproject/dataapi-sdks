#!/usr/bin/env python3
"""Script to validate example code files.

This script checks that all example files are syntactically correct
and follow basic coding standards.
"""

import ast
import os
import sys
from pathlib import Path
from typing import List, Tuple


def validate_python_syntax(file_path: Path) -> Tuple[bool, str]:
    """Validate Python syntax of a file.
    
    Args:
        file_path: Path to the Python file to validate.
        
    Returns:
        Tuple of (is_valid, error_message).
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Parse the AST to check syntax
        ast.parse(content, filename=str(file_path))
        return True, ""
        
    except SyntaxError as e:
        return False, f"Syntax error: {e}"
    except Exception as e:
        return False, f"Error reading file: {e}"


def check_imports(file_path: Path) -> Tuple[bool, str]:
    """Check if imports are valid.
    
    Args:
        file_path: Path to the Python file to check.
        
    Returns:
        Tuple of (is_valid, error_message).
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        tree = ast.parse(content)
        
        # Check for dataapi imports
        has_dataapi_import = False
        for node in ast.walk(tree):
            if isinstance(node, (ast.Import, ast.ImportFrom)):
                if isinstance(node, ast.Import):
                    for alias in node.names:
                        if alias.name.startswith('dataapi'):
                            has_dataapi_import = True
                elif isinstance(node, ast.ImportFrom):
                    if node.module and node.module.startswith('dataapi'):
                        has_dataapi_import = True
        
        if not has_dataapi_import:
            return False, "No dataapi imports found"
            
        return True, ""
        
    except Exception as e:
        return False, f"Error checking imports: {e}"


def check_async_usage(file_path: Path) -> Tuple[bool, str]:
    """Check if async/await is used correctly.
    
    Args:
        file_path: Path to the Python file to check.
        
    Returns:
        Tuple of (is_valid, error_message).
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        tree = ast.parse(content)
        
        # Check for async functions and await usage
        async_functions = []
        await_calls = []
        
        for node in ast.walk(tree):
            if isinstance(node, ast.AsyncFunctionDef):
                async_functions.append(node.name)
            elif isinstance(node, ast.Await):
                await_calls.append(node)
        
        # If there are await calls, there should be async functions
        if await_calls and not async_functions:
            return False, "Found await calls but no async functions"
            
        return True, ""
        
    except Exception as e:
        return False, f"Error checking async usage: {e}"


def validate_example_file(file_path: Path) -> List[str]:
    """Validate a single example file.
    
    Args:
        file_path: Path to the example file.
        
    Returns:
        List of error messages (empty if valid).
    """
    errors = []
    
    # Check syntax
    is_valid, error = validate_python_syntax(file_path)
    if not is_valid:
        errors.append(f"Syntax error: {error}")
        return errors  # Don't continue if syntax is invalid
    
    # Check imports
    is_valid, error = check_imports(file_path)
    if not is_valid:
        errors.append(f"Import error: {error}")
    
    # Check async usage
    is_valid, error = check_async_usage(file_path)
    if not is_valid:
        errors.append(f"Async error: {error}")
    
    return errors


def find_example_files(examples_dir: Path) -> List[Path]:
    """Find all Python example files.
    
    Args:
        examples_dir: Path to the examples directory.
        
    Returns:
        List of Python file paths.
    """
    python_files = []
    
    if not examples_dir.exists():
        return python_files
    
    for file_path in examples_dir.rglob('*.py'):
        if file_path.is_file():
            python_files.append(file_path)
    
    return python_files


def main() -> int:
    """Main function.
    
    Returns:
        Exit code (0 for success, 1 for failure).
    """
    # Get the project root directory
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    examples_dir = project_root / 'examples'
    
    print(f"Validating examples in: {examples_dir}")
    
    # Find all example files
    example_files = find_example_files(examples_dir)
    
    if not example_files:
        print("No example files found.")
        return 0
    
    print(f"Found {len(example_files)} example files.")
    
    # Validate each file
    total_errors = 0
    for file_path in example_files:
        relative_path = file_path.relative_to(project_root)
        print(f"\nValidating: {relative_path}")
        
        errors = validate_example_file(file_path)
        
        if errors:
            print(f"  ❌ {len(errors)} error(s):")
            for error in errors:
                print(f"    - {error}")
            total_errors += len(errors)
        else:
            print("  ✅ Valid")
    
    # Summary
    print(f"\n{'='*50}")
    print(f"Validation Summary:")
    print(f"  Files checked: {len(example_files)}")
    print(f"  Total errors: {total_errors}")
    
    if total_errors == 0:
        print("  🎉 All examples are valid!")
        return 0
    else:
        print(f"  ❌ Found {total_errors} error(s) in examples.")
        return 1


if __name__ == '__main__':
    sys.exit(main())