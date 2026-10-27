// FUNC 80021150 356 MAIN0
// MATCHING 80021150 356
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
} PTAG;
typedef struct {
    PTAG tag;
    int code[2];
} DRM;
extern DRM *DEBUG_NEXT_PRIM;
extern PTAG *CURRENT_OT;
extern int GetGraphType(void);
extern void FUN_80021478(int a, int b, int c, int d, int e);
extern void drawUiSprite(short x, short y, short c);
extern void SetDrawMode(DRM *p, int dfe, int dtd, int tpage, void *tw);

void drawLoadingSprites(int c, short s)
{
    int i, j;
    DRM *p;
    PTAG *ot;
    if (GetGraphType() != 1)
        GetGraphType();
    FUN_80021478(0xc0, 0xa0, 0, 0, s);
    for (j = 0; j < 6; j++) {
        for (i = 0; i < 7; i++) {
            if ((i + j) & 1)
                drawUiSprite(i * 0x30, j * 0x30, 1 - c);
            else
                drawUiSprite(i * 0x30, j * 0x30, c);
        }
    }
    p = DEBUG_NEXT_PRIM;
    SetDrawMode(p, 0, 0, 0, 0);
    ot = CURRENT_OT;
    p->tag.addr = ot->addr;
    ot->addr = (unsigned)p;
    DEBUG_NEXT_PRIM++;
}
