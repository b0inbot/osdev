/* file: sys/nova/HIP.hh
 *
 * An (incomplete) version of the Hypervisor Information Page as described
 * in the NOVA specification document.
 */
#pragma once

#define PACKED __attribute__((packed))

namespace nova {

struct HIP {
  uint32_t signature;
  uint16_t checksum;
  uint16_t length;

  uint64_t nova_start_address;
  uint64_t nova_end_address;

  uint64_t mbuf_start_address;
  uint64_t mbuf_end_address;

  uint64_t root_start_address;
  uint64_t root_end_address;

  uint64_t acpi_rsdp_address;
  uint64_t uefi_mm_address;
  uint32_t uefi_mm_size;
  uint16_t uefi_desc_size;
  uint16_t uefi_desc_version;

  uint64_t stc_freq;
  struct {
    uint8_t obj;
    uint8_t hst;
    uint8_t gst;
    uint8_t dma;
    uint8_t pio;
    uint8_t msr;
  } PACKED sbw;

  uint16_t cpu_bsp;

  struct {
    uint8_t obj;
    uint8_t hst;
    uint8_t gst;
    uint8_t dma;
    uint8_t pio;
    uint8_t msr;
  } PACKED mco;

  uint16_t gsi_pin;

} PACKED;

} // namespace nova
