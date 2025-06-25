#ifndef GRAPH_2D_H
#define GRAPH_2D_H

#include <cmath>
#include <stdexcept>
#include <cstdio>

#define PI 3.141592

class Color {
private:
    bool rgbCalculated;
    u_int32_t rgb;

    void setChannel(int & channel, int value) {
        if (channel < 0 || channel > 255)
            throw std::runtime_error("Channel is outside the range [0, 255]");

        channel = value;
        rgbCalculated = false;
    }

protected:
    int r;
    int g;
    int b;

public:
    Color() : r(0), g(0), b(0), rgbCalculated(false), rgb(0) { }
    explicit Color(int channel) : r(channel), g(channel), b(channel), rgbCalculated(false), rgb(0) { }
    Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b), rgbCalculated(false), rgb(0) {
        if (_r < 0 || _r > 255 || _g < 0 || _g > 255 || _b < 0 || _b > 255)
            throw std::runtime_error("Init color channel outside range [0, 255]");
    }

    u_int32_t getRGB() {
        if (!rgbCalculated) {
            // rgb = ((r << 8 | g) << 8 | b) << 8 | 255;
            rgb = (r << 24) | (g << 16) | (b << 8) | 255;
            rgbCalculated = true;
        }

        return rgb;
    }

    void setRed(int red) {
        setChannel(r, red);
    }

    void setGreen(int green) {
        setChannel(g, green);
    }

    void setBlue(int blue) {
        setChannel(b, blue);
    }

    void setChannels(int value) {
        setChannel(r, value);
        setChannel(g, value);
        setChannel(b, value);
    }

    int getRed() const {
        return r;
    }

    int getGreen() const {
        return g;
    }

    int getBlue() const {
        return b;
    }
};

class Point {
public:
    int x;
    int y;

    Point() : x(0), y(0) { }
    Point(int _x, int _y) : x(_x), y(_y) { }

    Point clone() const {
        return {x, y};
    }
};

typedef const void (* fun) (int, int, Color c);

void swap(int *a, int *b);

class Shape {
protected:
    int borderWidth;

public:
    Point location;
    Color color;

    Color borderColor;

    Shape(Point _location, Color _color)
        : location(_location), color(_color),
          borderWidth(0), borderColor() {}

    explicit Shape(Point _location)
        : location(_location), color(),
          borderWidth(0), borderColor() {}

    Shape()
        : location(), color(),
          borderWidth(0), borderColor() {}

    void setBorderWidth(int _borderWidth) {
        if (_borderWidth < 0)
            throw std::runtime_error("Border width cannot be negative");
        borderWidth = _borderWidth;
    }

    virtual void render(fun setPixel) { }
};

class Rectangle: public Shape {
protected:
    int width, height;
public:
    Rectangle(Point _point, int _width, int _height) : Shape(_point), width(_width), height(_height) { }
    Rectangle(int _width, int _height) : Shape(), width(_width), height(_height) { }

    void setWidth(int _width) {
        if (_width < 0)
            throw std::runtime_error("Width cannot be negative");

        width = _width;
    }

    void setHeight(int _height) {
        if (_height < 0)
            throw std::runtime_error("Width cannot be negative");

        height = _height;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    void render(fun setPixel) override {
        for (int x1 = location.x; x1 <= location.x + width; x1++)
            for (int y1 = location.y; y1 <= location.y + height; y1++)
                setPixel(x1, y1, color);
    }
};

class Circle: public Shape {
private:
    int progress = 0;
    int multiplier = 10;
    bool ended = false;

    void setCirclePixel(int i, int j, fun setPixel) const {
	    if (ended || progress > multiplier && progress < multiplier * 2)
            setPixel(j + location.x, i + location.y, borderColor);

        if (ended || progress > multiplier * 2 && progress < multiplier * 3)
            setPixel(i + location.x, j + location.y, borderColor);

        if (ended || progress > multiplier * 3 && progress < multiplier * 4)
            setPixel(-i + location.x, j + location.y, borderColor);

        if (ended || progress > multiplier * 4 && progress < multiplier * 5)
            setPixel(-j + location.x, i + location.y, borderColor);

        if (ended || progress > multiplier * 5 && progress < multiplier * 6)
            setPixel(-j + location.x, -i + location.y, borderColor);

        if (ended || progress > multiplier * 6 && progress < multiplier * 7)
            setPixel(-i + location.x, -j + location.y, borderColor);

        if (ended || progress > multiplier * 7 && progress < multiplier * 8)
            setPixel(i + location.x, -j + location.y, borderColor);

        if (ended || progress > multiplier * 8 && progress < multiplier * 9)
            setPixel(j + location.x, -i + location.y, borderColor);
    }

public:
    int radius;

