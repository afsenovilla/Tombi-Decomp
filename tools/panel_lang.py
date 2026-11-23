#!/usr/bin/env python3
"""The second-screen panel's own words (port2/runtime/panel*.c / .inc, the T("English", "Español") pairs) in the
other languages of the PAL disc: French, German, Italian, Dutch (--lang 1..4), Portuguese, Swedish and Danish
(--lang 6..8).  Writes port2/runtime/panel_lang.inc, which panel.c's T() reads for those languages (English and
Spanish stay at the call sites).  Words shared with the launcher follow tools/port_launcher_lang.py (évènements,
Ereignisse, gebeurtenissen...).

  python3 tools/panel_lang.py            writes the .inc
  python3 tools/panel_lang.py --check    checks that every T() pair of the panel sources has a row here (and every row
                                         a pair), that the placeholders (%d / %s) match, and that the .inc is current

A row: (English, Spanish, fr, de, it, nl, pt, sv, da).  The Spanish column only tells apart two English words the
Spanish chrome writes differently (a filter "Completados", a state "Completado"); it must be the call site's text.
The panel draws latin-1 with the game's glyphs (a letter the font lacks falls back on the bare letter), so no "œ".
"""
import glob, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "port2/runtime/panel_lang.inc")
LANGS = ["fr", "de", "it", "nl", "pt", "sv", "da"]   # --lang 1, 2, 3, 4, 6, 7, 8

