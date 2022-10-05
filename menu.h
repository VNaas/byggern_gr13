#ifndef MENU_H
#define MENU_H

#include "multifunction_board_driver.h"

typedef int (*menu_function)(void);
typedef struct
{
    char *name;
    menu_function func;
    struct node *parent;
    struct node *children[8];
    int numChildren;
} menu_node;

typedef enum
{
    PLAY_GAME,
    HIGHSCORE,
    SETTINGS,
} main_menu_t;

typedef enum
{
    CHANGE_BRIGHTNESS,
    SET_DIFFICULTY,
    RETURN
} settings_menu_t;

typedef enum
{
    HIGH,
    MEDIUM,
    LOW,
    RETURN
} brightness_menu_t;

typedef enum
{
    HIGH,
    MEDIUM,
    LOW,
    RETURN
} difficulty_menu_t;

void menu();

menu_node *new_menu_node(char *_name, menu_node *_parent, void (*fun)());

/**
 * @brief Initializes main menu and all sub-menus and returns pointer to
 * main menu
 * 
 * @return menu_node* 
 */
menu_node* init_menu();

#endif /* MENU_H */