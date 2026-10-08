// FUNC 800f2a30 360 X000
// MATCHING 800f2a30 360
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; char r[0x2e - 0x22]; short w2e; } G;
extern G *D_8009C330;
extern TObj *D_800A611C;
extern TObj *D_8009D2E8;
extern unsigned char D_801152E8[];
extern unsigned char D_8009CF06[], D_8009CE61;
void SfxPlay3(int, int);
void FUN_8005a9a4(int, int);
void func_800F2A30(TObj *o)
{
    if (*((unsigned char *)o + 0xc9)) D_8009C330->w2e = 0xff;
    o->ba7 = 0;
    D_8009C330->b8 = 0;
    SfxPlay3(0x1c, 0x7f);
    o->b9c = 0;
    if (*(unsigned char *)&o->wac >= 2) {
        D_8009D2E8 = D_800A611C;
        D_8009D2E8->b04 = 2;
        D_8009D2E8->step = 2;
        D_8009D2E8->state = 0;
    }
    *(unsigned char *)&o->wac = 0;
    if (o->velX > 0) {
        o->velX -= 0xd0;
        if (o->velX < 0) o->velX = 0;
    } else {
        o->velX += 0xd0;
        if (o->velX > 0) o->velX = 0;
    }
    o->wb2 = o->velX;
    o->velX = 0;
    o->velY = 0;
    o->d8c = D_801152E8[o->wb0];
    o->ba5 = 0;
    D_8009C330->w20 = 0;
    if (D_8009CF06[0] && o->subtype != 1) {
        o->subtype = 1;
        if (D_8009CE61 != 0xff) FUN_8005a9a4(0xbd, 1);
    }
}
