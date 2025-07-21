#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

BUILD_PATH="build"
TOOLS_PATH="tools"
ISO_PATH="${BUILD_PATH}/out/wind.iso"

# Ensure tools directory exists
if [ ! -d "$TOOLS_PATH" ]; then
    error_exit "Tools directory not found: $TOOLS_PATH"
fi

YELLOW="\e[33m"
RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"

log() {
	echo -e "[$(date '+%H:%M:%S')] $*"
}

error_exit() {
	echo -e "${RED}[ERROR] $*${RESET}" >&2
	exit 1
}

run_step() {
	local cmd="$*"
	log "➤ Running: $cmd"
	if ! eval "$cmd"; then
		error_exit "Step failed: $cmd"
	fi
}

usage() {
  cat <<EOF
Usage: $0 [OPTIONS]

  -b, --build-only    only build (do not launch QEMU)
  -r, --run-only      only launch QEMU (assumes build is up-to-date)
  -h, --help          display this help and exit

If no options are given, this script will build AND then run.
EOF
  exit 0
}

BUILD=true
RUN=true

while (( $# )); do
  case "$1" in
    -b|--build-only) BUILD=true; RUN=false ;;
    -r|--run-only)   BUILD=false; RUN=true ;;
    -h|--help)       usage ;;
    *)               error_exit "Invalid option: $1" ;;
  esac
  shift
done

if $BUILD; then
	run_step "${TOOLS_PATH}/header.sh"
	run_step "${TOOLS_PATH}/format.sh"
	run_step "${TOOLS_PATH}/create_disk.sh"
	run_step "make"
fi

if $RUN; then
	log "Launching QEMU..."
	make run
fi

log "${GREEN}All done!${RESET}"
