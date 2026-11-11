// FUNC 80069610 400 MAIN0
// MATCHING 80069610 400
// VERSION SCUS_94236 scus94236 80069058 364
// VERSION BETA_980203 beta980203 800677AC 364
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 80069610 (_padVsyncHandler.c): PAL C of src/_padVsyncHandler.c (0.942 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
/* vsync handler of the pad driver (US func_80069058, libpad/padmain.c): polls both ports. Library code: assembled with GNU as, see padlocal.h. */
typedef struct { unsigned char pad[0xf0]; } E;
typedef struct { unsigned char pad[0xa]; unsigned short a; unsigned short c; unsigned short e; } H;
extern H *PAD_SIO;
extern int D_80097578;
extern int _padChanStart;
extern int _padChanStop;
extern int D_8009B2F0[];
extern int D_8009B2F4[];
extern int _padIntExec;
extern E *_padInfoDir;
extern int _padSioState;
extern int _padSioChan;
extern int _padTotalCurr;
extern void (*_padFuncNextPort)(int);
int _padInitSioMode(E *e);
void padCommandStep(E *e);
int _padVsyncHandler(void)
{
    int t;
    if (PAD_SIO->a & 2) {
    }
    t = _padChanStart;
    D_80097578 = 1;
    if (t != 0 && D_8009B2F0[0] < 150) D_8009B2F0[0]++;
    if (_padChanStop == 0 && D_8009B2F4[0] < 150) D_8009B2F4[0]++;
    if (_padIntExec != 0 && _padChanStart <= _padChanStop) {
        _padSioState = 0;
        _padSioChan = _padChanStart;
        if (_padInitSioMode(&_padInfoDir[_padChanStart]) == 0) _padFuncNextPort(0xffff);
        _padTotalCurr = 0;
        while (_padSioChan <= _padChanStop) {
            padCommandStep(&_padInfoDir[_padSioChan]);
        }
        PAD_SIO->e = 0x88;
    }
    return 0;
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 80069610 (_padVsyncHandler.c): SCUS_94236 C of src/_padVsyncHandler.c (func_80069058_scus94236, twin score 0.800) [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 80069610 (_padVsyncHandler.c): PAL C of src/_padVsyncHandler.c (0.942 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
/* vsync handler of the pad driver (US func_80069058, libpad/padmain.c): polls both ports. Library code: assembled with GNU as, see padlocal.h. */
typedef struct { unsigned char pad[0xf0]; } E;
typedef struct { unsigned char pad[0xa]; unsigned short a; unsigned short c; unsigned short e; } H;
extern H *PAD_SIO;
extern int D_80097578;
extern int _padChanStart;
extern int _padChanStop;
extern int D_8009B2F0[];
extern int D_8009B2F4[];
extern int _padIntExec;
extern E *_padInfoDir;
extern int _padSioState;
extern int _padSioChan;
extern int _padTotalCurr;
extern void (*_padFuncNextPort)(int);
int _padInitSioMode(E *e);
void padCommandStep(E *e);
int _padVsyncHandler(void)
{
    int t;
    if (PAD_SIO->a & 2) {
    }
    t = _padChanStart;
    D_80097578 = 1;
    if (t != 0 && D_8009B2F0[0] < 150) D_8009B2F0[0]++;
    if (_padChanStop == 0 && D_8009B2F4[0] < 150) D_8009B2F4[0]++;
    if (_padIntExec != 0 && _padChanStart <= _padChanStop) {
        _padSioState = 0;
        _padSioChan = _padChanStart;
        if (_padInitSioMode(&_padInfoDir[_padChanStart]) == 0) _padFuncNextPort(0xffff);
        _padTotalCurr = 0;
        while (_padSioChan <= _padChanStop) {
            padCommandStep(&_padInfoDir[_padSioChan]);
        }
        PAD_SIO->e = 0x88;
    }
    return 0;
}
#else
/* vsync handler of the pad driver (US func_80069058, libpad/padmain.c): polls both ports. Library code: assembled with GNU as, see padlocal.h. */
typedef struct { unsigned char pad[0xf0]; } E;
typedef struct { unsigned char pad[0xa]; unsigned short a; unsigned short c; unsigned short e; } H;
extern H *PAD_SIO;
extern int D_80098214;
extern int _padChanStart;
extern int _padChanStop;
extern int D_8009BF88[];
extern int D_8009BF8C[];
extern int _padIntExec;
extern E *_padInfoDir;
extern int _padSioState;
extern int _padSioChan;
extern int _padTotalCurr;
extern void (*_padFuncNextPort)(int);
int _padInitSioMode(E *e);
void padCommandStep(E *e);
int _padVsyncHandler(void)
{
    int t;
    if (PAD_SIO->a & 2) {
        PAD_SIO->a = 0;
        return 0;
    }
    t = _padChanStart;
    D_80098214 = 1;
    if (t != 0 && D_8009BF88[0] < 150) D_8009BF88[0]++;
    if (_padChanStop == 0 && D_8009BF8C[0] < 150) D_8009BF8C[0]++;
    if (_padIntExec != 0 && _padChanStart <= _padChanStop) {
        _padSioState = 0;
        _padSioChan = _padChanStart;
        if (_padInitSioMode(&_padInfoDir[_padChanStart]) == 0) _padFuncNextPort(0xffff);
        _padTotalCurr = 0;
        while (_padSioChan <= _padChanStop) {
            padCommandStep(&_padInfoDir[_padSioChan]);
        }
        PAD_SIO->e = 0x88;
    }
    return 0;
}
#endif
