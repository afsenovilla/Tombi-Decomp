#!/usr/bin/env python3
"""patch_w2c.py W2CDIR: what the PS1 code needs from wasm2c's generated module (tombi-impl.h), as the i386
runtime's fault handlers give it:
  - the null page: an effective address below 64 KB (nothing of the image lives there: wasm-ld --global-base=65536)
    is the same byte of port_ram (the BIOS area), as runtime/lowmem.c emulates it after a SIGSEGV;
  - division by zero (and INT_MIN / -1) gives the R3000 result instead of a trap (lowmem.c's SIGFPE handler);
  - call_indirect does not check the function type: the game calls through pointers with other prototypes and the
    native ABI (x86-64 SysV, AAPCS64: integer arguments in registers) tolerates the arity mismatch as i386 did."""
import os, re, sys
d = sys.argv[1]
p = os.path.join(d, "tombi-impl.h")
s = open(p).read()
def rep(old, new):
    global s
    if old not in s: sys.exit("patch_w2c: pattern not found: " + old[:80])
    s = s.replace(old, new, 1)
rep("#define MEM_ADDR(mem, addr, n) &(mem)->data[addr]\n",
    "extern uint64_t port_w2c_low_base;   /* port2/wasm/host.c: linear address of port_ram */\n"
    "#define PORT_W2C_LOW(a) (UNLIKELY((uint64_t)(a) < 0x10000) ? (uint64_t)(a) + port_w2c_low_base : (uint64_t)(a))\n"
    "#define MEM_ADDR(mem, addr, n) &(mem)->data[PORT_W2C_LOW(addr)]\n")
rep("""#define CHECK_CALL_INDIRECT(table, ft, x)                \\
  (LIKELY((x) < table.size && table.data[x].func &&      \\
          func_types_eq(ft, table.data[x].func_type)) || \\
   TRAP(CALL_INDIRECT))""",
    """#define CHECK_CALL_INDIRECT(table, ft, x)                \\
  (LIKELY((x) < table.size && table.data[x].func) || \\
   TRAP(CALL_INDIRECT))""")
rep("""       ? TRAP(DIV_BY_ZERO)                                        \\
       : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \\""",
    """       ? (ut)((x) < 0 ? 1 : -1)                                   \\
       : (UNLIKELY((x) == min && (y) == -1)) ? (ut)(min)          \\""")
rep("""  ((UNLIKELY((y) == 0))      \\
       ? TRAP(DIV_BY_ZERO)   \\
       : (UNLIKELY((x) == min && (y) == -1)) ? 0 : (ut)((x) % (y)))""",
    """  ((UNLIKELY((y) == 0))      \\
       ? (ut)(x)             \\
       : (UNLIKELY((x) == min && (y) == -1)) ? 0 : (ut)((x) % (y)))""")
rep("""#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)""",
    """#define DIV_U(x, y) ((UNLIKELY((y) == 0)) ? (0 * (x) - 1) : ((x) / (y)))
#define REM_U(x, y) ((UNLIKELY((y) == 0)) ? (x) : ((x) % (y)))""")
open(p, "w").write(s)
# LLVM's FixFunctionBitcasts gives a direct call whose type it cannot adapt a stub that traps
# (`.Lname_bitcast_invalid`); the call goes to the function itself, through the stub's own type: the native ABI
# tolerates the mismatch as the i386 build does
stub_re = re.compile(r"^(\w[\w ]*?) (w2c_tombi_0x2EL(\w+?)_bitcast_invalid(?:0x2E\d+)?)\(w2c_tombi\* instance((?:, \w+ var_p\d+)*)\) \{\n  FUNC_PROLOGUE;\n(?:  [^\n]*\n)*?  UNREACHABLE;\n(?:  [^\n]*\n)*?\}", re.M)
nfix = 0
for f in sorted(os.listdir(d)):
    if not f.endswith(".c"): continue
    fp = os.path.join(d, f); c = open(fp).read()
    def fix(m):
        global nfix
        ret, name, target, params = m.group(1), m.group(2), m.group(3), m.group(4)
        types = [x.strip().split()[0] for x in params.split(",") if x.strip()]
        names = [x.strip().split()[1] for x in params.split(",") if x.strip()]
        call = "((%s (*)(w2c_tombi*%s))&w2c_tombi_%s)(instance%s)" % (ret.replace("static ", ""), "".join(", " + t for t in types), target, "".join(", " + n for n in names))
        nfix += 1
        return "%s %s(w2c_tombi* instance%s) {\n  %s%s;\n}" % (ret, name, params, "" if ret.endswith("void") else "return ", call)
    c2 = stub_re.sub(fix, c)
    if c2 != c: open(fp, "w").write(c2)
print("patch_w2c: %d bitcast_invalid stubs forward to their function" % nfix)
print("patch_w2c: tombi-impl.h patched (null page window, R3000 division, relaxed call_indirect)")
