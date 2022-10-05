#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "json/osd_config.h"
#include "util/time_util.h"

#define DATASIZE 20
#define TOP_ROW 5
#define BOTTOM_ROW 8

#include <stdbool.h>

typedef struct ToastItem
{
    char data[DATASIZE];
    struct timespec shown;
    struct ToastItem *nextNode;
} ToastItem;

ToastItem *bottomStackPointer = NULL;
ToastItem *topStackPointer = NULL;

struct timespec now, lasttoast;


int toast_pop()
{
    if (bottomStackPointer == NULL)
        return 0;

    char data[DATASIZE];
    strcpy(data, bottomStackPointer->data);
    bottomStackPointer->shown = (struct timespec){0, 0};
    ToastItem *TempPointer = bottomStackPointer;
    bottomStackPointer = bottomStackPointer->nextNode;
    free(TempPointer);
    if (bottomStackPointer == NULL)
        topStackPointer = NULL;
    return 1;
}

int toast(char *data, ...)
{
    ToastItem *TempPointer = malloc(sizeof(ToastItem));
    if (TempPointer == NULL)
        return 0;

    // printf
    va_list va;
    va_start(va, data);
    char copy[DATASIZE];
    printf("toast: %s\n", data);
    vsnprintf(copy, DATASIZE, data, va);
    printf("toast: %s\n", copy);
    // our fonts are caps only....
    for (int i = 0; copy[i]; i++)
    {
        printf("%c\n", copy[i]);
        copy[i] = toupper(copy[i]);
    }

    strncpy(TempPointer->data, copy, DATASIZE);
    TempPointer->shown = (struct timespec){0, 0};
    TempPointer->nextNode = NULL;
    if (bottomStackPointer == NULL)
        TempPointer = bottomStackPointer = TempPointer;
    else
        topStackPointer->nextNode = TempPointer;
    topStackPointer = TempPointer;
    return 1;
}

void do_toast(void (*display_print_string)(uint8_t init_x, uint8_t y, const char *string, uint8_t len))
{

    int numberoffNodes = 0;

    clock_gettime(CLOCK_MONOTONIC, &now);

    // only process/update 1ce per second
    if (timespec_subtract_ns(&now, &lasttoast) > NSEC_PER_SEC)
    {
        clock_gettime(CLOCK_MONOTONIC, &lasttoast);

        // if the last item has been up 3 seconds; chop it
        ToastItem *TempPointer = bottomStackPointer;
        if (TempPointer != NULL && TempPointer->shown.tv_sec > 0 && timespec_subtract_ns(&now, &TempPointer->shown) > ((uint8_t)NSEC_PER_SEC * 3))
        {
            TempPointer = TempPointer->nextNode;
            toast_pop();
        }

        // loop and display
        for (int row = BOTTOM_ROW; row >= TOP_ROW; row--)
        {
            // if we ran out of items blank out the rest of our rows
            if (TempPointer == NULL) {
                char empty[DATASIZE] = {0};
                display_print_string(0, row, empty, DATASIZE);
            } else {

                // set shown time if not set (ie: item not yet shown)
                if (TempPointer->shown.tv_sec == 0)
                {
                    clock_gettime(CLOCK_MONOTONIC, &TempPointer->shown);
                }

                // draw the current item in the current row
                display_print_string(0, row, TempPointer->data, DATASIZE);
                TempPointer = TempPointer->nextNode;
            }
        };
    }
    return;
}
