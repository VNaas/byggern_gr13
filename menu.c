#include "menu.h"
#include "OLED_driver.h"
#include "multifunction_board_driver.h"
#include "game.h"
#define F_CPU 4915200
#include <util/delay.h>
#include <stdio.h>
#include <ctype.h>

extern int button_flag;

MenuNode *new_menu_node(char *_name, MenuNode *_parent, void (*_func)())
{
    MenuNode *menu = malloc(sizeof(MenuNode));
    if (menu == NULL)
    {
        printf("Unable to allocate enough memory for menu node %s!\r\n", _name);
        return NULL;
    }
    menu->name = _name;
    menu->parent = _parent;
    menu->func = _func;
    return menu;
}

void show_high_scores(void)
{
    return;
}

void play_game()
{
    return;
}

int font_size_menu()
{
    return 0;
}

int set_difficulty()
{
    return 0;
}

void set_low_brightness()
{
    OLED_set_brightness(20);
}

void set_medium_brightness()
{
    OLED_set_brightness(100);
}

void set_high_brightness()
{
    OLED_set_brightness(255);
}

void print_menu(MenuNode *menu)
{
    OLED_reset();
    OLED_goto_line(0);
    OLED_print(menu->name);
    for (int i = 0; i < menu->numChildren; i++)
    {
        OLED_pos(i + 1, 8);
        OLED_print(menu->children[i]->name);
    }
    if (menu->isMenu && menu->parent != NULL)
    {
        OLED_pos(menu->numChildren + 1, 8);
        OLED_print("Return");
    }
}


MenuNode *init_menu()
{
    MenuNode *mainMenuNode = new_menu_node("Main Menu", NULL, NULL);
    MenuNode *playGameNode = new_menu_node("Play Game", mainMenuNode, &play_game);
    MenuNode *settingsNode = new_menu_node("Settings", mainMenuNode, NULL);
    MenuNode *highscoresNode = new_menu_node("Display Highscores", mainMenuNode, show_high_scores);

    MenuNode *brightnessNode = new_menu_node("Brightness", settingsNode, NULL);
    MenuNode *difficultyNode = new_menu_node("Set Difficulty", settingsNode, NULL);
    MenuNode *fontSizeNode = new_menu_node("Set font size", settingsNode, NULL);

    MenuNode *lowBrightnessNode = new_menu_node("Low", brightnessNode, &set_low_brightness);
    MenuNode *mediumBrightnessNode = new_menu_node("Medium", brightnessNode, &set_low_brightness);
    MenuNode *highBrightnessNode = new_menu_node("High", brightnessNode, &set_low_brightness);

    MenuNode *lowFontSizeNode = new_menu_node("Low", fontSizeNode, &set_low_brightness);
    MenuNode *mediumFontSizeNode = new_menu_node("Medium", fontSizeNode, &set_medium_brightness);
    MenuNode *highFontSizeNode = new_menu_node("High", fontSizeNode, &set_high_brightness);

    mainMenuNode->children[PLAY_GAME] = playGameNode;
    mainMenuNode->children[SETTINGS] = settingsNode;
    mainMenuNode->children[HIGHSCORE] = highscoresNode;
    mainMenuNode->numChildren = 3;
    mainMenuNode->isMenu = 1;

    settingsNode->children[CHANGE_BRIGHTNESS] = brightnessNode;
    settingsNode->children[SET_DIFFICULTY] = difficultyNode;
    settingsNode->children[SET_FONTSIZE] = fontSizeNode;
    settingsNode->numChildren = 3;
    settingsNode->isMenu = 1;

    fontSizeNode->children[LOW] = lowFontSizeNode;
    fontSizeNode->children[MEDIUM] = mediumFontSizeNode;
    fontSizeNode->children[HIGH] = highFontSizeNode;
    fontSizeNode->numChildren = 3;

    brightnessNode->children[LOW] = lowBrightnessNode;
    brightnessNode->children[MEDIUM] = mediumBrightnessNode;
    brightnessNode->children[HIGH] = highBrightnessNode;
    brightnessNode->numChildren = 3;
    brightnessNode->isMenu = 1;
    return mainMenuNode;
}

void menu()
{
    MenuNode *mainMenu = init_menu();
    MenuNode *currentMenu = mainMenu;

    int choice = 0;
    int dir = NEUTRAL;
    int res = 0;
    OLED_reset();
    print_menu(currentMenu);
    int counter = 0;
    printf("Number of children: %d\r\n",currentMenu->numChildren);
    OLED_print_arrow(choice + 1, 0);
    printf("Printing arrow at %d\r\n",(choice+1));
    while (1)
    {
        dir = joystick_get_direction();
        button_flag = joystick_get_button_flag();
        switch (dir)
        {
        case UP:
            OLED_delete_arrow(choice + 1);
            if (currentMenu->parent == NULL)
            {
                if(choice == 0){
                    choice = currentMenu->numChildren-1;
                }
                else
                {
                    choice = (choice - 1) % (currentMenu->numChildren);
                }
                OLED_print_arrow(choice + 1, 0);
            }
            else
            {
                choice = (choice - 1) % (currentMenu->numChildren + 1);
                OLED_print_arrow(choice + 1, 0);
            }
            _delay_ms(200);
            break;
        case DOWN:
            OLED_delete_arrow(choice + 1);
            if (currentMenu->parent == NULL)
            {
                choice = (choice + 1) % (currentMenu->numChildren);
                OLED_print_arrow(choice + 1, 0);
            }
            else
            {
                choice = (choice + 1) % (currentMenu->numChildren + 1);
                OLED_print_arrow(choice + 1, 0);
            }
            _delay_ms(200);
            break;
        default:
            break;
        }

        if (button_flag)
        {
            if (choice == currentMenu->numChildren)
            { // choise is Return
                currentMenu = currentMenu->parent;
                // OLED_delete_arrow(choice + 1);
                choice = 0;
                print_menu(currentMenu);
                OLED_print_arrow(choice + 1, 0);
            }
            else if (currentMenu->children[choice]->isMenu)
            {
                currentMenu = currentMenu->children[choice];
                // OLED_delete_arrow(choice + 1);
                choice = 0;
                print_menu(currentMenu);
                OLED_print_arrow(choice + 1, 0);
            }
            else
            {
                currentMenu->children[choice]->func();
            }
            joystick_reset_button_flag();
        }
    }
}