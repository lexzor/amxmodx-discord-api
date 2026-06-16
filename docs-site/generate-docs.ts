import fs from "fs";
import path from "path";

// ================= CONFIG =================

interface PageConfig {
    name: string;
    file: string;
    out: string;
}

const PAGES: PageConfig[] = [
    { name: "Bot",          file: "../amxmodx/scripting/include/discordapibot.inc",          out: "bot.mdx" },
    { name: "Bot Guilds",   file: "../amxmodx/scripting/include/discordapibotguilds.inc",    out: "bot-guilds.mdx" },
    { name: "Bot Channels", file: "../amxmodx/scripting/include/discordapibotchannels.inc",  out: "bot-channels.mdx" },
    { name: "Constants",    file: "../amxmodx/scripting/include/discordapiconsts.inc",        out: "constants.mdx" },
    { name: "Utilities",    file: "../amxmodx/scripting/include/discordapiutils.inc",         out: "utilities.mdx" },
];

const OUTPUT_DIR = "./docs";

// ================= TYPES =================

interface CommentData {
    desc: string[];
    note: string[];
    params: string[];
    returns: string[];
}

interface ParsedEntry {
    name: string;
    params: string;
    comment: CommentData | null;
}

interface EnumMember {
    name: string;
    value: string | null;
    note: string | null;
    deprecated: string | null;
}

interface ParsedEnum {
    name: string;
    comment: string | null;
    lineComment: string | null;
    members: EnumMember[];
}

interface ParsedConst {
    name: string;
    value: string;
    comment: string | null;
}

interface ParsedFile {
    natives: ParsedEntry[];
    forwards: ParsedEntry[];
    enums: ParsedEnum[];
    consts: ParsedConst[];
}

// ================= TOKENIZER =================

const TOKEN_REGEX =
    /\/\*\*[\s\S]*?\*\/|native\s+(?:bool:)?[a-zA-Z0-9_]+\s*\([\s\S]*?\);|forward\s+[a-zA-Z0-9_]+\s*\([\s\S]*?\);/g;

// ================= COMMENT PARSER =================

function parseComment(raw: string): CommentData {
    const lines = raw
        .split("\n")
        .map(l => l
            .replace(/^\s*\/\*\*/, "")
            .replace(/\*\/\s*$/, "")
            .replace(/^\s*\*\s?/, "")
            .trim()
        )
        .filter(Boolean);

    const data: CommentData = { desc: [], note: [], params: [], returns: [] };
    let mode: "desc" | "note" | "param" | "return" = "desc";

    for (const line of lines) {
        if (line.startsWith("@note")) {
            mode = "note";
            data.note.push(line.replace("@note", "").trim());
            continue;
        }
        if (line.startsWith("@param")) {
            mode = "param";
            const paramLine = line.replace("@param", "").trim();
            // split on FIRST space only: "identifier Bot identifier." → ["identifier", "Bot identifier."]
            const spaceIdx = paramLine.indexOf(" ");
            if (spaceIdx !== -1) {
                const paramName = paramLine.slice(0, spaceIdx);
                const paramDesc = paramLine.slice(spaceIdx + 1).trim();
                data.params.push(`\`${paramName}\`\t${paramDesc}`);
            } else {
                data.params.push(`\`${paramLine}\`\t`);
            }
            continue;
        }
        if (line.startsWith("@return") || line.startsWith("@returns")) {
            mode = "return";
            data.returns.push(line.replace(/@returns?/, "").trim());
            continue;
        }
        if (line.startsWith("@see")) {
            const url = line.replace("@see", "").trim();
            data.desc.push(`See: [${url}](${url})`);
            continue;
        }

        if (mode === "note" && data.note.length)
            data.note[data.note.length - 1] += "\n" + line;
        else if (mode === "param" && data.params.length) {
            // continuation of param description — append to the desc part only (after the tab)
            const last = data.params[data.params.length - 1];
            data.params[data.params.length - 1] = last + " " + line;
        } else if (mode === "return" && data.returns.length)
            data.returns[data.returns.length - 1] += " " + line;
        else
            data.desc.push(line);
    }

    return data;
}

