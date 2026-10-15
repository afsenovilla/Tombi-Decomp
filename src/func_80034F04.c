// FUNC 80034f04 84 MAIN0
// MATCHING 80034f04 84
typedef struct S { char p[5]; unsigned char t; } S;
extern void weapon5to7Swing(void);
void func_80034F04(S *o)
{
    switch (o->t) {
    case 0:
        weapon5to7Swing();
        break;
    case 1:
        weapon5to7Swing();
        break;
    case 2:
        weapon5to7Swing();
        break;
    }
}
