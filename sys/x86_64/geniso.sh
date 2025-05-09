#!/bin/sh
# file: sys/x86_64/geniso.sh
#
# Generates an grub2-mkrescue iso file from the given tar.
#
# NOTE: it would be nice to not require /usr/lib/grub, that would make the
# whole thing more hermetic even though we arent even close to hermetic.
#

OUT=$PWD/$1
IN=$PWD/$2

temp_dir=$(mktemp -d)

mkdir -p $temp_dir/iso
tar xf $IN -C $temp_dir/iso
grub2-mkrescue --directory /usr/lib/grub/x86_64-efi -o $OUT $temp_dir/iso/
rm -rf $temp_dir
