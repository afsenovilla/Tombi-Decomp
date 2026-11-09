// FUNC 8004d3b8 60 MAIN0
// MATCHING 8004d3b8 60
typedef struct { char p[3]; unsigned char k; } O;
extern void (*UNLAYERED_VARIANT_HANDLERS[])(O *);
void dispatchUnlayeredByVariant(O *o)
{
    UNLAYERED_VARIANT_HANDLERS[o->k](o);
}
