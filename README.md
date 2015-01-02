TerrainGenerator
================

I created this project as a learning exercise for Qt and OpenGL as well as a refresher for C++. The terrain generator was created using the following:

- C++
- Qt Creator 2.0.1, based on Qt 4.7.0
- OpenGL

Terrain is generated as a hexagonal grid by sampling a height map and a noise map.

#### Todo ####

- Convert to multi-threading to generate patches faster
- Use VAOs and VBOs instead of OpenGL's immediate mode rendering
- Implement mesh structure that stores adjacent vertices within and across patches
- Change smoothing and normal calculations to be based on adjacent vertices, rather than adjacent height map samples

#### Screenshots ####
![terraingenerator_default](https://cloud.githubusercontent.com/assets/8391408/5597428/57d720e2-925f-11e4-9c1e-5fa5fbab5432.jpg)
![terraingenerator_wire](https://cloud.githubusercontent.com/assets/8391408/5597429/57d8feda-925f-11e4-8c94-7993e28cc553.jpg)
![terraingenerator_texture](https://cloud.githubusercontent.com/assets/8391408/5597430/57dc3ee2-925f-11e4-817f-74c4aa4a45ce.jpg)
