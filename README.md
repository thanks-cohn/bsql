# BetterSQL

**The fastest way to remember your files.**

BetterSQL is a Linux-native, zero-dependency file memory engine built in C.
It turns your filesystem into a searchable, structured memory layer—without databases, daemons, or hidden state.

---

## What It Does

BetterSQL scans your files, builds a lightweight index, and lets you search them instantly using natural recall:

```bash
bsql location ~/Documents
bsql location ~/Pictures

bsql rebuild-all

bsql peek screenshot
bsql search "invoice march" --limit 10
bsql context <path>
```

It doesn’t try to replace your filesystem.
It **augments it**—giving you recall, structure, and speed.

---

## Core Capabilities

### 🔍 Instant Search Across Everything

Search hundreds of thousands (or millions) of files in milliseconds:

```bash
bsql search screenshot --limit 5
```

* Case-insensitive
* Works on filenames, paths, and metadata
* Scales linearly and predictably

---

### 👁️ Human-Friendly Recall (`peek`)

Quickly see recognizable results:

```bash
bsql peek screenshot
```

* Clean paths (`~`)
* Top results only
* Designed for thinking, not parsing

---

### 🧠 Structured Memory (Sidecars)

Attach meaning to files:

```bash
bsql tag file.txt invoice rent march
bsql note file.txt "paid late but confirmed"
bsql summary video.mp4 "client walkthrough recording"
```

Stored as:

```text
file.txt.wmeta.json
```

No hidden database. Everything is visible, portable, editable.

---

### 🧩 Full Context View

```bash
bsql context <file>
```

Shows:

* Path
* ID
* Indexed record
* Sidecar metadata
* History (future expansion)

---

### 🧬 Deterministic Identity

```bash
bsql id <file>
```

Every file gets a stable identifier based on its path.

---

### 🛠️ System Health + Auditability

```bash
bsql doctor
bsql status
bsql logs
```

You always know:

* what’s indexed
* what’s missing
* what changed
* what broke

No black boxes. No guessing.

---

### 🔄 Rebuild + Recovery

```bash
bsql rebuild <path>
bsql rebuild-all
```

The index can be destroyed and rebuilt at any time.

Your **true source of memory** is:

```text
filesystem + sidecars
```

---

## Mounted Drives & External Storage

BetterSQL works seamlessly with mounted drives:

```bash
bsql location /run/media/big-bro/ExternalDrive
bsql rebuild-all
bsql search screenshot
```

You can index:

* external HDDs
* SSDs
* NAS mounts
* archival drives

---

## Performance

BetterSQL is written in pure C with no runtime overhead.

Real-world behavior:

```text
~300,000 files → ~0.2s search
~75MB index → fully memory-streamed
```

### Theoretical Limits

On a powerful machine:

* Millions of files → still linear scan
* Index size → limited only by disk + RAM bandwidth
* Performance → bounded by file I/O, not architecture

There is **no artificial ceiling**.

---

## Why It’s Fast

* Sequential file scanning (cache-friendly)
* No database engine
* No query planner overhead
* No indexing layers beyond JSONL

Just:

```text
read → match → output
```

---

## Why It Helps Old Computers

BetterSQL is designed for minimal systems:

* No background services
* No RAM-heavy processes
* No database daemons
* No GPU / heavy dependencies

On older hardware:

```text
it trades complexity for predictability
```

You still get:

* fast searches
* deterministic behavior
* zero system strain

---

## Philosophy

BetterSQL follows a strict design model:

```text
Filesystem = truth
Sidecars   = memory
Index      = cache
```

Nothing is hidden. Nothing is magical.

You can:

* inspect everything
* delete everything
* rebuild everything

---

## Auditability

Every component is:

* Plain text
* Human-readable
* Diffable
* Recoverable

Logs:

```bash
bsql logs
```

Index:

```text
~/.local/share/bsql/index/bsql.index.jsonl
```

Metadata:

```text
<file>.wmeta.json
```

You are never locked in.

---

## Stability

BetterSQL is built to:

* never corrupt your files
* never modify original data
* never depend on external services

All operations are:

```text
read-only or additive
```

---

## Efficiency

BetterSQL offers:

```text
maximum capability
minimum surface area
```

No:

* background indexing
* watchers
* daemons
* cloud sync
* telemetry

Just a binary and your files.

---

## Commands Overview

```bash
bsql where
bsql status
bsql doctor
bsql logs

bsql location <path>
bsql rebuild <path>
bsql rebuild-all

bsql search <query> [--limit N]
bsql peek <query>

bsql tag <file> <tags...>
bsql note <file> <note>
bsql summary <file> <summary>

bsql context <file>
bsql id <file>
bsql history <file>

bsql missing
bsql repair
bsql clean
```

---

## The Idea

BetterSQL is not a database.

It is:

```text
a memory layer for your filesystem
```

Built to be:

* fast enough for millions of files
* simple enough to audit line-by-line
* small enough to trust completely

---

## Final Thought

On a powerful machine, BetterSQL scales to massive datasets.

On a weak machine, it still works.

That’s the point.

```text
The smallest,fastest auditable package.
```
