#!/bin/bash

headers_bad=false

read -r -d '' license_header <<'EOF'
// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Wind/Tempest Project
 *
 * Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>
 *
 * This file is part of the Wind (Operating System) and Tempest (The Kernel).
 * It is licensed under the GNU General Public License version 3.0  (GPLv3).
 */
EOF

for file in $(find . -type f -name "*.c" -o -name "*.h"); do
    header=$(head -n 25 "$file")

    # Check if header text already exists
    if ! echo "$header" | grep -q "Copyright (C) 2025 Wind Foundation <https://wind.tempestfoundation.org>"; then
        echo "Incorrect or missing header in: $file"
        headers_bad=true

        tmpfile="$file.tmp"

        {
            echo "// $(basename "$file")"
            echo ""
            echo "$license_header"
            echo ""
            cat "$file"
        } > "$tmpfile"

        mv "$tmpfile" "$file"
        echo "Fixed!"
    fi
done

if [ "$headers_bad" = false ]; then
    echo "Nah, everything is good."
fi
