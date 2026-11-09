// FUNC 8006f07c 48 MAIN0
// MATCHING 8006f07c 48
// VERSION SLPS_01144 slps01144 80058660 52
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *_interruptReg;
extern int _interruptMasks[];

int func_80058660(int a)
{
    _interruptReg->f4 |= _interruptMasks[a &= 0xffff];
    return a < 3;
}
#endif
#else
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern S8006F07C *_interruptReg;
extern int _interruptMasks[];

int StartRCnt(int a)
{
    _interruptReg->f4 |= _interruptMasks[a &= 0xffff];
    return a < 3;
}
#endif
