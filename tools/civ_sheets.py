#!/usr/bin/env python3
"""Explanatory sheets of the cut top-down towns of the Land of Civilization (AREA11/16/17 section 0), ES or EN.
See docs/CIVILIZATION.md (EN) / docs/CIVILIZACION.md (ES), section "The cut towns, one by one".

  tools/civ_sheets.py [--lang es|en|all] [--area 11|16|17|all] [--sheet esquema|explicado|perspectiva|all] [--out DIR]

Sheets (file names; *_en.png for English):
  civ_esquema_aNN.png       evidence-only schematic: A = AREA08 s0 (real picture) with the code's marks, B = the cut town
                            at the same scale on grey, real resident sprites, houses, patrols, props, exits; legend,
                            what each resident says, and where every element comes from (I0NFO2 / MAIN0 addresses)
  civ_explicado_aNN.png     flat plan with drawn house shapes, sprites, exit arrows and a side legend
  civ_perspectiva_aNN.png   (EN: civ_perspective_aNN_en.png) the same data in perspective; only the house height is invented
  civ_plan_aNN_1x[_transparent].png  (--sheet plan1x, no text) panel B alone at 1:1, grey ground or transparent;
                            civ_plan_aNN_1x_legend.png the same plan unscaled with an English legend panel on the right

Everything is read from YOUR extracted disc (game/disc/fs, or TOMBI_FS) through tools/topdown.py / tools/area000.py:
the AREA08 s0 picture, the I0NFO2 NPC sets / house rectangles, the SYS reload pages SYSTEM/A06100, A07600, A08000
(the surviving resident sprites) and the real resident palettes (16-colour rows of the areas' CLUTnn.GAM, see PAL_NOTE). The PNGs contain disc art: never commit them.
The resident lines are short glosses of the Japanese text (the PAL slots hold "···??"); which sprite stands for which
resident subtype, and which CLUT row each subtype uses, are guesses except the AREA16 strongman (row 53). Needs Pillow and the DejaVu fonts.
"""
import argparse, copy, math, os, struct, sys
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import area000 as A
import topdown as T
from PIL import Image, ImageDraw, ImageFont, ImageFilter

FONT_DIR = os.environ.get("CIV_FONT_DIR", "/usr/share/fonts/truetype/dejavu")
FB = os.path.join(FONT_DIR, "DejaVuSans-Bold.ttf"); FR = os.path.join(FONT_DIR, "DejaVuSans.ttf")
F = lambda s, b=False: ImageFont.truetype(FB if b else FR, s)

