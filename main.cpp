#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>

using namespace std;

class Framework {
public:
  // Contructor which initialize the parameters.
  Framework(int height_, int width_) : height(height_), width(width_) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldnt initialize SDL: %s",
                   SDL_GetError());
      // return 3;

    } // Initializing SDL as Video
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // setting draw color
    SDL_RenderClear(renderer);   // Clear the newly created window
    SDL_RenderPresent(renderer); // Reflects the changes done in the

    // As there are 8 frames with same width, we simply
    // get the width of a frame by dividing with 8
  }

  // Destructor
  ~Framework() {
    SDL_DestroyTexture(spriteSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
  }

  void draw_circle(int center_x, int center_y, int radius_) {
    // Setting the color to be RED with 100% opaque (0% trasparent).
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Drawing circle
    for (int x = center_x - radius_; x <= center_x + radius_; x++) {
      for (int y = center_y - radius_; y <= center_y + radius_; y++) {
        if ((pow(center_y - y, 2) + pow(center_x - x, 2)) <= pow(radius_, 2)) {
          SDL_RenderDrawPoint(renderer, x, y);
        }
      }
    }

    // Show the change on the screen
    SDL_RenderPresent(renderer);
  }

  void draw_sprite() {

    SDL_Surface *temp = IMG_Load("res/characters.png");
    spriteSheet = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);

    //'windowRect' defines the dimensions of the rendering sprite on window
    SDL_Rect window_rect;
    window_rect.x = 0;
    window_rect.y = 0;
    window_rect.w = 140;
    window_rect.h = 200;

    //'textureRect' defines the dimensions of the rendering sprite on texture
    SDL_Rect texture_rect;
    texture_rect.x = 0;
    texture_rect.y = 0;

    SDL_QueryTexture(spriteSheet, NULL, NULL, &texture_rect.w, &texture_rect.h);

    texture_rect.w /= 24;
    texture_rect.h /= 4;

    SDL_Event event;
    while (!(event.type == SDL_QUIT)) {

      // Total number of frames of the animation
      int totalFrames = 24;
      // Amount of delay in milliseconds for each frame
      int delayPerFrame = 100;
      // SDL_GetTicks() method gives us the time in milliseconds
      //'frame' will give us the index of frame we want to render
      // For example, if 'frame' is 2, it will give us the third frame
      int frame = (SDL_GetTicks() / delayPerFrame) % totalFrames;
      texture_rect.x = frame * texture_rect.w;
      SDL_PollEvent(&event);

      if (event.type == SDL_QUIT)
        return;

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      // Copying the texture on to the window using
      // renderer, texture rectangle and window rectangle
      SDL_RenderCopy(renderer, spriteSheet, &texture_rect, &window_rect);

      SDL_RenderPresent(renderer);
    }
  }

  void move_circle() {
    SDL_Event event;
    while (!(event.type == SDL_QUIT)) {
      for (int i = 0; i < height; i++) {
        SDL_Delay(10);
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
          return;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        draw_circle(width / 2, i, 25);
      }

      for (int i = height; i > 0; i++) {
        SDL_Delay(10);
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
          return;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        draw_circle(width / 2, i, 25);
      }
    }
  }

private:
  int height;                    // Height of the window
  int width;                     // Width of the window
  SDL_Renderer *renderer = NULL; // Pointer for the renderer
  SDL_Window *window = NULL;     // Pointer for the window
  SDL_Texture *spriteSheet = NULL;
};

int main(int argc, char *argv[]) {

  // Creating the object by passing Height and Width value.
  Framework fw(800, 600);

  // Calling the function that draws circle.
  // fw.move_circle();
  fw.draw_sprite();

  // Below while loop checks if the window has terminated using close in the
  //  corner.
  // while (!(event.type == SDL_QUIT)) {
  //   SDL_Delay(10);         // setting some Delay
  //   SDL_PollEvent(&event); // Catching the poll event.
  // }

  return 0;
}
