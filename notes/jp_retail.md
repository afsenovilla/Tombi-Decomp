# JP retail (SLPS-01144): Ghidra + PAL names

Etapa 1 de la decompilacion del disco japones retail (`game_jp/disc/tomba_jp.bin`), igual que se hizo con PAL.

## Como reproducirlo

```
python3 -I tools/jp_syms.py extract                 # game_jp/ <- SLPS_011.44, SYSTEM/X00,X10,X11,X16,X17.BIN, INFO2.BIN
python3 -I tools/jp_syms.py prep /opt/gproj_jp/in   # nombres + inicios de funcion desde config/slps01144/symbol_addrs_*.txt
tools/ghidra_pipeline_jp.sh import                  # proyecto Ghidra en /opt/gproj_jp (fuera de git)
tools/ghidra_pipeline_jp.sh apply                   # crea funciones, aplica nombres, exporta funcs.csv + C en /opt/gproj_jp/out
python3 -I tools/jp_syms.py match /opt/gproj_jp/out --csv matches.csv --md sin_nombre.txt
```

- Ejecutable en `0x80010000` (cargador PSX), overlays y INFO2 como binario crudo en `0x800E9AF8` (`BinaryLoader`).
  Los 0x2B680 primeros bytes de cada `X<nn>.BIN` son el codigo de X00 (cuenta solo en la fila `x00`).
- Un solo INFO2 en el disco (sha1 `15ea8894...`, identico en SYSTEM/ y AREA05/08/11/16/17/18/19); se importa una vez.
- `ghidra/scripts/CreateFunctionsRange.java` (nuevo): en los overlays el codigo del area queda como datos tras el
  analisis; limpia cada funcion hasta la siguiente antes de desensamblar. `ExportFuncs.java`: direccion, tamano, nombre.
- Bug corregido de paso: los simbolos de `symbol_addrs_x00` caen fuera de los otros overlays por encima de `0x80115178`
  (parte de area de AREA00); `jp_syms.py prep` solo los usa por debajo.

## Pasos del emparejamiento (`tools/jp_syms.py match`)

0. `symfile`: nombres ya presentes en `config/slps01144/` con comentario `PAL <modulo> 0x<dir>` (salida de `xmatch.py`).
1. `exact`: secuencia de palabras enmascarada (jal/lui/offsets de load-store a cero, como `xmatch.py`); `exact-twin`
   si varios modulos PAL tienen el mismo codigo.
2. `fuzzy`: difflib sobre las palabras enmascaradas, tamano +-30 %, mejor >= 0.80 con margen 0.03.
3. `callgraph`: los `jal` de un par ya emparejado se emparejan en orden de llamada.
4. `similar`: Dice de 4-gramas de opcode/funct >= 0.70 con margen (variantes con constantes u offsets de struct distintos).

## Cobertura (funciones de >= 8 bytes segun Ghidra)

| programa | funciones | con nombre PAL | % | bytes | bytes con nombre |
|---|---:|---:|---:|---:|---:|
| slps01144 | 1581 | 1266 | 80.1% | 338588 | 294828 |
| x00 | 550 | 381 | 69.3% | 291004 | 180432 |
| info2 | 165 | 161 | 97.6% | 86504 | 84332 |
| x10 | 354 | 103 | 29.1% | 72936 | 34932 |
| x11 | 59 | 7 | 11.9% | 10580 | 2856 |
| x16 | 58 | 21 | 36.2% | 9072 | 4808 |
| x17 | 50 | 15 | 30.0% | 8668 | 4800 |

`x10..x17` cuentan solo la parte de area (>= `0x80115178`). Ghidra ve 2063 funciones en el exe, pero 482 miden menos de
8 bytes (etiquetas/saltos) y no se cuentan. Muchos `exact-twin` son codigo identico en varios modulos PAL (se toma el
primero; el nombre es el mismo de verdad solo si el codigo compartido lo es).

## Funciones sin nombre PAL, por cercania al PAL (mejor Dice contra cualquier funcion PAL con nombre)

