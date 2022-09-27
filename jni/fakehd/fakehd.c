#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "json/osd_config.h"

#define FAKEHD_ENABLE_KEY "fakehd_enable"
#define FAKEHD_LOCK_CENTER_KEY "fakehd_lock_center"
#define FAKEHD_HIDE_THROTTLE_KEY "fakehd_hide_throttle_element"
#define FAKEHD_MENU_TRIGGER_CHAR_KEY "fakehd_menu_trigger"
#define FAKEHD_COLUMNS_KEY "fakehd_columns"
#define FAKEHD_ROWS_KEY "fakehd_rows"

#define INPUT_ROWS 16
#define INPUT_COLS 30

int fakehd_enabled = 0;
static int fakehd_hide_throttle_element = 0;
static int fakehd_lock_center = 0;
static int fakehd_menu_trigger_char = 0x04; // betaflight throttle icon
static int fakehd_trigger_x = 99;
static int fakehd_trigger_y = 99;
static char fakehd_columns = 'S';
static char fakehd_rows[INPUT_COLS] = "WWWWWWCCWWWWWWWD";

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) fprintf(stderr, fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

void load_fakehd_config()
{
    DEBUG_PRINT("checking for fakehd enabled\n");
    if (get_boolean_config_value(FAKEHD_ENABLE_KEY))
    {
        DEBUG_PRINT("fakehd enabled\n");
        fakehd_enabled = 1;
    }
    else
    {
        DEBUG_PRINT("fakehd disabled\n");
    }

    DEBUG_PRINT("checking for fakehd hide throttle \n");
    if (get_boolean_config_value(FAKEHD_HIDE_THROTTLE_KEY))
    {
        DEBUG_PRINT("fakehd hide throttle\n");
        fakehd_hide_throttle_element = 1;
    }
    else
    {
        DEBUG_PRINT("fakehd no hide throttle\n");
    }
    DEBUG_PRINT("checking for fakehd lock center \n");
    if (get_boolean_config_value(FAKEHD_LOCK_CENTER_KEY))
    {
        DEBUG_PRINT("fakehd lock center\n");
        fakehd_lock_center = 1;
    }
    else
    {
        DEBUG_PRINT("fakehd no lock center\n");
    }

    const char *trigger = get_string_config_value(FAKEHD_MENU_TRIGGER_CHAR_KEY);
    if (trigger)
    {
        fakehd_menu_trigger_char = (unsigned char)strtol(trigger, NULL, 16);
    }
    // trigger
    // rows
    const char * rows = get_string_config_value(FAKEHD_ROWS_KEY);
    if (rows) {
        memcpy(fakehd_rows, rows, INPUT_COLS);
    }

    const char * cols = get_string_config_value(FAKEHD_COLUMNS_KEY);
    if (cols)
    {
        fakehd_columns = cols[0];
    }
}

static void fakehd_get_column_config(int cols[INPUT_ROWS])
{
    switch (fakehd_columns)
    {
    case 'T':
        memcpy(cols, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, INPUT_ROWS);
        break;
    case 'M':
        memcpy(cols, (int[]){3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18}, INPUT_ROWS);
        break;
    case 'B':
        memcpy(cols, (int[]){6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21}, INPUT_ROWS);
        break;
    case 'S':
    default:
        memcpy(cols, (int[]){0, 1, 2, 3, 4, 8, 9, 10, 11, 12, 16, 17, 18, 19, 20, 21}, INPUT_ROWS);
        break;
    }

    // If more flexibility needed / when config allows - I suggest the 'default' switch block is separated
    // and used to lookup the mapping from the config file, letting the user define extras?

}

static void fakehd_get_row_config(int rownum, int row[INPUT_COLS])
{
    char rowmode = fakehd_rows[rownum];
    switch (rowmode)
    {
    case 'L':
        memcpy(row, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29}, INPUT_COLS);
        break;
    case 'C':
        memcpy(row, (int[]){15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, INPUT_COLS);
        break;
    case 'R':
        memcpy(row, (int[]){29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58}, INPUT_COLS);
        break;
    case 'T':
        memcpy(row, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58}, INPUT_COLS);
        break;
    case 'F':
        memcpy(row, (int[]){10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49}, INPUT_COLS);
        break;
    case 'D':
        memcpy(row, (int[]){11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40}, INPUT_COLS);
        break;
    case 'W':
    default:
        memcpy(row, (int[]){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58}, INPUT_COLS);
        break;
    }

    // If more flexibility needed / when config allows - I suggest the 'default' switch block is separated
    // and used to lookup the mapping from the config file, letting the user define extras?
}


