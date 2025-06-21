#ifndef GRAPH_UI_NODE_H
#define GRAPH_UI_NODE_H

#include <utility>

#include "2d.h"
#include "fonts/Font.h"

class UINode : public Circle {
public:
    UIText t;

    UINode(int x, int y, Color c, std::string text, Font & _f)
        : Circle(Point(x, y), c, 25), t(UIText(Point(), std::move(text), _f)) {
        t.setFontMultiplier(2);
        setBorderWidth(3);
    }

    void render(fun setPixel) override {
        Circle::render(setPixel);
        int len = t.getRealSize() / 2;
        t.location.x = (int) (center.x - len * t.text.length());
        t.location.y = center.y - len;
        t.renderText(setPixel);
    }
};

#endif //GRAPH_UI_NODE_H