# ------------------------------------------------------------------ surviving sprite pages (4 bpp part, first 64 words)
W={'A06100':448,'A07600':320,'A08000':448}
def page(n):
    d=A.read_resource('SYSTEM/'+n); w=W[n]
    return [[(d[y*w*2+x//2]>>((x&1)*4))&15 for x in range(256)] for y in range(256)]
def _clip_figure(p, bb):
    """Drop what the 2-pixel joining pulled in across a fully empty row or column (the atlas's marker tiles, "ooo"
    rings and X boxes next to a figure): keep the band with the most non-zero texels, rows then columns."""
    x0, y0, x1, y1 = bb
    for axis in (0, 1):
        n = (y1 - y0 + 1) if axis == 0 else (x1 - x0 + 1)
        filled = [sum(1 for x in range(x0, x1 + 1) if p[y0 + i][x]) if axis == 0 else sum(1 for y in range(y0, y1 + 1) if p[y][x0 + i]) for i in range(n)]
        bands, cur = [], None
        for i, f in enumerate(filled):
            if f and cur is None: cur = [i, i, 0]
            if f: cur[1] = i; cur[2] += f
            elif cur is not None: bands.append(cur); cur = None
        if cur is not None: bands.append(cur)
        a, b, _ = max(bands, key=lambda t: t[2])
        if axis == 0: y0, y1 = y0 + a, y0 + b
        else: x0, x1 = x0 + a, x0 + b
    return (x0, y0, x1, y1)


def lost_marker(scale=1.0):
    """Stand-in for a resident whose drawing is on a texture page that did not survive (its w1e page is not the
    reloaded SYS page): a grey capsule with a question mark. Not a drawing of the character."""
    w, h = int(18 * scale), int(36 * scale)
    im = Image.new('RGBA', (w, h), (0, 0, 0, 0)); d = ImageDraw.Draw(im)
    d.rounded_rectangle([0, 0, w - 1, h - 1], radius=max(2, w // 2), fill=(150, 150, 150, 140), outline=(90, 90, 90, 255), width=max(1, int(scale)))
    d.text((w / 2, h / 2), '?', font=F(max(8, int(14 * scale)), True), fill=(255, 255, 255, 255), anchor='mm')
    return im


# What decides a resident's drawing (see SUB_EVIDENCE and docs/CIVILIZATION.md "Which drawing each subtype is"):
# the actor def's w1e is the texture page the parts are drawn from (page = part.tpage + w1e; the parts use relative
# page 0 in the retail top-down tables). The surviving SYS reload pages sit at VRAM x 0x40.., so only w1e 1 (page x 0x40,
# the 4 bpp sprite area of SYSTEM/A06100 / A07600 / A08000) can be drawn from them; w1e 0x0A/0x0B/0x10/0x11/0x08 are pages
# that no surviving file fills. Within page 1, the retail tables (AREA05/08) pack frames in frame-id order, row by row,
# and each subtype's scripts use a consecutive frame range (A16: sub 0 from 30, sub 1 from 60, sub 2 from 90).
SUB_EVIDENCE = {
    11: {0: 'proven page (w1e 1); hard-hat worker = first frames 0..29, consistent with id order'},
    16: {0: 'proven page (w1e 1); consistent: frames from 30, first group of the page', 1: 'proven page (w1e 1); consistent: frames from 60, second group',
         2: 'proven page (w1e 1); consistent: frames from 90, third group; palette row 53 pixel-exact with TCRF', 3: 'drawn from page w1e 0x11: lost'},
    17: {0: 'proven page (w1e 1); spiky-haired resident, consistent with id order'},
}


def comps(p):
    m=[[1 if p[y][x] else 0 for x in range(256)] for y in range(256)]
    seen=[[0]*256 for _ in range(256)]; out=[]
    for y in range(256):
        for x in range(256):
            if m[y][x] and not seen[y][x]:
                st=[(x,y)]; seen[y][x]=1; bb=[x,y,x,y]; n=0
                while st:
                    cx,cy=st.pop(); n+=1
                    bb=[min(bb[0],cx),min(bb[1],cy),max(bb[2],cx),max(bb[3],cy)]
                    for dx in (-2,-1,0,1,2):
                        for dy in (-2,-1,0,1,2):
                            nx,ny=cx+dx,cy+dy
                            if 0<=nx<256 and 0<=ny<256 and m[ny][nx] and not seen[ny][nx]:
                                seen[ny][nx]=1; st.append((nx,ny))
                if n>60 and bb[3]-bb[1]>14: out.append(_clip_figure(p, tuple(bb)))
    return out

# ------------------------------------------------------------------ real palettes: 16-colour rows of the areas' CLUT files
# Female Stonetown Resident (AREA16 subtype 0): custom CLUT derived pixel-exact from the TCRF sheet with
# tools/clut_from_sprite.py (24 frames, 0 silhouette mismatches, 15/15 indices 100 % agreement); not found on any disc.
CUSTOM_CLUTS = {'woman16': [0x0000, 0x0000, 0x7FFF, 0x3A78, 0x25D2, 0x194F, 0x5233, 0x41B0,
                            0x314D, 0x1D0C, 0x1677, 0x0DD1, 0x096E, 0x1D4C, 0x10EA, 0x21D2]}


def clut_row(fn, row):
    """16 RGB colours of row `row` (16 u16 entries, file offset row*32) of an area CLUTnn.GAM: a real in-game palette.
    fn 'custom' takes `row` as a CUSTOM_CLUTS key instead."""
    if fn == 'custom':
        return [((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31) for v in CUSTOM_CLUTS[row]]
    d = A.read_resource(fn)
    return [((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31) for v in struct.unpack_from('<16H', d, row * 32)]


def sub_pal(C, sub):
    """(file, row) of the CLUT row drawn for resident subtype `sub`. The code does not say it: a resident's CLUT is in
    the parts of its lost SPR table (the actor inits clear b0d, so w08 is not used). Rows 48..54 of these files are the
    villager palettes (44..47 and 60..63 are flash/tint variants of the same row block). Rows 32 apart (4/36/68 in
    AREA11/CLUT02, 16/48/80 and 21/53 in AREA16/CLUT07) belong to the same set of resident palettes."""
    r = C['pals'][sub]
    return r if isinstance(r, tuple) else (C['clut'], r)


PAL_NOTE = {
    'es': {16: 'Subtipos 0-2 dibujados de la página que sobrevive (w1e 1, probado); el orden de fotogramas da tipo 0 = Female Stonetown Resident, 1 = el chico, 2 = Male Stonetown Resident (coherente). Paletas reales de AREA16/CLUT07: fila 53 para el subtipo 2 (píxel a píxel con TCRF), fila 37 para el subtipo 1, el chico (confirmada por los colores de TCRF), y para el subtipo 0 un CLUT propio derivado píxel a píxel de la hoja de TCRF (no está en ningún disco). Subtipo 3 (w1e 0x11): página perdida, cápsula gris',
           11: 'Sólo el subtipo 0 se dibuja de la página que sobrevive (w1e 1, probado): Lumberjack Factory Worker, paleta real AREA11/CLUT02 fila 4 (confirmada por colores de TCRF). Subtipos 1-4 (w1e 0x0A/0x0B/0x10/0x11): página perdida, cápsula gris',
           17: 'Sólo el subtipo 0 se dibuja de la página que sobrevive (w1e 1, probado), paleta real AREA17/CLUT03 fila 1 (según una captura del juego). Subtipos 1-4 (w1e 0x0A/0x0B/0x10/0x08): página perdida, cápsula gris'},
    'en': {16: 'Subtypes 0-2 are drawn from the surviving page (w1e 1, proven); frame order gives 0 = Female Stonetown Resident, 1 = the boy, 2 = Male Stonetown Resident (consistent). Real palettes AREA16/CLUT07: row 53 for subtype 2 (pixel-exact with TCRF), row 37 for subtype 1, the boy (confirmed by TCRF colours), and for subtype 0 a custom CLUT derived pixel-exact from the TCRF sheet (not found on any disc). Subtype 3 (w1e 0x11): page lost, grey capsule',
           11: 'Only subtype 0 is drawn from the surviving page (w1e 1, proven): Lumberjack Factory Worker, real palette AREA11/CLUT02 row 4 (confirmed by TCRF colours). Subtypes 1-4 (w1e 0x0A/0x0B/0x10/0x11): page lost, grey capsule',
           17: 'Only subtype 0 is drawn from the surviving page (w1e 1, proven), real palette AREA17/CLUT03 row 1 (matched to a retail screenshot). Subtypes 1-4 (w1e 0x0A/0x0B/0x10/0x08): page lost, grey capsule'}}


# ------------------------------------------------------------------ per-town data (ES) and the flat plan sheet
SC=1.5
CIVI='Ciudad de civilización (AREA18)'
AREAS={
16:dict(atlas='A07600',clut='AREA16/CLUT07',pals={0:('custom','woman16'),1:37,2:53},sub={0:9,1:28,2:34,3:None},
 title='AREA16 sección 0 — pueblo recortado de la gente de piedra',
 cap='Pueblo recortado de la gente de piedra (Ciudad de Piedra, AREA16, vista desde arriba): lo que el código coloca; el fondo dibujado se perdió.',
 exits=[((-140,520),'L','→ '+CIVI),((110,320),'D','→ Fábrica (AREA11 s2), puerta'),((220,60),'U','→ AREA16 s1 (también recortada)')],
 legend=[
 'Vecino que levanta algo: «¡Aúpa... YA!»',
 'Vecino que levanta algo: «¡Hop... AAAH!»',
 'Vecino junto al ascensor: «Esto pegado a la pared es un ascensor... pero no se mueve»',
 'Casa 1: «¿No puedes entrar en los otros pueblos? Ni lo sé ni me importa» → luego habla del engranaje y del generador',
 'Patrulla vertical: «La gente de los otros pueblos...» → «¿Un engranaje que no se rompa? Si fuera de hierro...»',
 'Patrulla diagonal: «¿Primera vez en este pueblo?» → «Hace mucho intenté meterme en el río...»',
 'Vecino: «¿Debería entrar...?» → «Hermano, vencerás al Cerdiablo que hechizó esta tierra, ¿verdad?»',
 'Patrulla diagonal (sale tras el 1er avance): «Abrí en secreto la puerta entre el pueblo de hierro y el de madera»',
 'Casa 4, artesano: «Mmm... quiero fabricar algo» / «Ahora que lo pienso...»',
 'Vecino (antes del 1er avance): no habla, activa una bandera',
 'Vecino (antes del 1er avance): no habla, activa una bandera',
 'Casa 2: sin diálogo en el código',
 'Al final: el chico que vio a la bruja Mizuno (texto PAL #229), inicia el evento 0x67'],
 ),
11:dict(atlas='A06100',clut='AREA11/CLUT02',pals={0:4},sub={0:0,1:None,2:None,3:None,4:None},
 title='AREA11 sección 0 — pueblo recortado de la gente de madera',
 cap='Pueblo recortado de la gente de madera (junto a la Fábrica, AREA11, vista desde arriba): lo que el código coloca; el fondo dibujado se perdió.',
 exits=[((560,410),'R','→ '+CIVI),((231,70),'U','→ Fábrica (AREA11 s1)')],
 legend=[
 'Casa 1: «...Ahora puedo hacer algo muy interesante...» (texto PAL #227)',
 'Casa 1: no habla, activa una bandera',
 'Casa 2: «No funciona, pero esta máquina es de hierro... me vendría bien»',
 'Patrulla diagonal: «En esta choza vive una adivina, pero pone una condición»',
 'Patrulla horizontal: «La madera es lo mejor; la piedra y el hierro son fríos»',
 'Patrulla horizontal: «No funciona, pero esta máquina es de hierro...»',
 'Vecino: «Bajo el suelo hay un laberinto; no te metas sin cuidado»',
 'Patrulla vertical: «A esa choza sólo se llega por debajo del suelo»',
 'Patrulla horizontal: «¿Un engranaje? De madera se rompería» → «Tengo sed, iré a por un coco» → «El Pueblo Karakuri se hundió»',
 'Patrulla vertical: «Al taller se entra por arriba; ve a echar un vistazo»',
 'Vecino: «Este cartel es viejo y peligroso» → «Siempre tan lleno de energía»',
 'Casa 3, sentado: probablemente el constructor de balsas («¿Quieres una balsa? Tráeme madera del Árbol Sabio»); el código no le da diálogo'],
 ),
17:dict(atlas='A08000',clut='AREA17/CLUT03',pals={0:1},sub={0:2,1:None,2:None,3:None,4:None},
 title='AREA17 sección 0 — pueblo recortado de los Hombres de hierro',
 cap='Pueblo recortado de los Hombres de hierro (Ciudad de hierro, AREA17, vista desde arriba): lo que el código coloca; el fondo dibujado se perdió.',
 exits=[((223,575),'D','→ '+CIVI),((223,30),'U','→ interior de la Ciudad de hierro (AREA17 s1)')],
 legend=[
 'Casa 4: «He oído que arreglaste la torre del reloj de piedra»',
 'Casa 4: «¡Buen material! Espera un momento...»',
 'Casa 5, el herrero: «¿Necesitas algo? ¿Un engranaje de hierro?» → te da el engranaje (objeto 0x3E)',
 'El fabricante de bombas: «No he visto tu cara...» → «Para hacer bombas necesito electricidad»',
 'Casa 1: sin diálogo en el código',
 'Vecino: «¿Quieres subir? Entra en esta choza»',
 'Patrulla horizontal: «Bienvenido, éste es el pueblo de los Hombres de hierro»',
 'Casa 2: «El hierro es resistente y duradero» → «¿Arreglaste hasta el generador?»',
 'Patrulla diagonal: «¿Por qué siempre andamos peleados?»',
 'Patrulla horizontal: «Todos con su madera y su piedra...» → «Madera, piedra y hierro: lo importante es el equilibrio»',
 'Vecino: «Tómame como referencia y baja recto para salir»',
 'Casa 3: «Antes el ascensor funcionaba con máquinas»',
 'Patrulla horizontal: «Sube al ascensor y por el camino del medio llegas al Castillo de hierro»',
 'Patrulla diagonal: «Ese viejo Gantetsu no me deja ir al Castillo» → «Perdona que me burlara de ti»',
 'Patrulla diagonal: «Esta tierra también está hechizada por los Cerdiablos»',
 'Patrulla diagonal: «Si buscas algo de hierro, ve a la choza del ascensor» → «¿Sabes nadar bien?»'],
 ),
}
def wrap(dr,t,f,w):
    out=[];cur=''
    for word in t.split():
        if dr.textlength(cur+' '+word,font=f)>w and cur: out.append(cur);cur=word
        else: cur=(cur+' '+word).strip()
    return out+[cur]
def arrow(dr,x,y,d,col):
    L=50;h=18
    dx,dy={'L':(-1,0),'R':(1,0),'U':(0,-1),'D':(0,1)}[d]
    x2,y2=x+dx*L,y+dy*L
    dr.line([x,y,x2,y2],fill=col,width=8)
    px,py=-dy,dx
    dr.polygon([(x2+dx*h,y2+dy*h),(x2+px*h,y2+py*h),(x2-px*h,y2-py*h)],fill=col)
    return x2+dx*h,y2+dy*h
def explicado_es(area,out):
    C=AREAS[area]; var=T.CUT[area]
    actors,props=T.npc_set(var); rects=T.trigger_rects(area)
    p=page(C['atlas']); cs=comps(p)
    def sprite(sub):
        if C['sub'][sub] is None: return lost_marker(SC)
        pal=clut_row(*sub_pal(C,sub))
        b=cs[C['sub'][sub]]; w,h=b[2]-b[0]+1,b[3]-b[1]+1
        im=Image.new('RGBA',(w,h),(0,0,0,0)); px=im.load()
        for y in range(h):
            for x in range(w):
                v=p[b[1]+y][b[0]+x]
                if v: px[x,y]=pal[v]+(255,)
        return im.resize((int(w*SC),int(h*SC)),Image.NEAREST)
    # world bounds
    xs=[a['x'] for a in actors]+[q['x'] for l in props for q in l]+[r[0] for r in rects]+[r[2] for r in rects]+[e[0][0] for e in C['exits']]
    ys=[a['y'] for a in actors]+[q['y'] for l in props for q in l]+[r[1] for r in rects]+[r[3] for r in rects]+[e[0][1] for e in C['exits']]
    x0,x1,y0,y1=min(xs)-110,max(xs)+110,min(ys)-110,max(ys)+70
    TW,TH=int((x1-x0)*SC),int((y1-y0)*SC)
    # reference
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]])
    ref=ref.resize((int(rw*SC),int(rh*SC)),Image.NEAREST)
    LW=820; M=30; top=150
    W=M+ref.width+M+TW+M+LW+M; H=top+max(ref.height,TH)+200
    im=Image.new('RGB',(W,H),(250,247,240)); dr=ImageDraw.Draw(im)
    dr.text((M,25),C['title'],font=F(44,True),fill=(30,30,30))
    dr.text((M,85),C['cap'],font=F(25),fill=(60,60,60))
    im.paste(ref,(M,top))
    dr.text((M,top+ref.height+12),'Referencia: una sección vista desde arriba que SÍ existe',font=F(24,True),fill=(30,30,30))
    dr.text((M,top+ref.height+44),'(Lago Baco, AREA08 s0), a la misma escala',font=F(24),fill=(30,30,30))
    ox=M+ref.width+M
    dr.rectangle([ox,top,ox+TW,top+TH],fill=(206,196,168),outline=(120,110,90),width=3)
    for gx in range(0,TW,int(32*SC)):
        dr.line([ox+gx,top,ox+gx,top+TH],fill=(196,186,158))
    for gy in range(0,TH,int(32*SC)):
        dr.line([ox,top+gy,ox+TW,top+gy],fill=(196,186,158))
    def P(x,y): return ox+(x-x0)*SC, top+(y-y0)*SC
    roof=[(150,70,50),(120,90,60),(90,90,110),(140,60,70),(110,80,50)]
    for i,(a,b,c,d) in enumerate(rects):
        X0,Y0=P(a,b);X1,Y1=P(c,d); col=roof[i%5]
        dr.rectangle([X0,Y0+(Y1-Y0)*0.35,X1,Y1],fill=(170,150,120),outline=(60,40,30),width=3)
        dr.polygon([(X0-8,Y0+(Y1-Y0)*0.45),((X0+X1)/2,Y0),(X1+8,Y0+(Y1-Y0)*0.45)],fill=col,outline=(50,30,20))
        for k in range(1,6):
            yy=Y0+(Y1-Y0)*0.45*k/6
            dr.line([((X0+X1)/2-(X1-X0+16)/2*k/6,yy),((X0+X1)/2+(X1-X0+16)/2*k/6,yy)],fill=tuple(max(0,v-40) for v in col),width=2)
        dr.rectangle([(X0+X1)/2-12,Y1-34,(X0+X1)/2+12,Y1],fill=(80,50,30))
        dr.text((X0+6,Y1-30),'Casa %d'%(i+1),font=F(22,True),fill=(255,255,255),stroke_width=2,stroke_fill=(0,0,0))
    for l in props[:1]:
        for q in l:
            X,Y=P(q['x'],q['y']); dr.rounded_rectangle([X-9,Y-9,X+9,Y+9],4,fill=(140,110,70),outline=(70,50,30),width=2)
    for i,a in enumerate(actors):
        if a['beh']:
            bx0,by0,bx1,by1=a['bx0'],a['by0'],a['bx1'],a['by1']
            if var==0x0E: bx0,by0,bx1,by1=a['x']+bx0,a['y']+by0,a['x']+bx1,a['y']+by1
            A_=P(bx0,by0);B_=P(bx1,by1)
            dr.rectangle([min(A_[0],B_[0]),min(A_[1],B_[1]),max(A_[0],B_[0]),max(A_[1],B_[1])],outline=(30,140,40),width=3)
    for i,a in enumerate(actors):
        sp=sprite(a['sub']); X,Y=P(a['x'],a['y'])
        im.paste(sp,(int(X-sp.width/2),int(Y-sp.height)),sp)
        dr.ellipse([X+sp.width/2-6,Y-sp.height-10,X+sp.width/2+26,Y-sp.height+22],fill=(200,30,30) if a['room']==0 else (150,30,160),outline=(255,255,255),width=2)
        dr.text((X+sp.width/2+10,Y-sp.height+6),str(i+1),font=F(22,True),fill=(255,255,255),anchor='mm')
    for (x,y),d,lab in C['exits']:
        X,Y=P(x,y); ex,ey=arrow(dr,X,Y,d,(0,90,200))
        f=F(24,True); tw=dr.textlength(lab,font=f)
        tx=min(max(ex-tw/2,ox+4),ox+TW-tw-4) if d in 'UD' else (ex-tw-10 if d=='L' else ex-tw-10)
        ty=ey+6 if d=='D' else (ey-34 if d=='U' else ey-50)
        tx=min(max(tx,ox+4),ox+TW-tw-4); ty=min(max(ty,top+4),top+TH-34)
        dr.rectangle([tx-6,ty-4,tx+tw+6,ty+32],fill=(255,255,255),outline=(0,90,200),width=2)
        dr.text((tx,ty),lab,font=f,fill=(0,60,160))
    dr.text((ox,top+TH+12),'Plano del pueblo recortado (posiciones del código, AREA%02d)'%area,font=F(24,True),fill=(30,30,30))
    yy=top+TH+44
    for ln in wrap(dr,'Muñecos: sprites de SYSTEM/%s. '%C['atlas']+PAL_NOTE['es'][area],F(22),W-ox-M):
        dr.text((ox,yy),ln,font=F(22),fill=(150,40,40)); yy+=28
    dr.text((ox,yy),'Verde = recorrido de patrulla · tejados = casas (al entrar se ve el interior) · cuadros marrones = objetos',font=F(22),fill=(60,60,60))
    lx=ox+TW+M; y=top
    dr.text((lx,y),'Quién es quién (y qué dice, del texto japonés)',font=F(28,True),fill=(30,30,30)); y+=46
    f=F(21)
    for i,t in enumerate(C['legend']):
        lines=wrap(dr,t,f,LW-50)
        col=(200,30,30) if actors[i]['room']==0 else (150,30,160)
        dr.ellipse([lx,y,lx+32,y+32],fill=col); dr.text((lx+16,y+16),str(i+1),font=F(20,True),fill=(255,255,255),anchor='mm')
        for ln in lines: dr.text((lx+44,y+4),ln,font=f,fill=(30,30,30)); y+=27
        y+=10
    y+=10
    dr.text((lx,y),'Rojo = en la calle · morado = dentro de una casa',font=F(21,True),fill=(60,60,60))
    im=im.crop((0,0,W,max(yy+50,y+60)))
    im.save(out); print(out,im.size)

# ------------------------------------------------------------------ English texts
CIVI_EN='Civilization Town (AREA18)'
EN={
16:dict(title='AREA16 section 0 — cut Stone People village',
 cap='Cut Stone People village (Stone Town, AREA16, top-down view): what the code places; the painted background is lost.',
 exits=['→ '+CIVI_EN,'→ Factory (AREA11 s2), door','→ AREA16 s1 (also cut)'],
 legend=['Neighbour lifting something: "Heave... HO!"','Neighbour lifting something: "Hup... HAAA!"',
 'Neighbour by the lift: "This thing on the wall is supposed to be a lift... but it doesn\'t move"',
 'House 1: "Can\'t get into the other towns? Don\'t know, don\'t care" → later talks about the gear and the generator',
 'Vertical patrol: "The folks from the other towns..." → "A gear that won\'t break? If it were iron..."',
 'Diagonal patrol: "First time in this town?" → "Long ago I tried to go into the river..."',
 'Neighbour: "Should I go in...?" → "You\'ll beat the Evil Pig who cursed this land, right?"',
 'Diagonal patrol (after the first milestone): "I secretly opened the door between the Iron and Wood towns"',
 'House 4, craftsman: "Hmm... I want to make something" / "Come to think of it..."',
 'Neighbour (before the first milestone): no line, sets a flag','Neighbour (before the first milestone): no line, sets a flag',
 'House 2: no dialogue in the code','At the end: the boy who saw Mizuno the witch (PAL text #229), starts event 0x67']),
11:dict(title='AREA11 section 0 — cut Wood People village',
 cap='Cut Wood People village (next to the Factory, AREA11, top-down view): what the code places; the painted background is lost.',
 exits=['→ '+CIVI_EN,'→ Factory (AREA11 s1)'],
 legend=['House 1: "...Now I can make something really interesting..." (PAL text #227)','House 1: no line, sets a flag',
 'House 2: "It doesn\'t work, but this machine is iron... I could use it"','Diagonal patrol: "A fortune-teller lives in this hut, but she has a condition"',
 'Horizontal patrol: "Wood is the best; stone and iron are cold"','Horizontal patrol: "It doesn\'t work, but this machine is iron..."',
 'Neighbour: "There\'s a maze under the floor; don\'t wander in carelessly"','Vertical patrol: "You can only reach that hut from under the floor"',
 'Horizontal patrol: "A gear? Made of wood it would break" → "I\'m thirsty, I\'ll get a coconut" → "Karakuri Village sank"',
 'Vertical patrol: "The workshop entrance is up there; go take a look"','Neighbour: "This sign is old and dangerous" → "You\'re always so full of energy"',
 'House 3, sitting: probably the raft maker ("Want a raft? Bring me wood from the Wise Old Tree"); the code gives him no dialogue']),
17:dict(title='AREA17 section 0 — cut Iron Men village',
 cap='Cut Iron Men village (Iron Town, AREA17, top-down view): what the code places; the painted background is lost.',
 exits=['→ '+CIVI_EN,'→ inside Iron Town (AREA17 s1)'],
 legend=['House 4: "I hear you fixed the stone clock tower"','House 4: "Good material! Wait a moment..."',
 'House 5, the smith: "Need something? An iron gear?" → gives you the gear (item 0x3E)','The bomb maker: "Haven\'t seen your face before..." → "To make bombs I need electricity"',
 'House 1: no dialogue in the code','Neighbour: "Want to go up? Go into this hut"','Horizontal patrol: "Welcome, this is the Iron Men\'s town"',
 'House 2: "Iron is sturdy and lasts" → "You even fixed the generator?"','Diagonal patrol: "Why are we always at each other\'s throats?"',
 'Horizontal patrol: "Everyone and their wood and stone..." → "Wood, stone and iron: balance is what matters"','Neighbour: "Use me as a landmark and go straight down to leave"',
 'House 3: "The lift used to run on machinery"','Horizontal patrol: "Ride the lift and take the middle path to the Iron Castle"',
 'Diagonal patrol: "That old Gantetsu won\'t let me go to the Castle" → "Sorry I made fun of you"','Diagonal patrol: "This land is under the Evil Pigs\' spell too"',
 'Diagonal patrol: "If you want something iron, try the lift hut" → "Are you a good swimmer?"']),
}
AREAS_EN = copy.deepcopy(AREAS)
for _a, _e in EN.items():
    _C = AREAS_EN[_a]; _C['title'] = _e['title']; _C['cap'] = _e['cap']; _C['legend'] = _e['legend']
    _C['exits'] = [(p, d, l) for (p, d, _), l in zip(_C['exits'], _e['exits'])]
def explicado_en(area,out):
    C=AREAS_EN[area]; var=T.CUT[area]
    actors,props=T.npc_set(var); rects=T.trigger_rects(area)
    p=page(C['atlas']); cs=comps(p)
    def sprite(sub):
        if C['sub'][sub] is None: return lost_marker(SC)
        pal=clut_row(*sub_pal(C,sub))
        b=cs[C['sub'][sub]]; w,h=b[2]-b[0]+1,b[3]-b[1]+1
        im=Image.new('RGBA',(w,h),(0,0,0,0)); px=im.load()
        for y in range(h):
            for x in range(w):
                v=p[b[1]+y][b[0]+x]
                if v: px[x,y]=pal[v]+(255,)
        return im.resize((int(w*SC),int(h*SC)),Image.NEAREST)
    # world bounds
    xs=[a['x'] for a in actors]+[q['x'] for l in props for q in l]+[r[0] for r in rects]+[r[2] for r in rects]+[e[0][0] for e in C['exits']]
    ys=[a['y'] for a in actors]+[q['y'] for l in props for q in l]+[r[1] for r in rects]+[r[3] for r in rects]+[e[0][1] for e in C['exits']]
    x0,x1,y0,y1=min(xs)-110,max(xs)+110,min(ys)-110,max(ys)+70
    TW,TH=int((x1-x0)*SC),int((y1-y0)*SC)
    # reference
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]])
    ref=ref.resize((int(rw*SC),int(rh*SC)),Image.NEAREST)
    LW=820; M=30; top=150
    W=M+ref.width+M+TW+M+LW+M; H=top+max(ref.height,TH)+200
    im=Image.new('RGB',(W,H),(250,247,240)); dr=ImageDraw.Draw(im)
    dr.text((M,25),C['title'],font=F(44,True),fill=(30,30,30))
    dr.text((M,85),C['cap'],font=F(25),fill=(60,60,60))
    im.paste(ref,(M,top))
    dr.text((M,top+ref.height+12),'Reference: a top-down section that DOES exist',font=F(24,True),fill=(30,30,30))
    dr.text((M,top+ref.height+44),'(Baccus Lake, AREA08 s0), same scale',font=F(24),fill=(30,30,30))
    ox=M+ref.width+M
    dr.rectangle([ox,top,ox+TW,top+TH],fill=(206,196,168),outline=(120,110,90),width=3)
    for gx in range(0,TW,int(32*SC)):
        dr.line([ox+gx,top,ox+gx,top+TH],fill=(196,186,158))
    for gy in range(0,TH,int(32*SC)):
        dr.line([ox,top+gy,ox+TW,top+gy],fill=(196,186,158))
    def P(x,y): return ox+(x-x0)*SC, top+(y-y0)*SC
    roof=[(150,70,50),(120,90,60),(90,90,110),(140,60,70),(110,80,50)]
    for i,(a,b,c,d) in enumerate(rects):
        X0,Y0=P(a,b);X1,Y1=P(c,d); col=roof[i%5]
        dr.rectangle([X0,Y0+(Y1-Y0)*0.35,X1,Y1],fill=(170,150,120),outline=(60,40,30),width=3)
        dr.polygon([(X0-8,Y0+(Y1-Y0)*0.45),((X0+X1)/2,Y0),(X1+8,Y0+(Y1-Y0)*0.45)],fill=col,outline=(50,30,20))
        for k in range(1,6):
            yy=Y0+(Y1-Y0)*0.45*k/6
            dr.line([((X0+X1)/2-(X1-X0+16)/2*k/6,yy),((X0+X1)/2+(X1-X0+16)/2*k/6,yy)],fill=tuple(max(0,v-40) for v in col),width=2)
        dr.rectangle([(X0+X1)/2-12,Y1-34,(X0+X1)/2+12,Y1],fill=(80,50,30))
        dr.text((X0+6,Y1-30),'House %d'%(i+1),font=F(22,True),fill=(255,255,255),stroke_width=2,stroke_fill=(0,0,0))
    for l in props[:1]:
        for q in l:
            X,Y=P(q['x'],q['y']); dr.rounded_rectangle([X-9,Y-9,X+9,Y+9],4,fill=(140,110,70),outline=(70,50,30),width=2)
    for i,a in enumerate(actors):
        if a['beh']:
            bx0,by0,bx1,by1=a['bx0'],a['by0'],a['bx1'],a['by1']
            if var==0x0E: bx0,by0,bx1,by1=a['x']+bx0,a['y']+by0,a['x']+bx1,a['y']+by1
            A_=P(bx0,by0);B_=P(bx1,by1)
            dr.rectangle([min(A_[0],B_[0]),min(A_[1],B_[1]),max(A_[0],B_[0]),max(A_[1],B_[1])],outline=(30,140,40),width=3)
    for i,a in enumerate(actors):
        sp=sprite(a['sub']); X,Y=P(a['x'],a['y'])
        im.paste(sp,(int(X-sp.width/2),int(Y-sp.height)),sp)
        dr.ellipse([X+sp.width/2-6,Y-sp.height-10,X+sp.width/2+26,Y-sp.height+22],fill=(200,30,30) if a['room']==0 else (150,30,160),outline=(255,255,255),width=2)
        dr.text((X+sp.width/2+10,Y-sp.height+6),str(i+1),font=F(22,True),fill=(255,255,255),anchor='mm')
    for (x,y),d,lab in C['exits']:
        X,Y=P(x,y); ex,ey=arrow(dr,X,Y,d,(0,90,200))
        f=F(24,True); tw=dr.textlength(lab,font=f)
        tx=min(max(ex-tw/2,ox+4),ox+TW-tw-4) if d in 'UD' else (ex-tw-10 if d=='L' else ex-tw-10)
        ty=ey+6 if d=='D' else (ey-34 if d=='U' else ey-50)
        tx=min(max(tx,ox+4),ox+TW-tw-4); ty=min(max(ty,top+4),top+TH-34)
        dr.rectangle([tx-6,ty-4,tx+tw+6,ty+32],fill=(255,255,255),outline=(0,90,200),width=2)
        dr.text((tx,ty),lab,font=f,fill=(0,60,160))
    dr.text((ox,top+TH+12),'Plan of the cut village (positions from the code, AREA%02d)'%area,font=F(24,True),fill=(30,30,30))
    yy=top+TH+44
    for ln in wrap(dr,'Figures: sprites from SYSTEM/%s. '%C['atlas']+PAL_NOTE['en'][area],F(22),W-ox-M):
        dr.text((ox,yy),ln,font=F(22),fill=(150,40,40)); yy+=28
    dr.text((ox,yy),'Green = patrol route · roofs = houses (entering shows the inside) · brown squares = props',font=F(22),fill=(60,60,60))
    lx=ox+TW+M; y=top
    dr.text((lx,y),'Who is who (lines from the Japanese text)',font=F(28,True),fill=(30,30,30)); y+=46
    f=F(21)
    for i,t in enumerate(C['legend']):
        lines=wrap(dr,t,f,LW-50)
        col=(200,30,30) if actors[i]['room']==0 else (150,30,160)
        dr.ellipse([lx,y,lx+32,y+32],fill=col); dr.text((lx+16,y+16),str(i+1),font=F(20,True),fill=(255,255,255),anchor='mm')
        for ln in lines: dr.text((lx+44,y+4),ln,font=f,fill=(30,30,30)); y+=27
        y+=10
    y+=10
    dr.text((lx,y),'Red = outdoors · purple = inside a house',font=F(21,True),fill=(60,60,60))
    im=im.crop((0,0,W,max(yy+50,y+60)))
    im.save(out); print(out,im.size)

# TCRF wiki names of the drawings (sprite group of each resident subtype; which code subtype wears which drawing is
# our identification, as is reading AREA11 as TCRF's "Lumberjack Town")
TCRF_NAMES = {
    11: {0: ('Lumberjack Factory Worker', 'trabajador de la fábrica')},
    16: {2: ('Male Stonetown Resident', 'vecino de Stonetown'), 0: ('Female Stonetown Resident', 'vecina de Stonetown')},
}


def _name_legends():
    for a, names in TCRF_NAMES.items():
        actors, _ = T.npc_set(T.CUT[a])
        for tab, es in ((AREAS, True), (AREAS_EN, False)):
            leg = tab[a]['legend']
            for i, act in enumerate(actors):
                n = names.get(act['sub'])
                if n and i < len(leg) and not leg[i].startswith(n[0]) and not leg[i].startswith('«' + n[0]):
                    leg[i] = ('«%s» (%s) — ' % n if es else '%s — ' % n[0]) + leg[i]


_name_legends()


# ------------------------------------------------------------------ evidence-only schematic (esquema)
SRC={11:dict(step='topdownSectionStepC',stepa='0x800E8D98',set='0D',lst='0x80100080',props='0x800FFE24',dlg='topdownA11ActorOpenDialogue 0x800F1C88',tab='',txt='AREA11/MFNT110S.WFM #249-#265',sys=61,
  es='AREA11 sección 0 (pueblo de la gente de madera, recortado)',en='AREA11 section 0 (Wood People village, cut)'),
 16:dict(step='topdownSectionStepD',stepa='0x800E8ED4',set='0E',lst='0x80100364',props='0x801001D4 / 0x801001D8',dlg='topdownA16ActorOpenDialogue 0x800F2E84',tab=' + TOPDOWN_A16_ACTOR_DIALOGUE_IDS 0x800FEF24',txt='AREA16/MFNT160S.WFM #230-#247',sys=76,
  es='AREA16 sección 0 (pueblo de piedra, recortado)',en='AREA16 section 0 (Stone People village, cut)'),
 17:dict(step='topdownSectionStepE',stepa='0x800E9040',set='0F',lst='0x80100784',props='0x80100368',dlg='topdownA17ActorOpenDialogue 0x800F4110 (+ DialogueBank2/3)',tab='',txt='AREA17/MFNT170S.WFM #213-#218, #257-#268, #270-#289',sys=80,
  es='AREA17 sección 0 (pueblo de los Hombres de hierro, recortado)',en='AREA17 section 0 (Iron Men village, cut)')}
S=2; RED=(220,30,30); MAG=(170,40,190); GRN=(20,170,40); BLU=(40,120,255); YEL=(240,190,0)
def overlay(img,dr,var,area,px0,py0,sprites=None):
    P=lambda x,y:((x+128-px0)*S,(y-40-py0)*S)
    actors,props=T.npc_set(var)
    for i,(a,b,c,d) in enumerate(T.trigger_rects(area)):
        A=P(a,b);B=P(c,d); dr.rectangle([A,B],outline=BLU,width=5)
        dr.text((A[0]+6,A[1]+4),'C%d'%(i+1),font=F(26,True),fill=BLU,stroke_width=3,stroke_fill=(255,255,255))
    for lst in props[:1]:
        for q in lst:
            X,Y=P(q['x'],q['y']); dr.rectangle([X-7,Y-7,X+7,Y+7],fill=YEL,outline=(0,0,0),width=2)
    for a in actors:
        if a['beh']:
            x0,y0,x1,y1=a['bx0'],a['by0'],a['bx1'],a['by1']
            if var==0x0E: x0,y0,x1,y1=a['x']+x0,a['y']+y0,a['x']+x1,a['y']+y1
            A=P(x0,y0);B=P(x1,y1)
            dr.rectangle([min(A[0],B[0])-3,min(A[1],B[1])-3,max(A[0],B[0])+3,max(A[1],B[1])+3],outline=GRN,width=5)
    for i in sorted(range(len(actors)),key=lambda k:actors[k]['y']):
        a=actors[i]; X,Y=P(a['x'],a['y']); col=RED if a['room']==0 else MAG
        if sprites:
            sp=sprites(a['sub']); img.paste(sp,(int(X-sp.width/2),int(Y-sp.height)),sp); top=Y-sp.height
        else:
            dr.ellipse([X-9,Y-9,X+9,Y+9],fill=col,outline=(255,255,255),width=2); top=Y-10
        dr.ellipse([X-6,Y-6,X+6,Y+6],fill=col)
        bx,by=X+22,top-4
        dr.ellipse([bx-19,by-19,bx+19,by+19],fill=col,outline=(255,255,255),width=3)
        dr.text((bx,by),str(i+1),font=F(22,True),fill=(255,255,255),anchor='mm')
    return P
def esquema_es(area,out):
    C=AREAS[area]; var=T.CUT[area]; Q=SRC[area]; NH=len(T.trigger_rects(area))
    # left: AREA08 s0
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]]); ref=ref.resize((rw*S,rh*S),Image.NEAREST).convert('RGBA')
    d0=ImageDraw.Draw(ref); overlay(ref,d0,0x0C,8,0,0)
    # right
    actors,props=T.npc_set(var); rects=T.trigger_rects(area)
    xs=[a['x'] for a in actors]+[q['x'] for l in props[:1] for q in l]+[r[0] for r in rects]+[r[2] for r in rects]+[e[0][0] for e in C['exits']]
    ys=[a['y'] for a in actors]+[q['y'] for l in props[:1] for q in l]+[r[1] for r in rects]+[r[3] for r in rects]+[e[0][1] for e in C['exits']]
    px0=(min(xs)+128-40)//32*32; py0=(min(ys)-40-70)//32*32
    px1=(max(xs)+128+40+31)//32*32; py1=(max(ys)-40+40+31)//32*32
    W,H=px1-px0,py1-py0
    pl=Image.new('RGBA',(W*S,H*S),(226,222,212,255)); d1=ImageDraw.Draw(pl)
    for g in range(0,W*S,64): d1.line([g,0,g,H*S],fill=(210,205,195))
    for g in range(0,H*S,64): d1.line([0,g,W*S,g],fill=(210,205,195))
    p=page(C['atlas']); cs=comps(p)
    def spr(sub):
        if C['sub'][sub] is None: return lost_marker(S)
        pal=clut_row(*sub_pal(C,sub))
        bb=cs[C['sub'][sub]]; w,h=bb[2]-bb[0]+1,bb[3]-bb[1]+1
        im=Image.new('RGBA',(w,h)); q=im.load()
        for y in range(h):
            for x in range(w):
                v=p[bb[1]+y][bb[0]+x]
                if v: q[x,y]=pal[v]+(255,)
        return im.resize((w*S,h*S),Image.NEAREST)
    P=overlay(pl,d1,var,area,px0,py0,spr)
    EX=[]
    for k,((x,y),dd,lab) in enumerate(C['exits']):
        X,Y=P(x,y); X=min(max(X,30),W*S-30); Y=min(max(Y,30),H*S-30)
        d1.rectangle([X-26,Y-26,X+26,Y+26],fill=(255,255,255),outline=(0,0,0),width=4)
        d1.text((X,Y),'S%d'%(k+1),font=F(24,True),fill=(0,0,0),anchor='mm')
    M=40; top=200; gap=60
    Wt=M+ref.width+gap+pl.width+M
    im=Image.new('RGB',(Wt,5000),(250,247,240)); D=ImageDraw.Draw(im)
    D.text((M,24),'Esquema con datos del código — '+Q['es'],font=F(46,True),fill=(20,20,20))
    D.text((M,90),'Sin arte inventado: sólo lo que el código coloca, en el espacio de pantalla del fondo (vista 3/4).',font=F(30),fill=(40,40,40))
    D.text((M,134),'Izquierda: una sección real con las mismas marcas.  Derecha: el pueblo recortado, misma escala (×2).',font=F(30),fill=(40,40,40))
    im.paste(ref,(M,top)); rx=M+ref.width+gap; im.paste(pl,(rx,top))
    y=top+max(ref.height,pl.height)+12
    D.text((M,y),'A. Lago Baco (AREA08 s0), fondo real del disco + marcas de su código',font=F(26,True),fill=(20,20,20))
    D.text((M,y+34),'(AREA08 no tiene lista de casas: sólo vecinos y objetos)',font=F(24),fill=(60,60,60))
    D.text((rx,y),'B. AREA%d s0: fondo PERDIDO (gris). Tamaño estimado %d × %d px'%(area,W,H),font=F(26,True),fill=(20,20,20))
    D.text((rx,y+34),'Muñecos: SYSTEM/%s, paletas reales del juego (ver abajo)'%C['atlas'],font=F(24,True),fill=(170,30,30))
    y+=100; f=F(25)
    def para(t,yy,x=M,w=Wt-2*M,font=f,col=(30,30,30)):
        for ln in wrap(D,t,font,w): D.text((x,yy),ln,font=font,fill=col); yy+=33
        return yy
    D.text((M,y),'Qué significa cada marca',font=F(32,True),fill=(20,20,20)); y+=48
    for c,t in [(BLU,'Rectángulo azul C1…C%d = casa:'%NH+' zona donde, al entrar Tombi, el tejado desaparece y se ve el interior (TOPDOWN_TRIGGER_RECT_LISTS).'),
                (RED,'Círculo rojo con número = sitio donde está un vecino en la calle (los pies del muñeco están en el punto).'),
                (MAG,'Círculo morado = vecino que está dentro de una casa (sólo se ve cuando Tombi entra en ella).'),
                (GRN,'Rectángulo verde = zona por la que camina ese vecino (su patrulla).'),
                (YEL,'Cuadradito amarillo = objeto o decorado del pueblo (tejados, carteles, rocas…); el código no dice cuál es cada uno.')]:
        D.rectangle([M,y+4,M+28,y+28],fill=c); y=para(t,y,M+44,Wt-2*M-44)+6
    y+=10; D.text((M,y),'Salidas (posiciones del código: '+Q['step']+')',font=F(32,True),fill=(20,20,20)); y+=48
    for k,(_,_,lab) in enumerate(C['exits']):
        y=para('S%d %s'%(k+1,lab.replace('→','lleva a')),y)
    y+=10; D.text((M,y),'Qué dice cada vecino (resumen en español del texto japonés)',font=F(32,True),fill=(20,20,20)); y+=48
    colw=(Wt-3*M)//2; ys=[y,y]
    for i,t in enumerate(C['legend']):
        c=0 if i<(len(C['legend'])+1)//2 else 1; x=M+c*(colw+M); yy=ys[c]
        cc=RED if actors[i]['room']==0 else MAG
        D.ellipse([x,yy,x+40,yy+40],fill=cc); D.text((x+20,yy+20),str(i+1),font=F(24,True),fill=(255,255,255),anchor='mm')
        for ln in wrap(D,t,f,colw-60): D.text((x+54,yy+6),ln,font=f,fill=(30,30,30)); yy+=33
        ys[c]=yy+12
    y=max(ys)+16
    D.text((M,y),'De dónde sale cada dato',font=F(32,True),fill=(20,20,20)); y+=48
    src=['Vecinos (posición, casa, patrulla, frase): NPC_SET_%s, lista en I0NFO2 %s (registros de 28 bytes); sus frases: %s%s → texto %s (en PAL "···??", el texto está en la versión japonesa).'%(Q['set'],Q['lst'],Q['dlg'],Q['tab'],Q['txt']),
         'Objetos: lista de objetos del NPC_SET_%s, puntero(s) en I0NFO2 %s (registros de 16 bytes).'%(Q['set'],Q['props']),
         'Casas: TOPDOWN_TRIGGER_RECT_LISTS, I0NFO2 0x800FEC9C (entrada de AREA%d).'%area,
         'Salidas: rectángulos dentro de %s (I0NFO2 %s); destinos: tabla de salidas de MAIN0 0x80079A48.'%(Q['step'],Q['stepa']),
         'Paso mundo → píxel del fondo (x+128, y-40): topdownCollideMapTile 0x800EE18C, comprobado con AREA08/AREA19.',
         'Muñecos: página de texturas SYSTEM/%s (copia de VRAM que recarga el menú, lista SYS %d); qué dibujo es cada tipo de vecino es una suposición.'%(C['atlas'],Q['sys']),
         'Panel A: fondo, cuadrícula y vecinos reales de AREA08 s0 (dibujo TIM del disco + NPC_SET_0C).',
         'Paletas: '+PAL_NOTE['es'][area]+'.', 'FALTA (perdido en el disco): el dibujo de fondo del pueblo, el mapa de colisiones y las tablas de sprites; por eso el panel B es gris y el tamaño del fondo es una estimación.']
    for t in src: y=para('• '+t,y)+4
    im=im.crop((0,0,Wt,y+30)); im.save(out); print(out,im.size)
