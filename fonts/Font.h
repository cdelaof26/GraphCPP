#ifndef GRAPH_FONT_H
#define GRAPH_FONT_H

#include <cstdio>
#include <fstream>
#include <utility>
#include "../2d.h"

// There are 128 comments in data: -- .+
// each character is 16x16

class Font {
public:
    bool data[128][16][16];
    bool fontLoaded;

    Font() : data(), fontLoaded(false) { }
};

void loadFont(Font & f);

class UIText {
private:
    Font & font;
protected:
    int fontMultiplier;

public:
    Point location;
    Color textColor;
    std::string text;

    UIText(Point _location, std::string _text, Font & _font)
        : location(_location), textColor(Color()), text(std::move(_text)),
          fontMultiplier(1), font(_font) { }

    UIText(std::string _text, Font & _font)
            : location(), textColor(Color()), text(std::move(_text)),
              fontMultiplier(1), font(_font) { }

    void setFontMultiplier(int _fontMultiplier) {
        if (_fontMultiplier < 0)
            throw std::runtime_error("Font multiplier cannot be less than 1");

        fontMultiplier = _fontMultiplier;
    }

    int getRealSize() const {
        return 16 * fontMultiplier;
    }

    void renderText(fun setPixel) {
        int padding = 0;
        for (char c : text) {
            for (int i = 0; i < getRealSize(); i++)
                for (int j = 0; j < getRealSize(); j++)
                    if (font.data[c][i / fontMultiplier][j / fontMultiplier])
                        setPixel(padding + location.x + i, location.y + j, textColor);

            padding += 16 * fontMultiplier;
        }
    }
};

#endif //GRAPH_FONT_H
