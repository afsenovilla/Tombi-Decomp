// FUNC 8001747c 68 MAIN0
// MATCHING 8001747c 68
extern void DrawSync(int);
extern int GetGp(void);

void setTaskEntry(int *p, int v)
{
    DrawSync(0);
    p[0] = v;
    p[1] = GetGp();
}
