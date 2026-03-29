#include "primlib.h"
#include <stdlib.h>

#define NUM_PEGS 5
#define NUM_DISCS 8

struct Rectangle {
    int x1;
    int y1;
    int x2;
    int y2;
    enum color c;
};

void drawBackground();
void drawPegs();
void drawDiscsForPeg(int pegIndex, int numDiscsOnPeg);
void drawAll();
void calculateDiscRect(int pegIndex, int discIndexFromBottom, int discSizeLevel, struct Rectangle *rect);
void calculatePegRect(int pegIndex, struct Rectangle *rect);

int main(int argc, char* argv[])
{
    if (gfx_init()) {
        exit(3);
    }

    while (1) {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

        drawAll();

        gfx_updateScreen();

        int key = gfx_pollkey();
        if (key == SDLK_ESCAPE || key == SDLK_RETURN) {
            break;
        }
        SDL_Delay(10);
    }

    return 0;
}

void drawBackground() {
    int screenW = gfx_screenWidth();
    int screenH = gfx_screenHeight();
    int baseHeight = screenH / 20;

    gfx_filledRect(0, screenH - baseHeight, screenW - 1, screenH - 1, BLUE);
}

void calculatePegRect(int pegIndex, struct Rectangle *rect) {
    int screenW = gfx_screenWidth();
    int screenH = gfx_screenHeight();
    int pegSpacing = screenW / (NUM_PEGS + 1);
    
    int pegWidth = pegSpacing / 10;
    if (pegWidth < 2) pegWidth = 2;

    int baseHeight = screenH / 20;
    int pegX = pegSpacing * (pegIndex + 1);
    int pegHeight = (screenH * 2) / 3;
    
    rect->x1 = pegX - pegWidth / 2;
    rect->y1 = screenH - baseHeight - pegHeight;
    rect->x2 = pegX + pegWidth / 2;
    rect->y2 = screenH - baseHeight;
    rect->c = YELLOW;
}

void drawPegs() {
    for (int i = 0; i < NUM_PEGS; ++i) {
        struct Rectangle r;
        calculatePegRect(i, &r);
        gfx_filledRect(r.x1, r.y1, r.x2, r.y2, r.c);
    }
}

void calculateDiscRect(int pegIndex, int discIndexFromBottom, int discSizeLevel, struct Rectangle *rect) {
    int screenW = gfx_screenWidth();
    int screenH = gfx_screenHeight();
    
    int baseHeight = screenH / 20;
    int pegSpacing = screenW / (NUM_PEGS + 1);
    int pegX = pegSpacing * (pegIndex + 1);
    int pegHeight = (screenH * 2) / 3;
    
    int maxDiscWidth = pegSpacing - (pegSpacing / 10);
    int minDiscWidth = pegSpacing / 5;
    
    int discWidthStep = 0;
    if (NUM_DISCS > 1) {
        discWidthStep = (maxDiscWidth - minDiscWidth) / (NUM_DISCS - 1);
    }
    
    int currentDiscWidth = maxDiscWidth - discSizeLevel * discWidthStep;
    
    int discBottomY = screenH - baseHeight - (discIndexFromBottom * pegHeight) / NUM_DISCS;
    int discTopY = screenH - baseHeight - ((discIndexFromBottom + 1) * pegHeight) / NUM_DISCS;
    
    rect->x1 = pegX - currentDiscWidth / 2;
    rect->x2 = pegX + currentDiscWidth / 2;
    rect->y2 = discBottomY;
    
    int calculatedHeight = discBottomY - discTopY;
    int discGap = (calculatedHeight > 4) ? 1 : 0;
    rect->y1 = discTopY + discGap;
    
    rect->c = RED;
}

void drawDiscsForPeg(int pegIndex, int numDiscsOnPeg) {
    // Let's draw discs from largest (bottom) to smallest (top)
    // discSizeLevel: 0 is largest, NUM_DISCS - 1 is smallest
    for (int i = 0; i < numDiscsOnPeg; ++i) {
        struct Rectangle r;
        int discSizeLevel = i; 
        calculateDiscRect(pegIndex, i, discSizeLevel, &r);
        gfx_filledRect(r.x1, r.y1, r.x2, r.y2, r.c);
        gfx_rect(r.x1, r.y1, r.x2, r.y2, WHITE);
    }
}

void drawAll() {
    drawBackground();
    drawPegs();
    
    drawDiscsForPeg(0, NUM_DISCS);
}
