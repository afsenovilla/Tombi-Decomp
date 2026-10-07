// Redefine la estructura TObj con mas campos (tipos inferidos del decompilado).
// Ejecutar en MAIN0.EXE y en X000.BIN despues de ApplyObj.
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;

public class DefineObj extends GhidraScript {
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
        DataType s32 = IntegerDataType.dataType;
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
        put(o, 0x04, u8, "b04");
        put(o, 0x05, u8, "step");
        put(o, 0x07, u8, "substep");
        put(o, 0x08, s16, "w08");
        put(o, 0x0a, u8, "b0a");
        put(o, 0x0b, u8, "b0b");
        put(o, 0x0c, u8, "b0c");
        put(o, 0x0d, u8, "b0d");
        put(o, 0x0f, u8, "b0f");
        put(o, 0x1d, u8, "b1d");
        put(o, 0x1e, s16, "w1e");
        put(o, 0x20, s16, "timer");
        put(o, 0x22, s16, "w22");
        put(o, 0x30, s32, "d30");
        put(o, 0x34, s32, "d34");
        put(o, 0x38, s32, "d38");
        put(o, 0x3c, s32, "d3c");
        put(o, 0x48, s16, "w48");
        put(o, 0x4a, s16, "w4a");
        put(o, 0x4c, s16, "w4c");
        put(o, 0x4e, s16, "w4e");
        put(o, 0x50, s16, "w50");
        put(o, 0x52, s16, "w52");
        put(o, 0x56, s16, "w56");
        put(o, 0x58, u16, "w58");
        put(o, 0x5c, s16, "w5c");
        put(o, 0x60, s32, "d60");
        put(o, 0x64, s32, "d64");
        put(o, 0x68, u8, "b68");
        put(o, 0x69, u8, "b69");
        put(o, 0x6a, u8, "b6a");
        put(o, 0x6b, u8, "b6b");
        put(o, 0x74, s16, "w74");
        put(o, 0x76, s16, "w76");
        put(o, 0x78, s16, "w78");
        put(o, 0x7a, s16, "w7a");
        put(o, 0x7c, s16, "velX");
        put(o, 0x84, s32, "d84");
        put(o, 0x88, s32, "d88");
        put(o, 0x8c, s32, "d8c");
        put(o, 0x90, s32, "d90");
        put(o, 0x94, s32, "d94");
        put(o, 0x98, s16, "w98");
        put(o, 0x9a, s16, "w9a");
        put(o, 0x9c, u8, "b9c");
        put(o, 0x9d, u8, "b9d");
        put(o, 0x9e, u8, "b9e");
        put(o, 0x9f, u8, "b9f");
        put(o, 0xa0, s32, "da0");
        put(o, 0xa4, u8, "ba4");
        put(o, 0xa5, u8, "ba5");
        put(o, 0xa6, u8, "ba6");
        put(o, 0xa7, u8, "ba7");
        put(o, 0xa8, s16, "wa8");
        put(o, 0xaa, s16, "waa");
        put(o, 0xac, s16, "wac");
        put(o, 0xae, s16, "wae");
        put(o, 0xb0, s16, "wb0");
        put(o, 0xb2, s16, "wb2");
        put(o, 0xb4, s16, "wb4");
        put(o, 0xb6, s16, "wb6");
        put(o, 0xb8, s16, "wb8");
        put(o, 0xba, s16, "wba");
        put(o, 0xbc, s16, "wbc");
        put(o, 0xbe, u8, "bbe");
        put(o, 0xbf, u8, "bbf");
        dtm.resolve(o, DataTypeConflictHandler.REPLACE_HANDLER);
        println("TObj redefinido con " + o.getNumDefinedComponents() + " campos");
    }
}
