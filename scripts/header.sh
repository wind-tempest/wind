#!/bin/bash

headers_bad=false

read -r -d '' agpl_header <<'EOF'
/*
 * This file is part of Wind/Tempest
 *
 * Wind/Tempest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wind/Tempest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
EOF

for file in $(find . -type f -name "*.c" -o -name "*.h"); do
    filename=$(basename "$file")
    expected="/* $filename */"

    header=$(head -n 15 "$file")

    if ! echo "$header" | grep -q "Wind/Tempest is free software"; then
        echo "Incorrect or missing header in: $file"
        headers_bad=true

        tmpfile="$file.tmp"

        {
            echo "$expected"
            echo ""
            echo "$agpl_header"
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
