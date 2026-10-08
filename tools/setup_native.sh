#!/bin/bash
# Installs the fast native toolchain (no Wine/DOSBox) for tools/ncheck.py:
#  - old-gcc gcc-2.7.2-psx (cc1 + cpp) from decompals/old-gcc -> /opt/oldgcc/gcc-2.7.2-psx
#    and gcc-2.8.1-psx (files with a `// CC gcc-2.8.1` header)  -> /opt/oldgcc/gcc-2.8.1-psx
#  - maspsx (ASPSX emulator)                                   -> /opt/maspsx
#  - binutils-mipsel-linux-gnu (as/objcopy/objdump)
set -e
for v in gcc-2.7.2-psx gcc-2.8.1-psx; do
  mkdir -p /opt/oldgcc/$v
  curl -fsSL https://github.com/decompals/old-gcc/releases/download/0.17/$v.tar.gz | tar xz -C /opt/oldgcc/$v
done
[ -d /opt/maspsx ] || git clone -q https://github.com/mkst/maspsx.git /opt/maspsx
command -v mipsel-linux-gnu-as >/dev/null || apt-get install -y binutils-mipsel-linux-gnu
# PSY-Q headers used by the functions ported from psx_tomba (Sony copyright: not stored in this repo)
cd "$(dirname "$0")/.."
if [ ! -d include/tomba/psyq ]; then
  [ -d /tmp/psx_tomba ] || git clone -q --depth 1 https://github.com/hansbonini/psx_tomba /tmp/psx_tomba
  cp -r /tmp/psx_tomba/include/psyq include/tomba/psyq
fi
echo "ready: python3 tools/ncheck.py src/<Name>.c"
