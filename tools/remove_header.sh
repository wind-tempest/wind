#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.h" \) | while read -r file; do
  if grep -qi "tempest foundation" "$file"; then
    echo "Removing header of: $file"

    awk '
      BEGIN {
        inside_block = 0
        skip_block   = 0
        printed      = 0
      }

      /^[[:space:]]*\/\/[[:space:]]*SPDX-License-Identifier:/ { next }
      /^[[:space:]]*\/\/[[:space:]]*Copyright.*Tempest Foundation/ { next }

      !printed && /^\s*\/\*/ {
        inside_block = 1
        block        = $0 "\n"
        skip_block   = 0
        next
      }

      inside_block {
        block = block $0 "\n"
        if ($0 ~ /Tempest Foundation/) skip_block = 1
        if ($0 ~ /\*\//) {
          inside_block = 0
          if (skip_block) {
            skip_block = 0
            next
          } else {
            printf "%s", block
          }
          block = ""
          next
        }
        next
      }

      !printed && ($0 ~ /^[[:space:]]*$/ || $0 ~ /^[[:space:]]*\/\/.*$/) { next }

      {
        printed = 1
        print
      }
    ' "$file" > "$file.tmp" && mv "$file.tmp" "$file"

    echo "Header removed!"
  fi
done
