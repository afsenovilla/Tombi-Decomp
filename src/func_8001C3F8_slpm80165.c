// VERSION SLPM_80165 slpm80165 8001C3F8 252
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: for entry n with D_8006B284[n] set, a message (func_8001C6E0) and func_8001CE14 with k, sound 0x29
// and a type 0x0D object for k == 0, sound 0x2B otherwise; then func_800266D0 with entry k of the n-th short table.
// Returns D_8009AC54[n].
#include "TOBJ.H"
extern unsigned char D_8006B284[], D_8006B364[], D_8009AC54[];
extern short *D_8006B354[];
extern void func_8001C6E0(int, int, int, int);
extern void func_8001CE14(int, int);
extern void func_8001E3D8(int);
extern TObj *ObjAlloc(void);
extern void func_800266D0(int);

unsigned char func_8001C3F8(int n, int k, int arg)
{
    if (D_8006B284[n]) {
        if (k == 0) {
            TObj *o;
            func_8001C6E0(n, 0, 0x3c, arg);
            func_8001CE14(n, 0);
            func_8001E3D8(0x29);
            o = ObjAlloc();
            if (o) {
                o->active = 1;
                o->type = 0xd;
                o->b0c = 0;
            }
        } else {
            func_8001C6E0(n, 1, 1, arg);
            func_8001CE14(n, 1);
            func_8001E3D8(0x2b);
        }
    }
    func_800266D0(D_8006B354[D_8006B364[n]][k]);
    return D_8009AC54[n];
}
#endif
