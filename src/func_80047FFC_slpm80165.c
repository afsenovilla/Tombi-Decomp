// VERSION SLPM_80165 slpm80165 80047FFC 172
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of AddDrawMode (src/AddDrawMode.c): the addPrim macro instead of the AddPrim call.
typedef struct DR { int tag; int code[2]; } DR;
typedef struct { unsigned addr : 24; unsigned len : 8; } PTag;
extern DR *NEXT_PRIM;
extern int CURRENT_OT;
extern void SetDrawMode(DR *p, int dfe, int dtd, int tpage, void *tw);

void AddDrawMode(short tpage, int n)
{
    DR *p = NEXT_PRIM;
    SetDrawMode(p, 0, 0, tpage, 0);
    ((PTag *)p)->addr = ((PTag *)(CURRENT_OT + n * 4))->addr, ((PTag *)(CURRENT_OT + n * 4))->addr = (unsigned long)p;
    NEXT_PRIM = NEXT_PRIM + 1;
}
#endif
