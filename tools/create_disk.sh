#!/usr/bin/env bash
# create_disk.sh — helper to create an EXT2 disk image for QEMU
#
# Usage: ./create_disk.sh [SIZE]
#   SIZE — optional disk size (default 64M). Accepts any dd-compatible
#           size suffix (e.g. 32M, 128M, 1G).
#
# The script produces build/disk.img, formatted with an empty EXT2
# filesystem.  Add it to your QEMU command line like so:
#   -drive file=build/disk.img,format=raw,if=ide
#
# Requirements: dd, mkfs.ext2 (from e2fsprogs)
set -euo pipefail

SIZE="${1:-64M}"
IMAGE_DIR="build"
IMAGE_PATH="$IMAGE_DIR/disk.img"

mkdir -p "$IMAGE_DIR"

if [[ -f "$IMAGE_PATH" ]]; then
  echo "[*] $IMAGE_PATH already exists. Removing and recreating."
  rm -f "$IMAGE_PATH"
fi

echo "[*] Creating disk image ($SIZE) at $IMAGE_PATH"

dd if=/dev/zero of="$IMAGE_PATH" bs="$SIZE" count=0 seek=1

echo "[*] Formatting image with EXT2 filesystem"
#  -O none     : disable all optional features (keeps it at revision 0)
#  -b 1024     : 1 KiB block size (matches driver defaults)
#  -I 128      : inode size 128 bytes (rev 0 standard)
# Enable default safe features (filetype) but disable journaling which our
# read-only driver can't handle. Keep 1 KiB block, 128-byte inodes.
mkfs.ext2 -F -q -O ^has_journal -b 1024 -I 128 "$IMAGE_PATH"

# Remove lost+found directory from the image
if debugfs -R "ls /lost+found" "$IMAGE_PATH" >/dev/null 2>&1; then
  echo "[*] Removing lost+found from disk image"
  debugfs -w -R "rmdir /lost+found" "$IMAGE_PATH"
fi

MNT_DIR="build/mnt-$$"
# If a folder named 'disk' exists at repository root, copy its contents into
# the image (recursively) using debugfs. This avoids needing sudo/loop mounts.
if [[ -d "disk" ]]; then
  echo "[*] Copying contents of ./disk into disk.img via debugfs"
  # Iterate directories from shallowest to deepest to make sure parents exist first
  # Create directories inside disk/ (but not disk/ itself)
  find disk -mindepth 1 -type d -print0 | while IFS= read -r -d '' dir; do
    rel="${dir#disk/}"
    debugfs -w -R "mkdir /$rel" "$IMAGE_PATH" >/dev/null 2>&1 || true
  done

  # Copy files inside disk/ (but not disk/ itself)
  find disk -type f -print0 | while IFS= read -r -d '' file; do
    rel="${file#disk/}"
    debugfs -w -R "write $file /$rel" "$IMAGE_PATH" >/dev/null 2>&1
  done
else
  echo "[*] No ./disk directory found; skipping copy step."
fi