// Exporta a main0_decomp.c el decompilado de las funciones FUN_ de .text
// Uso: Script Manager > Create New Script (Java) con nombre ExportC
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;

public class ExportC extends GhidraScript {
    @Override
    protected void run() throws Exception {
        long lo = 0x800163f4L, hi = 0x800778efL; // .text de MAIN0
        File f = new File(System.getProperty("user.home") + "\\Desktop\\main0_decomp.c");
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(f, "UTF-8");
        int n = 0;
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
            long a = fn.getEntryPoint().getOffset();
            if (a < lo || a > hi || !fn.getName().startsWith("FUN_")) continue;
            monitor.checkCancelled();
            DecompileResults r = di.decompileFunction(fn, 60, monitor);
            out.println("// ==== " + fn.getName() + " @ " + fn.getEntryPoint()
                + " size=" + fn.getBody().getNumAddresses());
            if (r != null && r.decompileCompleted()) out.println(r.getDecompiledFunction().getC());
            else out.println("// (no se pudo decompilar)");
            n++;
        }
        out.close();
        println("Escrito " + n + " funciones en " + f);
    }
}
