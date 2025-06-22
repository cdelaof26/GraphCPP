#include "2d.h"

void swap(int *a, int *b){
    int aux = *a;
    *a = *b;
    *b = aux;
}

void init_regular_star(Point points[], int n, int cx, int cy, int r, double iniAngle) {
    while (iniAngle > 360)
        iniAngle -= 360;

    double angl = iniAngle * PI / 180.0;
    double dAngl = (4 * PI) / n;

    if (n % 2 == 0) {
        int nHalf = n / 2;
        int i = 0;
        for (; i < nHalf; i++, angl += dAngl) {
            points[i].x = (int) round(cx + r * cos(angl));
            points[i].y = (int) round(cy + r * sin(angl));
        }

        angl += dAngl / 2;
        for (; i < n; i++, angl += dAngl) {
            points[i].x = (int) round(cx + r * cos(angl));
            points[i].y = (int) round(cy + r * sin(angl));
        }

        return;
    }

    for (int i = 0; i < n; i++, angl += dAngl) {
        points[i].x = (int) round(cx + r * cos(angl));
        points[i].y = (int) round(cy + r * sin(angl));
        // printf("%d: %d %d %f\n", i, points[i][0], points[i][1], angl); // Debug
    }
}