ROWS = [
    # tabs and page headers
    ("Status", "Estado", "Statut", "Status", "Stato", "Status", "Estado", "Status", "Status"),
    ("Events", "Eventos", "Évènements", "Ereignisse", "Eventi", "Gebeurtenissen", "Eventos", "Händelser", "Begivenheder"),
    ("Items", "Objetos", "Objets", "Objekte", "Oggetti", "Voorwerpen", "Objetos", "Föremål", "Genstande"),
    ("Map", "Mapa", "Carte", "Karte", "Mappa", "Kaart", "Mapa", "Karta", "Kort"),
    ("Top-down", "Cenital", "Vue de dessus", "Draufsicht", "Dall'alto", "Bovenaanzicht", "Vista de cima", "Uppifrån",
     "Ovenfra"),
    ("No map data", "Sin datos del mapa", "Pas de données de carte", "Keine Kartendaten", "Nessun dato della mappa",
     "Geen kaartgegevens", "Sem dados do mapa", "Inga kartdata", "Ingen kortdata"),
    ("Reading the map...", "Leyendo el mapa...", "Lecture de la carte...", "Karte wird gelesen...",
     "Lettura della mappa...", "Kaart lezen...", "A ler o mapa...", "Läser kartan...", "Læser kortet..."),
    ("Waiting for the game...", "Esperando a la partida...", "En attente de la partie...", "Warte auf das Spiel...",
     "In attesa della partita...", "Wachten op het spel...", "À espera do jogo...", "Väntar på spelet...",
     "Venter på spillet..."),
    ("Event %d", "Evento %d", "Évènement %d", "Ereignis %d", "Evento %d", "Gebeurtenis %d", "Evento %d", "Händelse %d",
     "Begivenhed %d"),
    ("Area %d", "Área %d", "Zone %d", "Gebiet %d", "Area %d", "Gebied %d", "Área %d", "Område %d", "Område %d"),
    # status page
    ("Vitality", "Vitalidad", "Vitalité", "Vitalität", "Vitalità", "Vitaliteit", "Vitalidade", "Vitalitet", "Vitalitet"),
    ("Lives", "Vidas", "Vies", "Leben", "Vite", "Levens", "Vidas", "Liv", "Liv"),
    ("Equipped", "Equipado", "Équipé", "Ausgerüstet", "Equipaggiato", "Uitgerust", "Equipado", "Utrustat", "Udstyret"),
    ("Weapon", "Arma", "Arme", "Waffe", "Arma", "Wapen", "Arma", "Vapen", "Våben"),
    ("Clothes", "Ropa", "Tenue", "Kleidung", "Vestiti", "Kleding", "Roupa", "Kläder", "Tøj"),
    ("Accessories", "Accesorios", "Accessoires", "Zubehör", "Accessori", "Accessoires", "Acessórios", "Tillbehör",
     "Tilbehør"),
    ("None", "Nada", "Rien", "Nichts", "Niente", "Niets", "Nada", "Inget", "Intet"),
    ("No accessories", "Sin accesorios", "Aucun accessoire", "Kein Zubehör", "Nessun accessorio", "Geen accessoires",
     "Sem acessórios", "Inga tillbehör", "Intet tilbehør"),
    ("Progress", "Progreso", "Progression", "Fortschritt", "Progressi", "Voortgang", "Progresso", "Framsteg",
     "Fremskridt"),
    # (the small Status page, one 8 px line: the long nouns take the short "n/N" form to fit the 3DS's box)
    ("%d of %d events", "%d de %d eventos", "%d sur %d évènements", "%d/%d Ereignisse", "%d di %d eventi",
     "%d/%d gebeurtenissen", "%d de %d eventos", "%d av %d händelser", "%d/%d begivenheder"),
    # (the large Status page: "<n>" big, then these two lines; the Dutch noun does not fit the first line)
    ("of %d events", "de %d eventos", "sur %d évènements", "von %d Ereignissen", "di %d eventi", "van %d voltooid",
     "de %d eventos", "av %d händelser", "af %d begivenheder"),
    ("completed", "completados", "terminés", "erledigt", "completati", "gebeurtenissen", "concluídos", "avklarade",
     "fuldførte"),
    ("Regions %d of %d", "Regiones %d de %d", "Régions %d sur %d", "Regionen %d von %d", "Regioni %d di %d",
     "Regio's %d van %d", "Regiões %d de %d", "Regioner %d av %d", "Regioner %d af %d"),
    ("Items %d", "Objetos %d", "Objets %d", "Objekte %d", "Oggetti %d", "Voorwerpen %d", "Objetos %d", "Föremål %d",
     "Genstande %d"),
    ("Regions  %d of %d", "Regiones  %d de %d", "Régions  %d sur %d", "Regionen  %d von %d", "Regioni  %d di %d",
     "Regio's  %d van %d", "Regiões  %d de %d", "Regioner  %d av %d", "Regioner  %d af %d"),
    ("Items  %d", "Objetos  %d", "Objets  %d", "Objekte  %d", "Oggetti  %d", "Voorwerpen  %d", "Objetos  %d",
     "Föremål  %d", "Genstande  %d"),
    ("Time", "Tiempo", "Temps", "Zeit", "Tempo", "Tijd", "Tempo", "Tid", "Tid"),
    ("Tracking", "Siguiendo", "Suivis", "Verfolgt", "Seguiti", "Gevolgd", "A seguir", "Följer", "Følger"),
    ("Next goal", "Objetivo", "Objectif", "Nächstes Ziel", "Obiettivo", "Volgend doel", "Objetivo", "Nästa mål",
     "Næste mål"),
    ("Nothing in progress", "Nada en curso", "Rien en cours", "Nichts läuft", "Niente in corso", "Niets bezig",
     "Nada em curso", "Inget pågår", "Intet i gang"),
    ("+%d in progress", "+%d en curso", "+%d en cours", "+%d laufend", "+%d in corso", "+%d bezig", "+%d em curso",
     "+%d pågår", "+%d i gang"),
    # events page
    ("In progress", "En curso", "En cours", "Laufend", "In corso", "Bezig", "Em curso", "Pågår", "I gang"),
    ("Completed", "Completados", "Terminés", "Erledigt", "Completati", "Voltooid", "Concluídos", "Avklarade",
     "Fuldførte"),
    ("Completed", "Completado", "Terminé", "Erledigt", "Completato", "Voltooid", "Concluído", "Avklarad", "Fuldført"),
    ("Not started", "Pendientes", "À faire", "Offen", "Da fare", "Nog te doen", "Por fazer", "Ej påbörjade",
     "Ikke startet"),
    ("Not started", "Pendiente", "À faire", "Offen", "Da fare", "Nog te doen", "Por fazer", "Ej påbörjad",
     "Ikke startet"),
    ("%d of %d", "%d de %d", "%d sur %d", "%d von %d", "%d di %d", "%d van %d", "%d de %d", "%d av %d", "%d af %d"),
    ("%d started, %d completed", "%d empezados, %d completados", "%d commencés, %d terminés", "%d begonnen, %d erledigt",
     "%d iniziati, %d completati", "%d begonnen, %d voltooid", "%d começados, %d concluídos",
     "%d påbörjade, %d avklarade", "%d startet, %d fuldført"),
    ("No events here", "No hay eventos", "Aucun évènement", "Keine Ereignisse", "Nessun evento", "Geen gebeurtenissen",
     "Sem eventos", "Inga händelser", "Ingen begivenheder"),
    ("4 events tracked already: unpin one first", "Ya sigues 4 eventos: quita uno antes",
     "Déjà 4 évènements suivis : retirez-en un", "Schon 4 Ereignisse verfolgt: erst eines entfernen",
     "Segui già 4 eventi: togline uno prima", "Je volgt al 4 gebeurtenissen: haal er eerst een weg",
     "Já segues 4 eventos: retira um primeiro", "Du följer redan 4 händelser: ta bort en först",
     "Du følger allerede 4 begivenheder: fjern en først"),
    ("Show", "Mostrar", "Afficher", "Zeigen", "Mostra", "Tonen", "Mostrar", "Visa", "Vis"),
    ("Hide", "Ocultar", "Masquer", "Verbergen", "Nascondi", "Verbergen", "Ocultar", "Dölj", "Skjul"),
    ("Hidden", "Oculto", "Caché", "Versteckt", "Nascosto", "Verborgen", "Oculto", "Dold", "Skjult"),
    ("Hidden - ", "Oculto - ", "Caché - ", "Versteckt - ", "Nascosto - ", "Verborgen - ", "Oculto - ", "Dold - ",
     "Skjult - "),
    ("Hidden: shown (%d)", "Ocultos: sí (%d)", "Cachés : oui (%d)", "Versteckte: ja (%d)", "Nascosti: sì (%d)",
     "Verborgen: ja (%d)", "Ocultos: sim (%d)", "Dolda: ja (%d)", "Skjulte: ja (%d)"),
    ("Hidden: off (%d)", "Ocultos: no (%d)", "Cachés : non (%d)", "Versteckte: nein (%d)", "Nascosti: no (%d)",
     "Verborgen: nee (%d)", "Ocultos: não (%d)", "Dolda: nej (%d)", "Skjulte: nej (%d)"),
    ("Track", "Seguir", "Suivre", "Verfolgen", "Segui", "Volgen", "Seguir", "Följ", "Følg"),
    ("Stop tracking", "Dejar de seguir", "Ne plus suivre", "Nicht verfolgen", "Smetti di seguire", "Niet meer volgen",
     "Deixar de seguir", "Sluta följa", "Stop med at følge"),
    ("Back", "Volver", "Retour", "Zurück", "Indietro", "Terug", "Voltar", "Tillbaka", "Tilbage"),
    ("Backspace: back    P: track    H: hide", "Retroceso: volver    P: seguir    H: ocultar",
     "Effacement : retour    P : suivre    H : masquer", "Rücktaste: zurück    P: verfolgen    H: verbergen",
     "Backspace: indietro    P: segui    H: nascondi", "Backspace: terug    P: volgen    H: verbergen",
     "Retrocesso: voltar    P: seguir    H: ocultar", "Backsteg: tillbaka    P: följ    H: dölj",
     "Backspace: tilbage    P: følg    H: skjul"),
    ("Who", "Quién", "Qui", "Wer", "Chi", "Wie", "Quem", "Vem", "Hvem"),
    ("Hint", "Pista", "Indice", "Hinweis", "Indizio", "Hint", "Pista", "Ledtråd", "Ledetråd"),
    ("Where", "Dónde", "Où", "Wo", "Dove", "Waar", "Onde", "Var", "Hvor"),
    ("Here", "Aquí", "Ici", "Hier", "Qui", "Hier", "Aqui", "Här", "Her"),
    ("This area", "Esta zona", "Cette zone", "Dieses Gebiet", "Quest'area", "Dit gebied", "Esta zona", "Detta område",
     "Dette område"),
    ("Elsewhere", "Otra zona", "Ailleurs", "Woanders", "Altrove", "Elders", "Outra zona", "Annat område", "Andet sted"),
    ("Anywhere", "Cualquiera", "Partout", "Überall", "Ovunque", "Overal", "Qualquer", "Var som helst", "Hvor som helst"),
    ("Use %s (anywhere)", "Usar %s (en cualquier lugar)", "Utiliser %s (n'importe où)", "%s benutzen (überall)",
     "Usa %s (ovunque)", "%s gebruiken (overal)", "Usar %s (em qualquer lugar)", "Använd %s (var som helst)",
     "Brug %s (hvor som helst)"),
    ("an item", "un objeto", "un objet", "ein Objekt", "un oggetto", "een voorwerp", "um objeto", "ett föremål",
     "en genstand"),
    ("zone not extracted yet", "zona aún sin extraer", "zone pas encore extraite", "Zone noch nicht ausgelesen",
     "zona non ancora estratta", "zone nog niet uitgelezen", "zona ainda não extraída", "zon ännu inte utläst",
     "zone endnu ikke udlæst"),
    ("Zone not extracted yet", "Zona aún sin extraer", "Zone pas encore extraite", "Zone noch nicht ausgelesen",
     "Zona non ancora estratta", "Zone nog niet uitgelezen", "Zona ainda não extraída", "Zon ännu inte utläst",
     "Zone endnu ikke udlæst"),
    ("Not known yet where it happens", "Aún no se sabe dónde ocurre", "On ne sait pas encore où",
     "Noch unbekannt, wo es passiert", "Non si sa ancora dove accade", "Nog onbekend waar het gebeurt",
     "Ainda não se sabe onde acontece", "Ännu okänt var det händer", "Endnu ukendt hvor det sker"),
    ("Show on the map", "Ver en el mapa", "Voir sur la carte", "Auf der Karte zeigen", "Mostra sulla mappa",
     "Op de kaart tonen", "Ver no mapa", "Visa på kartan", "Vis på kortet"),
    ("Someone here", "Alguien de aquí", "Quelqu'un d'ici", "Jemand von hier", "Qualcuno di qui", "Iemand van hier",
     "Alguém daqui", "Någon härifrån", "Nogen herfra"),
    ("Someone in this place", "Alguien de este lugar", "Quelqu'un de ce lieu", "Jemand an diesem Ort",
     "Qualcuno di questo posto", "Iemand op deze plek", "Alguém deste lugar", "Någon på den här platsen",
     "Nogen på dette sted"),
    ("Something in this place", "Algo de este lugar", "Quelque chose de ce lieu", "Etwas an diesem Ort",
     "Qualcosa di questo posto", "Iets op deze plek", "Algo deste lugar", "Något på den här platsen",
     "Noget på dette sted"),
    ("A chest", "Un cofre", "Un coffre", "Eine Truhe", "Uno scrigno", "Een kist", "Um baú", "En kista", "En kiste"),
    ("An item", "Un objeto", "Un objet", "Ein Objekt", "Un oggetto", "Een voorwerp", "Um objeto", "Ett föremål",
     "En genstand"),
    ("A scene", "Una escena", "Une scène", "Eine Szene", "Una scena", "Een scène", "Uma cena", "En scen", "En scene"),
    ("An object", "Un objeto del lugar", "Un objet du lieu", "Ein Ding vor Ort", "Un oggetto del posto",
     "Een ding op deze plek", "Um objeto do lugar", "En sak på platsen", "En ting på stedet"),
    # items page
    ("Keys", "Llaves", "Clés", "Schlüssel", "Chiavi", "Sleutels", "Chaves", "Nycklar", "Nøgler"),
    ("Weapons", "Armas", "Armes", "Waffen", "Armi", "Wapens", "Armas", "Vapen", "Våben"),
    ("Clothes and gear", "Ropa y equipo", "Vêtements et équipement", "Kleidung und Ausrüstung",
     "Vestiti e equipaggiamento", "Kleding en uitrusting", "Roupa e equipamento", "Kläder och utrustning",
     "Tøj og udstyr"),
    ("Key items", "Objetos importantes", "Objets importants", "Wichtige Objekte", "Oggetti importanti",
     "Belangrijke voorwerpen", "Objetos importantes", "Viktiga föremål", "Vigtige genstande"),
    ("Consumables", "Consumibles", "Consommables", "Verbrauchsgüter", "Consumabili", "Verbruiksvoorwerpen",
     "Consumíveis", "Förbrukningsvaror", "Forbrugsvarer"),
    ("Other", "Otros", "Autres", "Sonstiges", "Altro", "Overig", "Outros", "Övrigt", "Andet"),
    ("Item %d", "Objeto %d", "Objet %d", "Objekt %d", "Oggetto %d", "Voorwerp %d", "Objeto %d", "Föremål %d",
     "Genstand %d"),
    ("No items", "Sin objetos", "Aucun objet", "Keine Objekte", "Nessun oggetto", "Geen voorwerpen", "Sem objetos",
     "Inga föremål", "Ingen genstande"),
    ("No longer in the inventory", "Ya no está en el inventario", "N'est plus dans l'inventaire",
     "Nicht mehr im Inventar", "Non è più nell'inventario", "Niet meer in de inventaris", "Já não está no inventário",
     "Finns inte längre i förrådet", "Ikke længere i inventaret"),
    ("Use", "Usar", "Utiliser", "Benutzen", "Usa", "Gebruiken", "Usar", "Använd", "Brug"),
    # (Sort and the weapon's Equip share one row on the 3DS: short words where the usual ones do not fit it)
    ("Equip", "Equipar", "Équiper", "Anlegen", "Impugna", "Pakken", "Equipar", "Utrusta", "Udstyr"),
    ("Wear", "Poner", "Mettre", "Anziehen", "Indossa", "Aantrekken", "Vestir", "Ta på", "Tag på"),
    ("Take off", "Quitar", "Retirer", "Ablegen", "Togli", "Uittrekken", "Tirar", "Ta av", "Tag af"),
    ("Fly", "Volar", "Voler", "Fliegen", "Vola", "Vliegen", "Voar", "Flyg", "Flyv"),
    ("Cancel", "Cancelar", "Annuler", "Abbrechen", "Annulla", "Annuleren", "Cancelar", "Avbryt", "Annuller"),
    ("Sort", "Ordenar", "Trier", "Ordnen", "Ordina", "Ordenen", "Ordenar", "Sortera", "Sortér"),
    ("Where to?", "¿Adónde?", "Où aller ?", "Wohin?", "Dove?", "Waarheen?", "Para onde?", "Vart?", "Hvorhen?"),
    ("No place visited yet", "Ningún lugar visitado", "Aucun lieu visité", "Noch kein Ort besucht",
     "Nessun luogo visitato", "Nog geen plek bezocht", "Nenhum lugar visitado", "Ingen plats besökt än",
     "Intet sted besøgt endnu"),
    ("Flying to %s", "Volando a %s", "Vol vers %s", "Flug nach %s", "In volo verso %s", "Vliegen naar %s",
     "A voar para %s", "Flyger till %s", "Flyver til %s"),
    ("Completed: %s", "Completado: %s", "Terminé : %s", "Erledigt: %s", "Completato: %s", "Voltooid: %s",
     "Concluído: %s", "Avklarat: %s", "Fuldført: %s"),
    ("The item sub-screen data is not on this disc", "Faltan los datos del menú de objetos",
     "Les données du menu des objets manquent", "Die Daten des Objektmenüs fehlen", "Mancano i dati del menu oggetti",
     "De gegevens van het voorwerpenmenu ontbreken", "Faltam os dados do menu de objetos",
     "Data för föremålsmenyn saknas", "Data til genstandsmenuen mangler"),
    # why an item cannot be used now
    ("Not in a game", "Fuera de la partida", "Pas en partie", "Kein Spiel läuft", "Fuori dalla partita",
     "Niet in een spel", "Fora do jogo", "Inte i ett spel", "Ikke i et spil"),
    ("Not now (menu, loading or scene)", "Ahora no (menú, carga o escena)", "Pas maintenant (menu, chargement ou scène)",
     "Jetzt nicht (Menü, Laden oder Szene)", "Non ora (menu, caricamento o scena)", "Nu niet (menu, laden of scène)",
     "Agora não (menu, carregamento ou cena)", "Inte nu (meny, laddning eller scen)",
     "Ikke nu (menu, indlæsning eller scene)"),
    ("The game is paused", "El juego está en pausa", "Le jeu est en pause", "Das Spiel ist pausiert",
     "Il gioco è in pausa", "Het spel is gepauzeerd", "O jogo está em pausa", "Spelet är pausat",
     "Spillet er sat på pause"),
    ("The screen is fading", "La pantalla está en transición", "L'écran est en transition", "Der Bildschirm blendet über",
     "Lo schermo è in dissolvenza", "Het scherm vervaagt", "O ecrã está em transição", "Skärmen tonar över",
     "Skærmen toner over"),
    ("Not available in this scene", "No disponible en esta escena", "Indisponible dans cette scène",
     "In dieser Szene nicht verfügbar", "Non disponibile in questa scena", "Niet beschikbaar in deze scène",
     "Indisponível nesta cena", "Inte tillgängligt i den här scenen", "Ikke tilgængelig i denne scene"),
    ("Not available right now", "No disponible ahora", "Indisponible pour l'instant", "Gerade nicht verfügbar",
     "Non disponibile ora", "Nu niet beschikbaar", "Indisponível agora", "Inte tillgängligt just nu",
     "Ikke tilgængelig lige nu"),
    ("Tombi must be on the ground", "Tombi tiene que estar en el suelo", "Tombi doit être au sol",
     "Tombi muss am Boden sein", "Tombi deve essere a terra", "Tombi moet op de grond staan",
     "O Tombi tem de estar no chão", "Tombi måste stå på marken", "Tombi skal stå på jorden"),
    ("Tombi must stand still", "Tombi tiene que estar quieto", "Tombi doit être immobile", "Tombi muss stillstehen",
     "Tombi deve stare fermo", "Tombi moet stilstaan", "O Tombi tem de estar parado", "Tombi måste stå still",
     "Tombi skal stå stille"),
    ("No need using this.", "No hace falta usarlo.", "Inutile de l'utiliser.", "Nicht nötig.", "Non serve usarlo.",
     "Niet nodig.", "Não é preciso usar.", "Behövs inte.", "Ikke nødvendigt."),
    ("Already equipped.", "Ya equipado.", "Déjà équipé.", "Schon ausgerüstet.", "Già equipaggiato.", "Al uitgerust.",
     "Já equipado.", "Redan utrustat.", "Allerede udstyret."),
    ("Can't use this here.", "No se puede usar aquí.", "Impossible ici.", "Hier nicht nutzbar.",
     "Non si può usare qui.", "Kan hier niet.", "Não se pode usar aqui.", "Går inte här.", "Kan ikke bruges her."),
    ("Not enough.", "No es suficiente.", "Pas assez.", "Nicht genug.", "Non basta.", "Niet genoeg.", "Não chega.",
     "Inte tillräckligt.", "Ikke nok."),
]


