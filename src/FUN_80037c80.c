// FUNC 80037c80 320 MAIN0
// MATCHING 80037c80 320
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p0[2]; unsigned char type; char p3[0x24 - 3]; unsigned short *anim; } O;
extern int DAT_1f800348, DAT_1f800350;
extern unsigned short DAT_1f8001f4;
extern short DAT_1f8001f4s;
extern char *DAT_8009c330;
extern void FUN_800371c0(int, int, char *, int);
extern void FUN_800174fc(char *, short, int, int, int);
extern void ClearImage(RECT *, int, int, int);

void FUN_80037c80(O *o)
{
    RECT r;
    switch (o->type) {
    case 0:
        FUN_800371c0(DAT_1f800348, *o->anim, (char *)0x801fb000, 0);
        r.y = 2;
        r.w = 0x10;
        r.h = 0x50;
        r.x = DAT_1f8001f4 << 4;
        ClearImage(&r, 0, 0, 0);
        break;
    case 1:
    case 2:
    case 3:
        FUN_800371c0(DAT_1f800350, *o->anim, (char *)0x801fb000, 0);
        r.y = 2;
        r.w = 0xe;
        r.h = 0x3a;
        r.x = DAT_1f8001f4 << 4;
        ClearImage(&r, 0, 0, 0);
        break;
    }
    FUN_800174fc((char *)0x801fb000, DAT_1f8001f4 * 16 + 1, 4, 0x80, 0x1e0);
    *(unsigned short *)(DAT_8009c330 + DAT_1f8001f4s * 2 + 0x28) = *o->anim;
}
