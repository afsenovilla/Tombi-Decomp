"""User labels (names + notes) for anything the tools show: animations, backdrops, pages, sprites, screens, rooms...

Ids are stable strings: "anim:tombi:13", "backdrop:L22", "page:SYSTEM/A00900.000", "screen:now_loading", "ui:1", "clut:AREA00/CLUT01.GAM",
"wpp:AREA00/A004.000:frame:57", "room:AREA03:4", "list:AR05:3", "spr:<file>:<table>:<frame>".
Two JSON files {id: {"name": str, "note": str}} are merged: the repository defaults (tools/labels/labels.json, shipped with the programs)
and the user's file (%APPDATA%/TombiTools/labels.json, ~/.config/TombiTools/labels.json elsewhere); the user's labels win.
Export writes only the user's labels; import merges a file into the user's labels."""
import json, os, re, sys

_DEFAULT, _USER = None, None


def default_path():
    for base in (os.path.join(getattr(sys, "_MEIPASS", ""), "labels"), os.path.join(os.path.dirname(os.path.abspath(__file__)), "labels")):
        p = os.path.join(base, "labels.json")
        if os.path.isfile(p):
            return p
    return os.path.join(os.path.dirname(os.path.abspath(__file__)), "labels", "labels.json")


def user_path():
    base = os.environ.get("APPDATA") if os.name == "nt" else os.path.join(os.path.expanduser("~"), ".config")
    return os.path.join(base or os.path.expanduser("~"), "TombiTools", "labels.json")


def _read(p):
    try:
        with open(p, encoding="utf-8") as f:
            d = json.load(f)
        return {k: _norm(v) for k, v in d.items()} if isinstance(d, dict) else {}
    except (OSError, ValueError):
        return {}


def _norm(v):
    if isinstance(v, str):
        return {"name": v, "note": ""}
    return {"name": str(v.get("name", "")), "note": str(v.get("note", ""))}


def _load():
    global _DEFAULT, _USER
    if _DEFAULT is None:
        _DEFAULT = _read(default_path())
    if _USER is None:
        _USER = _read(user_path())


def reload():
    global _DEFAULT, _USER
    _DEFAULT = _USER = None


def get(id):
    """{"name", "note"} of the label of `id` (user over default), or None."""
    _load()
    v = _USER.get(id) or _DEFAULT.get(id)
    return v if v and (v["name"] or v["note"]) else None


def name(id, default=""):
    v = get(id)
    return v["name"] if v and v["name"] else default


def note(id):
    v = get(id)
    return v["note"] if v else ""


def show(id, default):
    """Display text: the label followed by the original name, or the original."""
    n = name(id)
    return "%s  (%s)" % (n, default) if n else default


def set(id, nm, nt=""):
    """Sets (or with empty name and note removes) the user's label and saves the user file."""
    _load()
    if nm.strip() or nt.strip():
        _USER[id] = {"name": nm.strip(), "note": nt.strip()}
    else:
        _USER.pop(id, None)
    save()


def save():
    p = user_path()
    os.makedirs(os.path.dirname(p), exist_ok=True)
    with open(p, "w", encoding="utf-8") as f:
        json.dump(dict(sorted(_USER.items())), f, indent=1, ensure_ascii=False)


def export_user(path):
    _load()
    with open(path, "w", encoding="utf-8") as f:
        json.dump(dict(sorted(_USER.items())), f, indent=1, ensure_ascii=False)
    return len(_USER)


def import_file(path):
    """Merges the labels of a JSON file into the user's labels (saved). Returns how many were read."""
    _load()
    d = _read(path)
    _USER.update(d)
    save()
    return len(d)


def slug(s):
    """File name friendly form of a label ('Bosque encantado' -> 'bosque_encantado')."""
    s = re.sub(r"[^\w]+", "_", s.strip().lower(), flags=re.UNICODE).strip("_")
    return s[:40]


def fname(id, default):
    """File name stem: the default (which carries the unique id) plus the label, e.g. 'L22' -> 'L22_bosque'."""
    n = slug(name(id))
    return "%s_%s" % (default, n) if n else default
