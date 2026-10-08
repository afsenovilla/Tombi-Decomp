// FUNC 8003e4ec 996 MAIN0
// wip: logica OK (stores por offset crudo => recargas de globales como el juego); falla asignacion de regs (o en a1, flag en s4) y sll v1 en las divisiones
#include "TOBJ.H"
#include "raw7.h"
extern char *FUN_80018568(void);
extern unsigned char DAT_8007b368[];
extern unsigned char DAT_8007b084[];
extern unsigned char *DAT_8007b14c[];
extern unsigned short D_1f8001c8;
extern Fix16 *DAT_800a6078;
extern Fix16 DAT_800a604c;
extern Fix16 *DAT_800a607c;

void ObjSpawn(int type, short sub, unsigned char flag, Fix16 *pos, short vx, short vy)
{
    char *o = FUN_80018568();
    unsigned char c;

    if (o == 0)
        return;
    c = DAT_8007b368[type];
    U8(o, 0xc) = flag | 0x80;
    U8(o, 2) = type;
    U8(o, 3) = sub;
    U8(o, 0) = c;
    c = DAT_8007b14c[DAT_8007b084[sub]][3];
    U16(o, 0x2e) = 0;
    U8(o, 0xf) = c;
    *(int *)PTR(o, 0x40) = pos[0].p.whole << 16;
    S32(o, 0x14) = pos[1].p.whole << 16;
    *(int *)PTR(o, 0x44) = pos[2].p.whole << 16;
    S16(o, 0x80) = vx;
    S16(o, 0x82) = vy;
    switch (type) {
    case 5:
        if (!(D_1f8001c8 & 1)) {
            S16(o, 0x80) = ((DAT_800a6078->p.whole - pos[0].p.whole) << 8) / 60;
            S16(o, 0x82) = ((DAT_800a604c.p.whole - pos[1].p.whole) << 8) / 60;
            S32(o, 0x30) = pos[0].p.whole << 16;
            S32(o, 0x34) = pos[1].p.whole << 16;
            S32(o, 0x38) = pos[2].p.whole << 16;
        } else {
            S16(o, 0x80) = ((DAT_800a6078->p.whole - pos[2].p.whole) << 8) / 60;
            S16(o, 0x82) = ((DAT_800a604c.p.whole - pos[1].p.whole) << 8) / 60;
            S32(o, 0x38) = pos[0].p.whole << 16;
            S32(o, 0x34) = pos[1].p.whole << 16;
            S32(o, 0x30) = pos[2].p.whole << 16;
        }
        break;
    case 8:
        if (!(D_1f8001c8 & 1)) {
            S16(o, 0x80) = ((pos[0].p.whole - DAT_800a6078->p.whole) << 8) / 60;
            S16(o, 0x82) = ((pos[1].p.whole - DAT_800a604c.p.whole) << 8) / 60;
            S32(o, 0x30) = DAT_800a6078->raw;
            S32(o, 0x34) = DAT_800a604c.raw;
            S32(o, 0x38) = DAT_800a607c->raw;
        } else {
            S16(o, 0x80) = ((pos[2].p.whole - DAT_800a6078->p.whole) << 8) / 60;
            S16(o, 0x82) = ((pos[1].p.whole - DAT_800a604c.p.whole) << 8) / 60;
            S32(o, 0x38) = DAT_800a6078->raw;
            S32(o, 0x34) = DAT_800a604c.raw;
            S32(o, 0x30) = DAT_800a607c->raw;
        }
        break;
    }
}
