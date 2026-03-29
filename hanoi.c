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
void drawDiscs(int gameState[NUM_PEGS][NUM_DISCS]);
void drawAll(int gameState[NUM_PEGS][NUM_DISCS]);
void calculateDiscRect(int pegIndex, int discIndexFromBottom, int discSizeLevel, struct Rectangle *rect);
void calculatePegRect(int pegIndex, struct Rectangle *rect);
int getTopDiscRow(int gameState[NUM_PEGS][NUM_DISCS], int pegIndex);
int keyToPegIndex(int key);
int checkWon(int gameState[NUM_PEGS][NUM_DISCS]);

int main(int argc, char* argv[])
{
    if (gfx_init()) {
        exit(3);
    }

    int gameState[NUM_PEGS][NUM_DISCS] = {0};
    for (int i = 0; i < NUM_DISCS; ++i) {
        gameState[0][i] = NUM_DISCS - i;
    }
    int sourcePeg = -1;
    int isWon = 0;

    while (1) {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

        drawAll(gameState);
        
        if (isWon) {
            int textX = gfx_screenWidth() / 2 - 60;
            int textY = gfx_screenHeight() / 6;
            gfx_textout(textX, textY, "Congratulation", YELLOW);

        }

        gfx_updateScreen();

        int key = gfx_pollkey();
        if (key == SDLK_ESCAPE || key == SDLK_RETURN) {
            break;
        }
        
        if (!isWon) {
            int pegClicked = keyToPegIndex(key);
            if (pegClicked != -1) {
                if (sourcePeg == -1) {
                    if (getTopDiscRow(gameState, pegClicked) != -1) {
                        sourcePeg = pegClicked;
                    }
                } else {
                    int targetPeg = pegClicked;
                    if (sourcePeg != targetPeg) {
                        int srcRow = getTopDiscRow(gameState, sourcePeg);
                        int dstRow = getTopDiscRow(gameState, targetPeg);
                        
                        int srcSize = gameState[sourcePeg][srcRow];
                        int dstSize = (dstRow == -1) ? (NUM_DISCS + 1) : gameState[targetPeg][dstRow];
                        
                        if (srcSize < dstSize) {
                            gameState[sourcePeg][srcRow] = 0;
                            gameState[targetPeg][dstRow + 1] = srcSize;
                        }
                    }
                    sourcePeg = -1;
                }
            }
            
            isWon = checkWon(gameState);
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

void drawDiscs(int gameState[NUM_PEGS][NUM_DISCS]) {
    for (int p = 0; p < NUM_PEGS; ++p) {
        for (int i = 0; i < NUM_DISCS; ++i) {
            int discValue = gameState[p][i];
            if (discValue == 0) continue;
            
            struct Rectangle r;
            int discSizeLevel = NUM_DISCS - discValue; 
            calculateDiscRect(p, i, discSizeLevel, &r);
            gfx_filledRect(r.x1, r.y1, r.x2, r.y2, r.c);
            gfx_rect(r.x1, r.y1, r.x2, r.y2, WHITE);
        }
    }
}

void drawAll(int gameState[NUM_PEGS][NUM_DISCS]) {
    drawBackground();
    drawPegs();
    drawDiscs(gameState);
}

int getTopDiscRow(int gameState[NUM_PEGS][NUM_DISCS], int pegIndex) {
    for (int i = NUM_DISCS - 1; i >= 0; --i) {
        if (gameState[pegIndex][i] != 0) {
            return i;
        }
    }
    return -1;
}

int keyToPegIndex(int key) {
    if (key >= SDLK_1 && key <= SDLK_9) {
        int idx = key - SDLK_1;
        if (idx < NUM_PEGS) return idx;
    } else if (key == SDLK_0) {
        if (9 < NUM_PEGS) return 9;
    }
    return -1;
}

int checkWon(int gameState[NUM_PEGS][NUM_DISCS]) {
    for (int p = 1; p < NUM_PEGS; ++p) {
        if (gameState[p][NUM_DISCS - 1] != 0) {
            return 1;
        }
    }
    return 0;
}
