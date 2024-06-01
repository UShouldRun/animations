#include "../include/cube.h"

void initialize_colors(void) {
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
  if (format) {
    Uint32 white_pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 255, 255, 255, 255);
    Uint32 black_pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888), 0, 0, 0, 255);

    SDL_GetRGBA(white_pixel, format, &white.r, &white.g, &white.b, &white.a);
    SDL_GetRGBA(black_pixel, format, &black.r, &black.g, &black.b, &black.a);

    SDL_FreeFormat(format);
  }
  else {
    white.r = white.g = white.b = white.a = 0;
    black.r = black.g = black.b = black.a = 0;
  }
}

void rotate(Cube* cube, Matrix** matrices, size_t len) {
  for (size_t i = 0; i < len; i++) {
    Vector* temp = cube->vector;
    cube->vector = matrix_vector_mul(matrices[i], temp);
    free_vector(temp);
  }
}

void get_cube_points(Cube* cube, Point2D* points) {
  double x, y, z, scaler;
  for (int i = 0; i < 8; i++, points++) {
    x = cube->origin->x + !!(i & 1) * cube->vector->data[0] * cube->edge;
    y = cube->origin->y + !!(i >> 1 & 1) * cube->vector->data[1] * cube->edge;
    z = cube->origin->z + !!(i >> 2 & 1) * cube->vector->data[2] * cube->edge;

    scaler = z ? FOV/abs_d(z) : 1;
    points->x = scaler * x;
    points->y = scaler * y;
  }
}

void camera_adjust(Camera* camera, Point2D* points, size_t len) {
  double x_offset, y_offset;
  Point2D* origin = points;
  x_offset = camera->center.x - origin->x;
  y_offset = camera->center.y - origin->y;
  origin->x += x_offset;
  origin->y += y_offset;
  points++;
  for (size_t i = 1; i < len; i++, points++) {
    points->x += x_offset;
    points->y += y_offset;
  }
}

void draw(SDL_Renderer* renderer, Point2D* points) {
  SDL_RenderDrawLine(renderer, (int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y);
  SDL_RenderDrawLine(renderer, (int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y);
  SDL_RenderDrawLine(renderer, (int)points[0].x, (int)points[0].y, (int)points[4].x, (int)points[4].y);
  SDL_RenderDrawLine(renderer, (int)points[7].x, (int)points[7].y, (int)points[3].x, (int)points[3].y);
  SDL_RenderDrawLine(renderer, (int)points[7].x, (int)points[7].y, (int)points[5].x, (int)points[5].y);
  SDL_RenderDrawLine(renderer, (int)points[7].x, (int)points[7].y, (int)points[6].x, (int)points[6].y);
  SDL_RenderDrawLine(renderer, (int)points[3].x, (int)points[3].y, (int)points[1].x, (int)points[1].y);
  SDL_RenderDrawLine(renderer, (int)points[3].x, (int)points[3].y, (int)points[2].x, (int)points[2].y);
  SDL_RenderDrawLine(renderer, (int)points[5].x, (int)points[5].y, (int)points[1].x, (int)points[1].y);
  SDL_RenderDrawLine(renderer, (int)points[5].x, (int)points[5].y, (int)points[4].x, (int)points[4].y);
  SDL_RenderDrawLine(renderer, (int)points[6].x, (int)points[6].y, (int)points[2].x, (int)points[2].y);
  SDL_RenderDrawLine(renderer, (int)points[6].x, (int)points[6].y, (int)points[4].x, (int)points[4].y);
}

int cube() {
  int ret_val = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("[FATAL]: Runtime Error : Could not initialize SDL : %s\n", SDL_GetError());
    ret_val = 1;
    goto quit;
  }

  SDL_Window* window = SDL_CreateWindow(TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("[FATAL]: Runtime Error : Could not initialize SDL window : %s\n", SDL_GetError());
    ret_val = 2;
    goto sdl_quit;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("[FATAL]: Runtime Error : Could not initialize SDL renderer : %s\n", SDL_GetError());
    ret_val = 3;
    goto destroy_window;
  }

  Cube* cube = (Cube*)malloc(sizeof(Cube));
  if (cube == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for cube\n");
    ret_val = 5;
    goto destroy_renderer;
  }
  cube->edge = WINDOW_WIDTH/8;
  cube->origin = (Point3D*)malloc(sizeof(Point3D));
  if (cube->origin == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for cube->origin\n");
    ret_val = 6;
    goto free_cube;
  }
  cube->origin->x = 0;
  cube->origin->y = 0;
  cube->origin->z = 0;
  cube->vector = (Vector*)malloc(sizeof(Vector));
  if (cube->vector == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for cube->vector\n");
    ret_val = 7;
    goto free_cube_origin;
  }
  cube->vector->len = 3;
  cube->vector->data = (double*)malloc(cube->vector->len*sizeof(double));
  if (cube->vector->data == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for cube->vector->data\n");  
    ret_val = 8;
    goto free_cube_vector;
  }
  cube->vector->data[0] = -1;
  cube->vector->data[1] = -1;
  cube->vector->data[2] = -1;

  Point2D* points = (Point2D*)malloc(FIGURE_VERTICES*sizeof(Point2D));
  if (points == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for points\n");
    ret_val = 9;
    goto free_cube_vector_data;
  }

  size_t rows, cols;
  rows = cols = 3;
  double a, b, c, d;
  a = cos(theta); b = sin(theta); c = -sin(theta); d = cos(theta);

  double entries[9] = { a, 0, b, 0, 1, 0, c, 0, d };
  Matrix* matrix = create_matrix(rows, cols);
  if (matrix == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for matrix\n");
    ret_val = 10;
    goto free_points;
  }
  define_matrix(matrix, entries, rows*cols);

  double entries1[9] = { 1, 0, 0, 0, a, b, 0, c, d };
  Matrix* matrix1 = create_matrix(rows,cols);
  if (matrix == NULL) {
    printf("[FATAL]: Runtime Error : Could not allocate memory in the heap for memory\n");
    ret_val = 11;
    goto free_matrix_l;
  }
  define_matrix(matrix1, entries1, rows*cols);

  Matrix* matrices[2] = { matrix, matrix1 };
  size_t len = 2;

  initialize_colors();

  int quit = 0;
  int delay;
  Uint32 frame_start, frame_time;
  SDL_Event event;

  while (!quit) {
    frame_time = SDL_GetTicks() - frame_start;
    delay = 1000/frame_rate - frame_time;

    if (delay > 0) SDL_Delay(delay);

    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) quit = 1;
    }

    get_cube_points(cube, points);
    camera_adjust(&camera, points, FIGURE_VERTICES);

    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
    draw(renderer, points);
    SDL_RenderPresent(renderer);

    rotate(cube, matrices, len);

    frame_start = SDL_GetTicks();
  }

  free_matrix1_l: free_matrix(matrix1);
  free_matrix_l: free_matrix(matrix);
  free_points: free(points);
  free_cube_vector_data: free(cube->vector->data);
  free_cube_vector: free(cube->vector);
  free_cube_origin: free(cube->origin);
  free_cube: free(cube);  
  destroy_renderer: SDL_DestroyRenderer(renderer);
  destroy_window: SDL_DestroyWindow(window);
  sdl_quit: SDL_Quit();
  quit: return ret_val;
}
