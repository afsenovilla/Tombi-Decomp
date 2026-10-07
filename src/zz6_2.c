// FUNC 80017238 52 MAIN0
// FLAGS -O2 -G0 -fno-schedule-insns2
typedef struct Th { short state; short wait; } Th;
extern Th *DAT_1f8001d4;
extern void ChangeTh(unsigned);

void ThreadWaitFrames(short n)
{
    Th *t = DAT_1f8001d4;
    t->wait = n;
    t->state = 1;
    ChangeTh(0xff000000);
}
