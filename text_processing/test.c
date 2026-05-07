#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static char* getLine(int* err);
static int   isOctalDigit(int c);
static int   addInto(char** sumBuf, size_t* sumLen, size_t* sumCap, const char* b);
static void  freeNumbers(char** arr, size_t count);

int main(void)
{
    char** numbers = NULL;
    size_t count = 0;
    size_t cap = 0;
    int errCode = 0;

    for (;;) {
        int err = 0;
        char* line = getLine(&err);
        if (err) { errCode = 2; break; }
        if (line == NULL) break;

        int valid = (line[0] != '\0');
        for (size_t i = 0; line[i] != '\0'; i++) {
            if (!isOctalDigit((unsigned char)line[i])) { valid = 0; break; }
        }
        if (!valid) {
            free(line);
            errCode = 1;
            break;
        }

        if (count == cap) {
            size_t newCap = cap == 0 ? 8 : cap * 2;
            char** tmp = realloc(numbers, newCap * sizeof(*tmp));
            if (tmp == NULL) {
                free(line);
                errCode = 2;
                break;
            }
            numbers = tmp;
            cap = newCap;
        }
        numbers[count++] = line;
    }

    if (errCode == 1) {
        fprintf(stderr, "Error: invalid input format\n");
        freeNumbers(numbers, count);
        return 1;
    }
    if (errCode == 2) {
        fprintf(stderr, "Error: memory allocation failure\n");
        freeNumbers(numbers, count);
        return 1;
    }

    char*  sumBuf = NULL;
    size_t sumLen = 0;
    size_t sumCap = 0;

    for (size_t i = 0; i < count; i++) {
        if (!addInto(&sumBuf, &sumLen, &sumCap, numbers[i])) {
            fprintf(stderr, "Error: memory allocation failure\n");
            free(sumBuf);
            freeNumbers(numbers, count);
            return 1;
        }
    }

    if (sumLen == 0) {
        if (sumCap < 2) {
            char* tmp = realloc(sumBuf, 2);
            if (tmp == NULL) {
                fprintf(stderr, "Error: memory allocation failure\n");
                free(sumBuf);
                freeNumbers(numbers, count);
                return 1;
            }
            sumBuf = tmp;
            sumCap = 2;
        }
        sumBuf[0] = '0';
        sumLen = 1;
    }

    while (sumLen > 1 && sumBuf[sumLen - 1] == '0') sumLen--;

    for (size_t i = 0; i < sumLen / 2; i++) {
        char t = sumBuf[i];
        sumBuf[i] = sumBuf[sumLen - 1 - i];
        sumBuf[sumLen - 1 - i] = t;
    }
    sumBuf[sumLen] = '\0';

    printf("Sum:\n%s\n\n", sumBuf);
    printf("Input numbers:\n");
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", numbers[i]);
    }

    free(sumBuf);
    freeNumbers(numbers, count);
    return 0;
}

static int isOctalDigit(int c)
{
    return c >= '0' && c <= '7';
}

static void freeNumbers(char** arr, size_t count)
{
    if (arr == NULL) return;
    for (size_t i = 0; i < count; i++) free(arr[i]);
    free(arr);
}

static char* getLine(int* err)
{
    *err = 0;
    int c = getchar();
    while (c != EOF && isspace(c)) c = getchar();
    if (c == EOF) return NULL;

    char* text = NULL;
    size_t len = 0;
    size_t cap = 0;
    while (c != EOF && !isspace(c)) {
        if (len + 1 >= cap) { /* +1 na '\0' */
            size_t newCap = cap == 0 ? 1024 : cap * 2;
            char* tmp = realloc(text, newCap);
            if (tmp == NULL) {
                free(text);
                *err = 1;
                return NULL;
            }
            text = tmp;
            cap = newCap;
        }
        text[len++] = (char)c;
        text[len] = '\0';
        c = getchar();
    }
    return text;
}

static int addInto(char** sumBuf, size_t* sumLen, size_t* sumCap, const char* b)
{
    size_t lb = strlen(b);
    size_t maxLen = (*sumLen > lb) ? *sumLen : lb;
    size_t needed = maxLen + 1; 

    if (*sumCap < needed) {
        size_t newCap = (*sumCap == 0) ? 16 : *sumCap;
        while (newCap < needed) newCap *= 2;
        char* tmp = realloc(*sumBuf, newCap);
        if (tmp == NULL) return 0;
        *sumBuf = tmp;
        *sumCap = newCap;
    }

    int carry = 0;
    size_t i;
    for (i = 0; i < maxLen; i++) {
        int da = (i < *sumLen) ? ((*sumBuf)[i] - '0') : 0;
        int db = (i < lb)      ? (b[lb - 1 - i] - '0') : 0;
        int s  = da + db + carry;
        (*sumBuf)[i] = (char)('0' + (s % 8));
        carry = s / 8;
    }
    if (carry) {
        (*sumBuf)[i++] = (char)('0' + carry);
    }
    *sumLen = i;
    return 1;
}
