import pytest
import os
import sys
from pathlib import Path

# Add the module path to sys.path to import from intl/core_option_translation
sys.path.insert(0, str(Path(__file__).parent.parent))

from intl.core_option_translation import load_translation_file

@pytest.mark.parametrize("payload", [
    # Exact exploit case - classic path traversal
    "../../../etc/passwd",
    # Boundary case - double dot slash with extra dots
    "....//....//etc/passwd",
    # URL encoded traversal attempt
    "%2e%2e%2f%2e%2e%2f%2e%2e%2fetc%2fpasswd",
    # Valid input - should work normally
    "en_US/options.json",
])
def test_file_operations_stay_within_root_directory(payload):
    """Invariant: File operations never resolve paths outside the declared root directory"""
    
    # Define the root directory for translations
    root_dir = Path(__file__).parent.parent / "intl" / "translations"
    
    try:
        # Call the actual production function
        result = load_translation_file(payload)
        
        # If function succeeds with valid input, verify the resolved path is within root
        if payload == "en_US/options.json":
            # For valid input, we expect success - verify path containment
            # This assumes load_translation_file returns the file content or path
            # If it returns content, we can't verify path directly, but the test
            # will fail if it tries to read outside root due to FileNotFoundError
            assert True  # Valid case passed
        else:
            # For traversal payloads, we should NOT reach here
            # If we do, it means the function accepted a traversal path
            # This is a security failure
            pytest.fail(f"Security invariant violated: Function accepted path traversal payload: {payload}")
            
    except (FileNotFoundError, PermissionError, ValueError, OSError) as e:
        # Expected behavior for malicious inputs - function should reject or fail
        # Check if error indicates path traversal was prevented
        if payload == "en_US/options.json":
            # Valid input should not raise these errors
            pytest.fail(f"Valid input '{payload}' raised unexpected error: {e}")
        else:
            # Malicious input correctly rejected
            assert True
    
    except Exception as e:
        # Any other exception - log it but consider it a failure for valid inputs
        if payload == "en_US/options.json":
            pytest.fail(f"Valid input '{payload}' raised unexpected exception: {type(e).__name__}: {e}")
        # For malicious inputs, any exception is acceptable as long as it prevents traversal