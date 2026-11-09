// VERSION SLPS_01144 info3 800ED8CC 352
// JP version of PAL i0nfo3 800ec26c (tunnelPlayerNpcCollision.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern TObj **CAT2_OBJECT_LIST;
extern short LIST_COUNTER, CAT2_OBJECT_COUNT;
extern void topdownResolveNpcContact(unsigned char *s, TObj *o);
extern unsigned char D_8009E4E2, D_8009E4EA[];
extern unsigned char D_800A7B78[];
extern TObj *D_800A7B9C;
extern signed char D_800A7BE1;
extern unsigned short D_800A7BE4[], D_800A7BE6[], D_800A7BE8[], D_800A7BEA[];
extern unsigned short D_800F0294[][4];

void func_800ED8CC(void)
{
    unsigned short *p;
    TObj **list;
    TObj *o;
    unsigned char *s;
    p = D_800F0294[*(short *)&D_800A7B9C->type];
    D_800A7BE4[0] = *p++;
    D_800A7BE6[0] = *p++;
    D_800A7BE8[0] = *p;
    D_800A7BEA[0] = p[1];
    s = D_800A7B78;
    if (D_8009E4EA[0] == 0) {
    switch (*s) {
    case 1: case 2: case 3:
    if (D_8009E4E2 != 0) {
        list = CAT2_OBJECT_LIST;
        D_800A7BE1 = 0;
        for (LIST_COUNTER = CAT2_OBJECT_COUNT; LIST_COUNTER != 0; ) {
            o = *list;
            LIST_COUNTER--;
            list++;
            if (o->active & 1) {
                switch (o->type) {
                case 0x44:
                case 0x45:
                case 0x4C:
                    topdownResolveNpcContact(s, o);
                }
            }
        }
    }
    }
    }
}
#endif
