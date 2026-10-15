// FUNC 800e8b08 396 G0OVER
// MATCHING 800e8b08 396
#include "TOBJ.H"
typedef struct { TObj o; char pad[0x14]; } GObj; /* 0xD4 bytes */
typedef struct { char p[0x4e]; unsigned short w4e; } Task;
extern GObj D_800A6610[];
extern char LOAD_BUFFER_ALT[];
extern Task *DAT_1f8001d4;
extern short DAT_1f8001f4;
extern int DAT_1f800164;
extern char *DAT_1f800334;
extern void func_800E8044(void);
extern void func_800E8194(void);
extern void func_800E8404(TObj *o, int hold);
extern void func_800E87A0(void);

void func_800E8B08(void)
{
    short i;
    TObj *o;

    switch (DAT_1f8001d4->w4e) {
    case 0:
        func_800E8044();
        func_800E8194();
        {
            int *tbl;
            tbl = (int *)DAT_1f800334;
            i = 0;
            tbl++;
            do {
                o = &D_800A6610[i].o;
                o->type = i;
                *(int *)&o->b04 = 0;
                o->da0 = (int)(DAT_1f800334 + tbl[i]);
                i++;
            } while (i < 8);
        }
        DAT_1f8001d4->w4e++;
        break;
    case 1:
        {
            short k = 0;
            do {
                func_800E8404(&D_800A6610[k].o, 1);
                k++;
            } while (k < 8);
        }
        DAT_1f800164 = (DAT_1f8001f4 * 0xc000 + (int)LOAD_BUFFER_ALT) & 0xffffff;
        func_800E87A0();
        break;
    }
}
