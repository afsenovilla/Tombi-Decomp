// FUNC 80017238 52 MAIN0
// MATCHING 80017238 52
typedef struct Th { short state; short wait; } Th;
extern Th *TASK_C;
extern void ChangeTh(unsigned);

void ThreadWaitFrames(short n)
{
    Th *t = TASK_C;
    t->wait = n;
    t->state = 1;
    ChangeTh(0xff000000);
}
