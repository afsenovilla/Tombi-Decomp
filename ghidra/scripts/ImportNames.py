# Aplica nombres desde un CSV "address,name" (p. ej. notes/functions.csv)
# @category Tombi
# @menupath Tools.Tombi.Import Names
from ghidra.program.model.symbol import SourceType

csv = askFile("CSV address,name", "Importar")
n = 0
for line in open(str(csv)).read().splitlines()[1:]:
    parts = line.split(",")
    if len(parts) < 2 or parts[1].startswith("FUN_"):
        continue
    addr = toAddr(parts[0])
    fn = getFunctionAt(addr)
    if fn is None:
        fn = createFunction(addr, parts[1])
    if fn is not None:
        fn.setName(parts[1], SourceType.USER_DEFINED)
        n += 1
print("Renombradas %d funciones" % n)
