#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | while IFS= read -r -d '' file; do
    if grep -q "Tempest Foundation" "$file"; then
        echo "Removing header of: $file"

        tmpfile="$file.tmp"
        rm -f "$tmpfile"

        in_license_header=false
        skip_spdx=true

        while IFS= read -r line; do
            # Skip SPDX line if it's the first line
            if [ "$skip_spdx" = true ] && echo "$line" | grep -q "^// SPDX-License-Identifier:"; then
                skip_spdx=false
                continue
            fi
            skip_spdx=false

            # Check if we're starting a comment block
            if echo "$line" | grep -q "^/\*" && [ "$in_license_header" = false ]; then
                # Look ahead to see if this is our license header
                temp_lines=("$line")
                is_license_header=false

                # Read more lines to check for BEGIN marker
                while IFS= read -r next_line; do
                    temp_lines+=("$next_line")

                    if echo "$next_line" | grep -q -- "-- BEGIN LICENSE HEADER --"; then
                        is_license_header=true
                        in_license_header=true
                        break
                    fi

                    # If we hit the end of comment without finding BEGIN marker, it's not our header
                    if echo "$next_line" | grep -q "\*/"; then
                        break
                    fi
                done

                # If this wasn't our license header, write out all the stored lines
                if [ "$is_license_header" = false ]; then
                    for temp_line in "${temp_lines[@]}"; do
                        echo "$temp_line" >> "$tmpfile"
                    done
                fi

                continue
            fi

            # If we're in license header, look for end marker
            if [ "$in_license_header" = true ]; then
                if echo "$line" | grep -q -- "-- END OF LICENSE HEADER --"; then
                    # Read one more line to get the closing */
                    IFS= read -r closing_line
                    if echo "$closing_line" | grep -q "\*/"; then
                        in_license_header=false
                    fi
                fi
                continue
            fi

            # Regular line, write it out
            echo "$line" >> "$tmpfile"

        done < "$file"

        # Remove leading empty lines and replace original file
        awk 'NF || started { started=1; print }' "$tmpfile" > "$file"
        rm -f "$tmpfile"
    fi
done
