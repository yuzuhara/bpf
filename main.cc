
#include "bpf/bpf.h"

uint8_t raw_packet[] = {
    /* arp packet */
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01,
    // 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
    // 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    // 0x00, 0x00,

    /* tcp packet */
    0x84, 0xdb, 0xac, 0x31, 0x1d, 0xfa, 0x80, 0xe6,
    0x50, 0x17, 0x18, 0x46, 0x08, 0x00, 0x45, 0x00,
    0x00, 0x80, 0x91, 0xb3, 0x40, 0x00, 0x40, 0x06,
    0x3b, 0x57, 0xc0, 0xa8, 0x64, 0x67, 0xa3, 0x2c,
    0xa5, 0x31, 0x81, 0x42, 0x00, 0x50, 0x9b, 0x4c,
    0x6e, 0x59, 0x4a, 0x9e, 0x73, 0x7e, 0x80, 0x18,
    0x00, 0xe5, 0x54, 0xd9, 0x00, 0x00, 0x01, 0x01,
    0x08, 0x0a, 0x00, 0x18, 0x00, 0x16, 0x4e, 0xf8,
    0xcd, 0x22, 0x47, 0x45, 0x54, 0x20, 0x2f, 0x20,
    0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31,
    0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20,
    0x73, 0x6c, 0x61, 0x6e, 0x6b, 0x64, 0x65, 0x76,
    0x2e, 0x6e, 0x65, 0x74, 0x0d, 0x0a, 0x55, 0x73,
    0x65, 0x72, 0x2d, 0x41, 0x67, 0x65, 0x6e, 0x74,
    0x3a, 0x20, 0x63, 0x75, 0x72, 0x6c, 0x2f, 0x37,
    0x2e, 0x35, 0x30, 0x2e, 0x33, 0x0d, 0x0a, 0x41,
    0x63, 0x63, 0x65, 0x70, 0x74, 0x3a, 0x20, 0x2a,
    0x2f, 0x2a, 0x0d, 0x0a, 0x0d, 0x0a,
};

inline void disas_x86(const void* ptr, size_t len)
{
  slankdev::capstone c;
  c.disasm(ptr, len, 0x000, 0);
  for (size_t i = 0; i < c.insn_len(); i++) {
    const cs_insn* insn = c.get_insn();
    printf("0x%04lx:    %-5s %-20s   ",
        insn[i].address,
        insn[i].mnemonic,
        insn[i].op_str
    );
    for (size_t j=0; j<insn[i].size; j++) {
      printf("%02x ", insn[i].bytes[j]);
    }
    printf("\n");

    if (strcmp(insn[i].mnemonic, "nop") == 0) break;
  }
}


int main()
{
  printf("Packet\n");
  slankdev::hexdump(stdout, raw_packet, sizeof(raw_packet));

  bpf s("tcp");
  printf("\n\nBPF JIT with Xbyak%s x86 ASM\n", s.getVersionString());
  int (*func)(const void*,size_t) = s.getCode<int (*)(const void*,size_t)>();
  disas_x86((void*)func, Xbyak::DEFAULT_MAX_CODE_SIZE);

  int ret = func(raw_packet,sizeof(raw_packet));
  printf("\n\nresult: %d (%s)\n", ret, ret==0?"eject":"pass");
}