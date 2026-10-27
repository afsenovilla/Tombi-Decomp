// FUNC 80059464 128 MAIN0
// MATCHING 80059464 128
typedef struct DR { int tag; int code[2]; } DR;
extern DR *NEXT_PRIM;
extern int CURRENT_OT;
extern void SetDrawMode(DR *p, int dfe, int dtd, int tpage, void *tw);
extern void AddPrim(int *ot, DR *p);

void AddDrawMode(short tpage, int n)
{
    DR *p = NEXT_PRIM;
    SetDrawMode(p, 0, 0, tpage, 0);
    AddPrim((int *)(CURRENT_OT + n * 4), p);
    NEXT_PRIM = NEXT_PRIM + 1;
}
