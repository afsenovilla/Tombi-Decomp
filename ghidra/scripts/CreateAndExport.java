// Crea funciones en MAIN0 en direcciones que solo llama un overlay y exporta su decompilado.
// Ejecutar con MAIN0.EXE abierto. Salida: Escritorio\main0_missing.c
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.File;
import java.io.PrintWriter;

public class CreateAndExport extends GhidraScript {
    @Override
    protected void run() throws Exception {
        long[] addrs = {
            0x800182acL, 0x8001e5f4L, 0x8001e76cL, 0x8001eaa4L, 0x8001f8e4L, 0x8001fa20L,
            0x8001fa60L, 0x8001fab4L, 0x8001fb20L, 0x8001fc14L, 0x8001fd48L, 0x8001fd94L,
            0x80020078L, 0x800201acL, 0x800203dcL, 0x80020d20L, 0x800216c4L, 0x80024ea0L,
            0x80025aa8L, 0x80025d90L, 0x80026bfcL, 0x80026e0cL, 0x800270a0L, 0x8002715cL,
            0x800277f8L, 0x80028420L, 0x80028b40L, 0x8002a3d4L, 0x8002a4d0L, 0x8002b920L,
            0x8002c654L, 0x8002cd20L, 0x8002dcf0L, 0x8002ee50L, 0x8002ff20L, 0x80030034L,
            0x80030b9cL, 0x80037e18L, 0x8003c7c4L, 0x8003e918L, 0x8003ecb0L, 0x8003f598L,
            0x8003f7ccL, 0x8003faccL, 0x8003fb90L, 0x8003fd78L, 0x800408d8L, 0x80041240L,
            0x80041ca8L, 0x8004232cL, 0x8004245cL, 0x8004258cL, 0x800428c0L, 0x80042fbcL,
            0x80043464L, 0x800435e0L, 0x800437e0L, 0x800439f4L, 0x80044424L, 0x8004461cL,
            0x8004886cL, 0x80048ffcL, 0x8004b57cL, 0x8004b6a0L, 0x8004baa8L, 0x8004bbc0L,
            0x8004fba8L, 0x8004fcc0L, 0x8004fd6cL, 0x8004fdc8L, 0x8004fe1cL
        };
        File f = new File(System.getProperty("user.home") + "\\Desktop\\main0_missing.c");
        println("Programa: " + currentProgram.getName());
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        PrintWriter out = new PrintWriter(f, "UTF-8");
        int n = 0;
        for (long x : addrs) {
            Address a = toAddr(x);
            Function fn = getFunctionAt(a);
            if (fn == null) {
                clearListing(a, a.add(3));
                disassemble(a);
                fn = createFunction(a, null);
            }
            if (fn == null) {
                out.println("// ==== NO_FUNC @ " + a + " prog=" + currentProgram.getName()
                    + " instr=" + getInstructionAt(a) + " data=" + getDataAt(a)
                    + " mem=" + currentProgram.getMemory().contains(a));
                continue;
            }
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
