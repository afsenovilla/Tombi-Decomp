// FUNC 8001eaa4 40 MAIN0
/* demoted: bytes match but some relocated addresses (globals/callees) differ from the game; run tools/ncheck.py to see which ("address of X differs"). Fix the extern names/offsets. */
extern void SfxPlay2(int a, int b);

void FUN_8001eaa4(int x)
{
    SfxPlay2((x & 0xff) | 0x1000, x);
}
