#!/usr/bin/env python3
"""Changelog of a PC release: the player-facing commits between two refs (local git history).

  python3 tools/release_notes.py PREV_TAG SHA      prints "- subject" lines, oldest first

A commit is listed when it touches something that ships in the PC build: the game / port sources (src/, port/,
port2/, the Android app included: the release ships the APK too), the text patches (patches/), or the PC launcher and
its helpers. Commits that only touch docs, notes, CI or research tools are left out. A commit and the change that undoes it both drop
out: git's "This reverts commit X" line, a subject starting with "Revert", or a subject saying "revert ..." /
"drop the ..." that names the earlier commit's subject. A body line "[skip changelog]" hides a commit.
Also left out: work on the other versions' decomps (subjects starting "JP", "JP demo", "PrePre", "beta", "US": they edit
shared src/ files under #if, nothing the PAL port runs; "README" and "docs" ones too), temporary commits ("TEMP" or "trial" in the subject; a "; TEMP ..." tail is
cut off), and a commit that hides / removes something a later commit of the same release brings back ("... back",
"... again", "restore ..." sharing two words of 5+ letters with it).
"""
import re, subprocess, sys

SHIPS = re.compile(r"^(src/|port/|patches/|port2/|tools/(port_launcher|save_import|prompt_custom|i18n|port_mods|game_library)\.py)")


def git(*a):
    return subprocess.run(["git", *a], capture_output=True, text=True, check=True).stdout


def main():
    prev, sha = sys.argv[1], sys.argv[2]
    log = git("log", "--reverse", "--format=%H%x1f%s%x1f%b%x1e", f"{prev}..{sha}")
    commits = []
    for rec in log.split("\x1e"):
        rec = rec.strip("\n")
        if not rec:
            continue
        h, subj, body = (rec.split("\x1f") + ["", ""])[:3]
        files = [f for f in git("show", "--name-only", "--format=", h).splitlines() if f]
        commits.append((h, subj.strip(), body))
        commits[-1] += (files,)
    drop = set()
    by_subj = {s: h for h, s, _, _ in commits}
    for h, s, b, _ in commits:
        if "[skip changelog]" in b:
            drop.add(h)
        m = re.search(r"This reverts commit ([0-9a-f]{7,40})", b)
        q = re.match(r'Revert "(.*)"', s)
        if m or q or re.search(r"\brevert(s|ed)?\b", s, re.I):
            drop.add(h)
            if m:
                drop.update(c for c, *_ in commits if c.startswith(m.group(1)))
            if q:
                drop.update(c for c, cs, *_ in commits if cs == q.group(1) or q.group(1).startswith(cs))
            # "port2 widescreen: revert the mirrored room-end fill": drop the earlier commit with the same scope
            # whose subject shares the reverted words
            words = set(re.findall(r"[a-z]{5,}", s.lower())) - {"revert", "reverts", "reverted", "keep", "change"}
            for c, cs, *_ in commits:
                if c != h and len(words & set(re.findall(r"[a-z]{5,}", cs.lower()))) >= 3 and cs.split(":")[0] == s.split(":")[0]:
                    drop.add(c)
    OTHER = re.compile(r"^(JP|JP demo|PrePre ?8?|beta|US|SLPS|SLPM|PCPX|README|docs)\b[^:]*:", re.I)
    for h, s, b, _ in commits:
        if OTHER.match(s) or re.match(r"^TEMP\b", s) or re.search(r"\btrial\b", s, re.I):
            drop.add(h)
    for i, (h, s, b, _) in enumerate(commits):          # hidden now, back later in the same release: neither happened
        if re.search(r"\b(hide|hides|hidden|remove|removes|removed|drop)\b", s, re.I):
            w = set(re.findall(r"[a-z]{5,}", s.lower()))
            for c, cs, *_ in commits[i + 1:]:
                if re.search(r"\b(back|again|restore[sd]?)\b", cs, re.I) and len(w & set(re.findall(r"[a-z]{5,}", cs.lower()))) >= 2:
                    drop.add(h)
    tidy = lambda t: re.split(r";\s*TEMP\b", t)[0].strip()
    out = [tidy(s) for h, s, b, f in commits if h not in drop and any(SHIPS.match(x) for x in f)]
    print("\n".join("- " + s for s in out[-150:]) if out else "- (no player-facing changes)")


if __name__ == "__main__":
    main()
