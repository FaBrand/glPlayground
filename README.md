# Playground rules

This is just a playground project to try random things.
If you see any major errors here, please point them out.
The main reason behind creating this was personal learning and playing around with certain things.

## Package requirements
Install glew as described [here](https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Linux#OpenGL_Installation_on_Linux)
```bash
sudo apt-get install libglew-dev libglm-dev
```

I decided to give glfw a try. The [Getting Started](http://www.glfw.org/docs/latest/quick_guide.html) is used here.
glfw needs to be compiled and installed locally
```bash
git clone https://github.com/glfw/glfw.git ~/glfw
cmake -B~/glfw/_build -H~/glfw -GNinja -DCMAKE_BUILD_TYPE=RELEASEninja
sudo ninja install -C ~/glfw/_build
```

## Building
Simply copy paste

```bash
cmake -B_build -H. -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=On
ninja -C _build
```

# Disclaimer
This is only meant for personal education.
If used, use at your own risk.
