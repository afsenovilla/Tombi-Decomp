// FUNC 80038514 152 MAIN0
// MATCHING 80038514 152
typedef struct {
    char pad[0x8a];
    unsigned short w8a;
    char pad2[0x1090 - 0x8c];
    int cnt[64];
} S8003A570;

extern S8003A570 *SCRIPT_CTX;
extern unsigned char *SCRIPT_CODE;

void func_80038514(void)
{
    S8003A570 *p = SCRIPT_CTX;
    unsigned char *code = SCRIPT_CODE;
    unsigned short tmp;
    int i;
    unsigned char *s;
    if (--p->cnt[code[p->w8a + 1]] > 0) {
        s = (unsigned char *)(p->w8a + (int)code) + 2;
        for (i = 0; i < 2; i++) {
            ((unsigned char *)&tmp)[i] = *s++;
        }
        p->w8a = tmp;
    } else {
        p->w8a += 4;
    }
}
