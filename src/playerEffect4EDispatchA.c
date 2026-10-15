// FUNC 8002fbc8 208 MAIN0
// MATCHING 8002fbc8 208
void playerEffect4EStepA(unsigned char *o);
void playerEffect4EStepC(unsigned char *o);
void playerEffect4EStepD(unsigned char *o);
void playerEffect4EStepB(unsigned char *o);
void SfxPlay2(int a, int b);
void playerEffect4EDispatchA(unsigned char *o)
{
    switch (o[3]) {
    case 0: playerEffect4EStepA(o); break;
    case 1: playerEffect4EStepC(o); break;
    case 2: playerEffect4EStepD(o); break;
    case 9: playerEffect4EStepB(o); break;
    }
    if (o[3] != 9 && (*(unsigned short *)0x1F8001F8 & 3) == 0) SfxPlay2(3, 0xc);
}
