// FUNC 80059464 128 MAIN0
// MATCHING 80059464 128
typedef struct DR { int tag; int code[2]; } DR;
extern DR *DAT_1f800164;
extern int DAT_1f8001e0;
extern void SetDrawMode(DR *p, int dfe, int dtd, int tpage, void *tw);
extern void AddPrim(int *ot, DR *p);

void AddDrawMode(short tpage, int n)
{
    DR *p = DAT_1f800164;
    SetDrawMode(p, 0, 0, tpage, 0);
    AddPrim((int *)(DAT_1f8001e0 + n * 4), p);
    DAT_1f800164 = DAT_1f800164 + 1;
}
