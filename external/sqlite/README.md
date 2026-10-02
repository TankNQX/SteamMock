# SQLite

The state file the backend can keep between runs is a SQLite database, and this is
SQLite's own amalgamation: `sqlite3.c` and `sqlite3.h`, two files, exactly as
sqlite.org publishes them. Nothing here is modified.

| | |
| --- | --- |
| Version | 3.53.4 |
| Source id | `2026-07-24 19:02:57 bf7c7f30031888f4e796e429ab3978879485813aaca6f641c7b33e4e09459bcc` |
| From | <https://sqlite.org/2026/sqlite-amalgamation-3530400.zip> |
| SHA3-256 of that zip | `628a44cfe82c66aed1ccbbe85a562d2e33ebe64b3288981ed76285612227934e` |
| Licence | public domain ([sqlite.org/copyright.html](https://sqlite.org/copyright.html)) |

## Why a vendored file rather than a submodule

The other third-party code here is a git submodule, because each of those projects
publishes its source tree and that tree is what a builder wants. SQLite does not:
`github.com/sqlite/sqlite` is the canonical source, and `sqlite3.c` is a **build
product** of it, produced by `make sqlite3.c` and published separately as the zip
above. There is no commit anywhere that holds both the version and these two files,
so a submodule would give a builder a directory that cannot be compiled without Tcl
and a `make`.

Vendoring the two published files is therefore the only way to pin the version, and
making the pin checkable is what the table above is for: the three checks below
reproduce it from the zip.

```powershell
# The size, the SHA3-256 sqlite.org prints on its download page, and the version.
Get-Item sqlite-amalgamation-3530400.zip | Select-Object Length   # 2946650
python -c "import hashlib,sys;h=hashlib.sha3_256();h.update(open(sys.argv[1],'rb').read());print(h.hexdigest())" sqlite-amalgamation-3530400.zip
Select-String -Path sqlite-amalgamation-3530400\sqlite3.h -Pattern 'SQLITE_VERSION '
```

## How it is built

`CMakeLists.txt` at the repository root builds `sqlite3.c` as its own static library
and compiles it with the defaults MSVC and clang-cl are given, not with the `/W4`
this tree holds its own code to: the amalgamation is not this project's code, and a
warning in it is not a finding anyone here acts on. That is the same treatment the
live view's Dear ImGui sources get.

Two of SQLite's compile-time options are set, and there is nothing else:

* `_CRT_SECURE_NO_WARNINGS`, so the CRT does not report SQLite's own `fopen` and
  `getenv` calls. The project's own targets are given this for the same reason, from
  the loop in `CMakeLists.txt` that they are not part of.
* `SQLITE_OMIT_LOAD_EXTENSION`, because a state file has no reason to be able to load
  code out of itself. It is the one thing SQLite does here that a debugging harness
  would never want, and leaving it out is what removes it rather than merely not
  calling it.

Everything else is stock, which is what keeps `SQLITE_SOURCE_ID` above meaningful: the
id is a hash of the source tree this file was built from, and it is the same source
tree because no other define changes it.
