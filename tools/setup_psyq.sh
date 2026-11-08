#!/bin/bash
# Installs the original Psy-Q toolchain for tools/matchcheck.py (the reference check), outside the repo:
#   /opt/psyq/cc40/CC1PSX.EXE  Psy-Q 4.0 CC1PSX (Win32, run by Wine): the one that reproduces the game's code
#   /opt/psyq/46/BIN/ASPSX.EXE Psy-Q 4.6 ASPSX 2.86 (Win32, Wine)
#   /opt/psyq/new/             Psy-Q 3.6 DOS tools (CPPPSX/CC1PSX/ASPSX for DOSBox, used only with NATIVE_CPP=0)
# plus wine32 and dosbox. The binaries come from the public archive that decomp.me uses
# (github.com/mkst/esa releases); they are never committed. Idempotent.
set -e
URL=https://github.com/mkst/esa/releases/download/psyq-binaries
D=/opt/psyq/dl
if [ ! -x /usr/bin/wine ] || [ ! -x /usr/bin/dosbox ]; then
    dpkg --add-architecture i386
    apt-get update -q
    DEBIAN_FRONTEND=noninteractive apt-get install -y -q wine wine32 dosbox
fi
mkdir -p $D /opt/psyq/cc40 /opt/psyq/46/BIN /opt/psyq/new /opt/psyq/w
for v in 3.6 4.0 4.6; do
    [ -d $D/psyq$v ] && continue
    curl -fsSL --retry 3 -o $D/psyq$v.tar.gz $URL/psyq$v.tar.gz
    tar xzf $D/psyq$v.tar.gz -C $D && rm $D/psyq$v.tar.gz
done
cp -n $D/psyq4.0/*.EXE /opt/psyq/cc40/
cp -n $D/psyq4.6/*.EXE /opt/psyq/46/BIN/
cp -n $D/psyq3.6/*.EXE /opt/psyq/new/
echo "Psy-Q ready: /opt/psyq/cc40 (CC1PSX 4.0), /opt/psyq/46/BIN (ASPSX 2.86), /opt/psyq/new (DOS 3.6)"
