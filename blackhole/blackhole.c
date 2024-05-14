#include <SDL2/SDL.h>
#include <stdio.h>

// Function to initialize SDL and create a window
int init(SDL_Window** window, SDL_Renderer** renderer, const char* windowTitle, int windowX, int windowY) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 0;
    }

    int SCREEN_WIDTH, SCREEN_HEIGHT;
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(0, &displayBounds);
    SCREEN_WIDTH = displayBounds.w;
    SCREEN_HEIGHT = displayBounds.h;

    // Create a window
    *window = SDL_CreateWindow(windowTitle, windowX, windowY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    // Create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

int main() {
    // Window variables
    const char* WINDOW_TITLE = "Blackhole animation";
    
    int WINDOW_X = SDL_WINDOWPOS_CENTERED; // Default to center
    int WINDOW_Y = SDL_WINDOWPOS_CENTERED; // Default to center

    // Initialize SDL components
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!init(&window, &renderer, WINDOW_TITLE, WINDOW_X, WINDOW_Y)) {
        return EXIT_FAILURE;
    }

    // Main loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

