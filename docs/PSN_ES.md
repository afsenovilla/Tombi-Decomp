# Versiones PS1 Classics (PSN)

| Región | Código PSN | Título en cabecera | Disco | Ejecutable |
|---|---|---|---|---|
| Japón | **NPJJ00566** | オレっ！トンバ | SLPS-01144 | `SLPS_011.44` (mismo id que el retail) |
| EE. UU. | **NPUJ94236** | Tomba! | SCUS-94236 | `SCUS_942.36` |

Cada versión trae `EBOOT.PBP` (el disco), `KEYS.BIN` (clave de versión de 16 bytes, una por título) y
`DOCUMENT.DAT` (el manual digital). Los ficheros del juego no se suben nunca; la CI los descarga de los
secretos `GAME_FILES_URL_PSN_JP` / `GAME_FILES_URL_PSN_USA` (job `report-psn` en `.github/workflows/report.yml`).

## Extracción: `tools/psn_eboot.py`

```
python3 tools/psn_eboot.py info    EBOOT.PBP KEYS.BIN            # funciona
python3 tools/psn_eboot.py extract EBOOT.PBP KEYS.BIN disc.bin   # experimental, aún no funciona
```

- `DATA.PSAR` es una imagen `PSISOIMG0000` de un solo disco.
- `PSAR+0x400` es un contenedor PGD (DRM tipo 1). Se descifra con sceDrmBBCipher tipo 1 (claves KIRK 0x39/0x63)
  y la clave de KEYS.BIN. **Funciona en las dos versiones.** Contiene el id del disco, la TOC, el título y el
  índice de bloques (+0x3C00, entradas de 32 bytes).
- Los bloques empiezan en `PSAR+0x100000`. Cada uno guarda 16 sectores raw (0x9300 bytes). En las dos versiones
  todos los bloques están comprimidos y todos los flags valen 1.
- **Pendiente:** ni el LZRC de PSP ni el LZ de Sony de Hykem (RPCS3 `lz.cpp`) descomprimen estos bloques; los dos
  fallan en el primer match. Todos los bloques, JP y USA, empiezan con los mismos bytes. Como las dos versiones usan
  claves distintas, los datos parecen sin cifrar, y la diferencia tiene que estar en la variante del descompresor o
  en algún paso previo. El siguiente paso es compararlo con el `lz.c` de psxtract, al que esta sesión no pudo acceder.
- `DOCUMENT.DAT` también es PGD, pero la clave del título no lo abre (hace falta la clave fija de manuales), así que
  no se ha examinado.

## Lo que se puede comparar sin la imagen

| | Retail | PSN |
|---|---|---|
| Sectores JP | 123057 | 123248 (7703 bloques); lead-out 27:25:18 |
| Sectores USA | (sin copia local) | 119136 (7446 bloques); lead-out 26:30:30 |
| Pistas | 1 de datos | 1 de datos |

La imagen PSN japonesa tiene unos 186 sectores (~430 KB) más que nuestro volcado retail. Puede ser relleno, otro
máster o ficheros extra, y es la pista principal cuando funcione la extracción. La comparación fichero a fichero,
los parches del ejecutable y cualquier rastro de los pueblos cortados de la Civilización (AREA11/16/17/18 s0)
dependen del descompresor. Por ahora no hay nada nuevo para `CONTENIDO_RECORTADO.md`.
