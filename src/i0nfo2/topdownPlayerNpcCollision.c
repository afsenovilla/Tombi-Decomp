// FUNC 800edeec 368 I0NFO2
// MATCHING 800edeec 368
#include "TOBJ.H"
extern unsigned char D_1F8001A5;
extern TObj **D_1F80021C;
extern short D_1F80019E, D_1F800246;
extern void topdownResolveNpcContact(unsigned char *s, TObj *o);
extern unsigned char D_8009C93A, D_8009C942[];
extern unsigned char D_800A6038[];
extern TObj *D_800A605C;
extern signed char D_800A60A1;
extern unsigned short D_800A60A4[], D_800A60A6[], D_800A60A8[], D_800A60AA[];
extern unsigned short D_800FEBDC[][4];

void topdownPlayerNpcCollision(void)
{
    unsigned short *p;
    TObj **list;
    TObj *o;
    unsigned char *s;
    if (D_1F8001A5 == 0) {
        p = D_800FEBDC[*(short *)&D_800A605C->type];
        D_800A60A4[0] = *p++;
        D_800A60A6[0] = *p++;
        D_800A60A8[0] = *p;
        D_800A60AA[0] = p[1];
        s = D_800A6038;
        if (D_8009C942[0] == 0) {
        switch (*s) {
        case 1: case 2: case 3:
        if (D_8009C93A != 0) {
            list = D_1F80021C;
            D_800A60A1 = 0;
            for (D_1F80019E = D_1F800246; D_1F80019E != 0; ) {
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
}
