#!/bin/bash
# Resolve the directory of this script
DIR="$(cd "$(dirname "$0")" && pwd)"

# Prepend our lib directory to LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$DIR/lib:$LD_LIBRARY_PATH"

# Default config.json path
CONFIG_FILE="$DIR/config.json"

# Run the app, always pointing to config.json 
if [ -f "$CONFIG_FILE" ]; then
    "$DIR/bin/xml_json_converter" --config "$CONFIG_FILE" "$@"
else
    echo "config.json not found at $CONFIG_FILE"
    exit 1
fi
