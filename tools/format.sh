#!/usr/bin/bash

while IFS= read -r -d '' file; do
  last_char=$(tail -c1 "$file")
  if [ "$last_char" != "" ]; then
    echo >> "$file"
  fi
done < <(find src/ -name "*.[ch]" -print0)

files=$(find src/ -name "*.[ch]")
if ! clang-format -i $files; then
  echo -e "\n\e[33mclang-format failed!\e[0m\n"
fi

for f in $files; do
  if grep -qE '^[[:space:]]*//.*' "$f"; then
    sed -i 's|^\([[:space:]]*\)//\(.*\)|\1/*\2 */|' "$f"
  fi
done
