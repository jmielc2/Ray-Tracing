# Ray Tracer
This is my implementation of a ray tracing program that is capable of rendering a simple scene of objects. The program runs entirely on the CPU. For each pixel in an image, the program generates a ray for the pixel (varies depending on camera type), computes object intersections for each ray, and calculates the final shading value for the pixel. This project was made with the help of the SDL, SDL_image, and glm libraries.

Currently a work in progress.

## Latest Working Version:
I've gotten basic object intersection working as seen here with the ground and sphere. Shading is next on the TODO list.

![alt text](https://github.com/jmielc2/Ray-Tracing/blob/master/docs/rendered-scene.png?raw=true)

Render Time (Release Version): 42.069 ms
