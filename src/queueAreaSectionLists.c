// FUNC 8004f490 168 MAIN0
// MATCHING 8004f490 168
extern int *AREA_LOAD_LISTS[];
extern int LOAD_NEXT_ADDR, LOAD_SLOTS, g2b0;
extern void SfxStopAll(int);
extern void queueLoadList(int);
void queueAreaSectionLists(int a, int b, int c)
{
    int *p;
    if (c == 0) {
        SfxStopAll(1);
    } else {
        p = AREA_LOAD_LISTS[a];
        queueLoadList(p[0]);
        LOAD_SLOTS = LOAD_NEXT_ADDR;
    }
    p = AREA_LOAD_LISTS[a];
    queueLoadList(p[b + 1]);
    g2b0 = LOAD_NEXT_ADDR;
}