function parseEnumMemberComment(raw: string): { note: string | null; deprecated: string | null } {
    const lines = raw
        .split("\n")
        .map(l => l.replace(/^\s*\/?\*+\/?\s?/, "").trim())
        .filter(Boolean);

    let note: string | null = null;
    let deprecated: string | null = null;
    let mode: "note" | "deprecated" | "see" | null = null;

    for (const line of lines) {
        if (line.startsWith("@note")) {
            mode = "note";
            note = line.replace("@note", "").trim();
            continue;
        }
        if (line.startsWith("@deprecated")) {
            mode = "deprecated";
            deprecated = line.replace("@deprecated", "").trim();
            continue;
        }
        if (line.startsWith("@see")) {
            const url = line.replace("@see", "").trim();
            if (note !== null) note += ` [link](${url})`;
            continue;
        }
        if (mode === "note" && note !== null)             note += " " + line;
        if (mode === "deprecated" && deprecated !== null) deprecated += " " + line;
    }

    return { note, deprecated };
}

// ================= ENUM PARSER =================

function stripInlineComments(body: string): string {
    return body.replace(/\/\/[^\n]*/g, "");
}

function parseEnums(text: string): ParsedEnum[] {
    const results: ParsedEnum[] = [];

    const enumRegex = /(\/\/[^\n]*)?\n?\s*(\/\*\*[\s\S]*?\*\/)?\s*enum\s+([a-zA-Z0-9_]+)\s*\{([\s\S]*?)\}/g;

    for (const match of text.matchAll(enumRegex)) {
        const lineComment = match[1]?.replace(/^\/\/\s*/, "").trim() ?? null;
        const blockComment = match[2] ?? null;
        const enumName = match[3];
        const rawBody = match[4];

        // strip // inline comments BEFORE member parsing to prevent binary literals
        // like 0b0000000000010000 from being matched as member names
        const body = stripInlineComments(rawBody);

        let enumDesc: string | null = null;
        if (blockComment) {
            const lines = blockComment
                .split("\n")
                .map(l => l.replace(/^\s*\/?\*+\/?\s?/, "").trim())
                .filter(l => Boolean(l) && !l.startsWith("@") && l !== "*");
            enumDesc = lines.join(" ").trim() || null;
        }

        // value regex: stop at comma, newline, closing brace, or forward-slash (start of //)
        const memberRegex = /(\/\*\*[\s\S]*?\*\/)?\s*\b([A-Z][A-Z0-9_]*)\b\s*(?:=\s*([^,\n}/]+))?/g;
        const members: EnumMember[] = [];

        for (const m of body.matchAll(memberRegex)) {
            const rawComment = m[1] ?? null;
            const name = m[2].trim();
            const value = m[3]?.trim() || null;

            const { note, deprecated } = rawComment
                ? parseEnumMemberComment(rawComment)
                : { note: null, deprecated: null };

            members.push({ name, value, note, deprecated });
        }

        results.push({ name: enumName, comment: enumDesc, lineComment, members });
    }

    return results;
}

// ================= CONST PARSER =================

function parseConsts(text: string): ParsedConst[] {
    const results: ParsedConst[] = [];

    // tight comment match: no [\s\S] to prevent spanning across enum blocks
    const constRegex = /(\/\*\*[^*]*(?:\*(?!\/)[^*]*)*\*\/)?\s*const\s+[a-zA-Z0-9_]+:([a-zA-Z0-9_]+)\s*=\s*([^;]+);/g;

    for (const match of text.matchAll(constRegex)) {
        const rawComment = match[1] ?? null;
        const name = match[2].trim();
        const value = match[3].trim();

        let comment: string | null = null;
        if (rawComment) {
            const lines = rawComment
                .split("\n")
                .map(l => l.replace(/^\s*\/?\*+\/?\s?/, "").trim())
                .filter(l => Boolean(l) && l !== "*" && !l.startsWith("@note") && !l.startsWith("@param"));
            comment = lines.join(" ").trim() || null;
        }

        results.push({ name, value, comment });
    }

    return results;
}

// ================= PARSE ONE FILE =================

