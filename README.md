# Tombi! (Tomba!) – descompilación PSX con Ghidra

## Progreso

![Progreso](docs/progress.svg)

Mapas de funciones por programa (cada bloque es una función, coloreada por estado):

![MAIN0](docs/map_main0.svg)

![X000](docs/map_x000.svg)

Detalle en [docs/PROGRESS.md](docs/PROGRESS.md). Se regenera con `python tools/progress.py`.

Proyecto de ingeniería inversa del juego *Tombi!* / *Tomba!* (PlayStation) usando Ghidra.

## Requisitos
- Ghidra (11.x recomendado) y JDK 17+.
- Plugin [ghidra_psx_ldr](https://github.com/lab313ru/ghidra_psx_ldr) instalado.
- Tu propia copia del juego (no se incluye en el repo).

## Flujo de trabajo
1. Extrae el ejecutable principal del disco (raíz; en la versión PAL española es `SCES_013.31`, ver `notes/disc_layout.md`) a `game/`.
2. Crea un proyecto Ghidra en `ghidra/project/` e importa el PS-X EXE
   (loader *Sony PlayStation PS-X Executable*, MIPS R3000 LE).
3. Ejecuta el análisis automático y guarda tus hallazgos en `notes/`.
4. Los scripts propios van en `ghidra/scripts/`.

## Estructura
- `game/` – ejecutable/archivos del juego (ignorados por git)
- `ghidra/project/` – proyecto de Ghidra (ignorado por git)
- `ghidra/scripts/` – scripts de Ghidra
- `notes/` – mapas de memoria, nombres de funciones, estructuras
- `docs/` – documentación
- `tools/` – utilidades auxiliares

## Herramientas
- `tools/psxexe_info.py <exe>` – muestra PC, GP, dirección de carga y tamaño de la cabecera PS-X EXE.
- `ghidra/scripts/ExportC.java` – exporta el decompilado de las funciones `FUN_` a un `.c` (Script Manager, Java).
- `ghidra/scripts/ImportNames.java` – aplica nombres desde `notes/names_main0.csv`.
- `tools/export_functions.py` – convierte un export de Ghidra en `notes/functions_*.csv` (solo metadatos).
- `tools/progress.py` – recalcula `docs/PROGRESS.md` (progreso de la descompilación).
