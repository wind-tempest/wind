#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | while IFS= read -r -d '' file; do
    if grep -q "Tempest Foundation" "$file"; then
        echo "Removing header of: $file"

        awk '
        BEGIN {inside=0}
        /^\/\/ SPDX-License-Identifier:/ {next}
        /^\/\*/ {inside=1; next}
        inside && /-- END OF LICENSE HEADER --/ {inside=0; skip_next=1; next}
        skip_next {skip_next=0; next}   # pula linha do */ final
        inside {next}
        {print}
        ' "$file" > "$file.tmp" && mv "$file.tmp" "$file"

        awk 'NR==1 {while (/^[[:space:]]*$/) {getline}} {print}' "$file" > "$file.tmp" && mv "$file.tmp" "$file"
    fi
done
