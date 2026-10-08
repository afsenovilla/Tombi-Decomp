// FUNC 80037c80 320 MAIN0
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p0[2]; unsigned char type; char p3[0x24 - 3]; unsigned short **anim; } O;
extern int DAT_1f800348, DAT_1f800350;
extern short DAT_1f8001f4;
extern short *DAT_8009c330;
extern char DAT_801fb000[];
extern void FUN_800371c0(int, int, char *, int);
extern void FUN_800174fc(char *, int, int, int, int);
extern void ClearImage(RECT *, int, int, int);

void FUN_80037c80(O *o)
{
    RECT r;
    int hh;
    if (o->type == 0) {
        FUN_800371c0(DAT_1f800348, **o->anim, DAT_801fb000, 0);
        r.y = 2;
        r.w = 0x10;
        hh = 0x50;
    } else {
        if (o->type > 3) goto skip;
        FUN_800371c0(DAT_1f800350, **o->anim, DAT_801fb000, 0);
        r.y = 2;
        r.w = 0xe;
        hh = 0x3a;
    }
    r.h = hh;
    r.x = DAT_1f8001f4 << 4;
    ClearImage(&r, 0, 0, 0);
    skip:
    FUN_800174fc(DAT_801fb000, DAT_1f8001f4 * 16 + 1, 4, 0x80, 0x1e0);
    *(unsigned short *)((char *)DAT_8009c330 + DAT_1f8001f4 * 2 + 0x28) = **o->anim;
}
