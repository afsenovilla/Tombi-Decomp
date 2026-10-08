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

extern int FUN_80020b64(int);
extern void FUN_80020c04(int);
extern void FUN_8003e300(int, int, int);
extern void FUN_8003e33c(int, int, int);
extern void FUN_8003e378(int, int, int, int, int);
extern void FUN_8003e3cc(int, int, int, int, int);
extern void FUN_8003e420(int, int, int, int, int);
extern void FUN_8003e474(int, int, int);
extern void FUN_80123e50(int, int, int, int);
extern void FUN_8003e4b0(int, int, int);
extern void FUN_8001e4f0(int);

void FUN_8003c7c4(SpawnDesc *d, int arg, short x, short y, int flag)
{
    if (FUN_80020b64(d->id) == 0) {
        switch (d->kind) {
        case 0: FUN_8003e300(d->a, d->b, arg); break;
        case 1: FUN_8003e33c(d->a, d->b, arg); break;
        case 2: FUN_8003e378(d->a, d->b, arg, x, y); break;
        case 3: FUN_8003e3cc(d->a, d->b, arg, x, y); break;
        case 4: FUN_8003e420(d->a, d->b, arg, x, y); break;
        case 5: FUN_8003e474(d->a, d->b, arg); break;
        case 6: FUN_80123e50(arg, d->id, x, y); break;
        case 8: FUN_8003e4b0(d->a, d->b, arg); break;
        }
        if (flag) FUN_80020c04(d->id);
        if (d->flag == 0) FUN_8001e4f0(0x15);
    }
}
