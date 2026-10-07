// FUNC 800277f8 24 MAIN0
extern short *DAT_800a607c;
typedef struct S { char pad[0x38]; short *p; } S;
void FUN_800277f8(S *s)
{
    s->p[1] = DAT_800a607c[1];
}
