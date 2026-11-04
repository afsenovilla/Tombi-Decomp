// VERSION SLPS_01144 slps01144 80024860 220
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern int func_80022B20(int);
extern int func_80022CF0(int, int);
extern int func_80022F34(int, int);
extern int func_8002311C(int, int);
extern int func_80024300(int, int, int);
extern int func_8002457C(int, int, int);
extern int func_8002374C(int, int);
extern int func_80023914(int, int);
extern int func_80023B64(int, int);
extern int func_80023D04(int, int);
extern int func_80023F0C(int, int);
extern int func_800240CC(int, int);
void func_80024860_slps01144(int a, int b, int c, unsigned char d)
{
    int v = func_80022B20(a);
    v = func_80022CF0(v, b);
    v = func_80022F34(v, b);
    v = func_8002311C(v, b);
    if (d) {
        v = func_80024300(c, v, b);
        v = func_8002457C(c, v, b);
        v = func_8002374C(v, b);
        func_80023914(v, b);
    } else {
        v = func_80023B64(v, b);
        v = func_80023D04(v, b);
        v = func_80023F0C(v, b);
        func_800240CC(v, b);
    }
}
#endif
