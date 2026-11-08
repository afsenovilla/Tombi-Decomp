// VERSION SLPS_01144 info2 800EF274 352
// JP version of PAL i0nfo2 800edeec (topdownPlayerNpcCollision.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern TObj **CAT2_OBJECT_LIST;
extern short D_1F80019E, CAT2_OBJECT_COUNT;
extern void topdownResolveNpcContact(unsigned char *s, TObj *o);
extern unsigned char D_8009E4E2, D_8009E4EA[];
extern unsigned char D_800A7B78[];
extern TObj *D_800A7B9C;
extern signed char D_800A7BE1;
extern unsigned short D_800A7BE4[], D_800A7BE6[], D_800A7BE8[], D_800A7BEA[];
extern unsigned short D_800FFD20[][4];

void func_800EF274(void)
{
    unsigned short *p;
    TObj **list;
    TObj *o;
    unsigned char *s;
    p = D_800FFD20[*(short *)&D_800A7B9C->type];
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
        for (D_1F80019E = CAT2_OBJECT_COUNT; D_1F80019E != 0; ) {
            o = *list;
            D_1F80019E--;
            list++;
            if (o->active & 1) {
                switch (o->type) {
                case 0x2D:
                case 0x2E:
                case 0x30:
                    topdownResolveNpcContact(s, o);
                }
            }
        }
    }
    }
    }
}
#endif
