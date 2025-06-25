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

class UIText: public Shape {
private:
    Font & font;

protected:
    int fontMultiplier;

public:
    std::string text;

    UIText(Point _location, std::string _text, Font & _font)
        : Shape(_location), text(std::move(_text)),
          fontMultiplier(1), font(_font) { }

    UIText(std::string _text, Font & _font)
        : Shape(), text(std::move(_text)),
          fontMultiplier(1), font(_font) { }

    explicit UIText(Font & _font) : Shape(), text(), fontMultiplier(1), font(_font) { }

    void setFontMultiplier(int _fontMultiplier) {
        if (_fontMultiplier < 0)
            throw std::runtime_error("Font multiplier cannot be less than 1");

        fontMultiplier = _fontMultiplier;
    }

    int getFontRealPointSize() const {
        return 16 * fontMultiplier;
    }

    int getWidth() const {
        return getFontRealPointSize() * (int) text.length();
    }

    void render(fun setPixel) override {
        int padding = 0;
        int realSize = getFontRealPointSize();
        for (char c : text) {
            for (int i = 0; i < getFontRealPointSize(); i++)
                for (int j = 0; j < getFontRealPointSize(); j++)
                    if (font.data[c][i / fontMultiplier][j / fontMultiplier])
                        setPixel(padding + location.x + i, location.y + j, color);

            padding += realSize;
        }
    }
};

#endif //GRAPH_FONT_H
