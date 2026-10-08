// FUNC 8011a148 432 X000
// MATCHING 8011a148 432
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
extern OX *FUN_80018448(void);
extern void FUN_8011a2f8(OX *, int);
extern unsigned char DAT_8009c966;
extern void *PTR_DAT_8013b1b8[];
extern void *PTR_DAT_8013b1b4;

void FUN_8011a148(int x, int y, int z, unsigned char sub)
{
    OX *o;
    OX *p;
    void *a;

    if (DAT_8009c966 < 3) {
        o = FUN_80018448();
        if (o != 0) {
            o->active = 1;
            o->type = 9;
            o->subtype = sub;
            o->pos.x = x << 16;
            if (sub == 0) {
                o->pos.y = (y << 16) - 0x100000;
                o->pos.z = (z << 16) - 0x680000;
            } else {
                o->pos.y = y << 16;
                o->pos.z = z << 16;
            }
            o->b1d = -1;
            o->pos30 = o->pos;
            a = PTR_DAT_8013b1b8[0];
            o->c0.x = 0x28;
            o->c0.y = 4;
            o->c0.z = 0x80;
            o->d90 = o;
            o->wba = 0;
            o->wbc = 0;
            o->wbe = 0;
            o->cc = 0x80;
            o->anim = a;
            o->c6.x = *(volatile short *)&o->c0.x;
            o->c6.y = *(volatile short *)&o->c0.y;
            o->c6.z = *(volatile short *)&o->c0.z;
            p = FUN_80018448();
            if (p != 0) {
                p->active = 1;
                p->type = 9;
                p->subtype = sub + 1;
                p->pos = o->pos;
                p->b1d = -1;
                p->pos30 = p->pos;
                p->anim = PTR_DAT_8013b1b4;
                o->d94 = p;
                p->d90 = o;
            }
            FUN_8011a2f8(o, 4);
        }
    }
}
