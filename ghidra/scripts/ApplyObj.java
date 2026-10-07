// Define la estructura TObj (objeto del juego) y la aplica como tipo del primer parametro
// de las funciones cuyo decompilado accede a >=3 campos conocidos de objeto.
// Ejecutar una vez en MAIN0.EXE y otra en X000.BIN.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.symbol.SourceType;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ApplyObj extends GhidraScript {
    private static final int[] KNOWN = {0x00, 0x01, 0x02, 0x03, 0x06, 0x10, 0x12, 0x14, 0x16,
        0x18, 0x1a, 0x1c, 0x24, 0x28, 0x2c, 0x2e, 0x40, 0x44, 0x6c, 0x6e, 0x70, 0x72, 0x7e,
        0x80, 0x82};

    private void put(StructureDataType s, int off, DataType dt, String name) {
        s.replaceAtOffset(off, dt, dt.getLength(), name, null);
    }

    @Override
    protected void run() throws Exception {
        println("Programa: " + currentProgram.getName());
        DataTypeManager dtm = currentProgram.getDataTypeManager();

        StructureDataType parts = new StructureDataType("FixParts", 0);
        parts.add(UnsignedShortDataType.dataType, "frac", null);
        parts.add(ShortDataType.dataType, "whole", null);
        UnionDataType fix = new UnionDataType("Fix16");
        fix.add(IntegerDataType.dataType, "raw", null);
        fix.add(parts, "p", null);
        DataType fixDt = dtm.resolve(fix, DataTypeConflictHandler.REPLACE_HANDLER);
        DataType fixPtr = new PointerDataType(fixDt, dtm);

        StructureDataType o = new StructureDataType("TObj", 0xC0);
        DataType u8 = UnsignedCharDataType.dataType;
        DataType u16 = UnsignedShortDataType.dataType;
        DataType s16 = ShortDataType.dataType;
        put(o, 0x00, u8, "active");
        put(o, 0x01, u8, "visible");
        put(o, 0x02, u8, "type");
        put(o, 0x03, u8, "subtype");
        put(o, 0x06, u8, "state");
        put(o, 0x10, fixDt, "a");
        put(o, 0x14, fixDt, "y");
        put(o, 0x18, fixDt, "b");
        put(o, 0x1c, u8, "category");
        put(o, 0x24, new PointerDataType(DataType.DEFAULT, dtm), "anim");
        put(o, 0x28, new PointerDataType(DataType.DEFAULT, dtm), "movetab");
        put(o, 0x2c, u16, "animTimer");
        put(o, 0x2e, u16, "animFrame");
        put(o, 0x40, fixPtr, "h");
        put(o, 0x44, fixPtr, "d");
        put(o, 0x6c, s16, "box0");
        put(o, 0x6e, s16, "box1");
        put(o, 0x70, s16, "box2");
        put(o, 0x72, s16, "box3");
        put(o, 0x7e, s16, "velY");
        put(o, 0x80, s16, "velH");
        put(o, 0x82, s16, "velV");
        DataType objDt = dtm.resolve(o, DataTypeConflictHandler.REPLACE_HANDLER);
        DataType objPtr = new PointerDataType(objDt, dtm);

        Set<Integer> known = new HashSet<Integer>();
        for (int k : KNOWN) known.add(k);
        Pattern pAdd = Pattern.compile("param_1 \\+ (0x[0-9a-f]+|\\d+)\\)");
        Pattern pIdx = Pattern.compile("param_1\\[(0x[0-9a-f]+|\\d+)\\]");

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        int done = 0, tried = 0;
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
            monitor.checkCancelled();
            if (fn.getParameterCount() < 1) continue;
            Parameter p = fn.getParameter(0);
            DataType dt = p.getDataType();
            if (dt.getLength() != 4 || "TObj *".equals(dt.getName())) continue;
            int unit = 1;
            if (dt instanceof Pointer) {
                DataType base = ((Pointer) dt).getDataType();
                if (base == null || base.getLength() < 1) continue;
                unit = base.getLength();
            }
            tried++;
            DecompileResults r = di.decompileFunction(fn, 30, monitor);
            if (r == null || !r.decompileCompleted()) continue;
            String c = r.getDecompiledFunction().getC();
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
                    p.setDataType(objPtr, SourceType.USER_DEFINED);
                    done++;
                } catch (Exception e) {
                    println("No se pudo en " + fn.getName() + ": " + e.getMessage());
                }
            }
        }
        println("TObj aplicado a " + done + " funciones de " + tried + " revisadas");
    }
}
