#!/bin/bash

SRC_DIR="./kernel"
OUT_DIR="./analyzed"

mkdir -p "$OUT_DIR"

IO_DIRS=$(find "$SRC_DIR/io" -type d)
INCLUDE_PATHS=$(for dir in $IO_DIRS; do echo -n "-I$dir "; done)
INCLUDE_PATHS="$INCLUDE_PATHS -I./include -I$SRC_DIR"

CLANG_FLAGS="-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wnull-dereference -Wdouble-promotion -Werror $INCLUDE_PATHS"

find . -name "*.c" | while read -r file; do
    echo "Analyzing $file with clang static analyzer..."
    out_file=$(echo "$file" | sed 's|^\./||;s|/|_|g')

    clang --analyze $INCLUDE_PATHS "$file" -Xanalyzer -analyzer-output=plist -o "$OUT_DIR/$out_file.plist"

    echo "Running clang-tidy on $file..."
    clang-tidy "$file" $INCLUDE_PATHS -- -std=c11 $CLANG_FLAGS > "$OUT_DIR/$out_file.clang-tidy.txt" 2>&1

    echo "Compiling $file with warnings hardcore..."
    clang $CLANG_FLAGS -fsyntax-only "$file"

    echo "Done $file"
done
