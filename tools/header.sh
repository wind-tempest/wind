#!/bin/bash

headers_bad=false

read -r -d '' agpl_header <<'EOF'
/*
 * ---------------------------------------------------------------------------
 *  Wind (operating system) / Tempest (kernel)
 *  Copyright (C) 2025 Wind/Tempest Foundation <https://wind.infernointeractive.win>
 * ---------------------------------------------------------------------------
 *  This file is part of the Wind/Tempest project, an open-source operating system
 *  and kernel designed to deliver performance, stability, and freedom to users.
 *
 *  - License:
 *  This software is licensed under the GNU Affero General Public License (AGPL)
 *  version 3 or any later version. You may redistribute and/or modify it under
 *  the terms of the AGPL as published by the Free Software Foundation.
 *
 *  - Warranty Disclaimer:
 *  Wind/Tempest is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
 *  details.
 *
 *  - Permissions:
 *  > You may modify this file and include your own copyright notices below.
 *  > Forks and contributions are welcome, as long as this license is respected.
 *
 *  - Additional Information:
 *  For more information about the project, visit:
 *  https://wind.infernointeractive.win
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 * ---------------------------------------------------------------------------
 */
EOF

for file in $(find . -type f -name "*.c" -o -name "*.h"); do
    filename=$(basename "$file")
    expected="/* $filename */"

    header=$(head -n 15 "$file")

    if ! echo "$header" | grep -q "This file is part of the Wind/Tempest project"; then
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
