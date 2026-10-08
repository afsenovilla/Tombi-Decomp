// FUNC 8001a43c 528 MAIN0
// score 2 (w5): only diff: game has nop in jal FUN_8001c314 slot (move a0,zero only in the bne slot); ours repeats move a0,zero. Tried arg=0 before if, c314(arg), full calls per branch, prototypes.
typedef struct G { char pad[0x4e]; unsigned short w4e; char pad2[0x5e - 0x50]; short w5e; char pad3[0x64 - 0x60]; unsigned short w64; } G;
extern G *DAT_1f8001d4;
extern unsigned char DAT_1f8001ce;
extern unsigned char DAT_1f8001cf;
extern unsigned char DAT_8009c974[];
extern unsigned short DAT_8009c960, DAT_8009d2a8, DAT_8009c962, DAT_8009d2aa;
extern int DAT_8009f7e4;
extern void FUN_8004fa80(int, int);
extern void FUN_800175f0(void);
extern void FUN_8001a0f0(void);
extern void FUN_8001f4bc(void);
extern void FUN_800212b4(int);
extern void FUN_8001c314(int);
extern void FUN_8001c218(int);
extern void FUN_8001be1c(void);

void FUN_8001a43c(void)
{
    int arg;
    switch (DAT_1f8001d4->w4e) {
    case 0:
        FUN_8004fa80(9, 1);
        DAT_1f8001d4->w4e++;
        break;
    case 1:
        if (DAT_1f8001ce == 0)
            break;
        DAT_1f8001d4->w4e++;
        break;
    case 2:
        DAT_1f8001d4->w4e++;
        FUN_800175f0();
        DAT_1f8001cf = 0;
        break;
    case 3:
        FUN_8001a0f0();
        break;
    case 5:
        FUN_8001f4bc();
        DAT_1f8001d4->w4e++;
        FUN_800175f0();
        DAT_1f8001cf = 0;
        DAT_1f8001d4->w5e = 0x78;
        DAT_1f8001d4->w64 = 0;
        break;
    case 6:
        DAT_1f8001d4->w64 = (DAT_1f8001d4->w64 + 0xc) & 0xff;
        FUN_800212b4(DAT_1f8001d4->w64);
        if (--DAT_1f8001d4->w5e != 0)
            break;
        arg = 1;
        if (DAT_8009c974[0] == 0) {
            DAT_8009c974[0] = 1;
        } else if (DAT_8009c960 == DAT_8009d2a8) {
            if (DAT_8009c962 == DAT_8009d2aa) {
                FUN_8001c314(0);
                break;
            }
            arg = 0;
        }
        FUN_8001c218(arg);
        DAT_8009f7e4 = 0;
        DAT_1f8001d4->w4e++;
        break;
    case 4:
    case 7:
        FUN_8001be1c();
        break;
    }
}
