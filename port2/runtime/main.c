/* Phase-1 runtime of the native-pointer port: logging, the layout check and a main() that only reports.
   The game's own main() (0x800163F4, asm only) is a stub until port/runtime/main_port.c is carried over. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

extern unsigned int port_main0_image[], port_main0_bss[], port_x000_image[], port_x000_bss[], port_hiram[],
    port_scratch[], port_io[], port_bios[];

void port_printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
}

void port_fatal(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
    fputc('\n', stderr);
    exit(1);
}

static int stub_count;
void port_stub_hit(const char *name) {
    if (stub_count++ < 50) fprintf(stderr, "[port2] stub: %s\n", name);
}

void *port_bios_tcb_table(void) { return port_bios; }

/* Phase 1 keeps the ordering-table links as 24-bit addresses, which is exact only while every game datum sits
   below 16 MB (see port_otaddr in port_prelude.h). */
int port_check_layout(void) {
    const unsigned char *ends[] = {
        (unsigned char *)port_main0_image + 0x89000, (unsigned char *)port_main0_bss + 0x4F028,
        (unsigned char *)port_x000_image + 0x54000, (unsigned char *)port_x000_bss + 0x4000,
        (unsigned char *)port_hiram + 0x2800, (unsigned char *)port_scratch + 0x400 };
    unsigned i, bad = 0;
    for (i = 0; i < sizeof ends / sizeof ends[0]; i++)
        if ((unsigned long)ends[i] >= 0x1000000ul) bad++;
    return bad == 0;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("tombi port2 (native pointers, %d-bit): data image at %p..%p, scratch at %p, layout %s\n",
           (int)(sizeof(void *) * 8), (void *)port_main0_image, (void *)port_x000_bss, (void *)port_scratch,
           port_check_layout() ? "ok (below 16 MB)" : "NOT below 16 MB: OT links would truncate");
    return port_check_layout() ? 0 : 2;
}
int port_verbose;
