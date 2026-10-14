// FUNC 8006a670 168 MAIN0
// MATCHING 8006a670 168
typedef struct { char pad[0x46]; unsigned char a; unsigned char b; unsigned char c; } S;
extern void padSetCmdQueryMode(S *, int);
extern void padSetCmdQueryAct(S *, int);
extern void padSetCmdQueryComb(S *, int);
extern void padSetCmd4B(S *);

void func_8006A670(S *s)
{
    switch (s->a) {
    case 2:
        padSetCmdQueryMode(s, s->b);
        break;
    case 3:
        padSetCmdQueryAct(s, s->b);
        break;
    case 4:
        if (s->c == 0) {
            padSetCmdQueryComb(s, s->b);
        } else {
            padSetCmd4B(s);
        }
        break;
    }
}
