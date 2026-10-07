// FUNC 8006ada8 32 MAIN0
extern int g(int,int); extern int (*fp)(void);
void f1(int a, char b){ g(a,b); g(b,a); }
int f2(int a, char b, char c){ int r = fp(); return g(r,b+c); }
void f3(int a, char b, char c){ int r = fp(); g(r,b+c); }
void f4(int a, char b, char c){ int r = fp(); g(r,b+c); g(r, c);}
