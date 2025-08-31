#!/usr/bin/bash

find sys/ include/ -name "*.[ch]" -print0 \
  | xargs -0 sed -i -E \
    's#^([[:space:]]*)/\*\s*(.*?)\s*\*/\s*$#\1// \2#'

while IFS= read -r -d '' file; do
  last_char=$(tail -c1 "$file")
  if [ "$last_char" != "" ]; then
    echo >> "$file"
  fi
done < <(find sys/ include/ -name "*.[ch]" -print0)

files=$(find sys/ include/ -name "*.[ch]")
if ! clang-format -i "$files"; then
  echo -e "\n\e[33mclang-format failed!\e[0m\n"
fi
