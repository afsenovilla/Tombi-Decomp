// FUNC 800e897c 396 G0OVER
// MATCHING 800e897c 396
#include "TOBJ.H"
typedef struct { TObj o; char pad[0x14]; } GObj; /* 0xD4 bytes */
typedef struct { char p[0x4e]; unsigned short w4e; } Task;
extern GObj OBJECT_POOL[];
extern char LOAD_BUFFER_ALT[];
extern Task *TASK_C;
extern short FRAME_BUFFER_INDEX;
extern int NEXT_PRIM;
extern char *DAT_1f800334;
extern void gameOverSetupCamera(void);
extern void gameOverSetupLighting(void);
extern void gameOverLetterStep(TObj *o, int hold);
extern void gameOverDrawLetters(void);

void gameOverScreenTask(void)
{
    short i;
    TObj *o;

    switch (TASK_C->w4e) {
    case 0:
        gameOverSetupCamera();
        gameOverSetupLighting();
        {
            int *tbl;
            tbl = (int *)DAT_1f800334;
            i = 0;
            tbl++;
            do {
                o = &OBJECT_POOL[i].o;
                o->type = i;
                *(int *)&o->b04 = 0;
                o->da0 = (int)(DAT_1f800334 + tbl[i]);
                i++;
            } while (i < 8);
        }
        TASK_C->w4e++;
        break;
    case 1:
        {
            short k = 0;
            do {
                gameOverLetterStep(&OBJECT_POOL[k].o, 0);
                k++;
            } while (k < 8);
        }
        NEXT_PRIM = (FRAME_BUFFER_INDEX * 0xc000 + (int)LOAD_BUFFER_ALT) & 0xffffff;
        gameOverDrawLetters();
        break;
    }
}
