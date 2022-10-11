#ifndef MENU_H
#define MENU_H


typedef void (*menu_function)(void);
typedef struct MenuNode
{
    char *name;
    menu_function func;
    struct MenuNode *parent;
    struct MenuNode *children[6];
    int numChildren;
    int isMenu;
}MenuNode;

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
    SET_FONTSIZE,
    SETTINGS_RETURN
} settings_menu_t;

typedef enum
{
    LOW,
    MEDIUM,
    HIGH,
    LEVEL_RETURN
} level_menu_t;

void menu();

MenuNode *new_menu_node(char *_name, MenuNode *_parent, void (*_func)());

/**
 * @brief Initializes main menu and all sub-menus and returns pointer to
 * main menu
 *
 * @return MenuNode*
 */
MenuNode *init_menu(void);
int brightness_menu(void);


#endif /* MENU_H */