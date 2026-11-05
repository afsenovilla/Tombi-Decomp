// VERSION SLPM_80165 x00 80112C08 448
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/spawnA00Tornado.c (func_80119278) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
typedef struct { int x, y, z; } V3;
typedef struct { short x, y, z; } S3;
typedef struct {
    unsigned char active, visible, type, subtype;
    unsigned char _p04[0xc];
    V3 pos;
    unsigned char _p1c;
    signed char b1d;
    unsigned char _p1e[6];
    void *anim;
    unsigned char _p28[8];
    V3 pos30;
    unsigned char _p3c[0x54];
    void *d90;
    void *d94;
    unsigned char _p98[0x22];
    short wba;
    short wbc;
    short wbe;
    S3 c0;
    S3 c6;
    short cc;
} OX;
extern OX *func_80017A1C_slpm80165(void);
extern void func_80112DC8_slpm80165(OX *, int);
extern unsigned int D_8009E52C_slpm80165;
extern unsigned char GAME_AREA00_INTRO_PHASE;
extern void *D_80138E28_slpm80165[];
extern void *D_80138E24_slpm80165[];

void func_80112C08_slpm80165(int x, int y, int z, unsigned char sub)
{
    OX *o;
    OX *p;
    void *a;

    int m;
    register int k asm("$7");
    register unsigned short cx asm("$4");
    register unsigned short cy asm("$5");
    register unsigned short cz asm("$6");
    if (!(D_8009E52C_slpm80165 & 1)) return;
    if (GAME_AREA00_INTRO_PHASE < 3) {
        o = func_80017A1C_slpm80165();
        if (o != 0) {
            o->active = 1;
            o->type = 9;
            o->subtype = sub;
            o->pos.x = x << 16;
            o->pos.y = y << 16;
            if (sub == 0) {
                o->pos.z = (z << 16) - 0x800000;
            } else {
                o->pos.z = z << 16;
            }
            m = -1;
            o->b1d = m;
            o->pos30 = o->pos;
            a = D_80138E28_slpm80165[0];
            k = 6;
            asm volatile("" : "=r"(k) : "0"(k));
            o->c0.x = 0x28;
            cx = *(volatile short *)&o->c0.x;
            o->c0.y = 4;
            cy = *(volatile short *)&o->c0.y;
            o->c0.z = 0x80;
            cz = *(volatile short *)&o->c0.z;
            o->d90 = o;
            o->wba = k;
            o->wbc = 2;
            o->wbe = k;
            o->cc = 0x80;
            o->anim = a;
            o->c6.x = cx;
            o->c6.y = cy;
            o->c6.z = cz;
            p = func_80017A1C_slpm80165();
            if (p != 0) {
                p->active = 1;
                p->type = 9;
                p->subtype = sub + 1;
                p->pos = o->pos;
                p->b1d = m;
                p->pos30 = p->pos;
                p->anim = D_80138E24_slpm80165[0];
                o->d94 = p;
                p->d90 = o;
            }
            func_80112DC8_slpm80165(o, 4);
        }
    }
}
#endif