def overlay_en(img,dr,var,area,px0,py0,sprites=None):
    P=lambda x,y:((x+128-px0)*S,(y-40-py0)*S)
    actors,props=T.npc_set(var)
    for i,(a,b,c,d) in enumerate(T.trigger_rects(area)):
        A=P(a,b);B=P(c,d); dr.rectangle([A,B],outline=BLU,width=5)
        dr.text((A[0]+6,A[1]+4),'H%d'%(i+1),font=F(26,True),fill=BLU,stroke_width=3,stroke_fill=(255,255,255))
    for lst in props[:1]:
        for q in lst:
            X,Y=P(q['x'],q['y']); dr.rectangle([X-7,Y-7,X+7,Y+7],fill=YEL,outline=(0,0,0),width=2)
    for a in actors:
        if a['beh']:
            x0,y0,x1,y1=a['bx0'],a['by0'],a['bx1'],a['by1']
            if var==0x0E: x0,y0,x1,y1=a['x']+x0,a['y']+y0,a['x']+x1,a['y']+y1
            A=P(x0,y0);B=P(x1,y1)
            dr.rectangle([min(A[0],B[0])-3,min(A[1],B[1])-3,max(A[0],B[0])+3,max(A[1],B[1])+3],outline=GRN,width=5)
    for i in sorted(range(len(actors)),key=lambda k:actors[k]['y']):
        a=actors[i]; X,Y=P(a['x'],a['y']); col=RED if a['room']==0 else MAG
        if sprites:
            sp=sprites(a['sub']); img.paste(sp,(int(X-sp.width/2),int(Y-sp.height)),sp); top=Y-sp.height
        else:
            dr.ellipse([X-9,Y-9,X+9,Y+9],fill=col,outline=(255,255,255),width=2); top=Y-10
        dr.ellipse([X-6,Y-6,X+6,Y+6],fill=col)
        bx,by=X+22,top-4
        dr.ellipse([bx-19,by-19,bx+19,by+19],fill=col,outline=(255,255,255),width=3)
        dr.text((bx,by),str(i+1),font=F(22,True),fill=(255,255,255),anchor='mm')
    return P
