// FUNC 80021aec 52 MAIN0
// MATCHING 80021aec 52
typedef struct E { char pad[6]; unsigned short v; } E;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern E *DAT_80078b7c[];
typedef struct S { char pad[0x32]; unsigned short v; } S;
void FUN_80021aec(S *o)
{
int a = DAT_8009c960; int b = DAT_8009c962; unsigned short *p = &DAT_80078b7c[a][b].v; o->v = *p;
}
