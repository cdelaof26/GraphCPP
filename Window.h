#ifndef GRAPH_SDL_RENDER_WINDOW_H
#define GRAPH_SDL_RENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <unistd.h>
#include <string>
#include <utility>
#include "2d.h"

#define MAX_UI_ELEMENTS 500

class Window {
private:
    int width, height;
    std::string name;
    int sleepTime;

    SDL_Surface * surface;
    SDL_Texture * texture;
    SDL_Renderer * renderer;
    SDL_Window * window;
    bool initialized;

    Shape * shapes[MAX_UI_ELEMENTS] = {nullptr};
    int createdShapes = 0;
    Shape * uiElements[MAX_UI_ELEMENTS] = {nullptr};
    int createdUIElements = 0;
public:
    Color background {};

    Window(int _width, int _height, std::string _name, int f)
            : width(_width),
              height(_height),
              name(std::move(_name)),
              sleepTime(0),

              surface(nullptr),
              texture(nullptr),
              renderer(nullptr),
              window(nullptr),
              initialized(false) {
        setTargetFramerate(f);
        initSDL();
    }

    ~Window() {
        // Release resources
        if (surface)
            SDL_FreeSurface(surface);
        if (texture)
            SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void initSDL() {
        // Init SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("Error starting SDL: %s\n", SDL_GetError());
            return;
        }

        // Create window
        window = SDL_CreateWindow(
                name.c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height, SDL_WINDOW_SHOWN
        );
        if (!window) {
            printf("Error creating window: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }

        // Create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            printf("Error creating renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }

        initialized = true;
    }

    bool isInitialized() const {
        return initialized;
    }

    void clearImage() {
        if (surface)
            SDL_FreeSurface(surface);
        surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

        // Clean the screen
        SDL_SetRenderDrawColor(renderer, background.getRed(), background.getGreen(), background.getBlue(), 255);
        SDL_RenderClear(renderer);
    }

    void updateImage() {
        if (texture)
            SDL_DestroyTexture(texture);

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    void setTargetFramerate(int framerate) {
        sleepTime = 1000000 / framerate;
    }

    int getSleepMicroseconds() const {
        return sleepTime;
    }

    void setPixel(int i, int j, Color c) {
        if (i < 0 || j < 0 || i >= width || j >= height)
            return;

        // j = height - j - 1;
        ((u_int32_t *) surface -> pixels)[j * width + i] = c.getRGB();
    }

    void renderShapes(fun setPixel) const {
        for (int k = createdShapes - 1; k > -1; k--)
            shapes[k] -> render(setPixel);

        for (int k = createdUIElements - 1; k > -1; k--)
            uiElements[k] -> render(setPixel);
    }

    void pushShape(Shape * s) {
        if (createdShapes > MAX_UI_ELEMENTS)
            throw std::runtime_error("Cannot save more Shapes");

        shapes[createdShapes++] = s;
    }

    void pushUIElement(Shape * s) {
        if (createdUIElements > MAX_UI_ELEMENTS)
            throw std::runtime_error("Cannot save more UI elements");

        uiElements[createdUIElements++] = s;
    }

    void deleteFirstUIElement() {
        if (createdUIElements < 1)
            return;

        delete uiElements[0];
        for (int i = 0; i < createdUIElements - 1; i++)
            uiElements[i] = uiElements[i + 1];
        createdUIElements--;
    }

    Shape * getShape(int index) const {
        return shapes[index];
    };

    int getCreatedShapes() const {
        return createdShapes;
    }

    Shape * getUIElement(int index) const {
        return uiElements[index];
    };

    int getCreatedUIElements() const {
        return createdUIElements;
    }
};

#endif // GRAPH_SDL_RENDER_WINDOW_H
