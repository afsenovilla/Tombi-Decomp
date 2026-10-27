// FUNC 800ec5e8 372 I0NFO
// MATCHING 800ec5e8 372
#include "TOBJ.H"
extern unsigned char D_1F8001A5;
extern TObj **D_1F80021C;
extern short D_1F80019E, D_1F800246;
extern void overheadResolveNpcContact(unsigned char *s, TObj *o);
extern unsigned char D_8009C93A, D_8009C942[];
extern unsigned char PLAYER[];
extern TObj *PLAYER_ANIM;
extern signed char D_800A60A1;
extern unsigned short D_800A60A4[], D_800A60A6[], D_800A60A8[], D_800A60AA[];
extern unsigned short D_800EE8C0[][4];

void overheadPlayerNpcCollision(void)
{
    unsigned short *p;
    TObj **list;
    TObj *o;
    unsigned char *s;
    if (D_1F8001A5 == 0) {
        p = D_800EE8C0[*(short *)&PLAYER_ANIM->type];
        D_800A60A4[0] = *p++;
        D_800A60A6[0] = *p++;
        D_800A60A8[0] = *p;
        D_800A60AA[0] = p[1];
        s = PLAYER;
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
                    case 0x19:
                    case 0x25:
                    case 0x26:
                        overheadResolveNpcContact(s, o);
                    }
                }
            }
        }
        }
        }
    }
}
