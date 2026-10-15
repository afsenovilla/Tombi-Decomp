// FUNC 800e8700 788 O0PTSUB0
// MATCHING 800e8700 788
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
extern S *TASK_C;
extern unsigned short DAT_1f8001fc;
extern short D_8009D4FC, D_8009D4FE;
extern volatile unsigned short D_8009D670;
extern unsigned short D_8009F7F2;
extern short D_8009E350, D_8009E352, D_8009E354, D_8009E356;
extern short D_8009F060, D_8009F062, D_8009F064, D_8009F066;
extern int D_800EB484, D_800EB488;
extern short D_800EB48C, D_800EB490;
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void updateSound(void);
extern void func_800E8C24(int dir);

int func_800E8700(void)
{
    int dir;
    int held;
    unsigned short b;

    switch (TASK_C->w4e) {
    case 0:
        D_800EB484 = 0;
        D_800EB488 = 0;
        D_800EB48C = D_8009D4FC;
        D_800EB490 = D_8009D4FE;
        TASK_C->w4e++;
        break;
    case 1:
        {
            volatile unsigned short *pad = &D_8009D670;
            dir = 0;
            if (*pad & 0x10) dir = 1;
            else if (*pad & 0x40) dir = 2;
            else if (*pad & 0x80) dir = 3;
            else if (*pad & 0x20) dir = 4;
        }
        held = ~D_8009F7F2 & 0xF0;
        if (held != D_800EB484) {
            D_800EB484 = held;
            D_800EB488 = 0x10;
        } else if (--D_800EB488 == 0) {
            D_800EB488 = 6;
            DAT_1f8001fc |= held;
        }
        b = DAT_1f8001fc;
        if (b & 0x10) {
            if (D_8009D4FE >= 5) {
                D_8009D4FE--;
                playSFX(8);
            }
        } else if (b & 0x40) {
            if (D_8009D4FE < 0x14) {
                D_8009D4FE++;
                playSFX(8);
            }
        } else if (b & 0x80) {
            if (D_8009D4FC >= -7) {
                D_8009D4FC--;
                playSFX(8);
            }
        } else if (b & 0x20) {
            if (D_8009D4FC < 0x18) {
                D_8009D4FC++;
                playSFX(8);
            }
        } else if (b & 0x4000) {
            playSFXAndSetNote(10, 10);
            TASK_C->w4e = 0;
            TASK_C->w4c = 1;
        } else if (b & 0x2000) {
            D_8009D4FC = D_800EB48C;
            D_8009D4FE = D_800EB490;
            playSFXAndSetNote(0x1B, 0x1C);
            TASK_C->w4e = 0;
            TASK_C->w4c = 1;
        }
        D_8009E354 = 0x100;
        D_8009E356 = 0x100;
        D_8009F064 = 0x100;
        D_8009F066 = 0x100;
        D_8009E350 = D_8009D4FC;
        D_8009E352 = D_8009D4FE;
        D_8009F060 = D_8009D4FC;
        D_8009F062 = D_8009D4FE;
        updateSound();
        break;
    }
    func_800E8C24(dir);
    return 0;
}
