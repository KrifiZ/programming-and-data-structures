#ifndef HANOI_H
#define HANOI_H

#include "primlib.h"

#define NUM_PEGS 5
#define NUM_DISCS 8
#define ANIMATION_STEP 10

struct Rectangle {
    int x1;
    int y1;
    int x2;
    int y2;
    enum color c;
};

struct AnimatedDisc {
    int active;
    int discSize;
    int fromPeg;
    int toPeg;
    int phase;
    int currentX;
    int currentY;
    int targetYDown;
    int targetYUp;
    int targetX;
};

void drawBackground();
void drawPegs();
void drawDiscs(int gameState[NUM_PEGS][NUM_DISCS]);
void drawAll(int gameState[NUM_PEGS][NUM_DISCS], struct AnimatedDisc *anim);
void drawAnimatedDisc(struct AnimatedDisc *anim);
void calculateDiscRect(int pegIndex, int discIndexFromBottom, int discSizeLevel, struct Rectangle *rect);
void calculatePegRect(int pegIndex, struct Rectangle *rect);
int getPegCenterX(int pegIndex);
int getDiscBottomY(int discIndexFromBottom);
int getDiscTopY(int discIndexFromBottom);
int getTopDiscRow(int gameState[NUM_PEGS][NUM_DISCS], int pegIndex);
int keyToPegIndex(int key);
int checkWon(int gameState[NUM_PEGS][NUM_DISCS]);
void updateAnimation(struct AnimatedDisc *anim, int gameState[NUM_PEGS][NUM_DISCS], int *isWon);
void handleInput(int key, int gameState[NUM_PEGS][NUM_DISCS], int *sourcePeg, struct AnimatedDisc *anim);

#endif /* HANOI_H */
