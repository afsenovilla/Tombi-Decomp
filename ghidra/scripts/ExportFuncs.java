// Exports every function as CSV: address,size,name,callees (callee entry addresses separated by ';').
// Headless argument: output csv path.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;
import java.util.Set;


public class ExportFuncs extends GhidraScript {
    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        File f = new File(args[0]);
        f.getParentFile().mkdirs();
        PrintWriter out = new PrintWriter(f, "UTF-8");
        out.println("address,size,name,callees");
        int n = 0;
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
            StringBuilder sb = new StringBuilder();
            Set<Function> cs = new java.util.LinkedHashSet<>();
            for (Function c : fn.getCalledFunctions(monitor)) cs.add(c);
            for (Function c : cs) { if (sb.length() > 0) sb.append(';'); sb.append(c.getEntryPoint()); }
            out.println(fn.getEntryPoint() + "," + fn.getBody().getNumAddresses() + "," + fn.getName() + "," + sb);
            n++;
        }
        out.close();
        println("ExportFuncs: " + n + " functions -> " + f);
    }
}
