// FUNC 800185f8 128 MAIN0
// MATCHING 800185f8 128
typedef struct Q { char pad[0x40]; char *a; char *b; } Q;
extern short FREE_OBJECT_COUNT_LAYER8;
extern Q **FREE_OBJECTS_LAYER8;
extern unsigned short SPAWN_AXIS_SWAP;
Q *allocObjectLayer8(void)
{
    Q *q; short n = FREE_OBJECT_COUNT_LAYER8;
    if (n > 0) {
        FREE_OBJECT_COUNT_LAYER8 = n - 1; q = *FREE_OBJECTS_LAYER8++;
        if ((SPAWN_AXIS_SWAP & 1) == 0) { q->a = (char *)q + 0x10; q->b = (char *)q + 0x18; }
        else { q->b = (char *)q + 0x10; q->a = (char *)q + 0x18; }
        return q;
    }
    return 0;
}
