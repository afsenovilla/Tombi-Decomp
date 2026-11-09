// FUNC 80069390 56 MAIN0
// MATCHING 80069390 56
// CC gcc-2.8.1
extern void *(*_padFuncPort2Info)(void);
extern int _padSetActAlign(void *, int);
int PadSetActAlign(int a, int b)
{
    return _padSetActAlign((*_padFuncPort2Info)(), b);
}
