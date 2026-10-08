// FUNC 80017328 88 MAIN0
// MATCHING 80017328 88
// wip: solo difiere el orden del prologo (gcc pone lw global antes de subu sp)
extern short *g1d4;
extern void FUN_8005b9c4(void);
extern void FUN_8005b9a4(int);
extern void FUN_8005b9d4(void);
extern void FUN_8005b9b4(int);
void FUN_80017328(int a)
{
    short *p = g1d4;
    *p = 3;
    *(int *)(p + 6) = a;
    FUN_8005b9c4();
    FUN_8005b9a4(*(int *)(g1d4 + 2));
    FUN_8005b9d4();
    FUN_8005b9b4(0xff000000);
}
