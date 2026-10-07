// FUNC 8001715c 92 MAIN0
typedef struct E { int w0; char rest[108]; } E;
extern E DAT_801fd80c[];
extern void FUN_8001747c(void *);
extern void FUN_800171b8(int, int);
void FUN_8001715c(int i)
{
    FUN_8001747c((E *)0x801fd80c + i);
    FUN_800171b8(i, DAT_801fd80c[i].w0);
}
