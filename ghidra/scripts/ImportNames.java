// Aplica nombres desde un CSV address,name,... (p. ej. notes/names_main0.csv)
// Uso: Script Manager > Create New Script (Java) con nombre ImportNames.
// Pide el CSV (names_main0.csv para MAIN0.EXE, names_x000.csv para X000.BIN)
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class ImportNames extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File f = args.length > 0 ? new File(args[0]) : askFile("CSV address,name,...", "Importar");
        BufferedReader in = new BufferedReader(new FileReader(f));
        in.readLine(); // cabecera
        String line;
        int ok = 0, fail = 0;
        while ((line = in.readLine()) != null) {
            String[] p = line.split(",");
            if (p.length < 2) continue;
            Address a = toAddr(p[0].trim());
            Function fn = getFunctionAt(a);
            if (fn == null) {
                println("No hay funcion en " + a);
                fail++;
                continue;
            }
            fn.setName(p[1].trim(), SourceType.USER_DEFINED);
            ok++;
        }
        in.close();
        println("Renombradas " + ok + " funciones, " + fail + " fallos");
    }
}
