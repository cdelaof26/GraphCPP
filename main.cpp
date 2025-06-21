#include "fonts/Font.h"
#include "Window.h"
#include "UI.h"

#define WIDTH  640
#define HEIGHT 480
#define MAX_UI_ELEMENTS 500

Window w(WIDTH, HEIGHT, "Graph", 60);

Shape * uiObjects[MAX_UI_ELEMENTS] = {nullptr};
int shapes = 0;

void pushShape(Shape * s) {
    if (shapes > MAX_UI_ELEMENTS)
        throw std::runtime_error("Cannot save more shapes");

    uiObjects[shapes++] = s;
}

void setPixel(int i, int j, Color c) {
    w.setPixel(i, j, c);
}

int main() {
    if (!w.isInitialized())
        return 1;

    w.background.setRed(255);
    w.background.setGreen(255);
    w.background.setBlue(255);

    Font f;
	loadFont(f);
    if (!f.fontLoaded) {
        printf("Font not loaded\n");
        return 1;
    }

    pushShape(new UINode(100, 100, Color(235, 235, 235), "A", f));
    pushShape(new UINode(200, 200, Color(235, 235, 235), "B", f));
    pushShape(new Line(uiObjects[0] -> center, uiObjects[1] -> center));


    // Main loop
    Point prevLocation;
    bool dragging = false;

	int quit = 0;
	SDL_Event e;
	while (!quit) {
		usleep(w.getSleepMicroseconds());
		
		// Manage events
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT)
                    dragging = true;
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT)
                    dragging = false;

            } else if (e.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    int xDiff = e.motion.x - prevLocation.x;
                    int yDiff = e.motion.y - prevLocation.y;
                    for (int k = shapes - 1; k > -1; k--) {
                        uiObjects[k] -> center.x += xDiff;
                        uiObjects[k] -> center.y += yDiff;
                    }
                }

                prevLocation.x = e.button.x;
                prevLocation.y = e.button.y;

            } else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
					break;
					case SDLK_LEFT:
						// SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] // Hold LShift
					break;
					case SDLK_a:
						// Pressed a
					break;
				}
			}
		}

        w.clearImage();

        // render logic
        for (int k = shapes - 1; k > -1; k--)
            uiObjects[k] -> render(reinterpret_cast<fun>(setPixel));

        w.updateImage();
	}

	return 0;
}
