// Aplica la estructura TObj (definida con DefineObj) como tipo del primer parametro de las
// funciones cuyo decompilado accede a >=3 campos conocidos. Ejecutar en MAIN0.EXE y X000.BIN.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.HighFunctionDBUtil;
import ghidra.program.model.pcode.HighSymbol;
import ghidra.program.model.pcode.LocalSymbolMap;
import ghidra.program.model.symbol.SourceType;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ApplyObj extends GhidraScript {
    private static final int[] KNOWN = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0a, 0x0b, 0x0c, 0x0d, 0x0f,
        0x10, 0x14, 0x18, 0x1c, 0x1d, 0x1e, 0x20, 0x22, 0x24, 0x28, 0x2c, 0x2e, 0x30, 0x34,
        0x38, 0x3c, 0x40, 0x44, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x56, 0x58, 0x5c, 0x60,
        0x64, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c,
        0x7e, 0x80, 0x82, 0x84, 0x88, 0x8c, 0x90, 0x94, 0x98, 0x9a, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8,
        0xba, 0xbc, 0xbe, 0xbf};

    @Override
    protected void run() throws Exception {
        println("Programa: " + currentProgram.getName());
        DataTypeManager dtm = currentProgram.getDataTypeManager();
        DataType objDt = dtm.getDataType("/TObj");
        if (objDt == null) {
            println("No existe TObj: ejecuta antes DefineObj en este programa");
            return;
        }
        DataType objPtr = new PointerDataType(objDt, dtm);

        Set<Integer> known = new HashSet<Integer>();
        for (int k : KNOWN) known.add(k);

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        int done = 0, tried = 0;
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
            monitor.checkCancelled();
            DecompileResults r = di.decompileFunction(fn, 30, monitor);
            if (r == null || !r.decompileCompleted()) continue;
            HighFunction hf = r.getHighFunction();
            if (hf == null) continue;
            LocalSymbolMap lsm = hf.getLocalSymbolMap();
            if (lsm.getNumParams() < 1) continue;
            HighSymbol ps = lsm.getParamSymbol(0);
            if (ps == null) continue;
            DataType dt = ps.getDataType();
            if (dt == null || dt.getLength() != 4 || "TObj *".equals(dt.getName())) continue;
            int unit = 1;
            if (dt instanceof Pointer) {
                DataType base = ((Pointer) dt).getDataType();
                if (base == null || base.getLength() < 1) continue;
                unit = base.getLength();
            }
            tried++;
            String c = r.getDecompiledFunction().getC();
            String pn = Pattern.quote(ps.getName());
            Pattern pAdd = Pattern.compile(pn + " \\+ (0x[0-9a-f]+|\\d+)\\)");
            Pattern pIdx = Pattern.compile(pn + "\\[(0x[0-9a-f]+|\\d+)\\]");
            Set<Integer> hits = new HashSet<Integer>();
            Matcher m = pAdd.matcher(c);
            while (m.find()) {
                int off = Integer.decode(m.group(1));
                if (known.contains(off)) hits.add(off);
            }
            m = pIdx.matcher(c);
            while (m.find()) {
                int off = Integer.decode(m.group(1)) * unit;
                if (known.contains(off)) hits.add(off);
            }
            if (hits.size() >= 3) {
                try {
                    HighFunctionDBUtil.updateDBVariable(ps, null, objPtr, SourceType.USER_DEFINED);
                    done++;
                } catch (Exception e) {
                    println("No se pudo en " + fn.getName() + ": " + e.getMessage());
                }
            }
        }
        println("TObj aplicado a " + done + " funciones de " + tried + " revisadas");
    }
}
