// FUNC 80021aec 52 MAIN0
// MATCHING 80021aec 52
typedef struct E { char pad[6]; unsigned short v; } E;
extern unsigned short DAT_80099960;
extern unsigned short DAT_80099962;
extern E *DAT_80077b7c[];
typedef struct S { char pad[0x32]; unsigned short v; } S;
void FUN_80021aec(S *o)
{
int a = DAT_80099960; int b = DAT_80099962; unsigned short *p = &DAT_80077b7c[a][b].v; o->v = *p;
}