    Circle(Point _center, Color _color, int _radius)
        : Shape(_center, _color), radius(_radius), progress(0) {}

    void render(fun setPixel) override {
        // Horizontal scanline
        for (int dy = -radius; dy < radius; dy++) {
            int dx = floor(sqrt(radius * radius - dy * dy));
            for (int i = location.x - dx; i <= location.x + dx; i++)
                setPixel(i, location.y - dy, color);
        }

        int r = radius;
        for (; r < radius + borderWidth; r++) {
            if (progress < 9 * multiplier) {
                progress++;
            } else {
                ended = true;
            }

            // Midpoint circle with second degree order increments
            int i = 0, j = r, d = 1 - r;
            int dE = 3, dSE = -2 * r + 5;

            setCirclePixel(i, j, setPixel);

            while (i < j) {
                if (d < 0) {
                    d += dE;
                    dE += 2;
                    dSE += 2;
                } else {
                    d += dSE;
                    dE += 2;
                    dSE += 4;
                    j--;
                }

                i++;
                setCirclePixel(i, j, setPixel);
            }
        }
    }
};

class Line: public Shape {
private:
    int t = 0;
    const int maxT = 80;

    void setLinePixel(int x, int y, fun setPixel) const {
        if (borderWidth == 1) {
            setPixel(x, y, color);
            return;
        }

        for (int i = -semiWidth; i < semiWidth; ++i)
            for (int j = -semiWidth; j < semiWidth; ++j)
                setPixel(x + i, y + j, color);
    }

protected:
    int semiWidth {};
public:
    Point & start, & end;

    Line(Point & _start, Point & _end) : Shape(), start(_start), end(_end) {
        setWidth(4);
    }

    void setWidth(int _width) {
        if (_width < 1)
            throw std::runtime_error("Line width cannot be less than 1");

        borderWidth = _width;
        semiWidth = _width / 2;
    }

    void render(fun setPixel) override {
        if (t < maxT) {
            // v = end - start
            Point v(end.x - start.x, end.y - start.y); // Not a point, though

            // x = start.x + v.x * t
            // y = start.y + v.y * t
            for (int _t = 0; _t < t; _t++)
                setLinePixel(start.x + v.x * _t / maxT, start.y + v.y * _t / maxT, setPixel);

            t++;
            return;
        }

        Point copyStart = start.clone();
        Point copyEnd = end.clone();

        // check if line is horizontal
        if (copyStart.y == copyEnd.y) {
            if (copyStart.x > copyEnd.x)
                swap(&copyStart.x, &copyEnd.x);

            for (int i = copyStart.x; i <= copyEnd.x; i++)
                setLinePixel(i, copyStart.y, setPixel);

            return;
        }

        // check if line is vertical
        if (copyStart.x == copyEnd.x) {
            if (copyStart.y > copyEnd.y)
                swap(&copyStart.y, &copyEnd.y);

            for (int j = copyStart.y; j <= copyEnd.y; j++)
                setLinePixel(copyStart.x, j, setPixel);

            return;
        }

        if (copyStart.x > copyEnd.x) {
            swap(&copyStart.x, &copyEnd.x);
            swap(&copyStart.y, &copyEnd.y);
        }

        // check if line is diagonal with slope 1
        if (copyEnd.x - copyStart.x == copyEnd.y - copyStart.y) {
            for (int i = copyStart.x, j = copyStart.y; i <= copyEnd.x; i++, j++)
                setLinePixel(i, j, setPixel);

            return;
        }

        // check if line is diagonal with slope -1
        if (copyEnd.x - copyStart.x == copyStart.y - copyEnd.y) {
            for (int i = copyStart.x, j = copyStart.y; i <= copyEnd.x; i++, j--)
                setLinePixel(i, j, setPixel);

            return;
        }

        // Incremental line
        int dx = copyEnd.x - copyStart.x;
        int dy = copyEnd.y - copyStart.y;

        double m = dy / (double) dx;

        if (m >= -1 && m <= 1) {
            double j = copyStart.y;
            for (int i = copyStart.x; i <= copyEnd.x; i++, j += m)
                setLinePixel(i, (int) round(j), setPixel);
        } else if (m > 1) {
            m = dx / (double) dy;

            double i = copyStart.x;
            for (int j = copyStart.y; j <= copyEnd.y; j++, i += m)
                setLinePixel((int) round(i), j, setPixel);
        } else { // m < -1
            m = dx / (double) dy;
            double i = copyStart.x;
            for (int j = copyStart.y; j >= copyEnd.y; j--, i -= m)
                setLinePixel((int) round(i), j, setPixel);
        }
    }
};

void init_regular_star(Point points[], int n, int cx, int cy, int r, double iniAngle);

#endif //GRAPH_2D_H
