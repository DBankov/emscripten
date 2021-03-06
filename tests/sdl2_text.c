#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <emscripten.h>

int result = 0;

void one() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_TEXTEDITING: assert(0); break;
      case SDL_TEXTINPUT:
        printf("Received %s\n", event.text.text);
        if (!strcmp("a", event.text.text)) {
          result = 1;
        } else if (!strcmp("A", event.text.text)) {
          REPORT_RESULT(result);
          emscripten_run_script("throw 'done'");
        }
        break;
    }
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer(600, 450, 0, &window, &renderer);
  SDL_StartTextInput();

  emscripten_run_script("simulateKeyEvent('a'.charCodeAt(0))"); // a
  emscripten_run_script("simulateKeyEvent('A'.charCodeAt(0))"); // A

  one();

  return 0;
}
