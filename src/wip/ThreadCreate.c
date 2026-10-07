// FUNC 800171b8 128 MAIN0
typedef struct T { short state; short pad; int th; int a; int pad2; int b; char pad3[0x70 - 0x14]; } T;
extern T DAT_801fd800[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
#define P(k) ((T*)((char*)DAT_801fd800 + (k)))
#define Q(k) ((T*)((k) + (char*)DAT_801fd800))
void ThreadCreate(int n, int fn)
{
    int k = n * 0x70;
    Q(k)->state = 2;
    EnterCriticalSection();
    P(k)->th = OpenTh(fn, Q(k)->a, P(k)->b);
    ExitCriticalSection();
}
