# Exporta las funciones del programa a notes/functions.csv
# @category Tombi
# @menupath Tools.Tombi.Export Functions
import os

path = os.path.join(str(askDirectory("Carpeta notes/", "Elegir")), "functions.csv")
fm = currentProgram.getFunctionManager()
with open(path, "w") as out:
    out.write("address,name,size\n")
    for fn in fm.getFunctions(True):
        out.write("%s,%s,%d\n" % (fn.getEntryPoint(), fn.getName(),
                                  fn.getBody().getNumAddresses()))
print("Escrito " + path)
