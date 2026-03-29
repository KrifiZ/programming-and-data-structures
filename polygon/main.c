
#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define NUM_VERTICES 5
#define ROTATION_STEP 0.03

void clear_screen() {
    gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
}

void draw_polygon(double radius, double start_angle) {
    int center_x = gfx_screenWidth() / 2;
    int center_y = gfx_screenHeight() / 2;

    for (int i = 0; i < NUM_VERTICES; i++) {
        double current_angle = start_angle + (i * 2.0 * M_PI / NUM_VERTICES);
        double next_angle = start_angle + ((i + 1) * 2.0 * M_PI / NUM_VERTICES);

        int start_x = center_x + (int)(radius * cos(current_angle));
        int start_y = center_y + (int)(radius * sin(current_angle));
        int end_x = center_x + (int)(radius * cos(next_angle));
        int end_y = center_y + (int)(radius * sin(next_angle));

        gfx_line(start_x, start_y, end_x, end_y, YELLOW);
    }
}

double rotate_polygon(double angle) {
    angle += ROTATION_STEP;
    if (angle >= 2.0 * M_PI) {
        angle -= 2.0 * M_PI;
    }
    return angle;
}

void scale_polygon(double *radius, double *radius_change) {
    *radius += *radius_change;
    if (*radius >= 200.0) {
        *radius = 200.0;
        *radius_change = -*radius_change;
    } else if (*radius <= 10.0) {
        *radius = 10.0;
        *radius_change = -*radius_change;
    }
}

int main(int argc, char* argv[]) {
    if (gfx_init()) {
        exit(3);
    }

    double current_angle = 0.0;
    double current_radius = 10.0;
    double radius_change = 1.0;

    while (gfx_pollkey() == -1) {
        clear_screen();
        
        draw_polygon(current_radius, current_angle);

        current_angle = rotate_polygon(current_angle);

        scale_polygon(&current_radius, &radius_change);

        gfx_updateScreen();
        SDL_Delay(10);
    }

    return 0;
}

