# pandoc

```shell
sudo pacman -S pandoc

pandoc input.md -o output.html
# 转换为完整的 HTML 文档（包含 <head>, <body> 标签）
pandoc -s input.md -o output.html
# 启用代码高亮 (需要先安装相关支持，例如 highlight.js)
pandoc -s --highlight-style zenburn input.md -o output.html
```

为了防止pandoc报警, 也为了文档的完整性, 可选在文档顶部加上YAML元数据块

```markdown
---
title: Modbus Host TCP 客户端库
author: Shins0u
date: 2025-10-24
---

# 正文标题
正文
```

# cmark-gfm

GitHub 官方 Markdown 解析器的 C 实现

```shell
yay -S cmark-gfm
cmark-gfm input.md > output.html
```

# pdftk

```shell
sudo pacman -S pdftk
# 合并
pdftk 1.pdf 2.pdf cat output merged.pdf
# 拆分
pdftk in.pdf cat 1-5 output out.pdf
```

# weasyprint

```shell
sudo pacman -S python-weasyprint
weasyprint example.html example.pdf
```
