#include "fonts/Font.h"
#include "Window.h"
#include "UI.h"

#include "algorithms/algorithm.h"

#define WIDTH  640
#define HEIGHT 480
#define MAX_UI_ELEMENTS 500

Window w(WIDTH, HEIGHT, "Graph", 60);
Font f;

Shape * shapes[MAX_UI_ELEMENTS] = {nullptr};
int createdShapes = 0;

void pushShape(Shape * s) {
    if (createdShapes > MAX_UI_ELEMENTS)
        throw std::runtime_error("Cannot save more Shapes");

    shapes[createdShapes++] = s;
}


Shape * uiElements[MAX_UI_ELEMENTS] = {nullptr};
int createdUIElements = 0;

void pushUIElement(Shape * s) {
    if (createdUIElements > MAX_UI_ELEMENTS)
        throw std::runtime_error("Cannot save more UI elements");

    uiElements[createdUIElements++] = s;
}


void setPixel(int i, int j, Color c) {
    w.setPixel(i, j, c);
}

bool started = false;

void start() {
    if (started)
        return;

    started = true;
    createdUIElements--;
    uiElements[0] = nullptr;

    int n = 5;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    std::vector<std::vector<int>> mst(n, std::vector<int>(n));
    create_matrix(n, matrix, 0);
    insert_edge(n, matrix, 0, 1, 2);
    insert_edge(n, matrix, 0, 4, 4);
    insert_edge(n, matrix, 1, 2, 6);
    insert_edge(n, matrix, 1, 4, 3);
    insert_edge(n, matrix, 2, 3, 7);
    insert_edge(n, matrix, 2, 4, 5);
    insert_edge(n, matrix, 3, 4, 1);
    printf("-------GRAPH------\n");
    print_matrix(n, matrix);

    Point v[n];
    init_regular_star(v, n, WIDTH / 2, HEIGHT / 2, 200, 0);
    for (int i = 0; i < n; i++)
        pushShape(new UINode(v[i], Color(235), std::string("") + (char) ('A' + i), f));

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            if (matrix[i][j] != 0)
                pushShape(new UILine(shapes[j] -> location, shapes[i] -> location, std::to_string(matrix[i][j]), f));
}

int main() {
    if (!w.isInitialized())
        return 1;

    w.background.setChannels(255);

	loadFont(f);
    if (!f.fontLoaded) {
        printf("Font not loaded\n");
        return 1;
    }

    /*
    pushShape(new UINode(100, 100, Color(235, 235, 235), "A", f));
    pushShape(new UINode(200, 200, Color(235, 235, 235), "B", f));
    pushShape(new Line(shapes[0] -> location, shapes[1] -> location));
    */

    pushUIElement(new UIButton(reinterpret_cast<action>(start), "Iniciar", f));
    auto * button = dynamic_cast<UIButton *>(uiElements[0]);
    button -> location.x = WIDTH - button -> getWidth() - 10;
    button -> location.y = HEIGHT - button -> getHeight() - 10;


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

                if (!dragging)
                    for (int k = createdUIElements - 1; k > -1; k--) {
                        try {
                            auto * b = dynamic_cast<UIButton *>(uiElements[k]);
                            b -> triggerAction();
                        } catch (std::runtime_error &e) { }
                    }

            } else if (e.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    int xDiff = e.motion.x - prevLocation.x;
                    int yDiff = e.motion.y - prevLocation.y;
                    for (int k = createdShapes - 1; k > -1; k--) {
                        shapes[k] -> location.x += xDiff;
                        shapes[k] -> location.y += yDiff;
                    }
                }

                prevLocation.x = e.button.x;
                prevLocation.y = e.button.y;

                for (int k = createdUIElements - 1; k > -1; k--) {
                    try {
                        auto * b = dynamic_cast<UIButton *>(uiElements[k]);
                        b -> setHover(e.motion.x, e.motion.y);
                    } catch (std::runtime_error &e) { }
                }

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
        for (int k = createdShapes - 1; k > -1; k--)
            shapes[k] -> render(reinterpret_cast<fun>(setPixel));

        for (int k = createdUIElements - 1; k > -1; k--)
            uiElements[k] -> render(reinterpret_cast<fun>(setPixel));

        w.updateImage();
	}

	return 0;
}
