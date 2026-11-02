// near-twin of PAL CD_initintr (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80064B24 80
#if defined(VERSION_SLPS_01144)
extern int D_8009F22C, D_8009DDB0;
extern void ResetCallback(void);
extern void func_80064BEC(int, void *);
extern void func_80059C6C(int);
extern void callback(void);

/* older libcd CD_initintr: stores its argument, CD_DEBUG = -1, InterruptCallback(0x20000001, callback), then func_80059C6C(0) */
void func_80064B24(int a) {
    D_8009F22C = a;
    D_8009DDB0 = -1;
    ResetCallback();
    func_80064BEC(0x20000001, callback);
    func_80059C6C(0);
}
#endif
