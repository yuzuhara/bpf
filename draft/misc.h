
#pragma once
#include <stdint.h>

namespace bpf {

uint8_t pack_arp[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00
};

uint8_t pack_tcp[] = {
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
    0x2f, 0x2a, 0x0d, 0x0a, 0x0d, 0x0a
};



struct bpf::insn pgm_01[] = {
    { 0xfffe, 0, 0, 0x00000000 }, // int
    { 0x0006, 0, 0, 0x00000000 }, // ret   #0
};

struct bpf::insn pgm_tcp[] = {
    /* tcpdump -dd tcp */
    { 0x0028, 0, 0, 0x0000000c }, // (000) ldh   [12]
    { 0x0015, 0, 5, 0x000086dd }, // (001) jeq   #0x86dd  jt 2   jf 7
    { 0x0030, 0, 0, 0x00000014 }, // (002) ldb   [20]
    { 0x0015, 6, 0, 0x00000006 }, // (003) jeq   #0x6     jt 10  jf 4
    { 0x0015, 0, 6, 0x0000002c }, // (004) jeq   #0x2c    jt 5   jf 11
    { 0x0030, 0, 0, 0x00000036 }, // (005) ldb   [54]
    { 0x0015, 3, 4, 0x00000006 }, // (006) jeq   #0x6     jt 10  jf 11
    { 0x0015, 0, 3, 0x00000800 }, // (007) jeq   #0x800   jt 8   jf 11
    { 0x0030, 0, 0, 0x00000017 }, // (008) ldb   [23]
    { 0x0015, 0, 1, 0x00000006 }, // (009) jeq   #0x6     jt 10  jf 11
    { 0x0006, 0, 0, 0x00040000 }, // (010) ret   #262144
    { 0x0006, 0, 0, 0x00000000 }, // (011) ret   #0
};

struct bpf::insn pgm_tcp_port_80[] = {
    /* tcpdump -dd "tcp port 80"*/
    { 0x28, 0 , 0 , 0x0000000c }, // (000) ldh   [12]
    { 0x15, 0 , 6 , 0x000086dd }, // (001) jeq   #0x86dd  jt 2   jf 8
    { 0x30, 0 , 0 , 0x00000014 }, // (002) ldb   [20]
    { 0x15, 0 , 15, 0x00000006 }, // (003) jeq   #0x6     jt 4   jf 19
    { 0x28, 0 , 0 , 0x00000036 }, // (004) ldh   [54]
    { 0x15, 12, 0 , 0x00000050 }, // (005) jeq   #0x50    jt 18  jf 6
    { 0x28, 0 , 0 , 0x00000038 }, // (006) ldh   [56]
    { 0x15, 10, 11, 0x00000050 }, // (007) jeq   #0x50    jt 18  jf 19
    { 0x15, 0 , 10, 0x00000800 }, // (008) jeq   #0x800   jt 9   jf 19
    { 0x30, 0 , 0 , 0x00000017 }, // (009) ldb   [23]
    { 0x15, 0 , 8 , 0x00000006 }, // (010) jeq   #0x6     jt 11  jf 19
    { 0x28, 0 , 0 , 0x00000014 }, // (011) ldh   [20]
    { 0x45, 6 , 0 , 0x00001fff }, // (012) jset  #0x1fff  jt 19  jf 13
    { 0xb1, 0 , 0 , 0x0000000e }, // (013) ldxb  4*([14]&0xf)
    { 0x48, 0 , 0 , 0x0000000e }, // (014) ldh   [x + 14]
    { 0x15, 2 , 0 , 0x00000050 }, // (015) jeq   #0x50    jt 18  jf 16
    { 0x48, 0 , 0 , 0x00000010 }, // (016) ldh   [x + 16]
    { 0x15, 0 , 1 , 0x00000050 }, // (017) jeq   #0x50    jt 18  jf 19
    { 0x6 , 0 , 0 , 0x00040000 }, // (018) ret   #262144
    { 0x6 , 0 , 0 , 0x00000000 }, // (019) ret   #0
};

struct bpf::insn pgm_arp_or_ip[] = {
    /* tcpdump -dd "arp or ip" */
    { 0x0028, 0, 0, 0x0000000c }, // (000) ldh   [12]
    { 0x0015, 1, 0, 0x00000806 }, // (001) jeq   #0x806   jt 3   jf 2
    { 0x0015, 0, 1, 0x00000800 }, // (002) jeq   #0x800   jt 3   jf 4
    { 0x0006, 0, 0, 0x00040000 }, // (003) ret   #262144
    { 0x0006, 0, 0, 0x00000000 }, // (004) ret   #0
};

struct bpf::insn pgm_arp[] = {
    /* tcpdump -dd "arp"*/
    { 0x28, 0, 0, 0x0000000c }, // (000) ldh     [12]
    { 0x15, 0, 1, 0x00000806 }, // (001) jeq     #0x806   jt 2   jf 3
    { 0x6 , 0, 0, 0x00040000 }, // (002) ret     #262144
    { 0x6 , 0, 0, 0x00000000 }, // (003) ret     #0
};

struct bpf::insn pgm_not_arp[] = {
    /* tcpdump -dd "not arp"*/
    { 0x28, 0, 0, 0x0000000c }, // (000) ldh     [12]
    { 0x15, 0, 1, 0x00000806 }, // (001) jeq     #0x806   jt 2   jf 3
    { 0x6 , 0, 0, 0x00000000 }, // (002) ret     #0
    { 0x6 , 0, 0, 0x00040000 }, // (003) ret     #262144
};


} /* namespace bpf */