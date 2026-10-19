// FUNC 80121ae0 72 X000
// MATCHING 80121ae0 72
void a00ApBox50kHideDigits(char *o)
{
    *(char *)(*(int *)(o + 0xb4) + 4) = 2;
    *(char *)(*(int *)(o + 0xb8) + 4) = 2;
    *(char *)(*(int *)(o + 0xbc) + 4) = 2;
    *(char *)(*(int *)(o + 0xc0) + 4) = 2;
    *(char *)(*(int *)(o + 0xc4) + 4) = 2;
    *(char *)(*(int *)(o + 0xc8) + 4) = 2;
}
