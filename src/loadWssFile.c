// FUNC 8004f38c 48 MAIN0
// MATCHING 8004f38c 48
extern int WSS_LOAD_LISTS[];
extern void queueLoadList(int);
void loadWssFile(int a, int b)
{
    queueLoadList(WSS_LOAD_LISTS[b]);
}
