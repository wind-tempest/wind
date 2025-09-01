#!/usr/bin/bash

find sys/ include/ -name "*.[ch]" -print0 \
  | xargs -0 sed -i -E 's#^([[:space:]]*)/\*\s*(.*?)\s*\*/\s*$#\1// \2#'

while IFS= read -r -d '' file; do
  last_char=$(tail -c1 "$file")
  if [ "$last_char" != "" ]; then
    echo >> "$file"
  fi
done < <(find sys/ include/ -name "*.[ch]" -print0)

find sys/ include/ -name "*.[ch]" -print0 | while IFS= read -r -d '' file; do
  if ! clang-format -i "$file"; then
    echo -e "\e[33mIgnored: $file (too long path or error)\e[0m"
  fi
done
