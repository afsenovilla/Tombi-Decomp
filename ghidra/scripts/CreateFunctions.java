// Crea funciones en las direcciones (hex, una por linea) del fichero dado como argumento.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.io.BufferedReader;
import java.io.FileReader;

public class CreateFunctions extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) { println("Uso: CreateFunctions <fichero con direcciones>"); return; }
        BufferedReader in = new BufferedReader(new FileReader(args[0]));
        String line;
        int ok = 0, fail = 0;
        while ((line = in.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty()) continue;
            Address a = toAddr(line);
            if (getFunctionAt(a) != null) continue;
            clearListing(a, a.add(3));
            disassemble(a);
            if (createFunction(a, null) != null) ok++; else { fail++; println("Fallo en " + a); }
        }
        in.close();
        println("Funciones creadas: " + ok + ", fallos: " + fail);
    }
}