def c_literal(s):
    """A C string literal of s in latin-1 (non-ASCII as \\xNN, split where a hex digit follows an escape)."""
    out, esc = ['"'], False
    for ch in s:
        o = ord(ch)
        if o > 255:
            raise ValueError(f"not latin-1: {ch!r} in {s!r}")
        if o >= 128:
            out.append("\\x%02X" % o); esc = True; continue
        if esc and ch in "0123456789abcdefABCDEF":
            out.append('" "')
        esc = False
        out.append("\\" + ch if ch in '"\\' else ch)
    return "".join(out) + '"'


def c_unescape(lit):
    """The text of a C string literal sequence as the panel sources write it."""
    out = []
    for body in re.findall(r'"((?:[^"\\]|\\.)*)"', lit):
        i = 0
        while i < len(body):
            if body[i] == "\\":
                if body[i + 1] == "x":
                    m = re.match(r"[0-9a-fA-F]+", body[i + 2:]); out.append(chr(int(m.group(0), 16)))
                    i += 2 + len(m.group(0)); continue
                out.append(body[i + 1]); i += 2; continue
            out.append(body[i]); i += 1
    return "".join(out)


def source_pairs():
    """The T(en, sp) pairs of the panel sources whose both sides are literals (PANEL_HERO read as Tombi)."""
    pairs = set()
    for f in sorted(glob.glob(os.path.join(ROOT, "port2/runtime/panel*.c")) +
                    glob.glob(os.path.join(ROOT, "port2/runtime/panel*.inc"))):
        if f == OUT:
            continue
        s = open(f, encoding="latin-1").read()
        for m in re.finditer(r'(?<![\w#])T\(', s):
            if s[max(0, m.start() - 8):m.start()].endswith("define "):
                continue
            i, d, j = m.end(), 1, m.end()
            while d and j < len(s):
                c = s[j]
                if c == '"':
                    j += 1
                    while s[j] != '"':
                        j += 2 if s[j] == "\\" else 1
                elif c in "()":
                    d += 1 if c == "(" else -1
                j += 1
            args = s[i:j - 1].replace("PANEL_HERO", '"Tombi"')
            parts = re.match(r'\s*((?:"(?:[^"\\]|\\.)*"\s*)+),\s*((?:"(?:[^"\\]|\\.)*"\s*)+)$', args, re.S)
            if parts:
                pairs.add((c_unescape(parts.group(1)), c_unescape(parts.group(2))))
    return pairs


