#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

MODULES=(
    "jlang-shared"
    "jlang-preprocessor"
    "jlang-core"
    "jlang-stl"
)

for module in "${MODULES[@]}"; do
    MODULE_DIR="$ROOT_DIR/$module"
    if [ ! -d "$MODULE_DIR" ]; then
        continue
    fi

    files=$(find "$MODULE_DIR" -name '*.cpp' -o -name '*.h')
    if [ -n "$files" ]; then
        echo "Formatting $module..."
        clang-format -i $files
    fi
done

echo "All files have been formatted!"