| programa | variante (>=0.6) | debil (0.4-0.6) | sin contraparte (<0.4) |
|---|---:|---:|---:|
| slps01144 | 13 | 34 | 268 |
| x00 | 9 | 55 | 105 |
| x10 | 5 | 19 | 227 |
| x11 | 0 | 1 | 51 |
| x16 | 1 | 0 | 36 |
| x17 | 0 | 1 | 34 |
| info2 | 0 | 1 | 3 |

(Las de menos de 32 bytes no se comparan y van en la ultima columna.)

## Sin contraparte PAL (mejor parecido < 0.4, >= 200 bytes): 148 funciones

Lo interesante: codigo que el PAL no tiene. La descripcion sale del C de Ghidra: las filas escritas a mano estan
leidas; el resto es una pista automatica (llamadas con nombre, textos, GTE) y hay que confirmarla.
Notas: en `x00` el bloque `0x800EBF70..0x80112000` es codigo del jugador y de AREA00; en X10 (jungla) 227 funciones no
tienen parecido con ninguna funcion PAL (hipotesis por confirmar: codigo de objetos distinto o recortado en PAL).

| programa | direccion | bytes | que parece hacer |
|---|---|---:|---|
| slps01144 | 80015D1C | 632 | main: arranque (ResetGraph, SetVideoMode), distinto del PAL |
| slps01144 | 80018F7C | 240 | llama SetDispMask, ThreadCreate, stopBgm |
| slps01144 | 8001906C | 956 | pantalla de copyright "@1997 WHOOPEE CAMP" + arranque de bgm |
| slps01144 | 8001B808 | 328 | pantalla "NOW LOADING" |
| slps01144 | 8001BD78 | 544 | llama resolveAreaVariant, loadAreaSoundBank |
| slps01144 | 8001C054 | 288 | pantalla " GAME OVER " |
| slps01144 | 8001C174 | 268 | pantalla " GAME OVER " (2) |
| slps01144 | 8001C2D8 | 880 | llama SfxStopAll |
| slps01144 | 8001CA50 | 1024 | llama allocObjectLayer8 |
| slps01144 | 8001E388 | 208 | llama queueSoundCommand |
| slps01144 | 8001E910 | 256 | llama SsUtKeyOnV |
| slps01144 | 800200C4 | 1656 | evalua una tabla de entradas (tipo 0x90/0xB0) contra flags de pad DAT_8009ee6b: condiciones de input |
| slps01144 | 80020794 | 200 | sin pista automatica |
| slps01144 | 80024860 | 220 | sin pista automatica |
| slps01144 | 80024B78 | 5112 | renderizador GTE grande (rtpt/nclip): mallas 3D |
| slps01144 | 8002BA74 | 904 | llama computeCameraMatrix |
| slps01144 | 800323A8 | 268 | sin pista automatica |
| slps01144 | 800324B4 | 268 | sin pista automatica |
| slps01144 | 80042280 | 1520 | colision con plano (getCollisionPlaneAt, angleFromDelta) |
| slps01144 | 80049704 | 424 | dibujo de glifos de mensaje (drawMessageGlyphs, SetSprt): texto JP |
| slps01144 | 8004CB14 | 1060 | dibujo de primitivas SPRT/TILE semitransparentes (SetSemiTrans) |
| slps01144 | 80052CF0 | 564 | update por frame segun modo DAT_800a7b78 (casos 1-3,7 / 4,6): cámara/estado de juego |
| slps01144 | 8005C3BC | 204 | Psy-Q libreria (revision distinta) |
| slps01144 | 80064234 | 352 | Psy-Q libreria (revision distinta) |
| slps01144 | 8006CDFC | 312 | Psy-Q libreria (revision distinta) |
| slps01144 | 8006D768 | 216 | Psy-Q libreria (revision distinta) |
| slps01144 | 80070274 | 236 | Psy-Q libreria (revision distinta) |
| slps01144 | 800710D4 | 244 | Psy-Q libreria (revision distinta) |
| x00 | 800EE3AC | 204 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 800F0FAC | 1092 | sin pista automatica |
| x00 | 800F13F0 | 1988 | estado del jugador: seleccion de animacion en suelo |
| x00 | 800F1BB4 | 1780 | estado del jugador: seleccion de animacion en suelo (2) |
| x00 | 800F27AC | 2960 | estado del jugador: salto/caida (ObjGravityStep, playerUpdateFall), buffer local grande |
| x00 | 800F343C | 2060 | jugador: playerSetJumpRiseVelocity |
| x00 | 800F3C98 | 1020 | jugador: playerSetJumpRiseVelocity |
| x00 | 800F6684 | 328 | sin pista automatica |
| x00 | 800F6874 | 828 | sin pista automatica |
| x00 | 800F89B4 | 2076 | jugador: playerFlipOntoObjectPlace |
| x00 | 800F9530 | 1908 | jugador: playerFlipPlaceAndAccelerate |
| x00 | 800FA5A4 | 1212 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 800FBC9C | 216 | sin pista automatica |
| x00 | 800FCE48 | 1492 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 800FF590 | 1764 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 80100E8C | 1148 | llama ObjGravityStep |
| x00 | 80101308 | 844 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 80101F30 | 1620 | jugador: PlayerSetAnimIfChanged |
| x00 | 801043E0 | 668 | jugador: PlayerSetAnimIfChanged |
| x00 | 80106754 | 240 | jugador: PlayerSetAnimIfChanged |
| x00 | 80106844 | 680 | jugador: playerSetAngleFromSlope, playerDecelerateSpeed |
| x00 | 80106D28 | 760 | jugador: playerSetAngleFromSlope |
| x00 | 801072B4 | 1512 | jugador: playerSlopeSlideUpdateAnim, playerSlopeSlideAccelerate |
| x00 | 80107E44 | 464 | jugador: PlayerSetAnimIfChanged |
| x00 | 80108014 | 824 | jugador: PlayerSetAnimIfChanged |
| x00 | 801092DC | 308 | jugador: PlayerSetAnimIfChanged, playerSetAngleFromSlope |
| x00 | 80109C5C | 944 | sin pista automatica |
| x00 | 8010A00C | 1244 | sin pista automatica |
| x00 | 8010B374 | 1136 | llama spawnWeaponObject |
| x00 | 8010C244 | 332 | sin pista automatica |
| x00 | 8010C390 | 260 | sin pista automatica |
| x00 | 8010CABC | 568 | sin pista automatica |
| x00 | 8010D2D0 | 872 | jugador: PlayerSetAnimIfChanged, playerSetAnim |
| x00 | 8010E0F4 | 556 | llama ObjMotionStep |
| x00 | 8010E500 | 1080 | sin pista automatica |
| x00 | 80115AD0 | 416 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 801165C0 | 240 | sin pista automatica |
| x00 | 80116954 | 276 | sin pista automatica |
| x00 | 80116E08 | 360 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 80116F70 | 484 | sin pista automatica |
| x00 | 80117AF8 | 200 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 80118EE4 | 320 | sin pista automatica |
| x00 | 8011AA10 | 316 | sin pista automatica |
| x00 | 8011AF70 | 764 | llama trap; con division (trap) |
| x00 | 8011B250 | 656 | llama trap; con division (trap) |
| x00 | 8011BE2C | 424 | llama trap; con division (trap) |
| x00 | 8011C650 | 328 | sin pista automatica |
| x00 | 8011D190 | 232 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 8011E2FC | 632 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 801219A8 | 552 | jugador: PlayerSetAnimIfChanged, playerSetAnimIfChangedAt |
| x00 | 80121B5C | 220 | jugador: playerSetAnimIfChangedAt |
| x00 | 80121F7C | 204 | jugador: PlayerSetAnimIfChanged |
| x00 | 80135AC4 | 764 | handler de objeto (maquina de estados +4/+5/+6) |
| x00 | 80136650 | 284 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 801158B4 | 348 | sin pista automatica |
| x10 | 80116458 | 360 | sin pista automatica |
| x10 | 801165C0 | 248 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 8011679C | 368 | sin pista automatica |
| x10 | 80116E20 | 336 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 80117378 | 540 | sin pista automatica |
| x10 | 80117AF8 | 200 | sin pista automatica |
| x10 | 80118268 | 232 | sin pista automatica |
| x10 | 801190E8 | 400 | sin pista automatica |
| x10 | 80119438 | 248 | sin pista automatica |
| x10 | 80119558 | 340 | sin pista automatica |
| x10 | 80119788 | 252 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 8011A214 | 240 | sin pista automatica |
| x10 | 8011A5E4 | 260 | jugador: PlayerSetAnimIfChanged |
| x10 | 8011A6E8 | 272 | jugador: PlayerSetAnimIfChanged |
| x10 | 8011BE2C | 452 | jugador: PlayerSetAnimIfChanged |
| x10 | 8011BFF0 | 292 | sin pista automatica |
| x10 | 8011C81C | 264 | llama trap; con division (trap) |
| x10 | 8011CB3C | 244 | sin pista automatica |
| x10 | 8011CE64 | 260 | sin pista automatica |
| x10 | 8011D08C | 400 | llama trap; con division (trap) |
| x10 | 8011D6E0 | 256 | llama trap; con division (trap) |
| x10 | 8011D7EC | 216 | llama trap; con division (trap) |
| x10 | 8011D8C4 | 272 | llama trap; con division (trap) |
| x10 | 8011DAC0 | 220 | sin pista automatica |
| x10 | 8011DCC8 | 1404 | colision enemigo/objeto vs objeto (dif. de posiciones +0x40/+0x44, radios +0x6C) |
| x10 | 8011F318 | 360 | sin pista automatica |
| x10 | 801205B8 | 224 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 8012188C | 212 | sin pista automatica |
| x10 | 80121B5C | 336 | sin pista automatica |
| x10 | 801220CC | 400 | sin pista automatica |
| x10 | 80123744 | 208 | sin pista automatica |
| x10 | 8012394C | 340 | sin pista automatica |
| x10 | 8012453C | 432 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 801249FC | 240 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 80126D88 | 280 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 80127CC4 | 312 | sin pista automatica |
| x10 | 801285A0 | 808 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 801288C8 | 232 | sin pista automatica |
| x10 | 8012926C | 336 | handler de objeto (maquina de estados +4/+5/+6) |
| x10 | 80129538 | 340 | sin pista automatica |
| x10 | 8012B174 | 228 | sin pista automatica |
| x10 | 8012BF54 | 288 | sin pista automatica |
| x11 | 80115448 | 228 | handler de objeto (maquina de estados +4/+5/+6) |
| x11 | 80115934 | 220 | sin pista automatica |
| x11 | 80116348 | 632 | evento/puerta: arranca cutscene (DAT_800a7b7c=5, flag 8009ee58=2), toca +0x24 sprite |
| x11 | 801167A8 | 428 | sin pista automatica |
| x11 | 801169A8 | 892 | handler de objeto (maquina de estados +4/+5/+6) |
| x11 | 80116D24 | 228 | handler de objeto (maquina de estados +4/+5/+6) |
| x11 | 801174C4 | 208 | sin pista automatica |
| x11 | 801177FC | 364 | sin pista automatica |
| x11 | 8011797C | 380 | sin pista automatica |
| x11 | 80117D1C | 364 | sin pista automatica |
| x11 | 80117EE8 | 228 | sin pista automatica |
| x11 | 80118724 | 220 | handler de objeto (maquina de estados +4/+5/+6) |
| x16 | 801158F0 | 288 | sin pista automatica |
| x16 | 80115A80 | 496 | sin pista automatica |
| x16 | 80115D28 | 732 | handler de objeto con estado +4 (1/2/3), usa pos local (4 shorts) |
| x16 | 80116868 | 236 | handler de objeto (maquina de estados +4/+5/+6) |
| x16 | 80117594 | 228 | sin pista automatica |
| x17 | 80115828 | 488 | objeto estático/decor: scroll respecto a camara (_DAT_1f800176/186), selecciona sprite por subtipo 6/10 |
| x17 | 80117414 | 384 | sin pista automatica |
| x17 | 801179A8 | 336 | jugador: playerSetAnim |
| info2 | 800EC5A8 | 1280 | handler de objeto INFO2: maquina de estados por byte +5 (0x30, 0x63=99, ...) con flags +0x2E |
| info2 | 800ECCA4 | 320 | handler de objeto (maquina de estados +4/+5/+6) |
| info2 | 800EEEEC | 292 | handler de objeto (maquina de estados +4/+5/+6) |
