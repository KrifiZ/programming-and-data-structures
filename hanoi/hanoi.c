#include "hanoi.h"
#include <stdlib.h>

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
    
    struct AnimatedDisc anim = {0};

    while (1) {
        gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

        drawAll(gameState, &anim);
        
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
            if (anim.active) {
                updateAnimation(&anim, gameState, &isWon);
            } else {
                handleInput(key, gameState, &sourcePeg, &anim);
            }
        }

        SDL_Delay(10);
    }

    return 0;
}

void updateAnimation(struct AnimatedDisc *anim, int gameState[NUM_PEGS][NUM_DISCS], int *isWon) {
    if (anim->phase == 0) {
        anim->currentY -= ANIMATION_STEP;
        if (anim->currentY <= anim->targetYUp) {
            anim->currentY = anim->targetYUp;
            anim->phase = 1;
        }
    } else if (anim->phase == 1) {
        if (anim->currentX < anim->targetX) {
            anim->currentX += ANIMATION_STEP;
            if (anim->currentX >= anim->targetX) {
                anim->currentX = anim->targetX;
                anim->phase = 2;
            }
        } else if (anim->currentX > anim->targetX) {
            anim->currentX -= ANIMATION_STEP;
            if (anim->currentX <= anim->targetX) {
                anim->currentX = anim->targetX;
                anim->phase = 2;
            }
        } else {
            anim->phase = 2;
        }
    } else if (anim->phase == 2) {
        anim->currentY += ANIMATION_STEP;
        if (anim->currentY >= anim->targetYDown) {
            anim->currentY = anim->targetYDown;
            int dstRow = getTopDiscRow(gameState, anim->toPeg);
            gameState[anim->toPeg][dstRow + 1] = anim->discSize;
            anim->active = 0;
            *isWon = checkWon(gameState);
        }
    }
}

void handleInput(int key, int gameState[NUM_PEGS][NUM_DISCS], int *sourcePeg, struct AnimatedDisc *anim) {
    int pegClicked = keyToPegIndex(key);
    if (pegClicked != -1) {
        if (*sourcePeg == -1) {
            if (getTopDiscRow(gameState, pegClicked) != -1) {
                *sourcePeg = pegClicked;
            }
        } else {
            int targetPeg = pegClicked;
            if (*sourcePeg != targetPeg) {
                int srcRow = getTopDiscRow(gameState, *sourcePeg);
                int dstRow = getTopDiscRow(gameState, targetPeg);
                
                int srcSize = gameState[*sourcePeg][srcRow];
                int dstSize = (dstRow == -1) ? (NUM_DISCS + 1) : gameState[targetPeg][dstRow];
                
                if (srcSize < dstSize) {
                    gameState[*sourcePeg][srcRow] = 0;
                    
                    anim->active = 1;
                    anim->discSize = srcSize;
                    anim->fromPeg = *sourcePeg;
                    anim->toPeg = targetPeg;
                    anim->phase = 0;
                    anim->currentX = getPegCenterX(*sourcePeg);
                    anim->currentY = getDiscBottomY(srcRow);
                    anim->targetYDown = getDiscBottomY(dstRow + 1);
                    int screenH = gfx_screenHeight();
                    int pegHeight = (screenH * 2) / 3;
                    int baseHeight = screenH / 20;
                    anim->targetYUp = screenH - baseHeight - pegHeight - 30;
                    anim->targetX = getPegCenterX(targetPeg);
                }
            }
            *sourcePeg = -1;
        }
    }
}

void drawBackground() {
    int screenW = gfx_screenWidth();
    int screenH = gfx_screenHeight();
    int baseHeight = screenH / 20;

    gfx_filledRect(0, screenH - baseHeight, screenW - 1, screenH - 1, BLUE);
}

int getPegCenterX(int pegIndex) {
    int screenW = gfx_screenWidth();
    int pegSpacing = screenW / (NUM_PEGS + 1);
    return pegSpacing * (pegIndex + 1);
}

int getDiscBottomY(int discIndexFromBottom) {
    int screenH = gfx_screenHeight();
    int baseHeight = screenH / 20;
    int pegHeight = (screenH * 2) / 3;
    return screenH - baseHeight - (discIndexFromBottom * pegHeight) / NUM_DISCS;
}

int getDiscTopY(int discIndexFromBottom) {
    int screenH = gfx_screenHeight();
    int baseHeight = screenH / 20;
    int pegHeight = (screenH * 2) / 3;
    return screenH - baseHeight - ((discIndexFromBottom + 1) * pegHeight) / NUM_DISCS;
}

void calculatePegRect(int pegIndex, struct Rectangle *rect) {
    int screenH = gfx_screenHeight();
    int screenW = gfx_screenWidth();
    int pegSpacing = screenW / (NUM_PEGS + 1);
    
    int pegWidth = pegSpacing / 10;
    if (pegWidth < 2) pegWidth = 2;

    int baseHeight = screenH / 20;
    int pegX = getPegCenterX(pegIndex);
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
    
    int pegSpacing = screenW / (NUM_PEGS + 1);
    int pegX = getPegCenterX(pegIndex);
    
    int maxDiscWidth = pegSpacing - (pegSpacing / 10);
    int minDiscWidth = pegSpacing / 5;
    
    int discWidthStep = 0;
    if (NUM_DISCS > 1) {
        discWidthStep = (maxDiscWidth - minDiscWidth) / (NUM_DISCS - 1);
    }
    
    int currentDiscWidth = maxDiscWidth - discSizeLevel * discWidthStep;
    
    int discBottomY = getDiscBottomY(discIndexFromBottom);
    int discTopY = getDiscTopY(discIndexFromBottom);
    
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

void drawAnimatedDisc(struct AnimatedDisc *anim) {
    if (!anim->active) return;
    
    int discSizeLevel = NUM_DISCS - anim->discSize;
    int screenW = gfx_screenWidth();
    int pegSpacing = screenW / (NUM_PEGS + 1);
    int maxDiscWidth = pegSpacing - (pegSpacing / 10);
    int minDiscWidth = pegSpacing / 5;
    
    int discWidthStep = 0;
    if (NUM_DISCS > 1) {
        discWidthStep = (maxDiscWidth - minDiscWidth) / (NUM_DISCS - 1);
    }
    
    int currentDiscWidth = maxDiscWidth - discSizeLevel * discWidthStep;
    
    int discBottomY = getDiscBottomY(0);
    int discTopY = getDiscTopY(0);
    int calculatedHeight = discBottomY - discTopY;
    int discGap = (calculatedHeight > 4) ? 1 : 0;
    
    int y2 = anim->currentY;
    int y1 = anim->currentY - calculatedHeight + discGap;
    int x1 = anim->currentX - currentDiscWidth / 2;
    int x2 = anim->currentX + currentDiscWidth / 2;
    
    gfx_filledRect(x1, y1, x2, y2, RED);
    gfx_rect(x1, y1, x2, y2, WHITE);
}

void drawAll(int gameState[NUM_PEGS][NUM_DISCS], struct AnimatedDisc *anim) {
    drawBackground();
    drawPegs();
    drawDiscs(gameState);
    if (anim != NULL) {
        drawAnimatedDisc(anim);
    }
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
