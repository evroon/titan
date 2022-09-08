#!/bin/bash
set -evo pipefail
git ls-files -z "*.cpp" "*.h" | while IFS= read -rd '' f; do tail -c1 < "$f" | read -r _ || echo >> "$f"; done
clang-format -style=file --dry-run **/*.h **/*.cpp