function parseFile(filePath: string): ParsedFile {
    const text = fs.readFileSync(filePath, "utf8");
    const tokens = [...text.matchAll(TOKEN_REGEX)];

    const result: ParsedFile = {
        natives: [],
        forwards: [],
        enums: parseEnums(text),
        consts: parseConsts(text),
    };

    let lastComment: CommentData | null = null;

    for (const t of tokens) {
        const str = t[0];

        if (str.startsWith("/**")) {
            lastComment = parseComment(str);
            continue;
        }

        if (str.startsWith("native")) {
            const m = str.match(/native\s+(?:bool:)?([a-zA-Z0-9_]+)\s*\(([\s\S]*?)\);/);
            if (!m) continue;
            result.natives.push({ name: m[1], params: m[2] ?? "", comment: lastComment });
            lastComment = null;
        }

        if (str.startsWith("forward")) {
            const m = str.match(/forward\s+([a-zA-Z0-9_]+)\s*\(([\s\S]*?)\);/);
            if (!m) continue;
            result.forwards.push({ name: m[1], params: m[2] ?? "", comment: lastComment });
            lastComment = null;
        }
    }

    return result;
}

// ================= JSON UTILS =================

// Finds the outermost balanced { } block in text.
// Returns indices [start, end] inclusive, or null if not found.
function findBalancedBraces(text: string): [number, number] | null {
    const start = text.indexOf("{");
    if (start === -1) return null;

    let depth = 0;
    for (let i = start; i < text.length; i++) {
        if (text[i] === "{") depth++;
        else if (text[i] === "}") {
            depth--;
            if (depth === 0) return [start, i];
        }
    }
    return null;
}

function tryFormatJSON(raw: string): string {
    try {
        const parsed = JSON.parse(raw);
        return "```json\n" + JSON.stringify(parsed, null, 2) + "\n```";
    } catch {
        // not valid JSON (has placeholders/comments) — clean up and wrap as-is
        const cleaned = raw
            .split("\n")
            .map(l => l.trim())
            .filter(Boolean)
            .join("\n");
        return "```json\n" + cleaned + "\n```";
    }
}

function renderNote(noteLines: string[]): string {
    const noteText = noteLines.join("\n");
    const braces = findBalancedBraces(noteText);

    let out = ":::info Note\n\n";

    if (braces) {
        const [start, end] = braces;
        const before = noteText.slice(0, start).trim();
        const json   = noteText.slice(start, end + 1);
        const after  = noteText.slice(end + 1).trim();

        if (before) out += before + "\n\n";
        out += tryFormatJSON(json) + "\n\n";
        if (after)  out += after + "\n\n";
    } else {
        out += noteText + "\n\n";
    }

    out += ":::\n\n";
    return out;
}

// ================= MDX ESCAPE =================

