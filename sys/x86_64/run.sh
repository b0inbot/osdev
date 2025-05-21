#!/bin/sh
# file: sys/x86_64/run.sh
#
# The run script which runs the whole system together. The qemu invocation was borrowed
# mostly from upstream NOVA and Genode documentation.
#

CODE=$1
VARS=$2
ISO=$3

# NOTE: Without the exec bits the CODE and VARS will not work
# in qemu.
chmod +w $CODE $VARS

qemu-system-x86_64 \
  -m 1024 \
  -smp 4 \
  -serial mon:stdio \
  -cpu host,vmx \
  -nographic \
  -M q35,accel=kvm,kernel-irqchip=split \
  -drive if=pflash,format=raw,readonly=on,file="$CODE" \
  -drive if=pflash,format=raw,file="$VARS" \
  -enable-kvm \
  -device intel-iommu,intremap=on \
  -cdrom "$ISO"
