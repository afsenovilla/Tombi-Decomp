// FUNC 8004d3b8 60 MAIN0
// MATCHING 8004d3b8 60
typedef struct { char p[3]; unsigned char k; } O;
extern void (*D_8007C1C8[])(O *);
void dispatchUnlayeredByVariant(O *o)
{
    D_8007C1C8[o->k](o);
}
