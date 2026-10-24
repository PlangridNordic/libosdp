# LibOSDP documentation

The prose behind <https://doc.osdp.dev>. It lives here so each release
branch carries the docs for the API it ships; the site builder in the
[doc.osdp.dev](https://github.com/Osprio/Doc.OSDP.dev) repo renders this
tree, plus the Doxygen comments in `include/osdp.h` and the Python
docstrings, into the published site.

- `manifest.json` declares the sidebar, the Doxygen inputs and, when the
  tree documents the Python bindings, the Python sources.
- `pages/` holds one markdown file per page. A comment such as
  `<!-- doxygen:function:osdp_cp_setup -->` or
  `<!-- pydoc:class:osdp.ControlPanel -->` is replaced with the rendered
  API reference for that symbol.

The site build fails when a marker names a symbol this tree's sources do
not have, so a page must only reference API that exists on its branch.

`format` in `manifest.json` is the version of this layout. The builder
rejects a format it does not know, and keeps accepting every format a
supported release still ships.

To preview, from a doc.osdp.dev checkout:

```sh
LIBOSDP_REPO_PATH=/path/to/libosdp npm run dev
```
