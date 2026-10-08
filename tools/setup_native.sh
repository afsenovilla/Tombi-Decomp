#!/bin/bash
# Installs the fast native toolchain (no Wine/DOSBox) for tools/ncheck.py:
#  - old-gcc gcc-2.7.2-psx (cc1 + cpp) from decompals/old-gcc -> /opt/oldgcc/gcc-2.7.2-psx
#  - maspsx (ASPSX emulator)                                   -> /opt/maspsx
#  - binutils-mipsel-linux-gnu (as/objcopy/objdump)
set -e
mkdir -p /opt/oldgcc/gcc-2.7.2-psx
curl -fsSL https://github.com/decompals/old-gcc/releases/download/0.17/gcc-2.7.2-psx.tar.gz | tar xz -C /opt/oldgcc/gcc-2.7.2-psx
[ -d /opt/maspsx ] || git clone -q https://github.com/mkst/maspsx.git /opt/maspsx
command -v mipsel-linux-gnu-as >/dev/null || apt-get install -y binutils-mipsel-linux-gnu
echo "ready: python3 tools/ncheck.py src/<Name>.c"
