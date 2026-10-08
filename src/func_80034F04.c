// FUNC 80034f04 84 MAIN0
// MATCHING 80034f04 84
typedef struct S { char p[5]; unsigned char t; } S;
extern void func_80033B44(void);
void func_80034F04(S *o)
{
    switch (o->t) {
    case 0:
        func_80033B44();
        break;
    case 1:
        func_80033B44();
        break;
    case 2:
        func_80033B44();
        break;
    }
}
