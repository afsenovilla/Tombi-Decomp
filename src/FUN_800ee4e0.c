// FUNC 800ee4e0 60 X000
// MATCHING 800ee4e0 60
typedef struct S { char pad[0x8c]; int v; char pad2[0xb0 - 0x90]; short idx; } S;
extern unsigned char DAT_801152e8[];
extern unsigned char DAT_8011530c[];

void FUN_800ee4e0(S *s, short f)
{
    unsigned char b;
    if (f) {
        b = DAT_8011530c[s->idx];
    } else {
        b = DAT_801152e8[s->idx];
    }
    s->v = b;
}
