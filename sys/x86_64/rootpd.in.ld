/* file: sys/x86_64/rootpd.in.ld
 *
 * The linker script for the rootpd. It is cobbled together
 * with details from the NOVA specification, experimentation and
 * invocations of readelf -l and objdump -x to look at exactly
 * how the binary gets laid out. It is not overly thought out or
 * meant to be set in stone.
 *
 * According to genode documentation, bss sections for root arent
 * supported in NOVA.
 */
PHDRS {
  text PT_LOAD AT(0x10000000) FLAGS(5);
  data PT_LOAD AT(0x10001000) FLAGS(6);
}

SECTIONS
{
  ENTRY(__start)

  . = 0x10000000 ;
  . = ALIGN(0x0);
  .text . + 0 : AT (ADDR (.text) - 0x0)
  {
      __start = .;
     *(.text .text.* .gnu.linkonce.t.*)
      _mmain = .;
     *(.rodata .rodata.*)
  } : text

  . = 0x10400000;
  .data :
  {
     . = 0x000004000;
     *(.data .data.*);
  } : data
}
