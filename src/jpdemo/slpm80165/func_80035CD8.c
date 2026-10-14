// JPDEMO slpm80165 80035CD8 324
// Demo version of PAL main0 80037C80 (uploadSpriteFrameToVram.c): only types 0 and 1.
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p0[2]; unsigned char type; char p3[0x24 - 3]; unsigned short *anim; } O;
extern int WPP_DATA, DAT_1f800350;
extern unsigned short DAT_1f8001f4;
extern short DAT_1f8001f4s;
extern char *DAT_8009c330;
extern void unpackSpriteFrame(int, int, char *, int);
extern void loadTIM(char *, short, int, int, int);
extern void ClearImage(RECT *, int, int, int);

void uploadSpriteFrameToVram(O *o)
{
    RECT r;
    switch (o->type) {
    case 0:
        unpackSpriteFrame(WPP_DATA, *o->anim, (char *)0x801fb000, 0);
        r.y = 2;
        r.w = 0x10;
        r.h = 0x50;
        r.x = DAT_1f8001f4 << 4;
        ClearImage(&r, 0, 0, 0);
        break;
    case 1:
        unpackSpriteFrame(DAT_1f800350, *o->anim, (char *)0x801fb000, 0);
        r.y = 2;
        r.w = 0xe;
        r.h = 0x3a;
        r.x = DAT_1f8001f4 << 4;
        ClearImage(&r, 0, 0, 0);
        break;
    }
    loadTIM((char *)0x801fb000, DAT_1f8001f4 * 16 + 1, 4, 0x80, 0x1e0);
    *(unsigned short *)(DAT_8009c330 + DAT_1f8001f4s * 2 + 0x28) = *o->anim;
}
