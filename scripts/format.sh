#!/usr/bin/bash

# add newline at end of each file if missing
find src/ -name "*.[ch]" -exec sh -c '
  for f; do
    [ "$(tail -c1 "$f")" != "" ] && echo >> "$f"
  done
' sh {} +

# try to format the src/ folder
if ! find src/ -name "*.[ch]" -exec clang-format -i {} +; then
    echo -e "\n\e[33mTried to format src/ folder using .clang-format, but failed!\e[0m\n"
fi

if ! find src/ -type f \( -name "*.c" -o -name "*.h" \) -exec sed -i 's|^\([[:space:]]*\)//\(.*\)|\1/*\2 */|' {} +; then
    echo -e "\n\e[33mTried to convert single-line comments to multi-line comments, but failed!\e[0m\n"
fi