// FUNC 8006a270 132 MAIN0
// MATCHING 8006a270 132
extern void padSetCmdQueryModel(char *);
extern void padSetCmdQueryMode(char *, int);
extern void padSetCmdQueryComb(char *, int);

void _padSendAtLoadInfo(char *o)
{
    switch ((unsigned char)o[0x46]) {
    case 2:
        padSetCmdQueryModel(o);
        break;
    case 3:
        padSetCmdQueryMode(o, (unsigned char)o[0xe4]);
        break;
    case 4:
        padSetCmdQueryComb(o, (unsigned char)o[0x47]);
        break;
    }
}
