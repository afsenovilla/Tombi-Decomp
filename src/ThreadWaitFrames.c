// FUNC 80017238 52 MAIN0
// MATCHING 80017238 52
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084) /* PrePre 8: the current task pointer is a RAM global */
#define TASK_C D_800DE0B0
#endif
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
