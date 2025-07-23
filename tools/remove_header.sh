#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.h" \) | while read -r file; do
  if grep -q "Wind/Tempest" "$file"; then
    echo "Removing header of: $file"

    awk '
      BEGIN { inside_comment = 0; header_skipped = 0; printed = 0 }

      # Remove single-line // comment header with Wind/Tempest
      NR == 1 && $0 ~ /^[[:space:]]*\/\/[[:space:]]*[^[:space:]]+\.(c|h)[[:space:]]*$/ { next }
      NR <= 5 && $0 ~ /^[[:space:]]*\/\/[[:space:]]*[^[:space:]]+\.(c|h)[[:space:]]*$/ { next }

      # Remove multi-line /* ... */ header containing Wind/Tempest
      NR == 1 && /^\s*\/\*.*\*\/\s*$/ { next }
      /^\/\*/ && header_skipped == 0 {
        inside_comment = 1
        comment_block = $0 "\n"
        next
      }
      inside_comment {
        comment_block = comment_block $0 "\n"
        if ($0 ~ /Wind\/Tempest/) {
          skip_this = 1
        }
        if ($0 ~ /\*\//) {
          inside_comment = 0
          if (skip_this) {
            header_skipped = 1
            skip_this = 0
            next
          } else {
            printf "%s", comment_block
          }
          comment_block = ""
          next
        }
        next
      }

      NR <= 5 && $0 ~ /^[[:space:]]*$/ { next }
      NR <= 5 && $0 ~ /^[[:space:]]*\/\/[[:space:]]*[^[:space:]]+\.c[[:space:]]*$/ { next }

      {
        if (!printed && $0 ~ /^[[:space:]]*$/) next
        printed = 1
        print
      }
    ' "$file" > "$file.tmp" && mv "$file.tmp" "$file"

    echo "Header removed!"
  fi
done
