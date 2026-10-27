// FUNC 800ec26c 368 I0NFO3
// MATCHING 800ec26c 368
#include "TOBJ.H"
extern unsigned char D_1F8001A5;
extern TObj **CAT2_OBJECT_LIST;
extern short D_1F80019E, CAT2_OBJECT_COUNT;
extern void tunnelResolveNpcContact(unsigned char *s, TObj *o);
extern unsigned char D_8009C93A, D_8009C942[];
extern unsigned char PLAYER[];
extern TObj *PLAYER_ANIM;
extern signed char D_800A60A1;
extern unsigned short D_800A60A4[], D_800A60A6[], D_800A60A8[], D_800A60AA[];
extern unsigned short D_800EECC0[][4];

void tunnelPlayerNpcCollision(void)
{
    unsigned short *p;
    TObj **list;
    TObj *o;
    unsigned char *s;
    if (D_1F8001A5 == 0) {
        p = D_800EECC0[*(short *)&PLAYER_ANIM->type];
        D_800A60A4[0] = *p++;
        D_800A60A6[0] = *p++;
        D_800A60A8[0] = *p;
        D_800A60AA[0] = p[1];
        s = PLAYER;
        if (D_8009C942[0] == 0) {
        switch (*s) {
        case 1: case 2: case 3:
        if (D_8009C93A != 0) {
            list = CAT2_OBJECT_LIST;
            D_800A60A1 = 0;
            for (D_1F80019E = CAT2_OBJECT_COUNT; D_1F80019E != 0; ) {
                o = *list;
                D_1F80019E--;
                list++;
                if (o->active & 1) {
                    switch (o->type) {
                    case 0x44:
                    case 0x45:
                    case 0x4C:
                        tunnelResolveNpcContact(s, o);
                    }
                }
            }
        }
        }
        }
    }
}
