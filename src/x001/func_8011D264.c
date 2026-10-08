// FUNC 8011d264 40 X001
// MATCHING 8011d264 40
typedef struct { short w[17]; } S22;
extern S22 D_800A4480[];

int func_8011D264(int i)
{
    return -D_800A4480[i].w[5] & 0xfff;
}
