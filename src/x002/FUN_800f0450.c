// FUNC 800f0450 64 X002
// MATCHING 800f0450 64
typedef struct S { char pad[0x16]; unsigned short h; char pad2[0x69 - 0x18]; char c; } S;
extern void func_8003fd78(S *s, int a, int b);

void FUN_800f0450(S *s)
{
    if (s->c) {
        s->h = s->h + 2;
    }
    func_8003fd78(s, 0, 0);
}
