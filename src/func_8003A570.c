// FUNC 8003a570 48 MAIN0
// MATCHING 8003a570 48
typedef struct {
    char pad[0x8a];
    unsigned short w8a;
    char pad2[0x1190 - 0x8c];
    int d1190;
} S8003A570;

extern S8003A570 *D_8009F0F0;
extern unsigned char D_8009CEA4[];

void func_8003A570(void)
{
    S8003A570 *p = D_8009F0F0;
    p->d1190 = D_8009CEA4[p->d1190];
    p->w8a++;
}