// when possible, this should be called on reconnect. it will do what's needed to put fakehd back
// into fresh booted state
void fakehd_reset() {
    // clear saved centering trigger position
    int fakehd_trigger_x = 99;
    int fakehd_trigger_y = 99;
}

void fakehd_map_sd_character_map_to_hd(uint16_t **msp_character_map, uint16_t **msp_render_character_map)
{
    int row[INPUT_COLS];
    int col[INPUT_ROWS];

    fakehd_get_column_config(col);

    int render_x = 0;
    int render_y = 0;
    for (int y = INPUT_ROWS-1; y >= 0; y--)
    {
        fakehd_get_row_config(y, row);
        for (int x = INPUT_COLS-1; x >= 0; x--)
        {
            // to visualise the layout better in dev
            // if (msp_character_map[x][y] == 0) {
            //     msp_character_map[x][y] = 0x30 + (x % 100);
            // }

            // skip if it's not a character
            if (msp_character_map[x][y] != 0)
            {
                // if current element is fly min or throttle icon
                // record the current position as the 'trigger' position
                if (fakehd_trigger_x == 99 &&
                    (msp_character_map[x][y] == 0x9c // fly minutes icon (armed time)
                     ||
                     msp_character_map[x][y] == fakehd_menu_trigger_char
                     ))
                {
                    DEBUG_PRINT("found fakehd triggger \n");
                    fakehd_trigger_x = x;
                    fakehd_trigger_y = y;
                }

                // if we have seen a trigger (see above) - and it's now gone, switch to centering
                // this is intented to center the menu + postflight stats, which don't contain
                // timer/battery symbols
                if (
                    fakehd_lock_center ||
                    (fakehd_trigger_x != 99 &&
                     msp_character_map[fakehd_trigger_x][fakehd_trigger_y] != 0x9c &&
                     msp_character_map[fakehd_trigger_x][fakehd_trigger_y] != fakehd_menu_trigger_char))
                {
                    render_x = x + 15;
                    render_y = y + 3;
                }
                else
                {
                    render_y = col[y];
                    render_x = row[x];
                    // if (y >= 10)
                    // {
                    //     render_y += 6;
                    // }
                    // else if (y >= 5)
                    // {
                    //     render_y += 3;
                    // }

                    // render_x = x;
                    // // a full/unspaced couple of rows for warnings...
                    // // and the bottom row may as well be as the edges just overlap the DJI built in bits
                    // if (y == 6 || y == 7)
                    // {
                    //     render_x += 15;
                    // }
                    // else if (y == 15)
                    // {
                    //     render_x += 11;
                    // }
                    // else if (x >= 20)
                    // {
                    //     render_x += 29;
                    // }
                    // else if (x >= 10)
                    // {
                    //     render_x += 15;
                    // }
                }

                // 0 out the throttle element if configured to do so
                // and also the three adjacent positions where the thottle percent will be
                if (fakehd_trigger_x != 99 &&
                    fakehd_hide_throttle_element &&
                    msp_character_map[x][y] == fakehd_menu_trigger_char)
                {
                    msp_render_character_map[render_x][render_y] = 0;
                    (render_x <= 57) && (msp_render_character_map[render_x + 1][render_y] = 0);
                    (render_x <= 56) && (msp_render_character_map[render_x + 2][render_y] = 0);
                    (render_x <= 55) && (msp_render_character_map[render_x + 3][render_y] = 0);
                }
                else
                {
                    // otherwise, the normal path
                    msp_render_character_map[render_x][render_y] = msp_character_map[x][y];
                }
            }
        }
    }
}
