// FUNC 80057d04 248 MAIN0
// MATCHING 80057d04 248
extern char VIEW_CAMERA[];
extern int GAME;
extern int CURRENT_OT;
extern unsigned char AREA_MODEL_DRAW_LIST[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void FUN_80120280(int, int);
extern void drawModelUnlit(int, int);

void drawAreaModelList(void)
{
    char pad;
    int i;
    int *p;
    unsigned char *base;
    SetRotMatrix(VIEW_CAMERA);
    SetTransMatrix(VIEW_CAMERA);
    base = AREA_MODEL_DRAW_LIST;
    if (GAME == 0x60009) {
        i = 0;
        if (i < AREA_MODEL_DRAW_LIST[3]) {
            p = (int *)base;
            do {
                FUN_80120280(p[1], CURRENT_OT + 0x10);
                p++;
                i++;
            } while (i < base[3]);
        }
    } else {
        i = 0;
        if (i < AREA_MODEL_DRAW_LIST[3]) {
            p = (int *)base;
            do {
                drawModelUnlit(p[1], CURRENT_OT + 0x10);
                p++;
                i++;
            } while (i < base[3]);
        }
    }
}
