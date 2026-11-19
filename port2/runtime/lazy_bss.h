/* The big static work tables of a unit kept out of the program image (the 3DS; port2/tools/lazy_bss.py writes the
   unit's copy and its port_lazy_ok()).  PORT_LAZY_OK() before the unit touches any of them: 1 when they have their
   memory (always, in a build without PORT_LAZY_BSS). */
#ifndef LAZY_BSS_H
#define LAZY_BSS_H
#ifdef PORT_LAZY_BSS
static int port_lazy_ok(void);
#define PORT_LAZY_OK() port_lazy_ok()
#else
#define PORT_LAZY_OK() 1
#endif
#endif
