// FUNC 8003c7c4 444 MAIN0
// MATCHING 8003c7c4 444
typedef struct {
    unsigned char kind;
    unsigned char a;
    unsigned char b;
    unsigned char id;
    unsigned char pad4;
    unsigned char flag;
} SpawnDesc;

extern int testSectionFlagBit(int);
extern void setSectionFlagBit(int);
extern void FUN_8003e300(int, int, int);
extern void FUN_8003e33c(int, int, int);
extern void spawnItemAtPos(int, int, int, int, int);
extern void spawnItemDropAtPos(int, int, int, int, int);
extern void spawnItemBounce(int, int, int, int, int);
extern void FUN_8003e474(int, int, int);
extern void FUN_80123e50(int, int, int, int);
extern void FUN_8003e4b0(int, int, int);
extern void FUN_8001e4f0(int);

void applyItemEffect(SpawnDesc *d, int arg, short x, short y, int flag)
{
    if (testSectionFlagBit(d->id) == 0) {
        switch (d->kind) {
        case 0: FUN_8003e300(d->a, d->b, arg); break;
        case 1: FUN_8003e33c(d->a, d->b, arg); break;
        case 2: spawnItemAtPos(d->a, d->b, arg, x, y); break;
        case 3: spawnItemDropAtPos(d->a, d->b, arg, x, y); break;
        case 4: spawnItemBounce(d->a, d->b, arg, x, y); break;
        case 5: FUN_8003e474(d->a, d->b, arg); break;
        case 6: FUN_80123e50(arg, d->id, x, y); break;
        case 8: FUN_8003e4b0(d->a, d->b, arg); break;
        }
        if (flag) setSectionFlagBit(d->id);
        if (d->flag == 0) FUN_8001e4f0(0x15);
    }
}
