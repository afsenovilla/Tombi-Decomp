// FUNC 8003e4ec 996 MAIN0
// MATCHING 8003e4ec 996
// VERSION SLPM_80165 slpm80165 8003B3B4 980
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003E4EC (ObjSpawn.c): animation set byte from the 20-byte item table.
#include "TOBJ.H"
extern TObj *allocObjectLayer5(void);
extern unsigned char DAT_8007b368[];
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];
extern unsigned short D_1f8001c8;
extern Fix16 *PLAYER_H[];
extern unsigned char D_8006DDEF[];
extern Fix16 PLAYER_Y_FIX[];
extern Fix16 *PLAYER_D[];

void ObjSpawn(int type, short sub, short flag, Fix16 *pos, short vx, short vy)
{
    TObj *o = allocObjectLayer5();
    unsigned char c;
    int d;

    if (o == 0)
        return;
    c = DAT_8007b368[type];
    o->type = type;
    o->subtype = sub;
    o->active = c;
    o->b0c = flag | 0x80;
    { unsigned char c2 = D_8006DDEF[sub * 20];
    o->animFrame = 0;
    o->b0f = c2; }
    o->h->raw = pos[0].p.whole << 16;
    o->y.raw = pos[1].p.whole << 16;
    o->d->raw = pos[2].p.whole << 16;
    o->velH = vx;
    o->velV = vy;
    switch (type) {
    case 5:
        if (!(D_1f8001c8 & 1)) {
            d = (PLAYER_H[0]->p.whole - pos[0].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (PLAYER_Y_FIX[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d38 = pos[2].p.whole << 16;
        } else {
            d = (PLAYER_H[0]->p.whole - pos[2].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (PLAYER_Y_FIX[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d30 = pos[2].p.whole << 16;
        }
        break;
    case 8:
        if (!(D_1f8001c8 & 1)) {
            d = (pos[0].p.whole - PLAYER_H[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - PLAYER_Y_FIX[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = PLAYER_H[0]->raw;
            o->d34 = PLAYER_Y_FIX[0].raw;
            o->d38 = PLAYER_D[0]->raw;
        } else {
            d = (pos[2].p.whole - PLAYER_H[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - PLAYER_Y_FIX[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = PLAYER_H[0]->raw;
            o->d34 = PLAYER_Y_FIX[0].raw;
            o->d30 = PLAYER_D[0]->raw;
        }
        break;
    }
}
#else
#include "TOBJ.H"
extern TObj *allocObjectLayer5(void);
extern unsigned char DAT_8007b368[];
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];
extern unsigned short D_1f8001c8;
extern Fix16 *PLAYER_H[];
extern Fix16 PLAYER_Y_FIX[];
extern Fix16 *PLAYER_D[];

void ObjSpawn(int type, short sub, short flag, Fix16 *pos, short vx, short vy)
{
    TObj *o = allocObjectLayer5();
    unsigned char c;
    int d;

    if (o == 0)
        return;
    c = DAT_8007b368[type];
    o->type = type;
    o->subtype = sub;
    o->active = c;
    o->b0c = flag | 0x80;
    { unsigned char c2 = DAT_8007b14c[DAT_8007b084[sub]][3];
    o->animFrame = 0;
    o->b0f = c2; }
    o->h->raw = pos[0].p.whole << 16;
    o->y.raw = pos[1].p.whole << 16;
    o->d->raw = pos[2].p.whole << 16;
    o->velH = vx;
    o->velV = vy;
    switch (type) {
    case 5:
        if (!(D_1f8001c8 & 1)) {
            d = (PLAYER_H[0]->p.whole - pos[0].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (PLAYER_Y_FIX[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d38 = pos[2].p.whole << 16;
        } else {
            d = (PLAYER_H[0]->p.whole - pos[2].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (PLAYER_Y_FIX[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d30 = pos[2].p.whole << 16;
        }
        break;
    case 8:
        if (!(D_1f8001c8 & 1)) {
            d = (pos[0].p.whole - PLAYER_H[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - PLAYER_Y_FIX[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = PLAYER_H[0]->raw;
            o->d34 = PLAYER_Y_FIX[0].raw;
            o->d38 = PLAYER_D[0]->raw;
        } else {
            d = (pos[2].p.whole - PLAYER_H[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - PLAYER_Y_FIX[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = PLAYER_H[0]->raw;
            o->d34 = PLAYER_Y_FIX[0].raw;
            o->d30 = PLAYER_D[0]->raw;
        }
        break;
    }
}
#endif
