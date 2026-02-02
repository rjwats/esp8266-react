Import("env")

# It appears that espressif32 currently relies on intelhex which isn't installed by default
# This script ensures the dependency is present before the build starts
try:
    import intelhex
except ImportError:
    print("Installing missing Python dependency: intelhex")
    env.Execute("$PYTHONEXE -m pip install intelhex")
