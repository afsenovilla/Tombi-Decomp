// FUNC 8006ae54 72 MAIN0
// MATCHING 8006ae54 72
// VERSION SCUS_94236 scus94236 8006A40C 64
// VERSION BETA_980203 beta980203 80068B50 64
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006AE54 (_padIfSioRW.c): PAL C of src/_padIfSioRW.c (0.882 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
/* US func_8006A40C (libpad/padif.c) plus the PAL `*response = 0` store. */
#include "padlocal.h"
extern int D_80097590;                  /* PAL D_8009822C at its USA address */
extern int (*D_80097528)(padPort*);     /* PAL D_800981C4 */

int _padIfSioRW(padPort *port) {
    D_80097590 = D_80097528(port);
    return _padSioRW(port, -2);
    *port->response_3c = 0;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8006AE54 (_padIfSioRW.c): SCUS_94236 C of src/_padIfSioRW.c (func_8006A40C_scus94236, twin score 0.800) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8006AE54 (_padIfSioRW.c): PAL C of src/_padIfSioRW.c (0.882 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
/* US func_8006A40C (libpad/padif.c) plus the PAL `*response = 0` store. */
#include "padlocal.h"
extern int D_80097590;                  /* PAL D_8009822C at its USA address */
extern int (*D_80097528)(padPort*);     /* PAL D_800981C4 */

int _padIfSioRW(padPort *port) {
    D_80097590 = D_80097528(port);
    return _padSioRW(port, -2);
    *port->response_3c = 0;
}
#else
/* US func_8006A40C (libpad/padif.c) plus the PAL `*response = 0` store. */
#include "padlocal.h"

int _padIfSioRW(padPort *port) {
    D_8009822C = D_800981C4(port);
    *port->response_3c = 0;
    return _padSioRW(port, -2);
}
#endif
