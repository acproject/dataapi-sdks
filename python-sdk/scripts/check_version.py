#!/usr/bin/env python3
"""Script to check version consistency across project files.

This script ensures that version numbers are consistent between
pyproject.toml and src/dataapi/__init__.py files.
"""

import re
import sys
from pathlib import Path
from typing import Optional

try:
    import tomllib
except ImportError:
    # Python < 3.11
    try:
        import tomli as tomllib
    except ImportError:
        print("Error: tomllib/tomli not available. Install with: pip install tomli")
        sys.exit(1)


def get_version_from_pyproject(pyproject_path: Path) -> Optional[str]:
    """Get version from pyproject.toml.
    
    Args:
        pyproject_path: Path to pyproject.toml file.
        
    Returns:
        Version string or None if not found.
    """
    try:
        with open(pyproject_path, 'rb') as f:
            data = tomllib.load(f)
        
        return data.get('project', {}).get('version')
        
    except Exception as e:
        print(f"Error reading pyproject.toml: {e}")
        return None


def get_version_from_init(init_path: Path) -> Optional[str]:
    """Get version from __init__.py.
    
    Args:
        init_path: Path to __init__.py file.
        
    Returns:
        Version string or None if not found.
    """
    try:
        with open(init_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Look for __version__ = "x.y.z" pattern
        version_pattern = r'__version__\s*=\s*["\']([^"\'
]+)["\']'
        match = re.search(version_pattern, content)
        
        if match:
            return match.group(1)
        
        return None
        
    except Exception as e:
        print(f"Error reading __init__.py: {e}")
        return None


def is_valid_version(version: str) -> bool:
    """Check if version follows semantic versioning.
    
    Args:
        version: Version string to validate.
        
    Returns:
        True if valid, False otherwise.
    """
    # Basic semantic versioning pattern
    pattern = r'^\d+\.\d+\.\d+(?:-[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*)?(?:\+[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*)?$'
    return bool(re.match(pattern, version))


def update_init_version(init_path: Path, new_version: str) -> bool:
    """Update version in __init__.py file.
    
    Args:
        init_path: Path to __init__.py file.
        new_version: New version string.
        
    Returns:
        True if successful, False otherwise.
    """
    try:
        with open(init_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Replace version
        version_pattern = r'(__version__\s*=\s*["\'])[^"\'
]+(["\'])'
        new_content = re.sub(version_pattern, f'\g<1>{new_version}\g<2>', content)
        
        if new_content != content:
            with open(init_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            return True
        
        return False
        
    except Exception as e:
        print(f"Error updating __init__.py: {e}")
        return False


def main() -> int:
    """Main function.
    
    Returns:
        Exit code (0 for success, 1 for failure).
    """
    # Get file paths
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    pyproject_path = project_root / 'pyproject.toml'
    init_path = project_root / 'src' / 'dataapi' / '__init__.py'
    
    print("Checking version consistency...")
    
    # Check if files exist
    if not pyproject_path.exists():
        print(f"Error: {pyproject_path} not found")
        return 1
    
    if not init_path.exists():
        print(f"Error: {init_path} not found")
        return 1
    
    # Get versions
    pyproject_version = get_version_from_pyproject(pyproject_path)
    init_version = get_version_from_init(init_path)
    
    print(f"pyproject.toml version: {pyproject_version}")
    print(f"__init__.py version: {init_version}")
    
    # Check if versions were found
    if pyproject_version is None:
        print("Error: Could not find version in pyproject.toml")
        return 1
    
    if init_version is None:
        print("Error: Could not find version in __init__.py")
        return 1
    
    # Validate version format
    if not is_valid_version(pyproject_version):
        print(f"Error: Invalid version format in pyproject.toml: {pyproject_version}")
        return 1
    
    if not is_valid_version(init_version):
        print(f"Error: Invalid version format in __init__.py: {init_version}")
        return 1
    
    # Check consistency
    if pyproject_version == init_version:
        print("✅ Version numbers are consistent!")
        return 0
    else:
        print(f"❌ Version mismatch detected!")
        print(f"  pyproject.toml: {pyproject_version}")
        print(f"  __init__.py: {init_version}")
        
        # Check if we should auto-fix
        if len(sys.argv) > 1 and sys.argv[1] == '--fix':
            print(f"\nAttempting to fix by updating __init__.py to {pyproject_version}...")
            
            if update_init_version(init_path, pyproject_version):
                print("✅ Successfully updated __init__.py")
                return 0
            else:
                print("❌ Failed to update __init__.py")
                return 1
        else:
            print("\nTo automatically fix this, run: python scripts/check_version.py --fix")
            return 1


if __name__ == '__main__':
    sys.exit(main())