#!/bin/sh
# Diagnostic script for Crestron processor
# Run this on the Crestron device: sh verify_libs.sh

echo "=== Checking library files ==="
cd /simpl/app02

echo "\n1. libZRCSdk.so:"
if [ -f "libZRCSdk.so" ]; then
    ls -lh libZRCSdk.so
    file libZRCSdk.so 2>/dev/null || echo "  file command not available"
    echo "  MD5: $(md5sum libZRCSdk.so 2>/dev/null || echo 'md5sum not available')"
else
    echo "  NOT FOUND"
fi

echo "\n2. libZrcSdkWrapper.so:"
if [ -f "libZrcSdkWrapper.so" ]; then
    ls -lh libZrcSdkWrapper.so
    file libZrcSdkWrapper.so 2>/dev/null || echo "  file command not available"
    echo "  MD5: $(md5sum libZrcSdkWrapper.so 2>/dev/null || echo 'md5sum not available')"
else
    echo "  NOT FOUND"
fi

echo "\n=== Checking library dependencies ==="
echo "3. ldd on libZrcSdkWrapper.so:"
ldd libZrcSdkWrapper.so 2>&1

echo "\n4. ldd on libZRCSdk.so:"
ldd libZRCSdk.so 2>&1

echo "\n=== System information ==="
echo "5. Kernel:"
uname -a

echo "\n6. CPU info:"
cat /proc/cpuinfo | grep -E "(model name|processor|Features)" | head -5

echo "\n7. Available libraries in /usr/lib:"
ls /usr/lib/libstdc++* 2>/dev/null || echo "  No libstdc++ found"
ls /usr/lib/libgcc* 2>/dev/null || echo "  No libgcc found"

echo "\n=== Done ==="
