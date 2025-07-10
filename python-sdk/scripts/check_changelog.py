#!/usr/bin/env python3
"""Script to validate CHANGELOG.md format and content.

This script checks that the changelog follows the Keep a Changelog format
and contains proper version entries.
"""

import re
import sys
from datetime import datetime
from pathlib import Path
from typing import List, Tuple, Optional


class ChangelogEntry:
    """Represents a changelog entry."""
    
    def __init__(self, version: str, date: Optional[str] = None, unreleased: bool = False):
        self.version = version
        self.date = date
        self.unreleased = unreleased
        self.sections = {
            'added': [],
            'changed': [],
            'deprecated': [],
            'removed': [],
            'fixed': [],
            'security': []
        }
    
    def __str__(self) -> str:
        if self.unreleased:
            return f"[Unreleased]"
        elif self.date:
            return f"[{self.version}] - {self.date}"
        else:
            return f"[{self.version}]"


def parse_changelog(changelog_path: Path) -> Tuple[List[ChangelogEntry], List[str]]:
    """Parse changelog file and extract entries.
    
    Args:
        changelog_path: Path to CHANGELOG.md file.
        
    Returns:
        Tuple of (entries, errors).
    """
    try:
        with open(changelog_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        return [], [f"Error reading changelog: {e}"]
    
    entries = []
    errors = []
    lines = content.split('\n')
    
    current_entry = None
    current_section = None
    
    # Patterns
    version_pattern = r'^## \[(.*?)\](?:\s*-\s*(.*))?$'
    section_pattern = r'^### (Added|Changed|Deprecated|Removed|Fixed|Security)$'
    
    for i, line in enumerate(lines, 1):
        line = line.strip()
        
        # Check for version headers
        version_match = re.match(version_pattern, line)
        if version_match:
            version = version_match.group(1)
            date = version_match.group(2)
            
            # Save previous entry
            if current_entry:
                entries.append(current_entry)
            
            # Create new entry
            unreleased = version.lower() == 'unreleased'
            current_entry = ChangelogEntry(version, date, unreleased)
            current_section = None
            continue
        
        # Check for section headers
        section_match = re.match(section_pattern, line)
        if section_match:
            if not current_entry:
                errors.append(f"Line {i}: Section header without version header")
                continue
            
            current_section = section_match.group(1).lower()
            continue
        
        # Check for list items
        if line.startswith('- ') and current_entry and current_section:
            item = line[2:].strip()
            if item:
                current_entry.sections[current_section].append(item)
    
    # Add last entry
    if current_entry:
        entries.append(current_entry)
    
    return entries, errors


def validate_version_format(version: str) -> bool:
    """Validate version format.
    
    Args:
        version: Version string to validate.
        
    Returns:
        True if valid, False otherwise.
    """
    if version.lower() == 'unreleased':
        return True
    
    # Semantic versioning pattern
    pattern = r'^\d+\.\d+\.\d+(?:-[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*)?(?:\+[a-zA-Z0-9]+(?:\.[a-zA-Z0-9]+)*)?$'
    return bool(re.match(pattern, version))


def validate_date_format(date: str) -> bool:
    """Validate date format (YYYY-MM-DD).
    
    Args:
        date: Date string to validate.
        
    Returns:
        True if valid, False otherwise.
    """
    try:
        datetime.strptime(date, '%Y-%m-%d')
        return True
    except ValueError:
        return False


def validate_changelog(entries: List[ChangelogEntry]) -> List[str]:
    """Validate changelog entries.
    
    Args:
        entries: List of changelog entries.
        
    Returns:
        List of validation errors.
    """
    errors = []
    
    if not entries:
        errors.append("No changelog entries found")
        return errors
    
    # Check if first entry is unreleased
    if not entries[0].unreleased:
        errors.append("First entry should be [Unreleased]")
    
    # Validate each entry
    for i, entry in enumerate(entries):
        # Validate version format
        if not validate_version_format(entry.version):
            errors.append(f"Invalid version format: {entry.version}")
        
        # Validate date format (skip unreleased)
        if not entry.unreleased and entry.date:
            if not validate_date_format(entry.date):
                errors.append(f"Invalid date format for {entry.version}: {entry.date}")
        
        # Check for empty sections
        has_content = any(entry.sections.values())
        if not has_content and not entry.unreleased:
            errors.append(f"Version {entry.version} has no content")
        
        # Check version ordering (newer versions should come first)
        if i > 0 and not entries[i-1].unreleased and not entry.unreleased:
            prev_version = entries[i-1].version
            curr_version = entry.version
            
            # Simple version comparison (assumes semantic versioning)
            try:
                prev_parts = [int(x) for x in prev_version.split('.')[0:3]]
                curr_parts = [int(x) for x in curr_version.split('.')[0:3]]
                
                if prev_parts < curr_parts:
                    errors.append(f"Version ordering issue: {prev_version} should come after {curr_version}")
            except ValueError:
                # Skip complex version comparisons
                pass
    
    return errors


def check_required_sections(changelog_path: Path) -> List[str]:
    """Check for required sections in changelog.
    
    Args:
        changelog_path: Path to CHANGELOG.md file.
        
    Returns:
        List of missing sections.
    """
    try:
        with open(changelog_path, 'r', encoding='utf-8') as f:
            content = f.read().lower()
    except Exception:
        return ["Could not read changelog file"]
    
    required_sections = [
        'changelog',
        'unreleased',
        'semantic versioning',
        'keep a changelog'
    ]
    
    missing = []
    for section in required_sections:
        if section not in content:
            missing.append(f"Missing reference to '{section}'")
    
    return missing


def main() -> int:
    """Main function.
    
    Returns:
        Exit code (0 for success, 1 for failure).
    """
    # Get changelog path
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    changelog_path = project_root / 'CHANGELOG.md'
    
    print("Validating CHANGELOG.md...")
    
    # Check if file exists
    if not changelog_path.exists():
        print(f"Error: {changelog_path} not found")
        return 1
    
    # Parse changelog
    entries, parse_errors = parse_changelog(changelog_path)
    
    # Validate entries
    validation_errors = validate_changelog(entries)
    
    # Check required sections
    missing_sections = check_required_sections(changelog_path)
    
    # Collect all errors
    all_errors = parse_errors + validation_errors + missing_sections
    
    # Report results
    print(f"Found {len(entries)} changelog entries")
    
    if entries:
        print("\nEntries:")
        for entry in entries[:5]:  # Show first 5 entries
            print(f"  - {entry}")
        if len(entries) > 5:
            print(f"  ... and {len(entries) - 5} more")
    
    if all_errors:
        print(f"\n❌ Found {len(all_errors)} error(s):")
        for error in all_errors:
            print(f"  - {error}")
        return 1
    else:
        print("\n✅ Changelog is valid!")
        return 0


if __name__ == '__main__':
    sys.exit(main())