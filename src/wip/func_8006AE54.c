// FUNC 8006ae54 72 MAIN0
typedef struct {
    char pad[0x3c];
    unsigned char *p3c;
} S8006AE54;

extern int (*D_800981C4)();
extern int D_8009822C;
extern void func_80069CE8();

void func_8006AE54(S8006AE54 *a)
{
    D_8009822C = D_800981C4();
    *a->p3c = 0;
    func_80069CE8(a, -2);
}
