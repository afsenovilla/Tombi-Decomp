// FUNC 8003a6b0 108 MAIN0
// r9: only a1/a2 swap for p/q (game p=a2,q=a1); reading SCRIPT_CTX directly fixes regs but reloads it at the end. Tried decl order, register, goto, temps.
typedef struct {
    char pad[0x8a];
    unsigned short w8a;
    char pad2[0x1190 - 0x8c];
    int d1190;
    int d1194;
} S8003A570;
typedef struct { char pad[0x6b]; unsigned char b6b; } Q;
extern S8003A570 *SCRIPT_CTX;
extern Q *SCRIPT_OBJECTS[];
extern signed char D_8009D2B0;
extern unsigned char D_800A60A3;

void func_8003A6B0(void)
{
    Q *q;
    S8003A570 *p = SCRIPT_CTX;
    q = SCRIPT_OBJECTS[p->d1190];
    if (D_8009D2B0 != 3 || D_800A60A3 == 1) {
        if (q != 0) {
            q->b6b = p->d1194;
        }
    }
    p->w8a++;
}
