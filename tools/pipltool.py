#!/usr/bin/env python3
import re
import sys


def c_escape_bytes(data: bytes) -> str:
    out = []
    for b in data:
        if b == ord("\\"):
            out.append("\\\\")
        elif b == ord('"'):
            out.append('\\"')
        elif 32 <= b <= 126:
            out.append(chr(b))
        else:
            out.append(f"\\{b:03o}")
    return "".join(out)


def pack_pascal_str(s: str) -> bytes:
    raw = s.encode("ascii", errors="ignore")[:255]
    buf = bytes([len(raw)]) + raw
    pad = (4 - (len(buf) % 4)) % 4
    return buf + (b"\x00" * pad)


def pack_c_str(s: str) -> bytes:
    raw = s.encode("ascii", errors="ignore") + b"\x00"
    pad = (4 - (len(raw) % 4)) % 4
    return raw + (b"\x00" * pad)


def parse_int(val_str: str, macros: dict) -> int:
    val_str = val_str.strip()
    if val_str in macros:
        val_str = macros[val_str]
    return int(val_str, 0)


def generate_pipl_rc(r_text: str) -> str:
    macros = {
        "PF_PLUG_IN_VERSION": "13",
        "PF_PLUG_IN_SUBVERS": "28",
    }
    for m in re.finditer(r"#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)\s+([^\r\n/]+)", r_text):
        macros[m.group(1)] = m.group(2).strip()

    # Strip comments while preserving quoted string literals
    clean_chars = []
    i = 0
    n = len(r_text)
    in_str = False
    while i < n:
        if in_str:
            if r_text[i] == "\\" and i + 1 < n:
                clean_chars.append(r_text[i : i + 2])
                i += 2
                continue
            if r_text[i] == '"':
                in_str = False
            clean_chars.append(r_text[i])
            i += 1
        else:
            if r_text[i] == '"':
                in_str = True
                clean_chars.append(r_text[i])
                i += 1
            elif r_text[i : i + 2] == "/*":
                end_c = r_text.find("*/", i + 2)
                i = (end_c + 2) if end_c != -1 else n
            elif r_text[i : i + 2] == "//":
                end_l = r_text.find("\n", i + 2)
                i = end_l if end_l != -1 else n
            else:
                clean_chars.append(r_text[i])
                i += 1
    clean = "".join(clean_chars)

    res_id = 16000
    m_res = re.search(r"resource\s+'PiPL'\s*\(\s*(\d+)", clean)
    if m_res:
        res_id = int(m_res.group(1))

    props = []

    m = re.search(r"Kind\s*\{\s*([A-Za-z0-9_]+)\s*\}", clean)
    if m:
        kind_sym = m.group(1)
        kind_4cc = "eFKT" if kind_sym == "AEEffect" else "SPEA" if kind_sym == "AEGP" else "eFKT"
        props.append(("dnik", 4, f'"{kind_4cc[::-1]}"'))

    m = re.search(r'Name\s*\{\s*"([^"]*)"\s*\}', clean)
    if m:
        p = pack_pascal_str(m.group(1))
        props.append(("eman", len(p), f'"{c_escape_bytes(p)}"'))

    m = re.search(r'Category\s*\{\s*"([^"]*)"\s*\}', clean)
    if m:
        p = pack_pascal_str(m.group(1))
        props.append(("gtac", len(p), f'"{c_escape_bytes(p)}"'))

    m = re.search(r'CodeWin64X86\s*\{\s*"([^"]*)"\s*\}', clean)
    if m:
        p = pack_c_str(m.group(1))
        props.append(("4668", len(p), f'"{c_escape_bytes(p)}"'))

    m = re.search(r"AE_PiPL_Version\s*\{\s*([^,{}]+)\s*,\s*([^,{}]+)\s*\}", clean)
    if m:
        v1 = parse_int(m.group(1), macros)
        v2 = parse_int(m.group(2), macros)
        props.append(("RVPe", 4, f"{v1}, {v2}"))

    m = re.search(r"AE_Effect_Spec_Version\s*\{\s*([^,{}]+)\s*,\s*([^,{}]+)\s*\}", clean)
    if m:
        v1 = parse_int(m.group(1), macros)
        v2 = parse_int(m.group(2), macros)
        props.append(("RVSe", 4, f"{v1}, {v2}"))

    m = re.search(r"AE_Effect_Version\s*\{\s*([^{}]+)\s*\}", clean)
    if m:
        v = parse_int(m.group(1), macros)
        props.append(("REVe", 4, f"{v}L"))

    m = re.search(r"AE_Effect_Info_Flags\s*\{\s*([^{}]+)\s*\}", clean)
    if m:
        v = parse_int(m.group(1), macros)
        props.append(("FNIe", 4, f"{v}L"))

    m = re.search(r"AE_Effect_Global_OutFlags\s*\{\s*([^{}]+)\s*\}", clean)
    if m:
        v = parse_int(m.group(1), macros)
        props.append(("OLGe", 4, f"{v}L"))

    m = re.search(r"AE_Effect_Global_OutFlags_2\s*\{\s*([^{}]+)\s*\}", clean)
    if m:
        v = parse_int(m.group(1), macros)
        props.append(("2LGe", 4, f"{v}L"))

    m = re.search(r'AE_Effect_Match_Name\s*\{\s*"([^"]*)"\s*\}', clean)
    if m:
        p = pack_pascal_str(m.group(1))
        props.append(("ANMe", len(p), f'"{c_escape_bytes(p)}"'))

    m = re.search(r"AE_Reserved_Info\s*\{\s*([^{}]+)\s*\}", clean)
    if m:
        v = parse_int(m.group(1), macros)
        props.append(("LFea", 4, f"{v}L"))

    m = re.search(r'AE_Effect_Support_URL\s*\{\s*"([^"]*)"\s*\}', clean)
    if m:
        p = pack_pascal_str(m.group(1))
        props.append(("LRUe", len(p), f'"{c_escape_bytes(p)}"'))

    lines = [
        f"{res_id} PiPL DISCARDABLE",
        "BEGIN",
        "    1,",
        "    0L,",
        f"    {len(props)}L,",
    ]
    for idx, (key_rev, plen, pval) in enumerate(props):
        comma = "," if idx + 1 < len(props) else ""
        lines.append('    "MIB8",')
        lines.append(f'    "{key_rev}",')
        lines.append("    0L,")
        lines.append(f"    {plen}L,")
        lines.append(f"    {pval}{comma}")
    lines.append("END")
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    if len(sys.argv) < 3:
        sys.stderr.write("Usage: pipltool <input.r/.rr> <output.rrc/.rc>\n")
        return 1
    in_path = sys.argv[1]
    out_path = sys.argv[2]
    with open(in_path, "r", encoding="utf-8", errors="ignore") as f:
        r_text = f.read()
    rc_text = generate_pipl_rc(r_text)
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(rc_text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
