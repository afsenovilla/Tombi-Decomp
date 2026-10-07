// FUNC 8001fe6c 28 MAIN0
typedef struct { char p0[6]; unsigned short v; } AnimHdr;
typedef struct { char p0[0x24]; AnimHdr *a; char p1[4]; short dur; } TAnim;

void AnimLoadDuration(TAnim *o)
{
    o->dur = o->a->v & 0x3fff;
}
