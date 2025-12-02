```shell
sudo pacman -S pip pyright ruff-lsp
pip config set global.index-url https://mirrors.aliyun.com/pypi/simple
pip config set install.trusted-host mirrors.aliyun.com
```

```shell
python -m venv .venv
source ./venv/bin/activate

pip install #...
python main.py
```