function escapeMDX(str: string): string {
    // Protect: fenced code blocks, HTML tags (details/summary), admonition markers
    const protectedPattern = /(```[\s\S]*?```|<\/?(?:details|summary)[^>]*>|:::[\s\S]*?:::)/g;
    const parts = str.split(protectedPattern);

    return parts.map((part, i) => {
        if (i % 2 === 1) return part; // protected segment — untouched
        return part
            .replace(/</g, "&lt;")
            .replace(/>/g, "&gt;")
            .replace(/\{/g, "\\{")
            .replace(/\[/g, "\\[");
    }).join("");
}

// ================= RENDER PARAMS TABLE =================

function renderParamsTable(params: string[]): string {
    if (!params.length) return "";

    let out = "#### Parameters\n\n";
    out += "| Parameter | Description |\n";
    out += "|-----------|-------------|\n";

    for (const p of params) {
        // format stored as "`name`\tdescription"
        const tabIdx = p.indexOf("\t");
        if (tabIdx !== -1) {
            const name = p.slice(0, tabIdx);
            const desc = p.slice(tabIdx + 1).trim();
            out += `| ${name} | ${desc} |\n`;
        } else {
            out += `| ${p} | |\n`;
        }
    }

    return out + "\n";
}

// ================= RENDER ENTRY (native/forward) =================

function renderEntry(fn: ParsedEntry): string {
    let out = `### \`${fn.name}\`\n\n`;

    if (fn.comment) {
        const c = fn.comment;

        if (c.desc.length)
            out += c.desc.join(" ") + "\n\n";

        if (c.note.length)
            out += renderNote(c.note);

        // plain signature block — no accordion, no blue bg
        out += "#### Signature\n\n";
        out += "```c\n";
        out += `${fn.name}(${fn.params});\n`;
        out += "```\n\n";

        out += renderParamsTable(c.params);

        if (c.returns.length) {
            out += "#### Returns\n\n";
            for (const r of c.returns) out += `${r}\n\n`;
        }
    } else {
        out += "```c\n";
        out += `${fn.name}(${fn.params});\n`;
        out += "```\n\n";
    }

    out += "---\n\n";
    return out;
}

// ================= RENDER ENUMS =================

function renderEnums(enums: ParsedEnum[]): string {
    if (!enums.length) return "";

    let out = "## Enums\n\n";

    for (const en of enums) {
        out += `### \`${en.name}\`\n\n`;

        if (en.lineComment) out += `> ${en.lineComment}\n\n`;
        if (en.comment)     out += `> ${en.comment}\n\n`;

        const hasValues = en.members.some(m => m.value !== null);

        if (hasValues) {
            out += "| Member | Value | Description |\n";
            out += "|--------|:-----:|-------------|\n";
            for (const m of en.members) {
                // render value as plain text — escapeMDX will handle < > later
                const value = m.value ? `\`${m.value.trim()}\`` : "—";
                let desc = m.note ?? "";
                if (m.deprecated) desc += ` ⚠️ **Deprecated:** ${m.deprecated}`;
                out += `| \`${m.name}\` | ${value} | ${desc} |\n`;
            }
        } else {
            out += "| Member | Description |\n";
            out += "|--------|-------------|\n";
            for (const m of en.members) {
                let desc = m.note ?? "";
                if (m.deprecated) desc += ` ⚠️ **Deprecated:** ${m.deprecated}`;
                out += `| \`${m.name}\` | ${desc} |\n`;
            }
        }

        out += "\n---\n\n";
    }

    return out;
}

// ================= RENDER CONSTS =================

function renderConsts(consts: ParsedConst[]): string {
    if (!consts.length) return "";

    let out = "## Constants\n\n";
    out += "| Name | Value | Description |\n";
    out += "|------|:-----:|-------------|\n";

    for (const c of consts) {
        out += `| \`${c.name}\` | \`${c.value}\` | ${c.comment ?? ""} |\n`;
    }

    out += "\n---\n\n";
    return out;
}

// ================= RENDER MDX =================

function toMDX(title: string, { natives, forwards, enums, consts }: ParsedFile): string {
    let md = `---\ntitle: ${title}\n---\n\n# ${title}\n\n`;

    if (natives.length) {
        md += "## Natives\n\n";
        for (const fn of natives) md += renderEntry(fn);
    }

    if (forwards.length) {
        md += "## Forwards\n\n";
        for (const fn of forwards) md += renderEntry(fn);
    }

    md += renderEnums(enums);
    md += renderConsts(consts);

    return escapeMDX(md);
}

// ================= OUTPUT =================

fs.mkdirSync(OUTPUT_DIR, { recursive: true });

for (const page of PAGES) {
    const parsed = parseFile(page.file);
    const mdx = toMDX(page.name, parsed);
    fs.writeFileSync(path.join(OUTPUT_DIR, page.out), mdx);
    console.log(
        `✅ ${page.out.padEnd(22)} — ${parsed.natives.length} natives, ${parsed.forwards.length} forwards, ${parsed.enums.length} enums, ${parsed.consts.length} consts`
    );
}

fs.writeFileSync(
    path.join(OUTPUT_DIR, "index.mdx"),
    `---
title: Discord API
---

# Discord API

| Page | Description |
|------|-------------|
${PAGES.map(p => `| [${p.name}](./${p.out.replace(".mdx", "")}) | — |`).join("\n")}
`
);

console.log("📄 Generated index.mdx");