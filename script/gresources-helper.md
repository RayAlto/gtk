# Gresources Helper

> From [RayAlto/gresources-helper](https://github.com/RayAlto/gresources-helper "RayAlto/gresources-helper: A python script help you generate XML file for glib-compile-resources.") by myself

A python script help you generate the XML file for `glib-compile-resources` and run `glib-compile-resources` to generate C code.

## Example

Your resources dir, like `/home/user/dev/project/res`:

```plain
.
├── bar
│   ├── aaa.txt
│   └── ooo.txt
├── bar.txt
├── foo
│   ├── bbb.txt
│   └── fff.txt
└── foo.txt
```

set `resources_dir` in this script (line 9) to `/home/user/dev/project/res` and run the script, `resources.cc` and `resources.xml` will be generated:

```plain
.
├── bar
│   ├── aaa.txt
│   └── ooo.txt
├── bar.txt
├── foo
│   ├── bbb.txt
│   └── fff.txt
├── foo.txt
├── resources.cc
└── resources.xml
```

> `resources.cc` and `resources.xml` in the root directory will be ignored in the generated `resources.xml`

resources.xml:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<gresources>
  <gresource prefix="/">
    <file>foo.txt</file>
    <file>bar.txt</file>
    <file>bar/ooo.txt</file>
    <file>bar/aaa.txt</file>
    <file>foo/bbb.txt</file>
    <file>foo/fff.txt</file>
  </gresource>
</gresources>
```