def render():
    lines = ["/* generated by tools/panel_lang.py: the panel's T() words in French, German, Italian, Dutch, Portuguese,",
             "   Swedish and Danish (English, Spanish, then --lang 1, 2, 3, 4, 6, 7, 8).  Do not edit: edit the tool. */",
             "static const char *const panel_lang_rows[][9] = {"]
    for r in ROWS:
        lines.append("    { " + ", ".join(c_literal(x) for x in r) + " },")
    lines.append("};")
    return "\n".join(lines) + "\n"


def check():
    bad = 0
    keys = [(r[0], r[1]) for r in ROWS]
    for k in set(keys):
        if keys.count(k) > 1:
            print("duplicate row:", k); bad += 1
    for r in ROWS:
        if len(r) != 9:
            print("row without 9 columns:", r[0]); bad += 1; continue
        ph = re.findall(r"%[ds]", r[0])
        for lang, t in zip(LANGS, r[2:]):
            if re.findall(r"%[ds]", t) != ph:
                print(f"placeholders differ ({lang}): {r[0]!r} -> {t!r}"); bad += 1
            c_literal(t)
    src = source_pairs()
    for p in sorted(src - set(keys)):
        print("no row for", p); bad += 1
    for p in sorted(set(keys) - src):
        if p[0] not in ("No need using this.", "Already equipped.", "Can't use this here.", "Not enough."):
            print("row not used by the sources:", p); bad += 1
    if not os.path.exists(OUT) or open(OUT, encoding="latin-1").read() != render():
        print("port2/runtime/panel_lang.inc is not current: python3 tools/panel_lang.py"); bad += 1
    print("ok" if not bad else f"{bad} problem(s)")
    return 1 if bad else 0


if __name__ == "__main__":
    if "--check" in sys.argv:
        sys.exit(check())
    open(OUT, "w", encoding="latin-1", newline="\n").write(render())
    print("wrote", os.path.relpath(OUT, ROOT), len(ROWS), "rows")
