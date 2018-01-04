# Triangles

A basic Window that only draws a default triangle on the window using vertex buffers.
May draw nothing at all depending on the default Shaders defined (or possibly not defined) by the Graphics driver.

Link to [Youtube Tutorial](https://www.youtube.com/watch?v=0p9VxImr7Y0) followed to do this.

# Driver Problems
While writing the basic shaders i encountered problems with certain features not being accepted by the GLSL compiler.
As it turned out i forgot to install the proprietary NVIDIA drivers.
After installing the drivers as described [here](https://wiki.ubuntuusers.de/Grafikkarten/Nvidia/nvidia/), everything did compile as expected.
