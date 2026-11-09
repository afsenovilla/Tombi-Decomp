// FUNC 80069004 76 MAIN0
// MATCHING 80069004 76
typedef struct { unsigned char pad[0xe8]; unsigned char e8; unsigned char pad2[0xf0 - 0xe9]; } E;
extern int _padModeMtap;
extern E *_padInfoDir;
int PadChkMtap(int a)
{
    if (_padModeMtap != 0) {
        return _padInfoDir[a >> 4].e8 == 8;
    }
    return 0;
}
