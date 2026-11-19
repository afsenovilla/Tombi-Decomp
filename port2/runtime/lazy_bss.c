/* port2/tools/lazy_bss.py's units ask here for their tables' block (runtime/lazy_bss.h).  Zeroed, as the statics
   were; never given back (a unit's other state stays as it was).  The 3DS has its own (n3ds/main_3ds.c: only when the
   memory is there). */
#if defined(PORT_LAZY_BSS) && !defined(__3DS__)
#include <stdlib.h>
#include "port_runtime.h"
void *port_lazy_alloc(const char *what, size_t n)
{
    void *p = calloc(1, n);
    port_printf("[lazy] %s: %u KB of tables%s\n", what, (unsigned)(n >> 10), p ? "" : ": no memory, off");
    return p;
}
#endif
#if !defined(PORT_LAZY_BSS) || defined(__3DS__)
typedef int port_lazy_bss_none;          /* (nothing here in this build) */
#endif
