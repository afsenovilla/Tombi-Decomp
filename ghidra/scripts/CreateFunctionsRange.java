// Like CreateFunctions but for overlays whose code was left undefined/data by auto analysis: clears the whole span of each
// function (up to the next listed start) before disassembling, so the delay slots and fall-through are present.
// Arg: file with one hex address per line (ascending not required).
// @category Tombi
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.TreeSet;

public class CreateFunctionsRange extends GhidraScript {
    @Override
    protected void run() throws Exception {
        TreeSet<Long> starts = new TreeSet<>();
        BufferedReader in = new BufferedReader(new FileReader(getScriptArgs()[0]));
        String line;
        while ((line = in.readLine()) != null) { line = line.trim(); if (!line.isEmpty()) starts.add(Long.parseLong(line, 16)); }
        in.close();
        int ok = 0, fail = 0, had = 0;
        for (long s : starts) {
            Address a = toAddr(s);
            if (getFunctionAt(a) != null) { had++; continue; }
            Long nx = starts.higher(s);
            long end = nx == null ? s + 0x400 : nx;
            if (end - s > 0x4000) end = s + 0x4000;
            try { clearListing(a, toAddr(end - 1)); } catch (Exception e) { }
            disassemble(a);
            if (createFunction(a, null) != null) ok++; else { fail++; println("Fallo en " + a); }
        }
        println("Funciones creadas: " + ok + ", ya existian: " + had + ", fallos: " + fail);
    }
}
