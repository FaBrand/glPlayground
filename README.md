# Playground rules

This is just a playground project to try random things.
If you see any major errors here, please point them out.
The main reason behind creating this was personal learning and playing around with certain things.

## Package requirements
```bash
sudo apt-get install libglew-dev
```
glfw needs to be compiled and installed locally
```bash
cd
git clone https://github.com/glfw/glfw.git
mkdir glfw/_build && cd $_
cmake ../ -GNinja
ninja
sudo ninja install
```

## Building
Simply copy paste

```bash
mkdir _build && cd $_
cmake ../ -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=On
ninja
```

# Disclaimer
This is only meant for personal education.
If used, use at your own risk.
