// FUNC 8006e024 36 MAIN0
// MATCHING 8006e024 36
typedef struct {
    int pad[9];
    unsigned char fb;
} S8006E024;

extern void SsUtSetReverbFeedback();

void func_8006E024(int a, int b, int c, S8006E024 s)
{
    SsUtSetReverbFeedback(s.fb);
}
