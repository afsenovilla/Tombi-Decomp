# Compilador (matching)

Probado: Psy-Q 4.6 (`CC1PSX.EXE`) bajo Wine -> **GCC 2.95.2 (SN Systems 4.0.0030)**.
Funciona (`tools/psyq_cc.sh`), pero la salida NO coincide con la del juego
(p.ej. `MulCos`: el juego usa `lui/addu/lh` con la macro `lh $2,sym($4)`; 2.95.2 emite `%hi/%lo`
y reordena instrucciones). El juego (1999) se compiló con un GCC mas antiguo (2.7.x / 2.8.x),
SDK ~4.0-4.3. Psy-Q 4.6 y 4.7 no sirven (4.7 solo trae libs/headers, sin compilador).
Pendiente: SDK anterior (3.x/4.0-4.3).
