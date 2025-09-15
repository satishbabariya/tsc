#!/bin/bash
set -e

echo "Collecting coverage data..."

# Copy .gcno files to coverage directory
find . -name "*.gcno" -exec cp {} coverage_data/ \;

# Copy .gcda files to coverage directory
find . -name "*.gcda" -exec cp {} coverage_data/ \;

# Generate coverage reports for each source file
cd coverage_data

for gcno_file in *.gcno; do
    if [ -f "$gcno_file" ]; then
        echo "Processing: $gcno_file"
        gcov "$gcno_file" >/dev/null 2>&1 || true
    fi
done

echo "Coverage data collection complete"
