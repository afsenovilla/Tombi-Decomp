// FUNC 800698c4 820 MAIN0
/* score 126: library code; game epilogue is "jr ra; addiu sp" with saved s-regs (not reproducible with CC1PSX 4.3 per guide). Also: phantom 8-byte stack var at -O2 (gone with -fno-rerun-cse-after-loop), &D_80098204 should be preloaded before the bltz, final return uses branches (bnez; move v0,zero; li v0,1) instead of sltiu. */
typedef struct { unsigned char pad[0xc]; int dc; unsigned char p10[0x37 - 0x10]; unsigned char b37; unsigned char p38[0x50 - 0x38]; unsigned char b50; unsigned char p51[0xe8 - 0x51]; unsigned char be8; } E;
typedef struct { unsigned char b0; unsigned char p1[3]; unsigned short w4; unsigned char p6[2]; unsigned short w8; unsigned short a; unsigned short c; unsigned short e; } H;
extern volatile H *D_80098210;
extern volatile unsigned int *D_8009820C;
extern int D_800981EC;
extern int D_80098204[];
extern void (*D_800981CC)(void *);
extern void (*D_800981D0)(void *);
extern void FUN_8006bf84(int);
extern int FUN_8006bfa4();
extern int func_8006A170(void);
extern void func_8006A200(void);

int func_800698C4(E *e)
{
    volatile H *h = D_80098210;

    h->a = 0x40;
    h->a = 0;
    h->w8 = 0xd;
    h->e = 0x88;
    FUN_8006bf84(e->be8 == 8 ? 0x50 : 0x91);
    D_80098210->a = D_800981EC ? 0x3003 : 0x1003;
    if (D_80098204[D_800981EC] >= 0) {
        while (D_80098204[D_800981EC] > 0) {
            D_800981CC((char *)e->dc + --D_80098204[D_800981EC] * 0xf0);
        }
        if (D_80098204[D_800981EC] == 0) {
            D_80098204[D_800981EC] = -1;
            D_800981CC(e);
            D_800981D0(e);
        }
    }
    h = D_80098210;
    if (h->w4 & 0x200) {
        h->a |= 0x10;
        if (h->w4 & 0x200) {
            while (FUN_8006bfa4() == 0) {
            }
            *(unsigned char *)D_80098210 = 1;
            FUN_8006bf84(100);
            if (func_8006A170() == 0) return 0;
            func_8006A200();
            D_80098210->b0;
            FUN_8006bf84(0x1ae);
            while (!(*D_8009820C & 0x80)) {
                if (FUN_8006bfa4()) return 0;
            }
            *(unsigned char *)D_80098210 = 0x42;
            FUN_8006bf84(0x3c);
            if (func_8006A170() == 0) return 0;
            func_8006A200();
            D_80098210->b0;
            FUN_8006bf84(0x1ae);
            while (!(*D_8009820C & 0x80)) {
                if (FUN_8006bfa4()) return 0;
            }
            *(unsigned char *)D_80098210 = 1;
            FUN_8006bf84(0x3c);
            if (func_8006A170() == 0) return 0;
            func_8006A200();
            D_80098210->b0;
            return 0;
        }
        *D_8009820C = ~0x80;
    }
    if (e->b50 == 0) return 1;
    if (e->b37 != 0) return 0;
    return 1;
}
