/*Emily Yu
ICS3U P4
May 2018
Planets Project - ICS3U Summative*/

#include <allegro5/allegro.h>
#include <time.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "planets.h"

#include<stdio.h>

int main(int argc, char *argv[]){
    initializeAllegro();
    srand(time(0));

    //setup
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *q = nullptr;
    ALLEGRO_BITMAP *sprite[8] = {nullptr};
    ALLEGRO_BITMAP *mImage = nullptr;
    ALLEGRO_BITMAP *background = nullptr;
    ALLEGRO_BITMAP *planet = nullptr;
    ALLEGRO_BITMAP *box = nullptr;
    ALLEGRO_KEYBOARD_STATE kState;
    ALLEGRO_FONT *font[iFonts] = {nullptr};
    ALLEGRO_USTR *text = al_ustr_new("Player1");

    //create and load
    display = al_create_display(SCREEN_W, SCREEN_H);
    timer = al_create_timer(1.0/FPS);
    q = al_create_event_queue();
    sprite[0] = al_load_bitmap("images/characters/character1.png");
    sprite[1] = al_load_bitmap("images/characters/character2.png");
    sprite[2] = al_load_bitmap("images/characters/character3.png");
    sprite[3] = al_load_bitmap("images/characters/character4.png");
    sprite[4] = al_load_bitmap("images/characters/character5.png");
    sprite[5] = al_load_bitmap("images/characters/character6.png");
    sprite[6] = al_load_bitmap("images/characters/character7.png");
    sprite[7] = al_load_bitmap("images/characters/character8.png");
    mImage = al_load_bitmap("images/meteorV3.png");
    background = al_load_bitmap("images/background.png");
    planet = al_load_bitmap("images/planet.png");
    box = al_load_bitmap("images/box.png");
    for (int i = 0; i < iFonts; i++)
        font[i] = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 80 - 10 * i, 0);

    if (checkSetup(display, sprite, mImage, background, planet, box, timer, q, font) != 0)
        return -1;

    al_set_window_title(display, "Planet Game");

    //register event sources
	al_register_event_source(q, al_get_timer_event_source(timer));
	al_register_event_source(q, al_get_display_event_source(display));
	al_register_event_source(q, al_get_keyboard_event_source());
	al_register_event_source(q, al_get_mouse_event_source());

    //declare objects
    Planet a;
    a.r = 150;
    a.x = SCREEN_W / 2;
    a.y = SCREEN_H / 2;

    Sprite s;
    s.xPos = a.x + a.r;
    s.yPos = a.y - a.r;
    s.xVel = 0.0;
    s.yVel = 0.0;
    s.shiftX = 0.0;
    s.shiftY = 0.0;
    s.frame = 0;
    s.dir = 1;
    s.airborne = false;

    Meteor m[maxMeteors];
    for (int i = 0; i < maxMeteors; i++){
        m[i].xPos = 0;
        m[i].yPos = 0;
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].w = al_get_bitmap_width(mImage);
        m[i].h = al_get_bitmap_height(mImage);
        m[i].available = true;
    }

    //buttons
    Button btnBack;
    strcpy(btnBack.text, "Back");
    setupBtn(btnBack, font, SCREEN_H - 125);
    //START
    Button btnInstructions;
    strcpy(btnInstructions.text, "Instructions");
    setupBtn(btnInstructions, font, SCREEN_H - 230);
    //GAMEOVER
    Button btnHighscores;
    strcpy(btnHighscores.text, "Highscores");
    setupBtn(btnHighscores, font, SCREEN_H - 300);
    Button btnToMain;
    strcpy(btnToMain.text, "Main Menu");
    setupBtn(btnToMain, font, SCREEN_H - 230);
    Button btnExit;
    strcpy(btnExit.text, "Exit");
    setupBtn(btnExit, font, SCREEN_H - 160);
    //NEWHIGHSCORE
    Button btnSubmit;
    strcpy(btnSubmit.text, "Submit");
    setupBtn(btnSubmit, font, SCREEN_H - 180);
    Button btnNo;
    strcpy(btnNo.text, "No Thanks");
    setupBtn(btnNo, font, SCREEN_H - 120);

	//additional setup
    al_start_timer(timer);
    bool paused = false;
    int counter = 0; //counter used for meteor spawning, flashing text
    float score = 0.0;
    bool running = true;
    //score
    char name[10][maxNameLength] = {""};
    int highscore[10] = {0};
    //screen
    Screen prevScr = START;
    Screen currentScr = START;

    //game loop
    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        switch (currentScr){
            //start screen
            case START:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    //draw layout and screen specific items
                    drawLayout(background, box, currentScr, font, score);
                    drawStart(font, btnInstructions, btnHighscores, counter);
                    al_flip_display();
                    counter = (counter + 1) % FPS;
                }
                //button
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnInstructions, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, INSTRUCTIONS);
                    if (btnIsClicked(btnHighscores, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, HIGHSCORES);
                }
                //start game
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
                    reset(m, score);
                    counter = 0;
                    currentScr = GAME;
                }
                break;
            //instructions screen
            case INSTRUCTIONS:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawInstructions(font, btnBack);
                    al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, prevScr);
                }
                break;
            //gameplay
            case GAME:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    //spawn a meteor once a second
                    if (counter == 0)
                        createMeteor(m, a, mImage);
                    //apply gravity
                    gravity(s, m, a);

                    //movement
                    if (al_key_down(&kState, ALLEGRO_KEY_UP))
                        jump(s, a);

                    if (al_key_down(&kState, ALLEGRO_KEY_LEFT))
                        shift(s, a, LEFT);
                    else if (al_key_down(&kState, ALLEGRO_KEY_RIGHT))
                        shift(s, a, RIGHT);
                    else shift(s, a, NONE);

                    //check sprite-meteor collision
                    for (int i = 0; i < maxMeteors; i++){
                        if (isCollision(s, al_get_bitmap_width(sprite[0]) * imageScale, al_get_bitmap_height(sprite[0]) * imageScale, m[i],
                                        al_get_bitmap_width(mImage) * imageScale, al_get_bitmap_height(mImage) * imageScale) && !m[i].available){
                            //check for high score --> send to submission screen
                            if (getHighscores(display, name, highscore) == 0 && score > highscore[9]){
                                counter = 0;
                                currentScr = NEWHIGHSCORE;
                            }
                            else currentScr = GAMEOVER;
                        }
                    }

                    //update new object locations and draw
                    getNewCoordinates(s, m);
                    drawLayout(background, box, currentScr, font, score);
                    drawObjects(planet, a, s, sprite, m, mImage);
                    al_flip_display();

                    //update variables
                    counter = (counter + 2) % FPS;
                    score += 10.0 / FPS;
                }

                if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if (ev.keyboard.keycode == ALLEGRO_KEY_P){
                        printf("Pause!");
                        togglePause(timer, paused);
                    }
                }

                if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){
                    paused = false;
                    togglePause(timer, paused);
                }
                break;

            case GAMEOVER:
            //game over screen
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawGameOver(font, score, btnHighscores, btnToMain, btnExit);
                    al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnHighscores, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, HIGHSCORES);
                    if (btnIsClicked(btnToMain, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, START);
                    if (btnIsClicked(btnExit, ev.mouse.x, ev.mouse.y))
                        running = false;
                }
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                    switchScr(prevScr, currentScr, START);
                }
                break;

            case NEWHIGHSCORE:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    drawNewHighscore(font, name, highscore, score, box, text, counter, btnSubmit, btnNo);
                    al_flip_display();
                    counter = (counter + 1) % FPS;
                }
                if (ev.type == ALLEGRO_EVENT_KEY_CHAR){
                    if (ev.keyboard.keycode != ALLEGRO_KEY_ENTER && ev.keyboard.keycode != ALLEGRO_KEY_SPACE && ev.keyboard.keycode != ALLEGRO_KEY_BACKSPACE
                                    && ev.keyboard.keycode != ALLEGRO_KEY_UP && ev.keyboard.keycode != ALLEGRO_KEY_DOWN
                                    && ev.keyboard.keycode != ALLEGRO_KEY_LEFT && ev.keyboard.keycode != ALLEGRO_KEY_RIGHT
                                    && ev.keyboard.keycode != ALLEGRO_KEY_TAB)
                        al_ustr_append_chr(text, ev.keyboard.unichar);
                    //text must be 15 characters or less
                    if (al_ustr_length(text) > maxNameLength)
                        al_ustr_remove_chr(text, maxNameLength);
                    //backspace removes the last character
                    if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                        al_ustr_remove_chr(text, al_ustr_length(text) - 1);

                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        submitScore(name, highscore, al_cstr(text), score, display);
                        switchScr(prevScr, currentScr, GAMEOVER);
                    }
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnNo, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, GAMEOVER);
                    if (btnIsClicked(btnSubmit, ev.mouse.x, ev.mouse.y)){
                        submitScore(name, highscore, al_cstr(text), score, display);
                        switchScr(prevScr, currentScr, GAMEOVER);
                    }
                }
                break;

            case HIGHSCORES:
                if (ev.type == ALLEGRO_EVENT_TIMER){
                    drawLayout(background, box, currentScr, font, score);
                    //if highscores file failed to load
                    if (drawHighscores(display, font, btnBack, name, highscore) != 0)
                        switchScr(prevScr, currentScr, GAMEOVER);
                    else al_flip_display();
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if (btnIsClicked(btnBack, ev.mouse.x, ev.mouse.y))
                        switchScr(prevScr, currentScr, prevScr);
                }
                break;
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                running = false;
    }

    //destroy and release data
    for (int i = 0; i < 8; i++)
        al_destroy_bitmap(sprite[i]);
    al_destroy_display(display);
    al_destroy_timer(timer);
    for (int i = 0; i < iFonts; i++)
        al_destroy_font(font[i]);

    return 0;
}
