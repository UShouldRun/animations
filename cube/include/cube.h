#ifndef CUBE_H
#define CUBE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../../math/include/nla.h"
#include "../../math/include/funcs.h"

#define TITLE "Cube Animation"

#define WINDOW_X 0
#define WINDOW_Y 0

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define FIGURE_VERTICES 8

static const int FOV = 90;
static const double theta = M_PI/2;
static const int frame_rate = 5;

typedef struct {
  double x, y, z;
} Point3D;

typedef struct {
  double x, y;
} Point2D;

typedef struct {
  Point3D* origin;
  Vector* vector;
  size_t edge; 
} Cube;

typedef struct {
  Uint8 r, g, b, a;
} Color_RGBA;

typedef struct {
  Point2D center;
  size_t width, height;
} Camera;

static Camera camera = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT };

static Color_RGBA white;
static Color_RGBA black;

int cube();

#endif // !CUBE_H
