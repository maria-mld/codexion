#!/bin/bash

echo "=== 1. Test 1 coder ==="
./codexion 1 500 200 200 200 1 0 fifo

echo ""
echo "=== 2. Test Invalid args ==="
./codexion 4 800 200 200 200 3 0 lifo > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo "✅ LIFO rejected: OK"
else
    echo "❌ LIFO rejected: FAIL"
fi

echo ""
echo "=== 3. Test Zombie logs (no logs after death) ==="
./codexion 4 310 200 100 100 5 0 fifo > /tmp/out.log
LINES_AFTER=$(sed -n '/burned out/,$p' /tmp/out.log | wc -l | tr -d ' ')
if [ "$LINES_AFTER" -eq 1 ]; then
    echo "✅ Zombie logs: OK (0 logs after burned out)"
else
    echo "❌ Zombie logs: FAIL ($LINES_AFTER lines found)"
fi

echo ""
echo "=== 4. Test Memory Leaks ==="
if command -v valgrind &> /dev/null; then
    valgrind --leak-check=full --error-exitcode=42 ./codexion 4 3000 400 200 200 2 100 edf > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "✅ Valgrind: OK (0 leaks)"
    else
        echo "❌ Valgrind: FAIL (leaks or errors detected)"
    fi
else
    echo "⚠️  Valgrind not installed (skipped on this machine)"
fi

rm -f /tmp/out.log
