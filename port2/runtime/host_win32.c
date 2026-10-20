/* Windows host helpers: the clock and sleeps (runtime/main.c has the POSIX ones).  Kept apart from main.c because
 * windows.h claims the Psy-Q names EnterCriticalSection / OpenEvent the game's main() calls. */
#ifdef _WIN32
#include <windows.h>
#include "port_runtime.h"

/* QueryPerformanceCounter for the clock; Sleep() wakes within about 1 ms once timeBeginPeriod(1) is set, so a
   wait sleeps until 2 ms before the deadline and yields the rest (the vblank schedule is absolute, shims/psyq.c) */
static LARGE_INTEGER qpc_freq;
unsigned long long port_time_us(void) {
    LARGE_INTEGER c;
    if (!qpc_freq.QuadPart) { QueryPerformanceFrequency(&qpc_freq); timeBeginPeriod(1); }
    QueryPerformanceCounter(&c);
    return (unsigned long long)((double)c.QuadPart * 1000000.0 / (double)qpc_freq.QuadPart);
}
void port_sleep_us(unsigned long long us) {
    unsigned long long t0 = port_time_us();
    if (us > 2000) Sleep((DWORD)((us - 2000) / 1000));
    while (port_time_us() - t0 < us) Sleep(0);
}
#endif
