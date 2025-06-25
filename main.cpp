#include "fonts/Font.h"
#include "Window.h"
#include "UI.h"

#include "algorithms/algorithm.h"

#define WIDTH  640
#define HEIGHT 480

Window w(WIDTH, HEIGHT, "Graph", 60);
Font f;

void setPixel(int i, int j, Color c) {
    w.setPixel(i, j, c);
}

std::vector<std::vector<int>> * matrix;
std::vector<std::vector<int>> * mst;
std::vector<std::vector<std::vector<int>>> * states;
std::vector<Point> savedLines;
int n = 0;
int step = 0;
int algorithm = -1;

void showNextStep() {
    if (step != 0 && step >= states -> size())
        return;

    if (step == 0) {
        states = algorithm == 0 ? kruskal(n, *matrix, *mst) :
                algorithm == 1 ? nullptr : nullptr;

        // printf("states: %zu\n", states->size());
        if (states == nullptr || states -> empty()) {
            auto * b = dynamic_cast<UIButton *>(w.getUIElement(0));
            b -> setText("Err");
            b -> location.x = WIDTH - b -> getWidth() - 10;
            b -> location.y = HEIGHT - b -> getHeight() - 10;
            b -> setAction(nullptr);
            b -> setTextColor(Color(255, 0, 0));
            step = 10000;
            return;
        }
    }

    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            if ((* states)[step][i][j] != 0) {
                // printf("non zero %d, %d %d\n", step, i, j);
                for (int k = 0; k < savedLines.size(); k++)
                    if (savedLines[k].x == i && savedLines[k].y == j) {
                        // There are n initial nodes added before the lines
                        w.getShape(n + k) -> color.setRed(255);
                        break;
                    }
            }

    step++;
    if (step == states -> size())
        w.deleteFirstUIElement();
    // printf("steps %d\n", step);
}

void createGraph() {
    const std::string name = "sample.txt";
    n = get_matrix_length(name);
    matrix = new std::vector<std::vector<int>>(n, std::vector<int>(n));
    mst = new std::vector<std::vector<int>>(n, std::vector<int>(n));

    std::string algorithmName;
    create_matrix_from_file(name, n, algorithmName, *matrix);

    auto * t = dynamic_cast<UIText *>(w.getUIElement(1));

    if (algorithmName.empty()) {
        t -> text = "Archivo invalido";
        return;
    }

    char c = (char) std::tolower(algorithmName.at(0));
    switch (c) {
        case 'k':
            algorithm = 0;
        break;
        case 'p':
            algorithm = 1;
        break;
        case 'd':
            algorithm = 2;
        break;
        default:
            t -> text = "Algoritmo invalido";
        return;
    }

    t -> text = algorithmName;

    printf("Algorithm %s\n", algorithmName.c_str());
    printf("-------GRAPH------\n");
    print_matrix(n, *matrix);

    Point v[n];
    init_regular_star(v, n, WIDTH / 2, HEIGHT / 2, 200, 0);
    for (int i = 0; i < n; i++)
        w.pushShape(new UINode(v[i], Color(235), std::string("") + (char) ('A' + i), f));

    for (int j = 0; j < n; j++)
        for (int i = j; i < n; i++)
            if ((*matrix)[i][j] != 0) {
                // printf("create\n");
                savedLines.emplace_back(i, j);
                w.pushShape(new UILine(w.getShape(j) -> location, w.getShape(i) -> location, std::to_string((*matrix)[i][j]), f));
            }

    // In the second for loop, i = j is required to avoid duplicated edges.
    // Those nested loops will only read from the matrix diagonal and onwards.
    // 0 2 0 0 4
    // - 0 6 0 3
    // - - 0 7 5
    // - - - 0 1
    // - - - - 0

    auto * b = dynamic_cast<UIButton *>(w.getUIElement(0));
    b -> setText("Sig");
    b -> location.x = WIDTH - b -> getWidth() - 10;
    b -> location.y = HEIGHT - b -> getHeight() - 10;
    b -> setAction(reinterpret_cast<action>(showNextStep));
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

    auto * b = new UIButton(reinterpret_cast<action>(createGraph), "Ini", f);
    b -> location.x = WIDTH - b -> getWidth() - 10;
    b -> location.y = HEIGHT - b -> getHeight() - 10;
    w.pushUIElement(b);

    auto * t = new UIText(Point(10, 10), "", f);
    t -> setFontMultiplier(2);
    w.pushUIElement(t);

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
                if (!dragging)
                    for (int k = w.getCreatedUIElements() - 1; k > -1; k--) {
                        Shape * s = w.getUIElement(k);
                        if (typeid(*s) != typeid(UIButton))
                            continue;

                        auto * button = dynamic_cast<UIButton *>(w.getUIElement(k));
                        button -> triggerAction();
                    }

                if (e.button.button == SDL_BUTTON_LEFT)
                    dragging = true;

            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT)
                    dragging = false;

            } else if (e.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    int xDiff = e.motion.x - prevLocation.x;
                    int yDiff = e.motion.y - prevLocation.y;
                    for (int k = w.getCreatedShapes() - 1; k > -1; k--) {
                        w.getShape(k) -> location.x += xDiff;
                        w.getShape(k) -> location.y += yDiff;
                    }
                }

                prevLocation.x = e.button.x;
                prevLocation.y = e.button.y;

                for (int k = w.getCreatedUIElements() - 1; k > -1; k--) {
                    Shape * s = w.getUIElement(k);
                    if (typeid(*s) != typeid(UIButton))
                        continue;

                    auto * button = dynamic_cast<UIButton *>(w.getUIElement(k));
                    button -> setHover(e.motion.x, e.motion.y);
                }

            } else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
					break;
				}
			}
		}

        w.clearImage();

        // render logic
        w.renderShapes(reinterpret_cast<fun>(setPixel));

        w.updateImage();
	}

	return 0;
}
