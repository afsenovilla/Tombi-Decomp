// FUNC 8001eb64 32 MAIN0
// MATCHING 8001eb64 32
extern void FUN_8001ec80(int);
void startAreaBgm(void)
{
#if defined(VERSION_PCPX_96084) /* PrePre 8 passes 1 */
    FUN_8001ec80(1);
#else
    FUN_8001ec80(-1);
#endif
}
