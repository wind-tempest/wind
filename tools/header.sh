#!/bin/bash

headers_bad=false

read -r -d '' license_header <<'EOF'
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2025 Tempest Foundation <https://wind.tempestfoundation.org>
 *
 * Authors:
 *	Russian95 (https://github.com/Russian95CrE) <russian95@tempestfoundation.org>
 */
EOF

find . -type f \( -name "*.c" -o -name "*.h" \) | while IFS= read -r file; do
    header=$(head -n 25 "$file")

    if ! echo "$header" | grep -q "Copyright (C) 2025 Tempest Foundation"; then
        echo "Incorrect or missing header in: $file"
        headers_bad=true

        tmpfile="$file.tmp"

        {
            echo "// $(basename "$file")"
            echo ""
            echo "$license_header"
            echo ""
            cat "$file"
            echo ""
        } > "$tmpfile"

        mv "$tmpfile" "$file"
        echo "Fixed!"
    fi
done

if [ "$headers_bad" = false ]; then
    echo "Nah, everything is good."
fi