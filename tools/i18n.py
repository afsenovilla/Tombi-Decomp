#!/usr/bin/env python3
"""Interface language of the Windows tools (tombi-text, tombi-gfx, tombi-assets): string catalogs, tr(), system language detection and a
persisted choice.  Separate from the game languages of the text editor (those are the dialogue files of the disc).

  import i18n
  i18n.add({"es": {"key": "texto {0}"}, "en": {"key": "text {0}"}})    # an app registers its strings
  i18n.tr("key", 5)                                                   # -> "text 5" (current language; falls back to en, es, then the key)
  i18n.selector(parent, on_change)                                    # tk combobox "Idioma / Language" -> sets, saves and calls on_change()

Languages: es, en, fr, de, it, pt.  Default = saved choice, else the system UI language, else English.  The choice is stored in
%APPDATA%/TombiTools/settings.json (Windows) or ~/.config/TombiTools/settings.json."""
import json, locale, os, sys

LANGS = {"es": "Español", "en": "English", "fr": "Français", "de": "Deutsch", "it": "Italiano", "pt": "Português"}
CAT = {k: {} for k in LANGS}
_cur = "en"


def add(cats):
    for lang, d in cats.items():
        CAT.setdefault(lang, {}).update(d)


def settings_path():
    if os.name == "nt":
        base = os.environ.get("APPDATA") or os.path.expanduser("~")
    else:
        base = os.environ.get("XDG_CONFIG_HOME") or os.path.join(os.path.expanduser("~"), ".config")
    return os.path.join(base, "TombiTools", "settings.json")


def load_settings():
    try:
        return json.load(open(settings_path(), encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def save_settings(**kw):
    s = load_settings()
    s.update(kw)
    try:
        p = settings_path()
        os.makedirs(os.path.dirname(p), exist_ok=True)
        json.dump(s, open(p, "w", encoding="utf-8"), indent=1)
    except OSError:
        pass


def system_language():
    code = ""
    try:
        if os.name == "nt":
            import ctypes
            lid = ctypes.windll.kernel32.GetUserDefaultUILanguage()
            code = locale.windows_locale.get(lid, "")
        else:
            for v in ("LC_ALL", "LC_MESSAGES", "LANG", "LANGUAGE"):
                if os.environ.get(v):
                    code = os.environ[v]
                    break
    except Exception:
        pass
    if not code:
        try:
            import warnings
            with warnings.catch_warnings():
                warnings.simplefilter("ignore")
                code = locale.getdefaultlocale()[0] or ""
        except Exception:
            code = ""
    c = code.lower()[:2]
    return c if c in LANGS else "en"


def current():
    return _cur


def set_language(lang, save=True):
    global _cur
    if lang in LANGS:
        _cur = lang
        if save:
            save_settings(language=lang)


def init():
    """Language from the settings file, else the system's."""
    s = load_settings().get("language")
    set_language(s if s in LANGS else system_language(), save=False)
    return _cur


def tr(key, *args):
    s = CAT.get(_cur, {}).get(key) or CAT["en"].get(key) or CAT["es"].get(key) or key
    if args:
        try:
            return s.format(*args)
        except (IndexError, KeyError, ValueError):
            return s
    return s


def selector(parent, on_change):
    """Combobox 'Idioma / Language'; choosing a language saves it and calls on_change() so the window can relabel/rebuild itself."""
    import tkinter as tk
    from tkinter import ttk
    f = ttk.Frame(parent)
    ttk.Label(f, text="Idioma / Language:").pack(side="left", padx=(0, 4))
    names = ["%s (%s)" % (v, k) for k, v in LANGS.items()]
    var = tk.StringVar(value="%s (%s)" % (LANGS[_cur], _cur))
    cb = ttk.Combobox(f, textvariable=var, values=names, state="readonly", width=16)
    cb.pack(side="left")

    def chosen(_=None):
        code = var.get().rsplit("(", 1)[1].rstrip(")")
        if code != _cur:
            set_language(code)
            on_change()
    cb.bind("<<ComboboxSelected>>", chosen)
    return f


add({
    "es": {"lang": "Idioma / Language", "ok": "Aceptar", "cancel": "Cancelar", "error": "Error", "warning": "Aviso", "info": "Información",
           "close": "Cerrar", "yes": "Sí", "no": "No"},
    "en": {"lang": "Idioma / Language", "ok": "OK", "cancel": "Cancel", "error": "Error", "warning": "Warning", "info": "Information",
           "close": "Close", "yes": "Yes", "no": "No"},
    "fr": {"ok": "OK", "cancel": "Annuler", "error": "Erreur", "warning": "Avertissement", "info": "Information", "close": "Fermer", "yes": "Oui", "no": "Non"},
    "de": {"ok": "OK", "cancel": "Abbrechen", "error": "Fehler", "warning": "Warnung", "info": "Information", "close": "Schließen", "yes": "Ja", "no": "Nein"},
    "it": {"ok": "OK", "cancel": "Annulla", "error": "Errore", "warning": "Avviso", "info": "Informazioni", "close": "Chiudi", "yes": "Sì", "no": "No"},
    "pt": {"ok": "OK", "cancel": "Cancelar", "error": "Erro", "warning": "Aviso", "info": "Informação", "close": "Fechar", "yes": "Sim", "no": "Não"},
})
init()
