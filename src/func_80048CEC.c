// FUNC 80048cec 80 MAIN0
// MATCHING 80048cec 80
extern unsigned short D_8009C960;
extern void func_80126CA4(void);
extern void func_8012032C(void);

void func_80048CEC(void)
{
    switch (D_8009C960) {
    case 0: func_80126CA4(); break;
    case 3: func_8012032C(); break;
    }
}
