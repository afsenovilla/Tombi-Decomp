// FUNC 8006a670 168 MAIN0
// MATCHING 8006a670 168
typedef struct { char pad[0x46]; unsigned char a; unsigned char b; unsigned char c; } S;
extern void FUN_8006addc(S *, int);
extern void FUN_8006adfc(S *, int);
extern void FUN_8006ae1c(S *, int);
extern void FUN_8006ae3c(S *);

void func_8006A670(S *s)
{
    switch (s->a) {
    case 2:
        FUN_8006addc(s, s->b);
        break;
    case 3:
        FUN_8006adfc(s, s->b);
        break;
    case 4:
        if (s->c == 0) {
            FUN_8006ae1c(s, s->b);
        } else {
            FUN_8006ae3c(s);
        }
        break;
    }
}
