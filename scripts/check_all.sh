#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "== C++23 engine =="
cmake -S "$ROOT/engine-cpp" -B "$ROOT/engine-cpp/build" >/dev/null
cmake --build "$ROOT/engine-cpp/build" -j2 >/dev/null
ctest --test-dir "$ROOT/engine-cpp/build" --output-on-failure

echo "== Go control =="
(cd "$ROOT/control-go" && go test ./...)

echo "== Python corruption lab =="
PYTHONPATH="$ROOT/lab-python" python3 -m unittest discover -s "$ROOT/lab-python/tests" -v

echo "== TypeScript studio =="
(cd "$ROOT/studio-ts" && npm test)

echo "== Java evidence plugin =="
rm -rf "$ROOT/plugin-java/out" && mkdir -p "$ROOT/plugin-java/out"
javac -d "$ROOT/plugin-java/out" $(find "$ROOT/plugin-java/src/main/java" "$ROOT/plugin-java/src/test/java" -name '*.java')
java -cp "$ROOT/plugin-java/out" dev.resurge.plugin.PluginTest

echo "== Cross-language integration =="
python3 "$ROOT/scripts/check_integration.py"

echo "== Optional research toolchains =="
for tool in nvcc zig julia lean iverilog; do
  if command -v "$tool" >/dev/null 2>&1; then echo "$tool: available (manual/research track)"; else echo "$tool: not installed; source included but not claimed verified"; fi
done

echo "ALL DEFAULT VERIFIED COMPONENTS PASSED"
