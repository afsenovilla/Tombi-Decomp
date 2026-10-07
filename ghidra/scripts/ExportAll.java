// Exporta el decompilado de todas las funciones del programa activo a <programa>_decomp.c
// Con argumento de script (modo headless): carpeta de salida. Sin argumento: el Escritorio.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;

public class ExportAll extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File dir = args.length > 0 ? new File(args[0])
            : new File(System.getProperty("user.home") + "\\Desktop");
        dir.mkdirs();
        String name = currentProgram.getName().replace('.', '_');
        File f = new File(dir, name + "_decomp.c");
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(f, "UTF-8");
        int n = 0;
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
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
