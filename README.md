# Ray Tracer
This is my implementation of a ray tracing program that is capable of rendering a simple scene of objects. The program runs entirely on the CPU. For each pixel in an image, the program generates a ray for the pixel (varies depending on camera type), computes object intersections for each ray, and calculates the final shading value for the pixel. This project was made with the help of the SDL, SDL_image, nlohmann/json, and glm libraries.

## Latest Working Version:

The files containing the scene description used to render each of the following images is in the ```Ray-Tracing/scenes``` folder.
Each pixel samples 16 rays and each ray has a maximum depth of 10 bounces.
Rendered on a Intel Core i5, 2 GHz

### Scene 1

![alt text](https://github.com/jmielc2/Ray-Tracing/blob/master/docs/rendered-scene-1.png?raw=true)

Render Time (Release Version): 561.90 ms

### Scene 2

![alt text](https://github.com/jmielc2/Ray-Tracing/blob/master/docs/rendered-scene-2.png?raw=true)

Render Time (Release Version): 367.58 ms

### Scene 3

![alt text](https://github.com/jmielc2/Ray-Tracing/blob/master/docs/rendered-scene-3.png?raw=true)

Render Time  (Release Version): 730.80 ms
