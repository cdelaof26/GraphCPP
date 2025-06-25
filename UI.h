#ifndef GRAPH_UI_NODE_H
#define GRAPH_UI_NODE_H

#include <utility>

#include "2d.h"
#include "fonts/Font.h"

typedef const void (* action) ();

class UINode : public Circle {
public:
    UIText t;

    UINode(int x, int y, Color c, std::string text, Font & _f)
        : Circle(Point(x, y), c, 25), t(UIText(Point(), std::move(text), _f)) {
        t.setFontMultiplier(2);
        setBorderWidth(3);
    }

    UINode(Point _point, Color c, std::string text, Font & _f)
            : Circle(_point, c, 25), t(UIText(Point(), std::move(text), _f)) {
        t.setFontMultiplier(2);
        setBorderWidth(3);
    }

    void render(fun setPixel) override {
        Circle::render(setPixel);
        int len = t.getRealSize() / 2;
        t.location.x = (int) (location.x - len * t.text.length());
        t.location.y = location.y - len;
        t.render(setPixel);
    }
};

class UILine : public Line {
protected:
    int padding;
    int semiPadding;

public:
    UIText t;
    Rectangle r;

    UILine(Point & _start, Point & _end, std::string text, Font & _f)
        : Line(_start, _end), t(std::move(text), _f), r(0, 0) {
        t.color.setChannels(255);
        r.color.setChannels(0);

        t.setFontMultiplier(1);
        padding = t.getRealSize();
        semiPadding = padding / 2;
        r.setWidth((int) (t.getRealSize() * t.text.length()) + padding);
        r.setHeight(2 * padding);
    };

    void render(fun setPixel) override {
        int x = start.x + (end.x - start.x) / 2;
        int y = start.y + (end.y - start.y) / 2;
        r.location.x = x - r.getWidth() / 2;
        r.location.y = y - r.getHeight() / 2;
        t.location.x = r.location.x + semiPadding;
        t.location.y = r.location.y + semiPadding;

        Line::render(setPixel);
        r.render(setPixel);
        t.render(setPixel);
    }
};

class UIButton : public Rectangle {
protected:
    Color hoverColor;
    Color tmp;
    bool hover = false;
    bool prevStateIsHover = false;
    int padding = 0;
    int semiPadding = 0;
    action fun;
    UIText t;

public:
    UIButton(action _fun, std::string text, Font & _f)
        : Rectangle(0, 0), fun(_fun), t(UIText(_f)),
          hoverColor(200), tmp() {
        setText(std::move(text));
        color.setChannels(235);
        tmp = color;
    }

    void setHover(int x, int y) {
        hover = x >= location.x && x <= location.x + width
                && y >= location.y && y <= location.y + height;

        if (hover) {
            if (prevStateIsHover)
                return;

            prevStateIsHover = true;
            tmp = color;
            color = hoverColor;
            return;
        }

        prevStateIsHover = false;
        color = tmp;
    }

    void triggerAction() const {
        if (hover && fun != nullptr)
            this -> fun();
    }

    void setAction(action _fun) {
        fun = _fun;
    }

    void setText(std::string _text) {
        t.text = std::move(_text);
        t.setFontMultiplier(2);
        padding = t.getRealSize();
        semiPadding = padding / 2;
        width = (int) (t.getRealSize() * t.text.length()) + padding;
        height = 2 * padding;
    }

    void setTextColor(Color c) {
        t.color = c;
    }

    void render(::fun setPixel) override {
        t.location.x = location.x + semiPadding;
        t.location.y = location.y + semiPadding;
        Rectangle::render(setPixel);
        t.render(setPixel);
    }
};

#endif //GRAPH_UI_NODE_H
