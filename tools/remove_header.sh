#!/bin/bash

find . -type f \( -name "*.c" -o -name "*.h" \) | while read -r file; do
    if grep -q "Wind/Tempest is free software" "$file"; then
        echo "Removing header of: $file"
        perl -0777 -i -pe '
            s{
                \A
                (?:/\* [^*]*? \*/\R)?
                /\*
                .*?Wind/Tempest\ is\ free\ software
                .*?\*/
                (?:\R){0,2}
            }{}xms;
        ' "$file"
        echo "Header removed!"
    fi
done
