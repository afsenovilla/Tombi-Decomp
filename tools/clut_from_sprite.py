#!/usr/bin/env python3
"""Derive a 16-colour CLUT from a colour sprite sheet (e.g. TCRF's) and the 4 bpp SYSTEM page it was drawn from.

  tools/clut_from_sprite.py SHEET.png [--page A07600] [--max-err 0.10]

Each opaque frame (8-connected component > 150 px) of SHEET is aligned (with and without x flip) to the page's
non-zero pixels; frames whose silhouette mismatch is under --max-err vote, per page index, for the sheet colour at
that pixel. Prints the winner per index with its agreement and the CLUT as 16 15-bit hex values (index 0 = 0000)."""
import sys, os, argparse
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import area000 as A, civ_sheets as C
from PIL import Image
import numpy as np
from collections import defaultdict, Counter
ap=argparse.ArgumentParser(); ap.add_argument('sheet'); ap.add_argument('--page',default='A07600')
ap.add_argument('--max-err',type=float,default=0.10); a=ap.parse_args()
im=np.array(Image.open(a.sheet).convert('RGBA'))
im=np.array(Image.open(sys.argv[1]).convert('RGBA'))
rgb=im[:,:,:3].astype(int); op=im[:,:,3]>=128
H,W=op.shape; lab=np.zeros((H,W),int); comps=[]
for y in range(H):
    for x in range(W):
        if op[y,x] and not lab[y,x]:
            n=len(comps)+1; st=[(y,x)]; lab[y,x]=n; ys=[];xs=[]
            while st:
                cy,cx=st.pop(); ys.append(cy); xs.append(cx)
                for dy in (-1,0,1):
                    for dx in (-1,0,1):
                        ny,nx=cy+dy,cx+dx
                        if 0<=ny<H and 0<=nx<W and op[ny,nx] and not lab[ny,nx]: lab[ny,nx]=n; st.append((ny,nx))
            comps.append((min(ys),min(xs),max(ys)+1,max(xs)+1,n,len(ys)))
comps=[c for c in comps if c[5]>150]
d=np.frombuffer(A.read_resource('SYSTEM/'+a.page),np.uint8); S=C.W[a.page]*2; PH=len(d)//S
b=d[:PH*S].reshape(PH,S)[:, :128]; p=np.zeros((PH,256),np.uint8); p[:,0::2]=b&15; p[:,1::2]=b>>4
nz=p>0
votes=defaultdict(Counter); used=0; errs=[]
for y0,x0,y1,x1,L,n in comps:
    m=(lab[y0:y1,x0:x1]==L); h,w=m.shape; best=None
    for fl in (0,1):
        M=m[:,::-1] if fl else m
        for oy in range(0,PH-h+1):
            for ox in range(0,256-w+1):
                s=int((nz[oy:oy+h,ox:ox+w]!=M).sum())
                if best is None or s<best[0]: best=(s,fl,ox,oy)
    s,fl,ox,oy=best; errs.append(round(s/n,3))
    if s>a.max_err*n: continue
    used+=1
    Cc=rgb[y0:y1,x0:x1]; M=m
    if fl: Cc=Cc[:,::-1]; M=M[:,::-1]
    t=p[oy:oy+h,ox:ox+w]; k=M&(t>0)
    for i,c in zip(t[k],map(tuple,Cc[k])): votes[int(i)][c]+=1
print('frames',len(comps),'aligned',used,'mismatch',sorted(errs)[:10])
out=[0]*16; missing=[]
for i in range(1,16):
    if not votes[i]: missing.append(i); continue
    (r,g,b),k=votes[i].most_common(1)[0]; tot=sum(votes[i].values())
    q=lambda c: round(c*31/255)          # nearest 5-bit level (each channel within ~4/255 of the sheet colour)
    out[i]=q(r) | q(g)<<5 | q(b)<<10
    print(i,(r,g,b),tot,round(k/tot,3))
print('no data',missing)
print(' '.join('%04X'%v for v in out))
