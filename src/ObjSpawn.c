// FUNC 8003e4ec 996 MAIN0
// MATCHING 8003e4ec 996
#include "TOBJ.H"
extern TObj *FUN_80018568(void);
extern unsigned char DAT_8007b368[];
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];
extern unsigned short D_1f8001c8;
extern Fix16 *DAT_800a6078[];
extern Fix16 DAT_800a604c[];
extern Fix16 *DAT_800a607c[];

void ObjSpawn(int type, short sub, short flag, Fix16 *pos, short vx, short vy)
{
    TObj *o = FUN_80018568();
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
            d = (DAT_800a6078[0]->p.whole - pos[0].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (DAT_800a604c[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d38 = pos[2].p.whole << 16;
        } else {
            d = (DAT_800a6078[0]->p.whole - pos[2].p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (DAT_800a604c[0].p.whole - pos[1].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = pos[0].p.whole << 16;
            o->d34 = pos[1].p.whole << 16;
            o->d30 = pos[2].p.whole << 16;
        }
        break;
    case 8:
        if (!(D_1f8001c8 & 1)) {
            d = (pos[0].p.whole - DAT_800a6078[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - DAT_800a604c[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d30 = DAT_800a6078[0]->raw;
            o->d34 = DAT_800a604c[0].raw;
            o->d38 = DAT_800a607c[0]->raw;
        } else {
            d = (pos[2].p.whole - DAT_800a6078[0]->p.whole) << 8;
            d /= 60;
            o->velH = d;
            d = (pos[1].p.whole - DAT_800a604c[0].p.whole) << 8;
            d /= 60;
            o->velV = d;
            o->d38 = DAT_800a6078[0]->raw;
            o->d34 = DAT_800a604c[0].raw;
            o->d30 = DAT_800a607c[0]->raw;
        }
        break;
    }
}
