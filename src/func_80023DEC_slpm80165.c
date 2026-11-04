// VERSION SLPM_80165 slpm80165 80023DEC 220
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80024860_slps01144.c (func_80024860_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern int func_80022B20_slpm80165(int);
extern int func_80022CF0_slpm80165(int, int);
extern int func_80022F34_slpm80165(int, int);
extern int func_8002311C_slpm80165(int, int);
extern int func_80024300_slpm80165(int, int, int);
extern int func_8002457C_slpm80165(int, int, int);
extern int func_8002374C_slpm80165(int, int);
extern int func_80023914_slpm80165(int, int);
extern int func_80023B64_slpm80165(int, int);
extern int func_80023D04_slpm80165(int, int);
extern int func_80023F0C_slpm80165(int, int);
extern int func_800240CC_slpm80165(int, int);
void func_80023DEC_slpm80165(int a, int b, int c, unsigned char d)
{
    int v = func_80022B20_slpm80165(a);
    v = func_80022CF0_slpm80165(v, b);
    v = func_80022F34_slpm80165(v, b);
    v = func_8002311C_slpm80165(v, b);
    if (d) {
        v = func_80024300_slpm80165(c, v, b);
        v = func_8002457C_slpm80165(c, v, b);
        v = func_8002374C_slpm80165(v, b);
        func_80023914_slpm80165(v, b);
    } else {
        v = func_80023B64_slpm80165(v, b);
        v = func_80023D04_slpm80165(v, b);
        v = func_80023F0C_slpm80165(v, b);
        func_800240CC_slpm80165(v, b);
    }
}
#endif
