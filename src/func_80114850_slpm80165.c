// VERSION SLPM_80165 x00 80114850 400
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_80118A1C_slps01144 (src/x009/updateScenerySprite.c): timer 7, the CLUT word and
// the flag at +0x0D set only for a sprite with a CLUT row, list at 0x1F8002E4, cull margin 64.
#include "spad.h"
#include "TOBJ.H"
extern unsigned short SCENERY_SPRITE_CLUT_Y[];
extern unsigned short SCENERY_SPRITE_CLUT_X[];
extern void **D_8012ABA0[];
extern int cullObjectToDrawListMargin(TObj *, int);
extern void ObjFree(TObj *);

void updateScenerySprite(TObj *o)
{
    unsigned int v;
    char pad[0];

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 7;
        o->b0d = 0;
        if ((short)SCENERY_SPRITE_CLUT_Y[o->subtype] != 0) {
            o->b0d = 1;
            o->w08 = ((SCENERY_SPRITE_CLUT_Y[o->subtype] + o->b0c) << 6) | ((SCENERY_SPRITE_CLUT_X[o->subtype] >> 4) & 0x3f);
        }
        o->d3c = ((int*)&SPAD.i2e4)[0];
        o->anim = *D_8012ABA0[o->subtype];
        v = *(unsigned char *)&o->box0 >> 4;
        if (v < 12)
            o->d64 = (v << 10) + 0x1000;
        else
            o->d64 = 0x1000 - ((v - 11) << 9);
        v = *(unsigned char *)&o->box0 & 0xf;
        if (v < 8)
            o->d8c = v << 2;
        else
            o->d8c = (unsigned char)(-((int)(v - 7) << 5) / 8);
        break;
    case 1:
        cullObjectToDrawListMargin(o, 64);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
