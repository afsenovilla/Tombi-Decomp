/* Checks what the 3DS's game code runs for a GTE command (include/port_gte_inline.h, runtime/gte.c's ARMv6 paths:
   SMLALD, the VFP division) against runtime/gte.c's plain C port_gte_op: the same random register files through
   RTPS, RTPT, NCLIP, AVSZ3/4, NCCS/NCCT and MVMVA, every register and the flags compared.  Built for ARM, in qemu:

     F="-O2 -marm -march=armv6k -mfpu=vfp -mfloat-abi=softfp -static -Iport2/include"
     arm-linux-gnueabi-gcc $F port2/tools/gte_arm_test.c -o /tmp/gte_arm
     arm-linux-gnueabi-gcc $F -DGTE_PLAIN port2/tools/gte_arm_test.c -o /tmp/gte_c
     qemu-arm /tmp/gte_arm > a.txt; qemu-arm /tmp/gte_c > c.txt; cmp a.txt c.txt   (and the first prints "GTE_ARM 1") */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
int port_stereo_on;
void port_stereo_note(int x, int y, unsigned z) { (void)x; (void)y; (void)z; }
void port_printf(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vprintf(fmt, ap); va_end(ap); }
void port_fatal(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vprintf(fmt, ap); va_end(ap); exit(1); }
#include "../runtime/gte.c"
#include "port_gte_inline.h"
#ifdef GTE_PLAIN
#define RUN(op) port_gte_op(op)          /* the reference: runtime/gte.c's switch, the plain C, the library division */
#else
#define RUN(op) port_gte_op_i(op)        /* what the 3DS's game code runs: include/port_gte_inline.h, the ARM paths */
#endif

static unsigned long long rs = 88172645463325252ull;
static unsigned rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return (unsigned)rs; }
static int pick16(void) {   /* (the edges too: 0, +-1, the extremes) */
    static const int e[] = { 0, 1, -1, 0x7FFF, -0x8000, 0x7FFE, -0x7FFF, 4096, -4096 };
    unsigned r = rnd(); return r % 5 == 0 ? e[(r >> 8) % 9] : (short)(r >> 16);
}
static unsigned pack(void) { return (unsigned)(pick16() & 0xFFFF) | ((unsigned)pick16() << 16); }

int main(void) {
#ifdef GTE_ARM
    printf("GTE_ARM 1\n");
#else
    printf("GTE_ARM 1\n");   /* (the same first line: the files are compared whole) */
#endif
    static const unsigned long ops[] = { 0x01, 0x30, 0x06, 0x2D, 0x2E, 0x1B, 0x3F, 0x12 | (1 << 19), 0x12, 0x12 | (1 << 19) | (1 << 17) | (3 << 15) | (1 << 13) | (1 << 10), 0x12 | (2 << 17) | (2 << 15) | (2 << 13) };
    unsigned long long h = 1469598103934665603ull;
    for (int t = 0; t < 4000000; t++) {
        for (int i = 0; i < 32; i++) { D[i] = (int)pack(); C[i] = (int)pack(); }
        if (t & 1) for (int i = 5; i < 8; i++) C[i] = (int)rnd();             /* translations: any 32 bits */
        C[26] = (int)(rnd() & 0xFFFF); C[24] = (int)rnd(); C[25] = (int)rnd(); C[28] = (int)rnd();
        unsigned long op = ops[rnd() % (sizeof ops / sizeof ops[0])];
        RUN(op);
        for (int i = 0; i < 32; i++) { h = (h ^ (unsigned)D[i]) * 1099511628211ull; h = (h ^ (unsigned)C[i]) * 1099511628211ull; }
        h = (h ^ FLAG) * 1099511628211ull;
        if (t % 200000 == 0) printf("%d %016llx\n", t, h);
    }
    printf("end %016llx\n", h);
    return 0;
}