def esquema_en(area,out):
    C=AREAS_EN[area]; var=T.CUT[area]; Q=SRC[area]; NH=len(T.trigger_rects(area))
    # left: AREA08 s0
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]]); ref=ref.resize((rw*S,rh*S),Image.NEAREST).convert('RGBA')
    d0=ImageDraw.Draw(ref); overlay_en(ref,d0,0x0C,8,0,0)
    # right
    actors,props=T.npc_set(var); rects=T.trigger_rects(area)
    xs=[a['x'] for a in actors]+[q['x'] for l in props[:1] for q in l]+[r[0] for r in rects]+[r[2] for r in rects]+[e[0][0] for e in C['exits']]
    ys=[a['y'] for a in actors]+[q['y'] for l in props[:1] for q in l]+[r[1] for r in rects]+[r[3] for r in rects]+[e[0][1] for e in C['exits']]
    px0=(min(xs)+128-40)//32*32; py0=(min(ys)-40-70)//32*32
    px1=(max(xs)+128+40+31)//32*32; py1=(max(ys)-40+40+31)//32*32
    W,H=px1-px0,py1-py0
    pl=Image.new('RGBA',(W*S,H*S),(226,222,212,255)); d1=ImageDraw.Draw(pl)
    for g in range(0,W*S,64): d1.line([g,0,g,H*S],fill=(210,205,195))
    for g in range(0,H*S,64): d1.line([0,g,W*S,g],fill=(210,205,195))
    p=page(C['atlas']); cs=comps(p)
    def spr(sub):
        if C['sub'][sub] is None: return lost_marker(S)
        pal=clut_row(*sub_pal(C,sub))
        bb=cs[C['sub'][sub]]; w,h=bb[2]-bb[0]+1,bb[3]-bb[1]+1
        im=Image.new('RGBA',(w,h)); q=im.load()
        for y in range(h):
            for x in range(w):
                v=p[bb[1]+y][bb[0]+x]
                if v: q[x,y]=pal[v]+(255,)
        return im.resize((w*S,h*S),Image.NEAREST)
    P=overlay_en(pl,d1,var,area,px0,py0,spr)
    EX=[]
    for k,((x,y),dd,lab) in enumerate(C['exits']):
        X,Y=P(x,y); X=min(max(X,30),W*S-30); Y=min(max(Y,30),H*S-30)
        d1.rectangle([X-26,Y-26,X+26,Y+26],fill=(255,255,255),outline=(0,0,0),width=4)
        d1.text((X,Y),'S%d'%(k+1),font=F(24,True),fill=(0,0,0),anchor='mm')
    M=40; top=200; gap=60
    Wt=M+ref.width+gap+pl.width+M
    im=Image.new('RGB',(Wt,5000),(250,247,240)); D=ImageDraw.Draw(im)
    D.text((M,24),'Schematic from the code — '+Q['en'],font=F(46,True),fill=(20,20,20))
    D.text((M,90),'No invented art: only what the code places, in the screen space of the background (3/4 view).',font=F(30),fill=(40,40,40))
    D.text((M,134),'Left: a real section with the same marks.  Right: the cut village, same scale (×2).',font=F(30),fill=(40,40,40))
    im.paste(ref,(M,top)); rx=M+ref.width+gap; im.paste(pl,(rx,top))
    y=top+max(ref.height,pl.height)+12
    D.text((M,y),'A. Baccus Lake (AREA08 s0): real background + its marks',font=F(26,True),fill=(20,20,20))
    D.text((M,y+34),'(AREA08 has no house list: only residents and props)',font=F(24),fill=(60,60,60))
    D.text((rx,y),'B. AREA%d s0: background LOST (grey). Estimated size %d × %d px'%(area,W,H),font=F(26,True),fill=(20,20,20))
    D.text((rx,y+34),'Figures: SYSTEM/%s, real in-game palettes (see below)'%C['atlas'],font=F(24,True),fill=(170,30,30))
    y+=100; f=F(25)
    def para(t,yy,x=M,w=Wt-2*M,font=f,col=(30,30,30)):
        for ln in wrap(D,t,font,w): D.text((x,yy),ln,font=font,fill=col); yy+=33
        return yy
    D.text((M,y),'What each mark means',font=F(32,True),fill=(20,20,20)); y+=48
    for c,t in [(BLU,'Blue rectangle H1…H%d = house:'%NH+' area where, when Tomba walks in, the roof disappears and the inside shows (TOPDOWN_TRIGGER_RECT_LISTS).'),
                (RED,'Red numbered circle = where a resident stands outdoors (the figure\'s feet are on the dot).'),
                (MAG,'Purple circle = resident inside a house (only visible when Tomba goes in).'),
                (GRN,'Green rectangle = the box that resident walks in (their patrol).'),
                (YEL,'Small yellow square = village prop or scenery (roofs, signs, rocks…); the code does not say which is which.')]:
        D.rectangle([M,y+4,M+28,y+28],fill=c); y=para(t,y,M+44,Wt-2*M-44)+6
    y+=10; D.text((M,y),'Exits (positions from the code: '+Q['step']+')',font=F(32,True),fill=(20,20,20)); y+=48
    for k,(_,_,lab) in enumerate(C['exits']):
        y=para('S%d %s'%(k+1,lab.replace('→','leads to')),y)
    y+=10; D.text((M,y),'What each resident says (English summary of the Japanese text)',font=F(32,True),fill=(20,20,20)); y+=48
    colw=(Wt-3*M)//2; ys=[y,y]
    for i,t in enumerate(C['legend']):
        c=0 if i<(len(C['legend'])+1)//2 else 1; x=M+c*(colw+M); yy=ys[c]
        cc=RED if actors[i]['room']==0 else MAG
        D.ellipse([x,yy,x+40,yy+40],fill=cc); D.text((x+20,yy+20),str(i+1),font=F(24,True),fill=(255,255,255),anchor='mm')
        for ln in wrap(D,t,f,colw-60): D.text((x+54,yy+6),ln,font=f,fill=(30,30,30)); yy+=33
        ys[c]=yy+12
    y=max(ys)+16
    D.text((M,y),'Where each piece of data comes from',font=F(32,True),fill=(20,20,20)); y+=48
    src=['Residents (position, house, patrol, line): NPC_SET_%s, list at I0NFO2 %s (28-byte records); their lines: %s%s → text %s (in PAL "···??"; the text survives in the Japanese release).'%(Q['set'],Q['lst'],Q['dlg'],Q['tab'],Q['txt']),
         'Props: the prop list of NPC_SET_%s, pointer(s) at I0NFO2 %s (16-byte records).'%(Q['set'],Q['props']),
         'Houses: TOPDOWN_TRIGGER_RECT_LISTS, I0NFO2 0x800FEC9C (AREA%d entry).'%area,
         'Exits: rectangles inside %s (I0NFO2 %s); destinations: MAIN0 exit table 0x80079A48.'%(Q['step'],Q['stepa']),
         'World → background pixel (x+128, y-40): topdownCollideMapTile 0x800EE18C, checked against AREA08/AREA19.',
         'Figures: texture page SYSTEM/%s (VRAM copy the menu reloads, SYS list %d); which drawing goes with each resident type is a guess.'%(C['atlas'],Q['sys']),
         'Panel A: real background, grid and residents of AREA08 s0 (TIM picture from the disc + NPC_SET_0C).',
         'Palettes: '+PAL_NOTE['en'][area]+'.', 'MISSING (lost from the disc): the village background picture, the collision map and the sprite tables; that is why panel B is grey and the background size is an estimate.']
    for t in src: y=para('• '+t,y)+4
    im=im.crop((0,0,Wt,y+30)); im.save(out); print(out,im.size)

