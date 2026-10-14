/* Force-included before every game source in the port build (port/tools/gen_port.py). */
#ifndef PORT_PRELUDE_H
#define PORT_PRELUDE_H
#define PORT 1
/* software GTE (port/runtime/gte.c); the gte_* macros of the sources are translated to these */
void port_gte_op(unsigned long op);
void port_gte_mtc2(int reg, unsigned long v);
unsigned long port_gte_mfc2(int reg);
void port_gte_ctc2(int reg, unsigned long v);
unsigned long port_gte_cfc2(int reg);
#define PORT_GTE_UNSUPPORTED(ins) port_gte_unsupported(ins)
void port_gte_unsupported(const char *ins);
/* BIOS kernel area replacements (port/runtime/threads.c) */
void *port_bios_tcb_table(void);
#endif
