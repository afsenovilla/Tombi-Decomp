// FUNC 8002fbc8 208 MAIN0
// MATCHING 8002fbc8 208
void func_8002F16C(unsigned char *o);
void func_8002F4E0(unsigned char *o);
void FUN_8002f960(unsigned char *o);
void FUN_8002f384(unsigned char *o);
void SfxPlay2(int a, int b);
void func_8002FBC8(unsigned char *o)
{
    switch (o[3]) {
    case 0: func_8002F16C(o); break;
    case 1: func_8002F4E0(o); break;
    case 2: FUN_8002f960(o); break;
    case 9: FUN_8002f384(o); break;
    }
    if (o[3] != 9 && (*(unsigned short *)0x1F8001F8 & 3) == 0) SfxPlay2(3, 0xc);
}