# ------------------------------------------------------------------ perspective diagram
TOWNS={
 16:dict(size=(448,544),exits=[(-140,524,"Ciudad de civilización (AREA18)","Civilization Town (AREA18)"),(110,320,"Fábrica, puerta (AREA11 s2)","Factory, door (AREA11 s2)"),(220,30,"AREA16 s1 (también recortada)","AREA16 s1 (also cut)")],
   es="pueblo de piedra recortado (AREA16, sección 0)",en="cut Stone People's village (AREA16, section 0)",
   src="topdownSectionStepD (I0NFO2 0x800E8ED4), destinos MAIN0 0x80079A48"),
 11:dict(size=(640,576),exits=[(575,410,"Ciudad de civilización (AREA18)","Civilization Town (AREA18)"),(231,60,"Fábrica, vista lateral (AREA11 s1)","Factory, side view (AREA11 s1)")],
   es="pueblo de madera recortado (AREA11, sección 0)",en="cut Wood People's village (AREA11, section 0)",
   src="topdownSectionStepC (I0NFO2 0x800E8D98), destinos MAIN0 0x80079A48"),
 17:dict(size=(704,480),exits=[(223,590,"Ciudad de civilización (AREA18)","Civilization Town (AREA18)"),(223,60,"Ciudad de hierro, vista lateral (AREA17 s1)","Iron Town, side view (AREA17 s1)")],
   es="pueblo de hierro recortado (AREA17, sección 0)",en="cut Iron Men's town (AREA17, section 0)",
   src="topdownSectionStepE (I0NFO2 0x800E9040), destinos MAIN0 0x80079A48"),
}
def perspectiva(area,lang,outd):
    t=TOWNS[area]; var=T.CUT[area]; actors,props=T.npc_set(var); houses=T.trigger_rects(area)
    P=lambda x,y: T.world_to_px(x,y)
    W,H=t['size']; HH=60
    pts=[P(a["x"],a["y"]) for a in actors]+[P(p["x"],p["y"]) for l in props for p in l]
    for (a0,b0,a1,b1) in houses: pts+= [P(a0,b0),P(a1,b1)]
    U0=min(0,min(q[0] for q in pts)-24); V0=min(0,min(q[1] for q in pts)-24)
    W=max(W,max(q[0] for q in pts)+24); H=max(H,max(q[1] for q in pts)+24)
    cu,cv_,cz=(U0+W)/2,H+700,520; pitch=math.radians(36)
    def raw(u,v,z=0):
        dy=cv_-v; dz=z-cz
        yc=dy*math.cos(pitch)-dz*math.sin(pitch); zc=dy*math.sin(pitch)+dz*math.cos(pitch)
        return ((u-cu)/yc,-zc/yc)
    cs=[raw(u,v,z) for u in (U0,W) for v in (V0,H) for z in (0,HH+40)]
    x0=min(c[0] for c in cs); x1=max(c[0] for c in cs); y0=min(c[1] for c in cs); y1=max(c[1] for c in cs)
    BX,BY,BW,BH=40,110,1320,900; s=min(BW/(x1-x0),BH/(y1-y0))
    ox=BX+(BW-(x1-x0)*s)/2; oy=BY+(BH-(y1-y0)*s)/2
    def proj(u,v,z=0):
        a,b=raw(u,v,z); return (ox+(a-x0)*s, oy+(b-y0)*s)
    im=Image.new('RGB',(1400,1500),(250,247,240)); d=ImageDraw.Draw(im,'RGBA')
    F=lambda n,b=False: ImageFont.truetype(FB if b else FR,n)
    poly=lambda pts,**k: d.polygon([proj(*p) for p in pts],**k)
    line=lambda a,b,col,w=2: d.line([proj(*a),proj(*b)],fill=col,width=w)
    poly([(U0,V0,0),(W,V0,0),(W,H,0),(U0,H,0)],fill=(228,224,212),outline=(120,110,90))
    for x in range(int(U0)//32*32+32,W+1,32): line((x,V0,0),(x,H,0),(205,200,188),1)
    for y in range(int(V0)//32*32+32,H+1,32): line((U0,y,0),(W,y,0),(205,200,188),1)
    for a in actors:
        if a["beh"]:
            a0,b0,a1,b1=a["bx0"],a["by0"],a["bx1"],a["by1"]
            if var==0x0E: a0,b0,a1,b1=a["x"]+a0,a["y"]+b0,a["x"]+a1,a["y"]+b1
            (u0,v0),(u1,v1)=P(a0,b0),P(a1,b1); u0,u1=sorted((u0,u1)); v0,v1=sorted((v0,v1))
            if u1-u0<3 or v1-v0<3: line((u0,v0,0),(u1,v1,0),(20,160,40),5)
            else: poly([(u0,v0,0),(u1,v0,0),(u1,v1,0),(u0,v1,0)],fill=(20,160,40,50),outline=(20,160,40),width=3)
    items=[]
    for i,(a0,b0,a1,b1) in enumerate(houses):
        (u0,v0),(u1,v1)=P(a0,b0),P(a1,b1); u0,u1=sorted((u0,u1)); v0,v1=sorted((v0,v1))
        items.append((v1,'house',(u0,v0,u1,v1,i+1)))
    for l in props:
        for p in l: u,v=P(p["x"],p["y"]); items.append((v,'prop',(u,v)))
    for n,a in enumerate(actors,1): u,v=P(a["x"],a["y"]); items.append((v+0.1,'npc',(u,v,n,a["room"])))
    for i,(x,y,_,_) in enumerate(t['exits'],1):
        u,v=P(x,y); u=min(max(u,U0+20),W-20); v=min(max(v,V0+20),H-20); items.append((v,'exit',(u,v,i)))
    items.sort(key=lambda q:q[0]); B=(40,110,230)
    hl='C' if lang=='es' else 'H'; el='S' if lang=='es' else 'E'
    for _,k,a in items:
        if k=='house':
            u0,v0,u1,v1,i=a
            for face in [[(u0,v0,0),(u1,v0,0),(u1,v0,HH),(u0,v0,HH)],[(u0,v0,0),(u0,v1,0),(u0,v1,HH),(u0,v0,HH)],[(u1,v0,0),(u1,v1,0),(u1,v1,HH),(u1,v0,HH)],[(u0,v1,0),(u1,v1,0),(u1,v1,HH),(u0,v1,HH)],[(u0,v0,HH),(u1,v0,HH),(u1,v1,HH),(u0,v1,HH)]]:
                poly(face,fill=(150,180,240,45),outline=B,width=2)
            tx,ty=proj((u0+u1)/2,(v0+v1)/2,HH); d.text((tx,ty),"%s%d"%(hl,i),fill=B,font=F(26,True),anchor='mm')
        elif k=='prop':
            u,v=a; e=5
            poly([(u-e,v+e,0),(u+e,v+e,0),(u+e,v+e,10),(u-e,v+e,10)],fill=(245,200,40),outline=(150,110,0))
            poly([(u-e,v-e,10),(u+e,v-e,10),(u+e,v+e,10),(u-e,v+e,10)],fill=(255,225,90),outline=(150,110,0))
        elif k=='npc':
            u,v,n,room=a; col=(210,30,30) if room==0 else (160,40,190)
            b=proj(u,v,0); tp=proj(u,v,30)
            d.ellipse((b[0]-6,b[1]-3,b[0]+6,b[1]+3),fill=col); d.line([b,tp],fill=col,width=4); r=14
            d.ellipse((tp[0]-r,tp[1]-2*r,tp[0]+r,tp[1]),fill=col,outline='white',width=2)
            d.text((tp[0],tp[1]-r),str(n),fill='white',font=F(15,True),anchor='mm')
        else:
            u,v,i=a; x,y=proj(u,v,0)
            d.rectangle((x-24,y-15,x+24,y+15),fill='white',outline='black',width=3); d.text((x,y),"%s%d"%(el,i),fill='black',font=F(18,True),anchor='mm')
    nh=len(houses); ex="   ".join("%s%d → %s"%(el,i,e[2 if lang=='es' else 3]) for i,e in enumerate(t['exits'],1))
    if lang=='es':
        title="Esquema en perspectiva — "+t['es']
        sub="Datos del código vistos en perspectiva. Lo único inventado es la altura de las casas: el código no la da."
        leg=[(B,"%s1–%s%d: casa. Al entrar Tombi desaparece el tejado y se ve el interior"%(hl,hl,nh)),((210,30,30),"Número rojo: vecino en la calle (el punto del suelo es donde pisa)"),((160,40,190),"Número morado: vecino dentro de una casa"),((20,160,40),"Verde en el suelo: zona por la que camina ese vecino"),((245,200,40),"Cubito amarillo: objeto o decorado"),((0,0,0),ex)]
        foot="De dónde sale: casas = TOPDOWN_TRIGGER_RECT_LISTS (I0NFO2 0x800FEC9C); vecinos, zonas y objetos = NPC_SET_%02X (I0NFO2);\nsalidas = %s. Se perdió el dibujo de fondo y la colisión: por eso no hay calles ni paredes.\nLas salidas están en el borde del mapa por el que se sale (el código solo da \"x mayor que...\" o \"y menor que...\")."%(var,t['src'])
        lh="Leyenda"
    else:
        title="Perspective diagram — "+t['en']
        sub="Data from the code, seen in perspective. The only invented value is the house height: the code doesn't store it."
        leg=[(B,"%s1–%s%d: house. When Tomba walks in, the roof disappears and the inside shows"%(hl,hl,nh)),((210,30,30),"Red number: resident outdoors (the dot on the ground is where they stand)"),((160,40,190),"Purple number: resident inside a house"),((20,160,40),"Green on the ground: area that resident walks around in"),((245,200,40),"Small yellow cube: object or scenery prop"),((0,0,0),ex)]
        foot="Sources: houses = TOPDOWN_TRIGGER_RECT_LISTS (I0NFO2 0x800FEC9C); residents, walk areas and props = NPC_SET_%02X (I0NFO2);\nexits = %s.\nBackground picture and collision map are lost from the disc, so there are no streets or walls.\nExits are drawn at the map edge you leave through (the code only gives \"x greater than...\" or \"y less than...\")."%(var,t['src'].replace('destinos','destinations'))
        lh="Legend"
    d.text((30,20),title,fill='black',font=F(28,True)); d.text((30,62),sub,fill=(60,60,60),font=F(19))
    y=1060; d.text((30,y-42),lh,fill='black',font=F(24,True))
    for c,tx in leg:
        d.rectangle((30,y+3,52,y+25),fill=c); d.text((64,y),tx,fill='black',font=F(19)); y+=38
    d.multiline_text((30,y+16),foot,fill=(60,60,60),font=F(16),spacing=6)
    out=os.path.join(outd,'civ_%s_a%d%s.png'%('perspectiva' if lang=='es' else 'perspective',area,'' if lang=='es' else '_en'))
    im.save(out); print(out)

PLAN_INFO = {
    11: dict(place='AREA11 section 0 - Lumberjack Town (TCRF name; the Wood People\'s village)', step='topdownSectionStepC',
             exits='Its own exits lead to Civilization Town (AREA18 s1) and the Factory (AREA11 s1)', sysl=61, set='0D',
             lst='0x80100080', props='0x800FFE24'),
    16: dict(place='AREA16 section 0 - Stonetown (TCRF name; the Stone People\'s village)', step='topdownSectionStepD',
             exits='Its own exits lead to Civilization Town (AREA18 s1), the Factory door (AREA11 s2) and AREA16 s1 (also cut)',
             sysl=76, set='0E', lst='0x80100364', props='0x801001D4 / 0x801001D8'),
    17: dict(place='AREA17 section 0 - Iron Town (the Iron Men\'s village)', step='topdownSectionStepE',
             exits='Its own exits lead to Civilization Town (AREA18 s1) and inside Iron Town (AREA17 s1)', sysl=80, set='0F',
             lst='0x80100784', props='0x80100368'),
}
PAL_STATUS = {
    11: ['AREA11/CLUT02 row 4: Lumberjack Factory Worker (hard hat), subtype 0 - confirmed by TCRF colours',
         'AREA11/CLUT02 row 36: Male Lumberjack Town Resident (old man; on the same page, frames 30..50 of subtype 0\'s extra scripts, which no placed resident uses) - confirmed by TCRF colours, not drawn here'],
    16: ['AREA16/CLUT07 row 53: Male Stonetown Resident (strongman), subtype 2 - pixel-exact match with TCRF\'s sprite',
         'Custom CLUT (CUSTOM_CLUTS woman16): Female Stonetown Resident, subtype 0 - custom CLUT derived pixel-exact from the TCRF sheet; not found on any disc',
         'AREA16/CLUT07 row 37 (also AREA16/CLUT06, AREA17/CLUT03/06, AREA18/CLUT02): Stonetown boy, subtype 1 - confirmed by TCRF colours (all 14)'],
    17: ['AREA17/CLUT03 row 1 (= AREA17/CLUT06 rows 1/129, AREA18/CLUT02 row 6): subtype 0 - matched to a retail screenshot of an Iron Town NPC (colour set, not pixel-exact)'],
}


def plan_legend(area, plan_png, size, frames):
    I = PLAN_INFO[area]; W, H = size
    blocks = [
        ('Place', [I['place'] + '.', 'A cut top-down section: the code and placements survive in I0NFO2, the load list is empty.',
                   'No retail exit leads into it. ' + I['exits'] + ' (exit rectangles in ' + I['step'] + ', destinations in the MAIN0 exit table 0x80079A48).']),
        ('Scale', ['1 image pixel = 1 background pixel. Picture %d x %d px, estimated from the placements (the background picture is lost).' % (W, H),
                   'World position (x, y) -> background pixel (x + 128, y - 40) (topdownCollideMapTile 0x800EE18C); this image starts at the estimated picture origin.']),
        ('Marks', ['Blue rectangle: house trigger rectangle (TOPDOWN_TRIGGER_RECT_LISTS 0x800FEC9C); inside it the roof hides and the interior shows.',
                   'Green box or line: the area a resident patrols (record bounds, behaviour 1-4).',
                   'Yellow dot: prop / scenery position. Props are dots because their sprite tables are lost.',
                   'Sprite: a resident; its feet are on the record position. Grey capsule: a resident whose drawing is lost (its page did not survive).']),
        ('Facing', ['In the game, each resident record sets its facing: field +4 (frame -> animFrame, 0..3) picks the direction and field +6 (anim set -> wb4) the animation; patrolling residents turn as they walk.',
                    'Values in this town (field +4 per resident, in record order): ' + ', '.join(str(f) for f in frames) + '.',
                    'This image does NOT apply them: every resident is drawn with one front-facing frame of the page. That frame is our choice.']),
        ('Sprites', ['Drawings from the surviving texture page SYSTEM/A%s (a VRAM copy the menu reloads, SYS list %d).' % ({11: '06100', 16: '07600', 17: '08000'}[area], I['sysl']),
                     'Which subtype uses this page is proven by the actor defs: w1e (the texture page) is 1 only for the subtypes drawn here; the others (grey capsules) use pages that did not survive. Which figure of the page each of them is follows the frame-id order of their animation scripts (consistent, see docs/CIVILIZATION.md).',
                     'Palettes (real 16-colour rows of the game\'s CLUT files):'] + ['  - ' + t for t in PAL_STATUS[area]]),
        ('Source', ['Decompiled data in I0NFO2: NPC set %s, resident list at %s (28-byte records), props at %s.' % (I['set'], I['lst'], I['props']),
                    'Generated by tools/civ_sheets.py --sheet plan1x (Tombi-Decomp).']),
    ]
    LW = 900; f = F(16); fb = F(18, True)
    tmp = Image.new('RGB', (10, 10)); dd = ImageDraw.Draw(tmp)
    lines = []
    for head, items in blocks:
        lines.append((head, fb))
        for t in items:
            for ln in wrap(dd, t, f, LW - 40): lines.append((ln, f))
        lines.append(('', f))
    LH = 30 + sum(24 if fo is fb else 21 for _, fo in lines)
    plan = Image.open(plan_png).convert('RGB')
    im = Image.new('RGB', (W + LW, max(H, LH)), (250, 247, 240)); im.paste(plan, (0, 0))
    dr = ImageDraw.Draw(im); y = 16
    for t, fo in lines:
        dr.text((W + 20, y), t, font=fo, fill=(20, 20, 20)); y += 24 if fo is fb else 21
    return im


# ------------------------------------------------------------------ panel B alone at 1:1 (civ_plan_aNN_1x*.png)
def plan1x(area, outd):
    """Panel B of the schematic at actual size (one image pixel = one background pixel, the estimated picture size),
    no labels: grey ground + house rectangles, patrols, props and resident sprites; and a transparent variant."""
    C = AREAS[area]; var = T.CUT[area]
    actors, props = T.npc_set(var); rects = T.trigger_rects(area)
    xs = [a['x'] for a in actors] + [q['x'] for l in props[:1] for q in l] + [r[0] for r in rects] + [r[2] for r in rects] + [e[0][0] for e in C['exits']]
    ys = [a['y'] for a in actors] + [q['y'] for l in props[:1] for q in l] + [r[1] for r in rects] + [r[3] for r in rects] + [e[0][1] for e in C['exits']]
    px0 = (min(xs) + 128 - 40) // 32 * 32; py0 = (min(ys) - 40 - 70) // 32 * 32
    px1 = (max(xs) + 128 + 40 + 31) // 32 * 32; py1 = (max(ys) - 40 + 40 + 31) // 32 * 32
    W, H = px1 - px0, py1 - py0
    P = lambda x, y: (x + 128 - px0, y - 40 - py0)
    p = page(C['atlas']); cs = comps(p)
    for bg, sfx in (((226, 222, 212, 255), ''), ((0, 0, 0, 0), '_transparent')):
        im = Image.new('RGBA', (W, H), bg); dr = ImageDraw.Draw(im)
        for (a, b, c, d) in rects:
            dr.rectangle([P(a, b), P(c, d)], outline=(40, 120, 255, 255), width=1)
        for q in (props[0] if props else []):
            X, Y = P(q['x'], q['y']); dr.rectangle([X - 1, Y - 1, X + 1, Y + 1], fill=(240, 190, 0, 255))
        for a in actors:
            if a['beh']:
                x0, y0, x1, y1 = a['bx0'], a['by0'], a['bx1'], a['by1']
                if var == 0x0E: x0, y0, x1, y1 = a['x'] + x0, a['y'] + y0, a['x'] + x1, a['y'] + y1
                A_, B_ = P(x0, y0), P(x1, y1)
                dr.rectangle([min(A_[0], B_[0]), min(A_[1], B_[1]), max(A_[0], B_[0]), max(A_[1], B_[1])], outline=(20, 170, 40, 255), width=1)
        for a in sorted(actors, key=lambda a: a['y']):
            X, Y = P(a['x'], a['y'])
            if C['sub'][a['sub']] is None:
                lm = lost_marker(1); im.paste(lm, (int(X - lm.width / 2), int(Y - lm.height)), lm); continue
            bb = cs[C['sub'][a['sub']]]; w, h = bb[2] - bb[0] + 1, bb[3] - bb[1] + 1
            pal = clut_row(*sub_pal(C, a['sub']))
            sp = Image.new('RGBA', (w, h)); q = sp.load()
            for y in range(h):
                for x in range(w):
                    v = p[bb[1] + y][bb[0] + x]
                    if v: q[x, y] = pal[v] + (255,)
            X, Y = P(a['x'], a['y'])
            im.paste(sp, (int(X - w / 2), int(Y - h)), sp)
        out = os.path.join(outd, 'civ_plan_a%d_1x%s.png' % (area, sfx))
        (im if sfx else im.convert('RGB')).save(out); print(out, im.size)
    lg = plan_legend(area, os.path.join(outd, 'civ_plan_a%d_1x.png' % area), (W, H), [a['frame'] for a in actors])
    out = os.path.join(outd, 'civ_plan_a%d_1x_legend.png' % area); lg.save(out); print(out, lg.size)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--lang", default="all", choices=("es", "en", "all"))
    ap.add_argument("--area", default="all", choices=("11", "16", "17", "all"))
    ap.add_argument("--sheet", default="all", choices=("esquema", "explicado", "perspectiva", "plan1x", "all"))
    ap.add_argument("--out", default=os.path.join(os.path.dirname(HERE), "build", "civ"))
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    areas = (11, 16, 17) if a.area == "all" else (int(a.area),)
    langs = ("es", "en") if a.lang == "all" else (a.lang,)
    sheets = ("esquema", "explicado", "perspectiva") if a.sheet == "all" else (a.sheet,)
    for ar in areas:
        for lg in langs:
            sfx = "" if lg == "es" else "_en"
            if "esquema" in sheets:
                (esquema_es if lg == "es" else esquema_en)(ar, os.path.join(a.out, "civ_esquema_a%d%s.png" % (ar, sfx)))
            if "explicado" in sheets:
                (explicado_es if lg == "es" else explicado_en)(ar, os.path.join(a.out, "civ_explicado_a%d%s.png" % (ar, sfx)))
            if "perspectiva" in sheets:
                perspectiva(ar, lg, a.out)
        if "plan1x" in sheets:
            plan1x(ar, a.out)


if __name__ == "__main__":
    main()
