// GLAlgorithms.cpp
#include "GLAlgorithms.h"

#include <cmath>
#include <iostream>
#include <vector>

namespace GLAlgorithms {

void DrawLine(int x1, int y1, int x2, int y2, std::vector<float>& vertices) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        vertices.push_back(static_cast<float>(x1));
        vertices.push_back(static_cast<float>(y1));

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

}  // namespace GLAlgorithms
