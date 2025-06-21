#include "Font.h"

void loadFont(Font & f) {
    std::ifstream file("fonts/font");
    if (!file.is_open())
        return;

    // Starts in -2 because of the two initial comments in font
    int character = -2;
    int i = 0, j = 0;

    bool skip = false;
    char ch;
    while (file.get(ch)) {
        if (ch == '-' && !skip) {
            skip = true;
            character++;
            j = 0;
        }

        if (skip) {
            if (ch == '\n')
                skip = false;
            continue;
        }

        if (ch == '\n') {
            i = 0;
            j++;
            continue;
        }

        f.data[character][i][j] = ch == '1';
        i++;
    }

    file.close();
    f.fontLoaded = true;
}
