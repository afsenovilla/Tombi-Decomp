// FUNC 80017414 104 MAIN0
// MATCHING 80017414 104
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084) /* PrePre 8: the task table is at 0x801FC000 */
#define TASK_TABLE_BASE 0x801fc000
#else
#define TASK_TABLE_BASE 0x801fd800
#endif
typedef struct E { unsigned short s; unsigned short n; char pad[0x6c]; } E;
void tickTaskTimers(void)
{
E *p = (E *)TASK_TABLE_BASE;
do {
    if (p->s == 1) {
        p->n--;
        if ((short)p->n == 0) p->s = 2;
    }
    p++;
} while (p <= (E *)(TASK_TABLE_BASE + 0x14f));
}
