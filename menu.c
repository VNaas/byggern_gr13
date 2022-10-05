#include "menu.h"
#include "OLED_driver.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 4915200
#include <util/delay.h>

char *scores[] = {'10', '23', '14', '62', '9'};
int scoreSize = 5;

struct menu_node *mainMenu;
struct menu_node *score;
struct menu_node *play;

extern int button_flag;

menu_node *new_menu_node(char *_name, menu_node *_parent, void (*fun)())
{
    menu_node *menu = malloc(sizeof(menu_node));
    if (menu == NULL)
    {
        fprint(stderr, "Unable to allocate enough memory for menu node %s!\r\n", _name);
        return NULL;
    }
    menu->name = _name;
    menu->parent = _parent;
    menu->func = fun;
    return menu;
}

int show_high_scores(void)
{
    for (int i = 0; i < scoreSize; i++)
    {
    }
}

int main_menu()
{
    enum joy_dir dir;
    *main_menu->while (1)
    {
        {
        }
    }
}

int settings_menu()
{
    return;
}

int set_high_brightness(){
    OLED_set_brightness(255);
}

int set_medium_brightness(){
    OLED_set_brightness(260);
}

int set_low_brightness(){
    OLED_set_brightness(20);
}

int brightness_menu()
{
    int choice = 0;
    int dir = NEUTRAL;
    while (1)
    {

        switch (dir)
        {
        case UP:
            choice = (choice + 1) % 3(currentTaskIndex + 1) % taskCount break;

        default:
            break;
        }
        switch (choice)
        {
        case HIGH:
            if (button_flag)
            {
                OLED_set_brightness(255);
                joystick_reset_button_flag()
                    _delay_ms(20);
            }
            break;
        case MEDIUM:
            if (button_flag)
            {
                OLED_set_brightness(160);
                joystick_reset_button_flag();
                _delay_ms(20);
            }

        default:
            break;
        }
    }
}

int set_difficulty()
{
    return 0;
}

menu_node *init__menu()
{
    menu_node *mainNode = new_menu_node("Main Menu", main_menu);
    menu_node *playGameNode = new_menu_node("Play Game", mainNode, play_game);
    menu_node *settingsNode = new_menu_node("Settings", mainNode, settings_menu);
    menu_node *highscoresNode = new_menu_node("Display Highscores", mainNode, show_high_scores);

    menu_node *brightnessNode = new_menu_node("Brightness", settingsNode, brightness_menu);
    menu_node *difficultyNode = new_menu_node("Set Difficulty", settingsNode, set_difficulty);

    mainMenu->children[SETTINGS] = settingsNode;
    mainMenu->children[CHANGE_BRIGHTNESS] = brightnessNode;
    mainMenu->children[PLAY_GAME] = playGameNode;
    mainMenu->numChildren = 3;

    settingsMenu
}
