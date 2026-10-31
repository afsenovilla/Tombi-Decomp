#!/usr/bin/env python3
"""Explanatory sheets of the cut top-down towns of the Land of Civilization (AREA11/16/17 section 0), ES or EN.
See docs/CIVILIZATION.md (EN) / docs/CIVILIZACION.md (ES), section "The cut towns, one by one".

  tools/civ_sheets.py [--lang es|en|all] [--area 11|16|17|18|all] [--sheet esquema|explicado|perspectiva|plan1x|personajes|all] [--out DIR]

Sheets (file names; *_en.png for English):
  civ_esquema_aNN.png       evidence-only schematic: A = AREA08 s0 (real picture) with the code's marks, B = the cut town
                            at the same scale on grey, real resident sprites, houses, patrols, props, exits; legend,
                            what each resident says, and where every element comes from (I0NFO2 / MAIN0 addresses)
  civ_explicado_aNN.png     flat plan with drawn house shapes, sprites, exit arrows and a side legend
  civ_perspectiva_aNN.png   (EN: civ_perspective_aNN_en.png) the same data in perspective; only the house height is invented
  civ_personajes_aNN.png    who goes where: every resident at its record position (also AREA18), numbered as in the docs, and a
                            table (name, sprite, confidence, where, behaviour, line); a drawing only when the disc gives it,
                            marked known / probable, otherwise a grey "sprite unknown" silhouette
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
# SUPERSEDED (kept for reference; the sheets now draw AREA16 subtype 0 with the real row 21 of AREA16/CLUT07).
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
    'es': {16: 'Subtipos 0-2 dibujados de la página que sobrevive (w1e 1, probado); el orden de fotogramas da tipo 0 = Female Stonetown Resident, 1 = el chico, 2 = Male Stonetown Resident (coherente). Paletas reales de AREA16/CLUT07: fila 53 para el subtipo 2 (píxel a píxel con TCRF), fila 37 para el subtipo 1, el chico (confirmada por los colores de TCRF), fila 21 para el subtipo 0 (fila real del disco, encontrada comparando el aspecto de la hoja de TCRF; los colores de TCRF están desplazados). Subtipo 3 (w1e 0x11): página perdida, cápsula gris',
           11: 'Sólo el subtipo 0 se dibuja de la página que sobrevive (w1e 1, probado): Lumberjack Factory Worker, paleta real AREA11/CLUT02 fila 4 (confirmada por colores de TCRF). Subtipos 1-4 (w1e 0x0A/0x0B/0x10/0x11): página perdida, cápsula gris',
           17: 'Sólo el subtipo 0 se dibuja de la página que sobrevive (w1e 1, probado), paleta real AREA17/CLUT03 fila 1 (confirmada: la misma fila que usa el vecino lateral de la Ciudad de hierro del juego final). Subtipos 1-4 (w1e 0x0A/0x0B/0x10/0x08): página perdida, cápsula gris'},
    'en': {16: 'Subtypes 0-2 are drawn from the surviving page (w1e 1, proven); frame order gives 0 = Female Stonetown Resident, 1 = the boy, 2 = Male Stonetown Resident (consistent). Real palettes AREA16/CLUT07: row 53 for subtype 2 (pixel-exact with TCRF), row 37 for subtype 1, the boy (confirmed by TCRF colours), row 21 for subtype 0 (real disc row, found by matching the appearance of TCRF\'s sheet; TCRF colours are shifted). Subtype 3 (w1e 0x11): page lost, grey capsule',
           11: 'Only subtype 0 is drawn from the surviving page (w1e 1, proven): Lumberjack Factory Worker, real palette AREA11/CLUT02 row 4 (confirmed by TCRF colours). Subtypes 1-4 (w1e 0x0A/0x0B/0x10/0x11): page lost, grey capsule',
           17: 'Only subtype 0 is drawn from the surviving page (w1e 1, proven), real palette AREA17/CLUT03 row 1 (confirmed: same row the retail side-view Iron Town NPC uses). Subtypes 1-4 (w1e 0x0A/0x0B/0x10/0x08): page lost, grey capsule'}}


# ------------------------------------------------------------------ per-town data (ES) and the flat plan sheet
SC=1.5
CIVI='Ciudad de civilización (AREA18)'
AREAS={
16:dict(atlas='A07600',clut='AREA16/CLUT07',pals={0:21,1:37,2:53},sub={0:9,1:28,2:34,3:None},
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
 'Patrulla diagonal: «En esta choza vive un adivino, pero pone una condición»',
 'Patrulla horizontal: «La madera es lo mejor; la piedra y el hierro son fríos»',
 'Patrulla horizontal: «No funciona, pero esta máquina es de hierro...»',
 'Vecino: «Bajo el suelo hay un laberinto; no te metas sin cuidado»',
 'Patrulla vertical: «A esa choza sólo se llega por debajo del suelo»',
 'Patrulla horizontal: «¿Un engranaje? De madera se rompería» → «Tengo sed, iré a por un coco» → «El Pueblo Karakuri se hundió»',
 'Patrulla vertical: «Al taller se entra por arriba; ve a echar un vistazo»',
 'Vecino: «Este cartel es viejo y peligroso» → «Siempre tan lleno de energía»',
 'El adivino verde (subtipo 4) — Casa 3, sentado: «sólo hablo con quien tenga 300.000 PA» (k3 dx16..19; el código no le da diálogo, el port lo reconstruye)'],
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
    pr=perimeter(area)
    if pr: x0,y0,x1,y1=min(x0,-128),min(y0,40),max(x1,pr[1]-128),max(y1,pr[2]+40)
    TW,TH=int((x1-x0)*SC),int((y1-y0)*SC)
    # reference
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]])
    ref=ref.resize((int(rw*SC),int(rh*SC)),Image.NEAREST)
    LW=820; M=30; top=150
    W=M+ref.width+M+TW+M+LW+M; H=top+max(ref.height,TH)+420
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
    draw_perimeter_on(dr,area,lambda u,v:P(u-128,v+40),5,3)
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
    dr.text((ox,yy),'Verde = recorrido de patrulla · tejados = casas (al entrar se ve el interior) · cuadros marrones = objetos',font=F(22),fill=(60,60,60)); yy+=30
    for t_,c_ in ((PERIM_ES,PERIM_RED),(BORDE_ES,PERIM_PUR)):
        for ln in wrap(dr,t_,F(22),W-ox-M): dr.text((ox,yy),ln,font=F(22),fill=c_); yy+=28
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
 'House 2: "It doesn\'t work, but this machine is iron... I could use it"','Diagonal patrol: "A fortune-teller lives in this hut, but he has a condition"',
 'Horizontal patrol: "Wood is the best; stone and iron are cold"','Horizontal patrol: "It doesn\'t work, but this machine is iron..."',
 'Neighbour: "There\'s a maze under the floor; don\'t wander in carelessly"','Vertical patrol: "You can only reach that hut from under the floor"',
 'Horizontal patrol: "A gear? Made of wood it would break" → "I\'m thirsty, I\'ll get a coconut" → "Karakuri Village sank"',
 'Vertical patrol: "The workshop entrance is up there; go take a look"','Neighbour: "This sign is old and dangerous" → "You\'re always so full of energy"',
 'The Green Fortune Teller (subtype 4) — House 3, sitting: "I only talk to those with 300,000 AP" (k3 dx16..19; the code gives him no dialogue, the port reconstructs it)']),
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
    pr=perimeter(area)
    if pr: x0,y0,x1,y1=min(x0,-128),min(y0,40),max(x1,pr[1]-128),max(y1,pr[2]+40)
    TW,TH=int((x1-x0)*SC),int((y1-y0)*SC)
    # reference
    s=T.section_data(8,0); rw,rh,clut,pix=T.parse_tim(s['tim'])
    ref=Image.new('RGB',(rw,rh)); ref.putdata([clut[v] for v in pix[:rw*rh]])
    ref=ref.resize((int(rw*SC),int(rh*SC)),Image.NEAREST)
    LW=820; M=30; top=150
    W=M+ref.width+M+TW+M+LW+M; H=top+max(ref.height,TH)+420
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
    draw_perimeter_on(dr,area,lambda u,v:P(u-128,v+40),5,3)
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
    dr.text((ox,yy),'Green = patrol route · roofs = houses (entering shows the inside) · brown squares = props',font=F(22),fill=(60,60,60)); yy+=30
    for t_,c_ in ((PERIM_EN,PERIM_RED),(BORDER_EN,PERIM_PUR)):
        for ln in wrap(dr,t_,F(22),W-ox-M): dr.text((ox,yy),ln,font=F(22),fill=c_); yy+=28
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
    pr=perimeter(area)
    if pr: px0,py0,px1,py1=min(px0,0),min(py0,0),max(px1,pr[1]),max(py1,pr[2])
    W,H=px1-px0,py1-py0
    pl=Image.new('RGBA',(W*S,H*S),(226,222,212,255)); d1=ImageDraw.Draw(pl)
    for g in range(0,W*S,64): d1.line([g,0,g,H*S],fill=(210,205,195))
    for g in range(0,H*S,64): d1.line([0,g,W*S,g],fill=(210,205,195))
    draw_perimeter_on(d1,area,lambda x,y:((x-px0)*S,(y-py0)*S),6,3)
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
    D.text((rx,y),'B. AREA%d s0: fondo PERDIDO (gris). Tamaño reconstruido %d × %d px'%(area,W,H),font=F(26,True),fill=(20,20,20))
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
                (YEL,'Cuadradito amarillo = objeto o decorado del pueblo (tejados, carteles, rocas…); el código no dice cuál es cada uno.'),
                (PERIM_RED,PERIM_ES),(PERIM_PUR,BORDE_ES)]:
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
         'Paletas: '+PAL_NOTE['es'][area]+'.', 'FALTA (perdido en el disco): el dibujo de fondo del pueblo, el mapa de colisiones y las tablas de sprites; por eso el panel B es gris. El tamaño del fondo y el contorno del pueblo salen de la reconstrucción (tools/civ_rebuild.py, colisión trazada), no de datos originales.']
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
    pr=perimeter(area)
    if pr: px0,py0,px1,py1=min(px0,0),min(py0,0),max(px1,pr[1]),max(py1,pr[2])
    W,H=px1-px0,py1-py0
    pl=Image.new('RGBA',(W*S,H*S),(226,222,212,255)); d1=ImageDraw.Draw(pl)
    for g in range(0,W*S,64): d1.line([g,0,g,H*S],fill=(210,205,195))
    for g in range(0,H*S,64): d1.line([0,g,W*S,g],fill=(210,205,195))
    draw_perimeter_on(d1,area,lambda x,y:((x-px0)*S,(y-py0)*S),6,3)
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
    D.text((rx,y),'B. AREA%d s0: background LOST (grey). Reconstructed size %d × %d px'%(area,W,H),font=F(26,True),fill=(20,20,20))
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
                (YEL,'Small yellow square = village prop or scenery (roofs, signs, rocks…); the code does not say which is which.'),
                (PERIM_RED,PERIM_EN),(PERIM_PUR,BORDER_EN)]:
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
         'Palettes: '+PAL_NOTE['en'][area]+'.', 'MISSING (lost from the disc): the village background picture, the collision map and the sprite tables; that is why panel B is grey. The background size and the town outline come from the reconstruction (tools/civ_rebuild.py, traced collision), not from original data.']
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
    pr=perimeter(area)
    if pr: W,H=max(W,pr[1]),max(H,pr[2])
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
    if pr: draw_perimeter_on(d,area,lambda u,v:proj(u,v,0),3,2)
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
        leg=[(B,"%s1–%s%d: casa. Al entrar Tombi desaparece el tejado y se ve el interior"%(hl,hl,nh)),((210,30,30),"Número rojo: vecino en la calle (el punto del suelo es donde pisa)"),((160,40,190),"Número morado: vecino dentro de una casa"),((20,160,40),"Verde en el suelo: zona por la que camina ese vecino"),((245,200,40),"Cubito amarillo: objeto o decorado"),((0,0,0),ex)]+([(PERIM_RED,"Rojo grueso: perímetro del pueblo (RECONSTRUCCIÓN, colisión trazada; no son datos originales)"),(PERIM_PUR,"Morado fino: borde del fondo reconstruido")] if pr else [])
        foot="De dónde sale: casas = TOPDOWN_TRIGGER_RECT_LISTS (I0NFO2 0x800FEC9C); vecinos, zonas y objetos = NPC_SET_%02X (I0NFO2);\nsalidas = %s. Se perdió el dibujo de fondo y la colisión: no hay calles ni paredes; el contorno sale de la reconstrucción.\nLas salidas están en el borde del mapa por el que se sale (el código solo da \"x mayor que...\" o \"y menor que...\")."%(var,t['src'])
        lh="Leyenda"
    else:
        title="Perspective diagram — "+t['en']
        sub="Data from the code, seen in perspective. The only invented value is the house height: the code doesn't store it."
        leg=[(B,"%s1–%s%d: house. When Tomba walks in, the roof disappears and the inside shows"%(hl,hl,nh)),((210,30,30),"Red number: resident outdoors (the dot on the ground is where they stand)"),((160,40,190),"Purple number: resident inside a house"),((20,160,40),"Green on the ground: area that resident walks around in"),((245,200,40),"Small yellow cube: object or scenery prop"),((0,0,0),ex)]+([(PERIM_RED,"Thick red: town perimeter (RECONSTRUCTION from traced collision, not original data)"),(PERIM_PUR,"Thin purple: reconstructed picture border")] if pr else [])
        foot="Sources: houses = TOPDOWN_TRIGGER_RECT_LISTS (I0NFO2 0x800FEC9C); residents, walk areas and props = NPC_SET_%02X (I0NFO2);\nexits = %s.\nBackground picture and collision map are lost from the disc: no streets or walls; the outline comes from the reconstruction.\nExits are drawn at the map edge you leave through (the code only gives \"x greater than...\" or \"y less than...\")."%(var,t['src'].replace('destinos','destinations'))
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
         'AREA16/CLUT07 row 21 (also AREA16/CLUT06, AREA17/CLUT03/06, AREA18/CLUT02): Female Stonetown Resident, subtype 0 - real disc row, found by matching TCRF sheet\'s appearance; TCRF colours are shifted (supersedes the custom CLUT woman16)',
         'AREA16/CLUT07 row 37 (also AREA16/CLUT06, AREA17/CLUT03/06, AREA18/CLUT02): Stonetown boy, subtype 1 - confirmed by TCRF colours (all 14)'],
    17: ['AREA17/CLUT03 row 1 (= AREA17/CLUT06 rows 1/129, AREA18/CLUT02 row 6): subtype 0 - confirmed: same row the retail side-view Iron Town NPC uses (AREA17/D205 frames 94-112, part CLUT word 0x7809; handler not pinned)'],
}


PLAN_ES = {
    11: dict(place='AREA11 sección 0 - Lumberjack Town (nombre de TCRF; el pueblo de la gente de madera)',
             exits='Sus salidas llevan a la Ciudad de civilización (AREA18 s1) y a la Fábrica (AREA11 s1)'),
    16: dict(place='AREA16 sección 0 - Stonetown (nombre de TCRF; el pueblo de la gente de piedra)',
             exits='Sus salidas llevan a la Ciudad de civilización (AREA18 s1), a la puerta de la Fábrica (AREA11 s2) y a AREA16 s1 (también recortada)'),
    17: dict(place='AREA17 sección 0 - Ciudad de hierro (el pueblo de los hombres de hierro)',
             exits='Sus salidas llevan a la Ciudad de civilización (AREA18 s1) y al interior de la Ciudad de hierro (AREA17 s1)'),
}
PAL_STATUS_ES = {
    11: ['AREA11/CLUT02 fila 4: Lumberjack Factory Worker (casco), subtipo 0 - confirmada por los colores de TCRF',
         'AREA11/CLUT02 fila 36: Male Lumberjack Town Resident (anciano; en la misma página, fotogramas 30..50 de los scripts extra del subtipo 0, que ningún vecino colocado usa) - confirmada por los colores de TCRF, no dibujada aquí'],
    16: ['AREA16/CLUT07 fila 53: Male Stonetown Resident (forzudo), subtipo 2 - coincide píxel a píxel con el sprite de TCRF',
         'AREA16/CLUT07 fila 21 (también AREA16/CLUT06, AREA17/CLUT03/06, AREA18/CLUT02): Female Stonetown Resident, subtipo 0 - fila real del disco, encontrada comparando el aspecto de la hoja de TCRF; los colores de TCRF están desplazados (sustituye a la CLUT a medida woman16)',
         'AREA16/CLUT07 fila 37 (también AREA16/CLUT06, AREA17/CLUT03/06, AREA18/CLUT02): el chico de Stonetown, subtipo 1 - confirmada por los colores de TCRF (los 14)'],
    17: ['AREA17/CLUT03 fila 1 (= AREA17/CLUT06 filas 1/129, AREA18/CLUT02 fila 6): subtipo 0 - confirmada: la misma fila que usa el vecino lateral de la Ciudad de hierro del juego final (AREA17/D205 fotogramas 94-112, palabra CLUT 0x7809; handler sin localizar)'],
}


def plan_blocks_es(area, W, H, frames):
    I = PLAN_INFO[area]; E = PLAN_ES[area]
    return [
        ('Lugar', [E['place'] + '.', 'Una sección vista desde arriba recortada: el código y las posiciones sobreviven en I0NFO2, la lista de carga está vacía.',
                   'Ninguna salida del juego final lleva a ella. ' + E['exits'] + ' (rectángulos de salida en ' + I['step'] + ', destinos en la tabla de salidas de MAIN0 0x80079A48).']),
        ('Escala', ['1 píxel de la imagen = 1 píxel del fondo. Imagen %d x %d px: las posiciones más el fondo reconstruido (el dibujo de fondo original se perdió).' % (W, H),
                    'Posición del mundo (x, y) -> píxel del fondo (x + 128, y - 40) (topdownCollideMapTile 0x800EE18C).']),
        ('Marcas', ['Rectángulo azul: rectángulo de casa (TOPDOWN_TRIGGER_RECT_LISTS 0x800FEC9C); dentro, el tejado se oculta y se ve el interior.',
                    'Caja o línea verde: zona que patrulla un vecino (límites del registro, comportamiento 1-4).',
                    'Punto amarillo: posición de un objeto / decorado. Son puntos porque sus tablas de sprites se perdieron.',
                    PERIM_ES, BORDE_ES,
                    'Muñeco: un vecino; sus pies están en la posición del registro. Cápsula gris: vecino cuyo dibujo se perdió (su página no sobrevivió).']),
        ('Orientación', ['En el juego cada registro de vecino fija su orientación: el campo +4 (fotograma -> animFrame, 0..3) elige la dirección y el campo +6 (juego de animaciones -> wb4) la animación; los que patrullan giran al andar.',
                         'Valores en este pueblo (campo +4 por vecino, en orden de registro): ' + ', '.join(str(f) for f in frames) + '.',
                         'Esta imagen NO los aplica: todos los vecinos se dibujan con un mismo fotograma de frente de la página. Ese fotograma es elección nuestra.']),
        ('Muñecos', ['Dibujos de la página de texturas que sobrevive SYSTEM/A%s (copia de VRAM que recarga el menú, lista SYS %d).' % ({11: '06100', 16: '07600', 17: '08000'}[area], I['sysl']),
                     'Qué subtipo usa esta página lo prueban las definiciones de actor: w1e (la página de texturas) vale 1 sólo para los subtipos dibujados aquí; los demás (cápsulas grises) usan páginas que no sobrevivieron. Qué figura de la página es cada uno sigue el orden de fotogramas de sus scripts de animación (coherente, ver docs/CIVILIZACION.md).',
                     'Paletas (filas reales de 16 colores de los ficheros CLUT del juego):'] + ['  - ' + t for t in PAL_STATUS_ES[area]]),
        ('Fuente', ['Datos decompilados de I0NFO2: juego de NPC %s, lista de vecinos en %s (registros de 28 bytes), objetos en %s.' % (I['set'], I['lst'], I['props']),
                    'Perímetro: tools/civ_rebuild.py (reconstrucción). Generado por tools/civ_sheets.py --sheet plan1x (Tombi-Decomp).']),
    ]


def plan_legend(area, plan_png, size, frames, lang='en'):
    I = PLAN_INFO[area]; W, H = size
    blocks = plan_blocks_es(area, W, H, frames) if lang == 'es' else [
        ('Place', [I['place'] + '.', 'A cut top-down section: the code and placements survive in I0NFO2, the load list is empty.',
                   'No retail exit leads into it. ' + I['exits'] + ' (exit rectangles in ' + I['step'] + ', destinations in the MAIN0 exit table 0x80079A48).']),
        ('Scale', ['1 image pixel = 1 background pixel. Image %d x %d px: the placements plus the reconstructed picture (the original background picture is lost).' % (W, H),
                   'World position (x, y) -> background pixel (x + 128, y - 40) (topdownCollideMapTile 0x800EE18C).']),
        ('Marks', ['Blue rectangle: house trigger rectangle (TOPDOWN_TRIGGER_RECT_LISTS 0x800FEC9C); inside it the roof hides and the interior shows.',
                   'Green box or line: the area a resident patrols (record bounds, behaviour 1-4).',
                   'Yellow dot: prop / scenery position. Props are dots because their sprite tables are lost.',
                   'Thick red contour: town perimeter = outer outline of the area walkable from the entry spawn. RECONSTRUCTION, NOT ORIGINAL DATA: flood fill over the traced collision of tools/civ_rebuild.py (map8E.bin, cell 9 blocks); the original map is lost.',
                   'Thin purple frame: the reconstructed picture border (camera limits x <= W-288, y <= H-112).',
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


# ------------------------------------------------------------------ town perimeter from the reconstruction (not original)
MAPS_DIR = os.environ.get("CIV_MAPS", "/dev/shm/maps")


def perimeter(area):
    """(reach mask in px, picture W, H) from tools/civ_rebuild.py's cell map MAPS_DIR/AREAnn/map8E.bin, flood-filled
    from the layout spawn (cells 9 block, everything else passes), or None when the map is missing.
    RECONSTRUCTION: the collision is traced, the original map is lost."""
    import json, numpy as np
    fn = os.path.join(MAPS_DIR, 'AREA%d' % area, 'map8E.bin')
    if not os.path.exists(fn):
        print('warning: %s missing (run tools/civ_rebuild.py --out %s): no perimeter' % (fn, MAPS_DIR)); return None
    b = open(fn, 'rb').read(); cw, ch = struct.unpack_from('<HH', b)
    c = np.frombuffer(b[4:4 + cw * ch], np.uint8).reshape(ch, cw)
    L = json.load(open(os.path.join(HERE, 'civ_rebuild_layout.json')))
    sp = L['towns'][str(area)]['spawn']
    sx, sy = T.world_to_px(*sp)
    ok = np.zeros((ch, cw), bool); st = [(sy // 8, sx // 8)]
    while st:
        y, x = st.pop()
        if 0 <= y < ch and 0 <= x < cw and not ok[y, x] and (c[y, x] & 0x7F) != 9:
            ok[y, x] = True; st += [(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)]
    # outer outline only: fill the holes (solid bodies inside the town) = everything not reachable from the border
    out = np.zeros((ch + 2, cw + 2), bool); P = np.pad(ok, 1); st = [(0, 0)]
    while st:
        y, x = st.pop()
        if 0 <= y < ch + 2 and 0 <= x < cw + 2 and not out[y, x] and not P[y, x]:
            out[y, x] = True; st += [(y + 1, x), (y - 1, x), (y, x + 1), (y, x - 1)]
    town = ~out[1:-1, 1:-1]
    return np.kron(town, np.ones((8, 8), bool)), cw * 8, ch * 8


def perimeter_segments(area):
    """(segments [((px, py), (px, py))] along the cell edges of the town outline, picture W, H) in background px, or None."""
    r = perimeter(area)
    if r is None: return None
    m, PW, PH = r; t = m[::8, ::8]; ch, cw = t.shape; seg = []
    g = lambda y, x: 0 <= y < ch and 0 <= x < cw and t[y, x]
    for y in range(ch):
        for x in range(cw):
            if not t[y, x]: continue
            X, Y = x * 8, y * 8
            if not g(y - 1, x): seg.append(((X, Y), (X + 8, Y)))
            if not g(y + 1, x): seg.append(((X, Y + 8), (X + 8, Y + 8)))
            if not g(y, x - 1): seg.append(((X, Y), (X, Y + 8)))
            if not g(y, x + 1): seg.append(((X + 8, Y), (X + 8, Y + 8)))
    return seg, PW, PH


PERIM_ES = ('Contorno rojo grueso = perímetro del pueblo: borde exterior de la zona por la que se puede andar desde la entrada. '
            'RECONSTRUCCIÓN, NO DATOS ORIGINALES: relleno sobre la colisión trazada de tools/civ_rebuild.py (map8E.bin, la celda 9 bloquea); el mapa original se perdió.')
PERIM_EN = ('Thick red contour = town perimeter: outer outline of the area walkable from the entry. '
            'RECONSTRUCTION, NOT ORIGINAL DATA: flood fill over the traced collision of tools/civ_rebuild.py (map8E.bin, cell 9 blocks); the original map is lost.')
BORDER_EN = 'Thin purple frame = reconstructed picture border (camera limits x <= W-288, y <= H-112).'
BORDE_ES = 'Marco morado fino = borde del fondo reconstruido (límites de cámara x <= W-288, y <= H-112).'
PERIM_RED = (220, 30, 30); PERIM_PUR = (120, 60, 160)


def draw_perimeter_on(dr, area, f, w=3, wb=2):
    """Draw the town outline (thick red) and the reconstructed picture border (thin purple); f maps background px to
    image coordinates. Returns False when the reconstruction is missing."""
    r = perimeter_segments(area)
    if r is None: return False
    seg, PW, PH = r
    dr.line([f(0, 0), f(PW, 0), f(PW, PH), f(0, PH), f(0, 0)], fill=PERIM_PUR, width=wb)
    for a, b in seg: dr.line([f(*a), f(*b)], fill=PERIM_RED, width=w)
    return True


def draw_perimeter(im, area, px0, py0):
    draw_perimeter_on(ImageDraw.Draw(im), area, lambda x, y: (x - px0, y - py0), 3, 2)


# ------------------------------------------------------------------ panel B alone at 1:1 (civ_plan_aNN_1x*.png)
def plan1x(area, outd, langs=('en',)):
    """Panel B of the schematic at actual size (one image pixel = one background pixel, the estimated picture size),
    no labels: grey ground + house rectangles, patrols, props and resident sprites; and a transparent variant."""
    C = AREAS[area]; var = T.CUT[area]
    actors, props = T.npc_set(var); rects = T.trigger_rects(area)
    xs = [a['x'] for a in actors] + [q['x'] for l in props[:1] for q in l] + [r[0] for r in rects] + [r[2] for r in rects] + [e[0][0] for e in C['exits']]
    ys = [a['y'] for a in actors] + [q['y'] for l in props[:1] for q in l] + [r[1] for r in rects] + [r[3] for r in rects] + [e[0][1] for e in C['exits']]
    px0 = (min(xs) + 128 - 40) // 32 * 32; py0 = (min(ys) - 40 - 70) // 32 * 32
    px1 = (max(xs) + 128 + 40 + 31) // 32 * 32; py1 = (max(ys) - 40 + 40 + 31) // 32 * 32
    pr = perimeter(area)
    if pr: px0, py0, px1, py1 = min(px0, 0), min(py0, 0), max(px1, pr[1]), max(py1, pr[2])
    W, H = px1 - px0, py1 - py0
    P = lambda x, y: (x + 128 - px0, y - 40 - py0)
    p = page(C['atlas']); cs = comps(p)
    for bg, sfx in (((226, 222, 212, 255), ''), ((0, 0, 0, 0), '_transparent')):
        im = Image.new('RGBA', (W, H), bg); draw_perimeter(im, area, px0, py0); dr = ImageDraw.Draw(im)
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
    for lang in langs:
        lg = plan_legend(area, os.path.join(outd, 'civ_plan_a%d_1x.png' % area), (W, H), [a['frame'] for a in actors], lang)
        out = os.path.join(outd, 'civ_plan_a%d_1x_%s.png' % (area, 'leyenda' if lang == 'es' else 'legend')); lg.save(out); print(out, lg.size)


# ------------------------------------------------------------------ "who goes where" sheet (civ_personajes_aNN[_en].png)
# One sheet per town: every resident of the I0NFO2 NPC set at its record position, numbered in record order (the numbers
# of the docs' tables), with its drawing only when the disc gives it, a confidence mark for every drawing and name, and a
# table (no. | name | sprite | confidence | where | behaviour | what they say). Houses, exits and props from the same data.
CONF_COL = {'sabido': (20, 130, 50), 'probable': (215, 120, 0), 'suposicion': (130, 70, 180), 'desconocido': (120, 120, 120)}
CONF_LBL = {'es': {'sabido': 'sabido', 'probable': 'probable', 'suposicion': 'suposición', 'desconocido': 'desconocido'},
            'en': {'sabido': 'known', 'probable': 'probable', 'suposicion': 'guess', 'desconocido': 'unknown'}}


def _teller_page():
    """AREA02/B300 words 64..127 (the Green/Blue Fortune Teller page, docs: "The fortune-teller of house H3")."""
    d = A.read_resource('AREA02/B300')
    return [[(d[y * 384 + 128 + x // 2] >> ((x & 1) * 4)) & 15 for x in range(256)] for y in range(256)]


# (area, subtype) -> (page, component index on the page, (CLUT file, row), confidence, why ES, why EN). Only these
# subtypes have a drawing on a surviving page; all others are drawn as a neutral silhouette.
PERS_SPRITES = {
    (11, 0): ('A06100', 0, ('AREA11/CLUT02', 4), 'probable',
              'página probada por el código (w1e 1); figura por orden de fotogramas; paleta fila 4 por los colores de TCRF',
              'page proven by the code (w1e 1); figure by frame order; palette row 4 from TCRF colours'),
    (11, 4): ('B300', 3, ('AREA02/CLUT04', 21), 'probable',
              'su página en AREA11 se perdió; mismo dibujo en AREA02/B300 con la misma disposición de fotogramas; fila 21 = colores de la hoja de la comunidad (ningún fichero la usa)',
              'his AREA11 page is lost; the same drawing is on AREA02/B300 with the same frame layout; row 21 = the community sheet colours (no file sets it)'),
    (16, 0): ('A07600', 9, ('AREA16/CLUT07', 21), 'probable',
              'página probada (w1e 1), figura por orden de fotogramas; paleta fila 21 hallada por parecido con TCRF (colores desplazados hasta 9 niveles)',
              'page proven (w1e 1), figure by frame order; palette row 21 found by likeness to TCRF (colours off by up to 9 levels)'),
    (16, 1): ('A07600', 28, ('AREA16/CLUT07', 37), 'probable',
              'página probada (w1e 1), figura por orden de fotogramas; paleta fila 37 por los colores de TCRF',
              'page proven (w1e 1), figure by frame order; palette row 37 from TCRF colours'),
    (16, 2): ('A07600', 34, ('AREA16/CLUT07', 53), 'sabido',
              'página probada (w1e 1); fila 53 coincide píxel a píxel con el sprite de TCRF; sus scripts 16-23 son las animaciones de levantar peso',
              'page proven (w1e 1); row 53 matches TCRF\'s sprite pixel for pixel; its scripts 16-23 are the lifting anims'),
    (17, 0): ('A08000', 2, ('AREA17/CLUT03', 1), 'sabido',
              'página probada (w1e 1); fila 1 = la que usa el juego final para este mismo personaje (AREA17/D205)',
              'page proven (w1e 1); row 1 = the row the retail game uses for this same character (AREA17/D205)'),
}
PERS_LOST = {  # why a subtype has no drawing
    11: ('su página de texturas (w1e 0x0A/0x0B/0x10) no sobrevive', 'its texture page (w1e 0x0A/0x0B/0x10) did not survive'),
    16: ('su página de texturas (w1e 0x11) no sobrevive', 'its texture page (w1e 0x11) did not survive'),
    17: ('su página de texturas (w1e 0x0A/0x0B/0x10) no sobrevive', 'its texture page (w1e 0x0A/0x0B/0x10) did not survive'),
    18: ('la página A08600 sobrevive (w1e 1), pero no se ha identificado qué figura es cada subtipo', 'page A08600 survives (w1e 1), but which figure each subtype is has not been identified'),
    (17, 4): ('el subtipo 4 no tiene tabla de animación (0 scripts): el juego no lo dibujaría', 'subtype 4 has no anim table (0 scripts): the game would not draw it'),
}

# Exits: (x0, y0, x1, y1) in world coordinates, None = open towards the map edge (the code only tests x > / y <), and the
# destination. From topdownSectionStepC/D/E + the MAIN0 exit table 0x80079A48 (docs: "Step 2"); names from D0SPSUB 0x80108E5C.
PERS_EXITS = {
    11: [((0x22B, 0x169, None, 0x1CC), 'Ciudad de civilización (AREA18 s1, punto 1)', 'Civilization Town (AREA18 s1, spawn 1)'),
         ((0xC7, None, 0x107, 0x4F), 'Lumberjack Factory (AREA11 s1, punto 0)', 'Lumberjack Factory (AREA11 s1, spawn 0)')],
    16: [((None, 0x208, -0x74, 0x211), 'Ciudad de civilización (AREA18 s1, punto 3)', 'Civilization Town (AREA18 s1, spawn 3)'),
         ((0x6B, 0x13F, 0x72, 0x140), 'Dried Wishing Well (AREA11 s2, punto 0), por un hoyo de 7 x 1 px', 'Dried Wishing Well (AREA11 s2, spawn 0), through a 7 x 1 px hole'),
         ((0x93, None, None, 0x4A), 'Clock Tower (AREA16 s1, punto 0; también recortada)', 'Clock Tower (AREA16 s1, spawn 0; also cut)')],
    17: [((0xA7, 0x23A, 0x117, None), 'Ciudad de civilización (AREA18 s1, punto 2)', 'Civilization Town (AREA18 s1, spawn 2)'),
         ((0xB0, None, 0x10F, 0x50), 'Iron Castle (AREA17 s1, punto 0)', 'Iron Castle (AREA17 s1, spawn 0)')],
    18: [],
}

# Per resident, in record order: name (ES, EN), name source (ES, EN), name confidence, line(s) (ES, EN), extra note (ES, EN).
# Lines are the docs' glosses of the JP text (CIVILIZACION.md / CIVILIZATION.md, "The cut towns, one by one").
def _r(n, ns, nc, say, note=('', '')):
    return dict(n=n, ns=ns, nc=nc, say=say, note=note)


_LFW = ('Lumberjack Factory Worker (trabajador con casco)', 'Lumberjack Factory Worker (hard-hat worker)')
_TCRF = ('nombre de TCRF', 'TCRF name')
_NONAME = ('sin nombre', 'no name')
_NN = lambda sub: ('vecino sin nombre (subtipo %d)' % sub, 'unnamed resident (subtype %d)' % sub)
_DOC_LFW = ('la doc lo llama «Lumberjack Factory Worker», pero es el subtipo 1 (página perdida): sin base',
            'the docs call it "Lumberjack Factory Worker", but it is subtype 1 (lost page): unsupported')
PERS = {
    11: [
        _r(_LFW, _TCRF, 'probable', ('«...Ahora puedo hacer algo muy interesante...» (texto PAL #227)', '"...Now I can make something really interesting..." (PAL text #227)')),
        _r(_LFW, _TCRF, 'probable', ('No habla: activa una bandera (D_8009CFA9)', 'No line: sets a flag (D_8009CFA9)')),
        _r(_LFW, _TCRF, 'probable', ('«No funciona, pero esta máquina es de hierro... me vendría bien»', '"It doesn\'t work, but this machine is iron... I could use it"')),
        _r(_LFW, _TCRF, 'probable', ('«En esta choza vive un adivino, pero pone una condición»', '"A fortune-teller lives in this hut, but he has a condition"')),
        _r(_LFW, _TCRF, 'probable', ('«La madera es lo mejor; la piedra y el hierro son fríos»', '"Wood is the best; stone and iron are cold"')),
        _r(_NN(1), _DOC_LFW, None, ('«No funciona, pero esta máquina es de hierro...»', '"It doesn\'t work, but this machine is iron..."')),
        _r(('¿Male Lumberjack Town Resident?', 'Male Lumberjack Town Resident?'),
           ('nombre de la doc, sin base: el anciano está en los fotogramas extra del subtipo 0; éste es el subtipo 2 (página perdida)',
            'docs name, unsupported: the old man is in subtype 0\'s extra frames; this is subtype 2 (lost page)'), 'suposicion',
           ('«Bajo el suelo hay un laberinto; no te metas sin cuidado»', '"There\'s a maze under the floor; don\'t wander in carelessly"')),
        _r(_NN(1), _DOC_LFW, None, ('«A esa choza sólo se llega por debajo del suelo»', '"You can only reach that hut from under the floor"')),
        _r(_NN(1), _DOC_LFW, None, ('Según el progreso: «¿Un engranaje? De madera se rompería» → «Tengo sed, iré a por un coco» → «El Pueblo Karakuri se hundió»',
                                    'By progress: "A gear? Made of wood it would break" → "I\'m thirsty, I\'ll get a coconut" → "Karakuri Village sank"')),
        _r(_NN(1), _DOC_LFW, None, ('«Al taller se entra por arriba; ve a echar un vistazo»', '"The workshop entrance is up there; go take a look"')),
        _r(('¿Male Lumberjack Town Resident (el anciano)?', 'Male Lumberjack Town Resident (the old man)?'),
           ('nombre de la doc, sin base: subtipo 3 (página perdida); la reconstrucción pone aquí al anciano del vídeo por suposición',
            'docs name, unsupported: subtype 3 (lost page); the reconstruction puts the FMV\'s old man here as a guess'), 'suposicion',
           ('«Este cartel es viejo y peligroso» → «Siempre tan lleno de energía»', '"This sign is old and dangerous" → "You\'re always so full of energy"')),
        _r(('Adivino verde (Green Fortune Teller)', 'Green Fortune Teller'),
           ('pista japonesa del evento 89 («escucha al adivino verde») + sus fotogramas = los del adivino del túnel',
            'JP hint of event 89 ("listen to the Green Fortune Teller") + his frames = the tunnel teller\'s'), 'probable',
           ('«Sólo hablo con quien tenga 300.000 PA» (k3 dx16-19). Su dlg 11 no tiene caso: en el código no dice nada (el port lo reconstruye)',
            '"I only talk to those with 300,000 AP" (k3 dx16-19). His dlg 11 has no case: in the code he says nothing (the port reconstructs it)')),
    ],
    16: [
        _r(_NN(3), _NONAME, None, ('«¡Aúpa... YA!» (levanta algo)', '"Heave... HO!" (lifting something)')),
        _r(_NN(3), _NONAME, None, ('«¡Hop... AAAH!» (levanta algo)', '"Hup... HAAA!" (lifting something)')),
        _r(('Female Stonetown Resident (mujer con capucha)', 'Female Stonetown Resident (hooded woman)'), _TCRF, 'probable',
           ('«Esto pegado a la pared es un ascensor... pero no se mueve»', '"This thing on the wall is supposed to be a lift... but it doesn\'t move"')),
        _r(('Male Stonetown Resident (el forzudo)', 'Male Stonetown Resident (the strongman)'), _TCRF, 'sabido',
           ('«¿No puedes entrar en los otros pueblos? Ni lo sé ni me importa» → luego habla del engranaje y del generador',
            '"Can\'t get into the other towns? Don\'t know, don\'t care" → later talks about the gear and the generator')),
        _r(('Female Stonetown Resident (mujer con capucha)', 'Female Stonetown Resident (hooded woman)'), _TCRF, 'probable',
           ('«La gente de los otros pueblos...» → «¿Un engranaje que no se rompa? Si fuera de hierro...»', '"The folks from the other towns..." → "A gear that won\'t break? If it were iron..."')),
        _r(_NN(3), _NONAME, None, ('«¿Primera vez en este pueblo?» → «Hace mucho intenté meterme en el río...»', '"First time in this town?" → "Long ago I tried to go into the river..."')),
        _r(('Stonetown Kid (el chico)', 'Stonetown Kid (the boy)'), ('nombre de Folkin (TCRF); frases infantiles', 'Folkin\'s (TCRF) name; childish lines'), 'probable',
           ('«¿Debería entrar...?» → «Vencerás al Cerdiablo que hechizó esta tierra, ¿verdad?»', '"Should I go in...?" → "You\'ll beat the Evil Pig who cursed this land, right?"')),
        _r(('Stonetown Kid (el chico)', 'Stonetown Kid (the boy)'), ('nombre de Folkin (TCRF); frases infantiles', 'Folkin\'s (TCRF) name; childish lines'), 'probable',
           ('«Abrí en secreto la puerta entre el pueblo de hierro y el de madera... ¡es un secreto!» → «Descubrieron mi travesura...»',
            '"I secretly opened the door between the Iron and Wood towns... it\'s a secret!" → "They found out my prank..."')),
        _r(('artesano', 'craftsman'), ('por su frase (nuestra lectura)', 'from his line (our reading)'), 'suposicion',
           ('«Mmm... quiero fabricar algo» / «Ahora que lo pienso...»', '"Hmm... I want to make something" / "Come to think of it..."')),
        _r(_NN(3), _NONAME, None, ('No habla: activa una bandera (D_8009CFA7)', 'No line: sets a flag (D_8009CFA7)')),
        _r(_NN(3), _NONAME, None, ('No habla: activa una bandera (D_8009CFA7)', 'No line: sets a flag (D_8009CFA7)')),
        _r(_NN(3), _NONAME, None, ('Sin diálogo en el código (dlg 11 sin caso)', 'No dialogue in the code (dlg 11 has no case)')),
        _r(('vecino que vio a la bruja Mizuno', 'resident who saw Mizuno the witch'),
           ('la doc lo llama «el chico», pero es el subtipo 3, no el del chico (1)', 'the docs call him "the boy", but he is subtype 3, not the kid\'s (1)'), 'suposicion',
           ('Habla de la bruja Mizuno (texto PAL #229) e inicia el evento 0x67', 'Talks about Mizuno the witch (PAL text #229) and starts event 0x67')),
    ],
    17: [
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«He oído que arreglaste la torre del reloj de piedra»', '"I hear you fixed the stone clock tower"')),
        _r(_NN(1), _NONAME, None, ('«¡Buen material! Espera un momento...»', '"Good material! Wait a moment..."')),
        _r(('el herrero', 'the smith'), ('por su código (topdownA17ActorDialogueBank2)', 'from his code (topdownA17ActorDialogueBank2)'), 'sabido',
           ('«¿Necesitas algo? ¿Un engranaje de hierro?» (evento 0x63) → te da el engranaje (objeto 0x3E)', '"Need something? An iron gear?" (event 0x63) → gives you the gear (item 0x3E)')),
        _r(('el fabricante de bombas', 'the bomb maker'), ('por su código (topdownA17ActorDialogueBank3)', 'from his code (topdownA17ActorDialogueBank3)'), 'sabido',
           ('«No he visto tu cara...» → (evento 0x69) «Primero necesito electricidad» → «Ahora la mezcla de la bomba» → «¿Más bombas?»',
            '"Haven\'t seen your face before..." → (event 0x69) "First I need electricity" → "Now the bomb mixture" → "More bombs?"')),
        _r(_NN(4), _NONAME, None, ('Sin diálogo en el código (dlg 4 sin caso)', 'No dialogue in the code (dlg 4 has no case)'),
           ('el registro dice casa 1, pero (223, 0) está fuera de C1 y dentro de la zona de la salida 1', 'the record says house 1, but (223, 0) is outside H1 and inside the exit 1 zone')),
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«¿Quieres subir? Entra en esta choza»', '"Want to go up? Go into this hut"')),
        _r(_NN(1), _NONAME, None, ('«Bienvenido, éste es el pueblo de los Hombres de hierro»', '"Welcome, this is the Iron Men\'s town"')),
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«El hierro es resistente y duradero» → «¿Arreglaste hasta el generador?»', '"Iron is sturdy and lasts" → "You even fixed the generator?"')),
        _r(_NN(3), _NONAME, None, ('«¿Por qué siempre andamos peleados?»', '"Why are we always at each other\'s throats?"')),
        _r(_NN(1), _NONAME, None, ('«Todos con su madera y su piedra...» → «Madera, piedra y hierro: lo importante es el equilibrio»',
                                   '"Everyone and their wood and stone..." → "Wood, stone and iron: balance is what matters"')),
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«Tómame como referencia y baja recto para salir»', '"Use me as a landmark and go straight down to leave"')),
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«Antes el ascensor funcionaba con máquinas»', '"The lift used to run on machinery"')),
        _r(_NN(1), _NONAME, None, ('«Sube al ascensor y por el camino del medio llegas al Castillo de hierro»', '"Ride the lift and take the middle path to the Iron Castle"')),
        _r(_NN(3), _NONAME, None, ('«Ese viejo Gantetsu no me deja ir al Castillo» → «Perdona que me burlara de ti»', '"That old Gantetsu won\'t let me go to the Castle" → "Sorry I made fun of you"')),
        _r(('Hombre de hierro', 'Iron Man'), ('vecino de la Ciudad de hierro (doc)', 'Iron Town resident (docs)'), 'sabido',
           ('«Esta tierra también está hechizada por los Cerdiablos»', '"This land is under the Evil Pigs\' spell too"')),
        _r(_NN(2), _NONAME, None, ('«Si buscas algo de hierro, ve a la choza del ascensor» → «¿Sabes nadar bien?»', '"If you want something iron, try the lift hut" → "Are you a good swimmer?"')),
    ],
}
PERS[18] = [None] * 10   # filled from the records (sprite viewer, no talk code)

PERS_TITLE = {
    11: ('Lumberjack Town (Ciudad leñadora) — AREA11 sección 0: quién va dónde', 'Lumberjack Town — AREA11 section 0: who goes where'),
    16: ('Stonetown (Ciudad de piedra) — AREA16 sección 0: quién va dónde', 'Stonetown — AREA16 section 0: who goes where'),
    17: ('Iron Town (Ciudad de hierro) — AREA17 sección 0: quién va dónde', 'Iron Town — AREA17 section 0: who goes where'),
    18: ('Pueblo reunido — AREA18 sección 0 (sólo datos de prueba): quién va dónde', 'Reunited town — AREA18 section 0 (test data only): who goes where'),
}
PERS_SRC = {11: ('0D', '0x80100080', '0x800FFE24', 'topdownA11ActorOpenDialogue 0x800F1C88', 'topdownSectionStepC 0x800E8D98'),
            16: ('0E', '0x80100364', '0x801001D4', 'topdownA16ActorOpenDialogue 0x800F2E84', 'topdownSectionStepD 0x800E8ED4'),
            17: ('0F', '0x80100784', '0x80100368', 'topdownA17ActorOpenDialogue 0x800F4110', 'topdownSectionStepE 0x800E9040'),
            18: ('10', '-', '-', 'topdownA18ActorHandler 0x800F548C', '-')}


def _prop_note(area, i, q, lang):
    """What the code (or the docs) says about prop i (0-based record index = its wb8)."""
    es = lang == 'es'
    if q['room']:
        return ('interior, sólo se ve dentro de C%d' if es else 'interior, only shown inside H%d') % q['room'], 'sabido'
    if area == 16 and i == 5:
        return ('cambia de anim y de sitio con D_8009CFA8 (código); doc: probablemente el reloj reparado' if es else
                'changes anim and place with D_8009CFA8 (code); docs: most likely the repaired clock'), 'probable'
    if area == 17 and i in (13, 14):
        return ('plataforma móvil = ascensor (topdownA17SceneryUpdate, lleva a Tombi)' if es else
                'moving platform = lift (topdownA17SceneryUpdate, carries Tomba)'), 'sabido'
    if area == 17 and i == 15:
        return ('su anim suma D_8009CFA1: cambia con el progreso' if es else 'its anim adds D_8009CFA1: changes with progress'), 'sabido'
    if area == 17 and q['anim'] == 23:
        return ('fila de iguales (anim 23); ¿raíles del ascensor o muro? (la plataforma P%d está a la misma y)' % (14 if q['y'] > 200 else 15) if es else
                'row of identical props (anim 23); lift rails or wall? (platform P%d is at the same y)' % (14 if q['y'] > 200 else 15)), 'suposicion'
    return ('el código no dice qué es' if es else 'the code does not say what it is'), 'desconocido'


def _sil(w, h, col=(150, 150, 150)):
    """Neutral silhouette for a resident without a known drawing (not a drawing of the character)."""
    im = Image.new('RGBA', (w, h), (0, 0, 0, 0)); d = ImageDraw.Draw(im)
    hr = w * 0.30
    d.ellipse([w / 2 - hr, 1, w / 2 + hr, 1 + 2 * hr], fill=col + (200,), outline=(80, 80, 80, 255), width=2)
    d.rounded_rectangle([w * 0.12, 2 * hr + 3, w * 0.88, h - 1], radius=int(w * 0.3), fill=col + (200,), outline=(80, 80, 80, 255), width=2)
    d.text((w / 2, 2 * hr + 3 + (h - 2 * hr - 3) / 2), '?', font=F(int(w * 0.6), True), fill=(255, 255, 255, 255), anchor='mm')
    return im


def _dashed_rect(dr, box, col, w=3, dash=8):
    x0, y0, x1, y1 = box
    for (a, b, c, d) in ((x0, y0, x1, y0), (x1, y0, x1, y1), (x1, y1, x0, y1), (x0, y1, x0, y0)):
        L = math.hypot(c - a, d - b); n = max(1, int(L // (2 * dash)))
        for k in range(n + 1):
            t0 = min(1, 2 * k * dash / L) if L else 0; t1 = min(1, (2 * k + 1) * dash / L) if L else 0
            if t1 > t0: dr.line([a + (c - a) * t0, b + (d - b) * t0, a + (c - a) * t1, b + (d - b) * t1], fill=col, width=w)


def _dashed_line(dr, a, b, col, w=2, dash=7):
    L = math.hypot(b[0] - a[0], b[1] - a[1]); n = int(L // (2 * dash)) + 1
    for k in range(n):
        t0 = 2 * k * dash / L if L else 0; t1 = min(1, (2 * k + 1) * dash / L) if L else 0
        if t0 < 1: dr.line([a[0] + (b[0] - a[0]) * t0, a[1] + (b[1] - a[1]) * t0, a[0] + (b[0] - a[0]) * t1, a[1] + (b[1] - a[1]) * t1], fill=col, width=w)


_PAGES = {}


def _pers_sprite(area, sub):
    """(RGBA 1x drawing or None, confidence, why ES, why EN)."""
    k = (area, sub)
    if k in PERS_SPRITES:
        pg, ci, (cf, row), conf, wes, wen = PERS_SPRITES[k]
        if pg not in _PAGES:
            p = _teller_page() if pg == 'B300' else page(pg); _PAGES[pg] = (p, comps(p))
        p, cs = _PAGES[pg]; pal = clut_row(cf, row); b = cs[ci]
        w, h = b[2] - b[0] + 1, b[3] - b[1] + 1
        im = Image.new('RGBA', (w, h), (0, 0, 0, 0)); px = im.load()
        for y in range(h):
            for x in range(w):
                v = p[b[1] + y][b[0] + x]
                if v: px[x, y] = pal[v] + (255,)
        return im, conf, wes, wen
    wes, wen = PERS_LOST.get(k, PERS_LOST[area])
    return None, 'desconocido', wes, wen


BEH_ES = {0: 'quieto', 1: 'patrulla vertical', 2: 'patrulla horizontal', 3: 'patrulla diagonal (tipo 3)', 4: 'patrulla diagonal (tipo 4)', 5: 'guionizado', 6: 'guionizado'}
BEH_EN = {0: 'stands still', 1: 'vertical patrol', 2: 'horizontal patrol', 3: 'diagonal patrol (type 3)', 4: 'diagonal patrol (type 4)', 5: 'scripted', 6: 'scripted'}
COND16 = {8: ('sólo aparece tras el 1er avance', 'only after the first milestone'), 11: ('sólo antes del 1er avance', 'only before the first milestone'),
          12: ('sólo antes del 1er avance', 'only before the first milestone'), 15: ('sólo aparece al final', 'only at the end')}


def personajes(area, lang, out):
    es = lang == 'es'; L = 0 if es else 1; HL = 'C' if es else 'H'
    var = T.CUT[area]; actors, props = T.npc_set(var); rects = T.trigger_rects(area)
    props0 = props[0] if props else []; exits = PERS_EXITS[area]
    rows = PERS[area]
    if area == 18:
        rows = [_r(('vecino del pueblo reunido (subtipo %d)' % a['sub'], 'reunited-town resident (subtype %d)' % a['sub']), _NONAME, None,
                   ('No habla: su manejador sólo cambia de animación mientras se pulsa un botón (visor de sprites). El texto JP de AREA18 (#209-#221) no tiene llamada',
                    'No line: its handler only cycles the animation while a button is held (a sprite viewer). AREA18\'s JP text (#209-#221) has no caller'))
                for a in actors]
    assert len(rows) == len(actors), (area, len(rows), len(actors))
    boxes = []
    for a in actors:
        if not a['beh']: boxes.append(None); continue
        x0, y0, x1, y1 = a['bx0'], a['by0'], a['bx1'], a['by1']
        if var == 0x0E: x0, y0, x1, y1 = a['x'] + x0, a['y'] + y0, a['x'] + x1, a['y'] + y1
        boxes.append((min(x0, x1), min(y0, y1), max(x0, x1), max(y0, y1)))
    xs = [a['x'] for a in actors] + [q['x'] for q in props0] + [r[0] for r in rects] + [r[2] for r in rects] + [b[0] for b in boxes if b] + [b[2] for b in boxes if b]
    ys = [a['y'] for a in actors] + [q['y'] for q in props0] + [r[1] for r in rects] + [r[3] for r in rects] + [b[1] for b in boxes if b] + [b[3] for b in boxes if b]
    for (r, _, _) in exits:
        xs += [v for v in (r[0], r[2]) if v is not None]; ys += [v for v in (r[1], r[3]) if v is not None]
    wx0, wx1, wy0, wy1 = min(xs) - 60, max(xs) + 60, min(ys) - 100, max(ys) + 50
    wx0, wy0 = wx0 // 32 * 32, wy0 // 32 * 32; wx1, wy1 = -(-wx1 // 32) * 32, -(-wy1 // 32) * 32
    S = 1.5; AX = 56; M = 30; TOP = 150
    PW, PH = int((wx1 - wx0) * S), int((wy1 - wy0) * S)
    ox, oy = M + AX, TOP + 34
    P = lambda x, y: (ox + (x - wx0) * S, oy + (y - wy0) * S)
    # ---- sprites
    spr = []
    for a in actors:
        im, conf, wes, wen = _pers_sprite(area, a['sub'])
        if im is None:
            big = area == 16 and a['sub'] == 3   # box 16x32 instead of 8x16
            im = _sil(int((28 if big else 24) * S), int((54 if big else 46) * S))
            spr.append((im, conf, wes, wen, False))
        else:
            spr.append((im.resize((int(im.width * S), int(im.height * S)), Image.NEAREST), conf, wes, wen, True))
    # ---- table layout (to the right of the plan)
    fT = F(18); fTb = F(19, True); fS = F(15); fH = F(19, True)
    cols = [('Nº' if es else 'No.', 58), ('nombre' if es else 'name', 320), ('sprite', 120), ('confianza del sprite' if es else 'sprite confidence', 250),
            ('dónde' if es else 'where', 210), ('comportamiento' if es else 'behaviour', 250), ('qué dice (resumen del texto japonés)' if es else 'what they say (summary of the JP text)', 500)]
    TW = sum(c[1] for c in cols)
    tmp = ImageDraw.Draw(Image.new('RGB', (8, 8)))
    cells = []
    for i, (a, r) in enumerate(zip(actors, rows)):
        im, conf, wes, wen, real = spr[i]
        where = (('dentro de %s%d' % (HL, a['room'])) if es else ('inside %s%d' % (HL, a['room']))) if a['room'] else ('en la calle' if es else 'outdoors')
        where += ' (%d, %d)' % (a['x'], a['y'])
        if r['note'][L]: where += '. ' + r['note'][L]
        beh = (BEH_ES if es else BEH_EN)[a['beh']]
        if boxes[i]:
            b = boxes[i]
            beh += (': x %d..%d, y %d..%d' % (b[0], b[2], b[1], b[3])) if (b[0] != b[2] and b[1] != b[3]) else (': x %d..%d' % (b[0], b[2]) if b[1] == b[3] else ': y %d..%d' % (b[1], b[3]))
        if area == 16 and a['bc'] in COND16: beh += '; ' + COND16[a['bc']][L]
        beh += '; dlg %d' % a['dlg']
        nm = wrap(tmp, r['n'][L], fTb, cols[1][1] - 14); ns = wrap(tmp, r['ns'][L], fS, cols[1][1] - 14)
        cf = wrap(tmp, ('' if real else ('Sprite desconocido: ' if es else 'Sprite unknown: ')) + (wes if es else wen), fS, cols[3][1] - 14)
        wh = wrap(tmp, where, fT, cols[4][1] - 14); bh = wrap(tmp, beh, fT, cols[5][1] - 14); sy = wrap(tmp, r['say'][L], fT, cols[6][1] - 14)
        hgt = max(len(nm) * 24 + len(ns) * 19 + 8, 34 + len(cf) * 19, len(wh) * 23, len(bh) * 23, len(sy) * 23, (im.height / S * 2 if real else im.height) + 8, 50) + 16
        cells.append((nm, ns, cf, wh, bh, sy, int(hgt)))
    TH = 44 + sum(c[6] for c in cells)
    # ---- notes below the plan
    notes = []   # (title, [(colour or None, text)])
    marks = [((40, 110, 230), ('Rectángulo azul %s1…: casa (TOPDOWN_TRIGGER_RECT_LISTS); al entrar Tombi se oculta el tejado y se ven los de dentro' % HL) if es else
              ('Blue rectangle %s1…: house (TOPDOWN_TRIGGER_RECT_LISTS); when Tomba walks in the roof hides and the residents inside show' % HL)),
             ((215, 40, 40), 'Círculo rojo con número: vecino en la calle; la línea lleva a sus pies (posición del registro)' if es else 'Red numbered circle: resident outdoors; the line leads to the feet (record position)'),
             ((150, 40, 170), 'Círculo morado: vecino dentro de una casa' if es else 'Purple circle: resident inside a house'),
             ((30, 150, 50), 'Línea o caja verde: por dónde camina (patrulla); en diagonal, caja con su diagonal' if es else 'Green line or box: where they walk (patrol); diagonal walkers get a box with its diagonal'),
             ((235, 130, 20), 'Rectángulo naranja: salida (rectángulo del código; un lado abierto llega al borde)' if es else 'Orange rectangle: exit (code rectangle; an open side runs to the edge)'),
             ((240, 200, 20), 'Cuadrado amarillo P#: objeto / decorado (lista de objetos del juego de NPC)' if es else 'Yellow square P#: prop / scenery (prop list of the NPC set)')]
    notes.append(('Marcas del plano' if es else 'Plan marks', marks))
    conf = [(CONF_COL['sabido'], 'SABIDO (marco verde continuo): dibujo de la página que el código prueba y paleta probada en el disco' if es else
             'KNOWN (solid green frame): drawing from the page the code proves and a palette proven on the disc'),
            (CONF_COL['probable'], 'PROBABLE (marco naranja discontinuo): dibujo real del disco, pero la paleta o qué dibujo es sale de comparar con TCRF / la comunidad' if es else
             'PROBABLE (dashed orange frame): real disc drawing, but its palette or which drawing it is comes from matching TCRF / community sheets'),
            (CONF_COL['suposicion'], 'SUPOSICIÓN: deducción nuestra (sólo nombres y papeles; nunca se dibuja un sprite supuesto)' if es else
             'GUESS: our own deduction (names and roles only; a guessed sprite is never drawn)'),
            (CONF_COL['desconocido'], 'DESCONOCIDO (silueta gris con ?): sprite desconocido, su página de texturas se perdió; la silueta no es el personaje' if es else
             'UNKNOWN (grey silhouette with ?): sprite unknown, its texture page is lost; the silhouette is not the character'),
            (None, 'El fotograma (de frente) es elección nuestra; el registro fija la orientación real (campo +4).' if es else
             'The (front) frame is our choice; the record sets the real facing (field +4).')]
    notes.append(('Confianza' if es else 'Confidence', conf))
    hs = []
    for i, r in enumerate(rects):
        who = [str(k + 1) for k, a in enumerate(actors) if a['room'] == i + 1]
        pr = ['P%d' % (k + 1) for k, q in enumerate(props0) if q['room'] == i + 1]
        t = '%s%d  x %d..%d, y %d..%d: ' % (HL, i + 1, r[0], r[2], r[1], r[3])
        if who: t += ('dentro ' if es else 'inside ') + ', '.join('#' + w for w in who)
        else: t += 'nadie dentro' if es else 'nobody inside'
        if pr: t += ('; objeto ' if es else '; prop ') + ', '.join(pr)
        hs.append((None, t))
    if area == 17:
        hs.append((None, 'Nota: #5 tiene casa 1 en su registro, pero su posición (223, 0) cae fuera de C1.' if es else
                   'Note: #5 has house 1 in its record, but its position (223, 0) is outside H1.'))
    if not rects:
        hs.append((None, 'Ninguna: AREA18 no tiene lista de casas en TOPDOWN_TRIGGER_RECT_LISTS.' if es else 'None: AREA18 has no entry in TOPDOWN_TRIGGER_RECT_LISTS.'))
    notes.append(('Casas y quién está dentro' if es else 'Houses and who is inside', hs))
    ex = []
    for k, (r, des, den) in enumerate(exits):
        def rng(lo, hi, ax):
            if lo is None: return '%s < %d' % (ax, hi)
            if hi is None: return '%s > %d' % (ax, lo)
            return '%s %d..%d' % (ax, lo, hi)
        ex.append((None, ('Salida %d (%s, %s) → %s' if es else 'Exit %d (%s, %s) → %s') % (k, rng(r[0], r[2], 'x'), rng(r[1], r[3], 'y'), des if es else den)))
    if area == 18:
        ex.append((None, 'Sin rectángulos: AREA18 s0 no tiene controlador de sección; su tabla de salidas (0x800799D0) apunta a AREA01 s5 y AREA03 s1, marcadores.' if es else
                   'No rectangles: AREA18 s0 has no section controller; its exit table (0x800799D0) points to AREA01 s5 and AREA03 s1, placeholders.'))
    notes.append(('Salidas (adónde llevan)' if es else 'Exits (where they lead)', ex))
    pl = []; groups = {}
    for k, q in enumerate(props0):
        t, c = _prop_note(area, k, q, lang)
        groups.setdefault((t, c), []).append('P%d (%d, %d)' % (k + 1, q['x'], q['y']))
    for (t, c), lst in groups.items():
        pl.append((CONF_COL[c], ', '.join(lst) + ': ' + t))
    if props0: pl.append((None, 'Cada registro de objeto da tipo, casa, anim, x, y y subtipo (página); la lista no dice qué dibujo es.' if es else
               'Each prop record gives type, room, anim, x, y and subtype (page); the list does not say which drawing it is.'))
    if area == 16:
        pl.append((None, 'Con D_8009CFA8 == 4 se usa la lista alternativa: sólo cambia la anim de P6 (10 → 12).' if es else 'With D_8009CFA8 == 4 the alternative list is used: only P6\'s anim changes (10 → 12).'))
    if not props0:
        pl.append((None, 'Ninguno (el juego de NPC 0x10 no tiene objetos).' if es else 'None (NPC set 0x10 has no props).'))
    notes.append(('Objetos (color = confianza de lo que es)' if es else 'Props (colour = confidence of what it is)', pl))
    Q = PERS_SRC[area]
    src = [(None, ('Vecinos: NPC_SET_%s, lista en I0NFO2 %s (registros de 28 bytes: tipo, casa, orientación, caja de patrulla, dlg, comportamiento, x, y, subtipo, b0c); el número es el orden del registro, como en docs/CIVILIZACION.md.' if es else
                   'Residents: NPC_SET_%s, list at I0NFO2 %s (28-byte records: type, room, facing, patrol box, dlg, behaviour, x, y, subtype, b0c); the number is the record order, as in docs/CIVILIZATION.md.') % (Q[0], Q[1])),
           (None, ('Frases: %s → texto japonés (PAL tiene «···??»); objetos: I0NFO2 %s; salidas: %s + tabla MAIN0 0x80079A48.' if es else
                   'Lines: %s → JP text (PAL holds "···??"); props: I0NFO2 %s; exits: %s + MAIN0 table 0x80079A48.') % (Q[3], Q[2], Q[4])),
           (None, 'Coordenadas del mundo (las de la doc); píxel del fondo = (x + 128, y - 40). El dibujo de fondo, la colisión y las tablas de sprites se perdieron.' if es else
            'World coordinates (as in the docs); background pixel = (x + 128, y - 40). The background picture, the collision map and the sprite tables are lost.'),
           (None, 'Generado por tools/civ_sheets.py --sheet personajes con tu propio disco; contiene arte del disco, no se sube al repositorio.' if es else
            'Made by tools/civ_sheets.py --sheet personajes from your own disc; contains disc art, never committed.')]
    if area == 18:
        src[:2] = [(None, 'Vecinos: NPC_SET_10 de I0NFO2 (spawnNpcSet10, mismos registros de 28 bytes), tools/topdown.py npcs; manejador topdownA18ActorHandler 0x800F548C (sin código de diálogo).' if es else
                    'Residents: I0NFO2 NPC_SET_10 (spawnNpcSet10, same 28-byte records), tools/topdown.py npcs; handler topdownA18ActorHandler 0x800F548C (no talk code).')]
    notes.append(('Fuentes' if es else 'Sources', src))
    NW = max(PW + AX, 1000); fN = F(19); fNb = F(23, True)
    def notes_h():
        h = 0
        for t, items in notes:
            h += 38
            for c, s in items: h += 25 * len(wrap(tmp, s, fN, NW - 40)) + 4
            h += 14
        return h
    W = M + max(AX + PW, NW) + 40 + TW + M
    H = oy + PH + TH + notes_h() + 200
    im = Image.new('RGB', (W, H), (250, 247, 240)); dr = ImageDraw.Draw(im, 'RGBA')
    dr.text((M, 22), PERS_TITLE[area][L], font=F(42, True), fill=(20, 20, 20))
    sub = ('Cada vecino en su posición exacta del código (juego de NPC %s de I0NFO2), numerado como en la doc. Sólo se dibuja el sprite si el disco lo da; si no, silueta «sprite desconocido».' % Q[0]) if es else \
          ('Every resident at its exact code position (I0NFO2 NPC set %s), numbered as in the docs. A sprite is drawn only when the disc gives it; otherwise a "sprite unknown" silhouette.' % Q[0])
    dr.text((M, 80), sub, font=F(22), fill=(60, 60, 60))
    dr.text((M, 110), ('Plano (coordenadas del mundo, cuadrícula de 32 px, escala ×1,5)' if es else 'Plan (world coordinates, 32 px grid, scale ×1.5)'), font=F(20, True), fill=(40, 40, 40))
    # ---- plan
    dr.rectangle([ox, oy, ox + PW, oy + PH], fill=(232, 222, 196), outline=(120, 105, 80), width=3)
    for gx in range(wx0, wx1 + 1, 32):
        X = P(gx, 0)[0]; dr.line([X, oy, X, oy + PH], fill=(214, 202, 172) if gx % 64 else (200, 186, 154), width=1)
        if gx % 64 == 0: dr.text((X, oy - 4), str(gx), font=F(14), fill=(90, 80, 60), anchor='mb')
    for gy in range(wy0, wy1 + 1, 32):
        Y = P(0, gy)[1]; dr.line([ox, Y, ox + PW, Y], fill=(214, 202, 172) if gy % 64 else (200, 186, 154), width=1)
        if gy % 64 == 0: dr.text((ox - 6, Y), str(gy), font=F(14), fill=(90, 80, 60), anchor='rm')
    # exits
    for k, (r, des, den) in enumerate(exits):
        x0 = r[0] if r[0] is not None else wx0; x1 = r[2] if r[2] is not None else wx1
        y0 = r[1] if r[1] is not None else wy0; y1 = r[3] if r[3] is not None else wy1
        A_, B_ = P(x0, y0), P(x1, y1)
        if B_[0] - A_[0] < 14: c = (A_[0] + B_[0]) / 2; A_, B_ = (c - 7, A_[1]), (c + 7, B_[1])
        if B_[1] - A_[1] < 14: c = (A_[1] + B_[1]) / 2; A_, B_ = (A_[0], c - 7), (B_[0], c + 7)
        dr.rectangle([A_, B_], fill=(245, 150, 40, 90), outline=(225, 110, 10), width=4)
    # houses
    for i, r in enumerate(rects):
        A_, B_ = P(r[0], r[1]), P(r[2], r[3])
        dr.rectangle([A_, B_], fill=(60, 120, 240, 28), outline=(40, 110, 230), width=4)
    # patrols
    for i, b in enumerate(boxes):
        if not b: continue
        A_, B_ = P(b[0], b[1]), P(b[2], b[3]); G = (30, 150, 50)
        if b[0] == b[2] or b[1] == b[3]:
            dr.line([A_, B_], fill=G, width=5)
            for e in (A_, B_): dr.ellipse([e[0] - 5, e[1] - 5, e[0] + 5, e[1] + 5], fill=G)
        else:
            dr.rectangle([A_, B_], fill=(30, 150, 50, 35), outline=G, width=3)
            if actors[i]['beh'] == 3: _dashed_line(dr, A_, B_, G, 2)
            else: _dashed_line(dr, (A_[0], B_[1]), (B_[0], A_[1]), G, 2)
    # props
    for k, q in enumerate(props0):
        X, Y = P(q['x'], q['y']); dr.rectangle([X - 6, Y - 6, X + 6, Y + 6], fill=(245, 205, 30), outline=(60, 50, 0), width=2)
    # residents (back to front), remember sprite boxes
    sbox = [None] * len(actors)
    for i in sorted(range(len(actors)), key=lambda k: actors[k]['y']):
        a = actors[i]; X, Y = P(a['x'], a['y']); sp, conf, _, _, real = spr[i]
        x0, y0 = int(X - sp.width / 2), int(Y - sp.height)
        im.paste(sp, (x0, y0), sp); sbox[i] = (x0, y0, x0 + sp.width, y0 + sp.height)
        if conf == 'probable': _dashed_rect(dr, (x0 - 3, y0 - 3, x0 + sp.width + 3, y0 + sp.height + 3), CONF_COL['probable'], 3, 6)
        elif conf == 'sabido': dr.rectangle([x0 - 3, y0 - 3, x0 + sp.width + 3, y0 + sp.height + 3], outline=CONF_COL['sabido'], width=2)
        col = (215, 40, 40) if a['room'] == 0 else (150, 40, 170)
        dr.ellipse([X - 5, Y - 5, X + 5, Y + 5], fill=col, outline=(255, 255, 255), width=1)
    # labels: houses, exits, props (fixed), then resident bubbles avoiding them
    fixed = []; R = 17
    def put_label(x, y, text, font, fg, bg, border, anchor_xy=None):
        tw = dr.textlength(text, font=font); hgt = font.size + 8
        bx = (x - 4, y - 3, x + tw + 4, y + hgt - 3)
        dr.rectangle(bx, fill=bg, outline=border, width=2); dr.text((x, y), text, font=font, fill=fg)
        fixed.append(bx); return bx
    for i, r in enumerate(rects):
        A_, B_ = P(r[0], r[1]), P(r[2], r[3])
        put_label(A_[0] + 6, A_[1] + 5, '%s%d' % (HL, i + 1), F(22, True), (255, 255, 255), (40, 110, 230, 255), (20, 60, 150))
    for k, (r, des, den) in enumerate(exits):
        x0 = r[0] if r[0] is not None else wx0; x1 = r[2] if r[2] is not None else wx1
        y0 = r[1] if r[1] is not None else wy0; y1 = r[3] if r[3] is not None else wy1
        A_, B_ = P(x0, y0), P(x1, y1)
        lab = ('Salida %d → %s' if es else 'Exit %d → %s') % (k, (des if es else den).split(' (')[0].replace('hoyo de 7 x 1 px → ', '').replace('7 x 1 px hole → ', ''))
        lab = lab.replace(' → ', ' →\n', 1)
        f = F(19, True); tw = max(dr.textlength(t_, font=f) for t_ in lab.split('\n')); th_ = 2 * 24
        cx_, cy_ = (A_[0] + B_[0]) / 2, (A_[1] + B_[1]) / 2
        if B_[1] - A_[1] < 40: cy_ = A_[1] - 40
        keep = [(b[0], b[1] - 2 * R - 12, b[2], b[3]) for b in sbox if b] + fixed
        def clash(lx, ly):
            bb = (lx - 6, ly - 5, lx + tw + 6, ly + th_ + 4)
            return sum(1 for b in keep if not (b[2] < bb[0] or b[0] > bb[2] or b[3] < bb[1] or b[1] > bb[3]))
        best = None
        for xs_ in ((cx_ - tw / 2, A_[0] + 8, B_[0] - tw - 8), (ox + PW - tw - 10, ox + 8)):
            for dy in range(0, 400, 20):
                for sg in (1, -1):
                    for lx in xs_:
                        lx = min(max(lx, ox + 6), ox + PW - tw - 10); ly = min(max(cy_ - th_ / 2 + sg * dy, oy + 6), oy + PH - th_ - 6)
                        n = clash(lx, ly)
                        if best is None or n < best[0]: best = (n, lx, ly)
                if best[0] == 0: break
            if best[0] == 0: break
        _, lx, ly = best
        bx = (lx - 5, ly - 4, lx + tw + 5, ly + th_ + 2)
        if bx[2] < A_[0] or bx[0] > B_[0] or bx[3] < A_[1] or bx[1] > B_[1]:   # label away from its exit: leader line
            ex_, ey_ = min(max(cx_, A_[0]), B_[0]), min(max((A_[1] + B_[1]) / 2, A_[1]), B_[1])
            lx_, ly_ = min(max(ex_, bx[0]), bx[2]), min(max(ey_, bx[1]), bx[3])
            dr.line([lx_, ly_, ex_, ey_], fill=(225, 110, 10), width=3)
        dr.rectangle(bx, fill=(255, 240, 220, 240), outline=(225, 110, 10), width=2)
        dr.multiline_text((lx, ly), lab, font=f, fill=(140, 60, 0), spacing=4); fixed.append(bx)
    fP = F(14, True)
    for k, q in enumerate(props0):
        X, Y = P(q['x'], q['y']); t = 'P%d' % (k + 1); tw = dr.textlength(t, font=fP)
        best = None
        for (dx, dy) in ((8, -2), (-10 - tw, -2), (-tw / 2, 8), (-tw / 2, -24), (8, -18), (8, 12), (-10 - tw, -18), (-10 - tw, 12), (20, -2), (-22 - tw, -2)):
            bx = (X + dx, Y + dy, X + dx + tw + 2, Y + dy + 17)
            n = sum(1 for b in sbox + fixed if b and not (b[2] < bx[0] or b[0] > bx[2] or b[3] < bx[1] or b[1] > bx[3]))
            if best is None or n < best[0]: best = (n, bx)
            if n == 0: break
        bx = best[1]; dr.text((bx[0] + 1, bx[1]), t, font=fP, fill=(100, 80, 0), stroke_width=2, stroke_fill=(255, 250, 225))
        fixed.append(bx)
    pos = [[(sbox[i][0] + sbox[i][2]) / 2, sbox[i][1] - R - 8] for i in range(len(actors))]
    def hit(c, b, pad=3):
        return b[0] - R - pad < c[0] < b[2] + R + pad and b[1] - R - pad < c[1] < b[3] + R + pad
    for it in range(400):
        moved = False
        for i in range(len(pos)):
            for j in range(i + 1, len(pos)):
                dx, dy = pos[j][0] - pos[i][0], pos[j][1] - pos[i][1]; d = math.hypot(dx, dy)
                if d < 2 * R + 6:
                    if d < 0.01: dx, dy, d = 1.0, 0.0, 1.0
                    s = (2 * R + 6 - d) / 2 + 0.5; pos[i][0] -= dx / d * s; pos[i][1] -= dy / d * s; pos[j][0] += dx / d * s; pos[j][1] += dy / d * s; moved = True
            for b in [x for x in sbox] + fixed:
                if hit(pos[i], b):
                    c = pos[i]; opts = [(b[1] - R - 4 - c[1], 0, 1), (b[3] + R + 4 - c[1], 0, 1), (b[0] - R - 4 - c[0], 1, 0), (b[2] + R + 4 - c[0], 1, 0)]
                    opts[0] = (opts[0][0] * 0.8, 0, 1)
                    m = min(opts, key=lambda o: abs(o[0]))
                    if m[1]: c[0] += m[0] * 0.6 + (0.5 if m[0] > 0 else -0.5)
                    else: c[1] += m[0] * 0.6 + (0.5 if m[0] > 0 else -0.5)
                    moved = True
            pos[i][0] = min(max(pos[i][0], ox + R + 2), ox + PW - R - 2); pos[i][1] = min(max(pos[i][1], oy + R + 2), oy + PH - R - 2)
        if not moved: break
    for i, a in enumerate(actors):
        X, Y = P(a['x'], a['y']); col = (215, 40, 40) if a['room'] == 0 else (150, 40, 170)
        bx, by = pos[i]; tx, ty = (sbox[i][0] + sbox[i][2]) / 2, sbox[i][1]
        b = sbox[i]; cx, cy = min(max(bx, b[0]), b[2]), min(max(by, b[1]), b[3])
        if b[0] < bx < b[2] and by < b[1]: cx, cy = (b[0] + b[2]) / 2, b[1]
        dr.line([bx, by, cx, cy], fill=col, width=2)
        dr.ellipse([bx - R, by - R, bx + R, by + R], fill=col, outline=(255, 255, 255), width=3)
        dr.text((bx, by), str(i + 1), font=F(19, True), fill=(255, 255, 255), anchor='mm')
    # ---- table
    tx0 = M + max(AX + PW, NW) + 40; y = oy
    dr.text((tx0, TOP - 40), ('Quién es quién (%d vecinos)' % len(actors)) if es else ('Who is who (%d residents)' % len(actors)), font=F(26, True), fill=(20, 20, 20))
    x = tx0; dr.rectangle([tx0, y, tx0 + TW, y + 40], fill=(60, 60, 70))
    for name, w in cols: dr.text((x + 7, y + 9), name, font=fH, fill=(255, 255, 255)); x += w
    y += 44
    for i, (a, c) in enumerate(zip(actors, cells)):
        nm, ns, cf, wh, bh, sy, hgt = c; sp, conf, _, _, real = spr[i]; r = rows[i]
        dr.rectangle([tx0, y, tx0 + TW, y + hgt], fill=(255, 255, 255) if i % 2 == 0 else (244, 241, 234))
        dr.line([tx0, y + hgt, tx0 + TW, y + hgt], fill=(210, 205, 195))
        x = tx0; col = (215, 40, 40) if a['room'] == 0 else (150, 40, 170)
        dr.ellipse([x + 10, y + 10, x + 46, y + 46], fill=col, outline=(255, 255, 255), width=2)
        dr.text((x + 28, y + 28), str(i + 1), font=F(19, True), fill=(255, 255, 255), anchor='mm'); x += cols[0][1]
        yy = y + 8
        for ln in nm: dr.text((x + 7, yy), ln, font=fTb, fill=(20, 20, 20)); yy += 24
        nc = r['nc']
        for k, ln in enumerate(ns): dr.text((x + 7, yy), ln, font=fS, fill=CONF_COL[nc] if nc else (110, 110, 110)); yy += 19
        x += cols[1][1]
        th = sp.resize((int(sp.width / S * 2), int(sp.height / S * 2)), Image.NEAREST if real else Image.BILINEAR)
        if th.height > hgt - 16: th = th.resize((int(th.width * (hgt - 16) / th.height), hgt - 16), Image.NEAREST)
        px0, py0 = int(x + cols[2][1] / 2 - th.width / 2), int(y + (hgt - th.height) / 2)
        im.paste(th, (px0, py0), th)
        if conf == 'probable': _dashed_rect(dr, (px0 - 4, py0 - 4, px0 + th.width + 4, py0 + th.height + 4), CONF_COL['probable'], 3, 6)
        elif conf == 'sabido': dr.rectangle([px0 - 4, py0 - 4, px0 + th.width + 4, py0 + th.height + 4], outline=CONF_COL['sabido'], width=3)
        x += cols[2][1]
        lab = CONF_LBL[lang][conf].upper()
        bw = dr.textlength(lab, font=F(16, True))
        dr.rounded_rectangle([x + 7, y + 8, x + 15 + bw, y + 32], 6, fill=CONF_COL[conf]); dr.text((x + 11, y + 10), lab, font=F(16, True), fill=(255, 255, 255))
        yy = y + 38
        for ln in cf: dr.text((x + 7, yy), ln, font=fS, fill=(70, 70, 70)); yy += 19
        x += cols[3][1]
        for lines, w in ((wh, cols[4][1]), (bh, cols[5][1]), (sy, cols[6][1])):
            yy = y + 8
            for ln in lines: dr.text((x + 7, yy), ln, font=fT, fill=(30, 30, 30)); yy += 23
            x += w
        y += hgt
    # column rules
    x = tx0
    for name, w in cols[:-1]:
        x += w; dr.line([x, oy, x, y], fill=(215, 210, 200))
    dr.rectangle([tx0, oy, tx0 + TW, y], outline=(120, 120, 130), width=2)
    # ---- notes: each block goes under the plan or under the table, whichever column ends higher
    colx = [(M, NW - 40), (tx0, TW - 40)]; ny = [oy + PH + 30, y + 40]
    for t, items in notes:
        c = 0 if ny[0] <= ny[1] else 1; x0n, wn = colx[c]; yy = ny[c]
        dr.text((x0n, yy), t, font=fNb, fill=(20, 20, 20)); yy += 38
        for cc, s_ in items:
            lines = wrap(dr, s_, fN, wn - 28)
            if cc: dr.rectangle([x0n, yy + 4, x0n + 18, yy + 22], fill=cc)
            for ln in lines: dr.text((x0n + 28, yy), ln, font=fN, fill=(30, 30, 30)); yy += 25
            yy += 4
        ny[c] = yy + 14
    im = im.crop((0, 0, W, max(y, ny[0], ny[1]) + 30))
    im.save(out); print(out, im.size)
    return [s[1] for s in spr]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--lang", default="all", choices=("es", "en", "all"))
    ap.add_argument("--area", default="all", choices=("11", "16", "17", "18", "all"))
    ap.add_argument("--sheet", default="all", choices=("esquema", "explicado", "perspectiva", "plan1x", "personajes", "all"))
    ap.add_argument("--out", default=os.path.join(os.path.dirname(HERE), "build", "civ"))
    a = ap.parse_args()
    os.makedirs(a.out, exist_ok=True)
    areas = (11, 16, 17, 18) if a.area == "all" else (int(a.area),)
    langs = ("es", "en") if a.lang == "all" else (a.lang,)
    sheets = ("esquema", "explicado", "perspectiva", "plan1x", "personajes") if a.sheet == "all" else (a.sheet,)
    for ar in areas:
        for lg in langs:
            sfx = "" if lg == "es" else "_en"
            if "personajes" in sheets:
                personajes(ar, lg, os.path.join(a.out, "civ_personajes_a%d%s.png" % (ar, sfx)))
            if ar == 18: continue   # only the personajes sheet covers the never-implemented AREA18 s0
            if "esquema" in sheets:
                (esquema_es if lg == "es" else esquema_en)(ar, os.path.join(a.out, "civ_esquema_a%d%s.png" % (ar, sfx)))
            if "explicado" in sheets:
                (explicado_es if lg == "es" else explicado_en)(ar, os.path.join(a.out, "civ_explicado_a%d%s.png" % (ar, sfx)))
            if "perspectiva" in sheets:
                perspectiva(ar, lg, a.out)
        if "plan1x" in sheets and ar != 18:
            plan1x(ar, a.out, langs)


if __name__ == "__main__":
    main()
