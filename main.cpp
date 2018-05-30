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
    ALLEGRO_FONT *bodyFont = nullptr;
    ALLEGRO_FONT *titleFont = nullptr;

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
    bodyFont = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 20, 0);
    titleFont = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 80, 0);

    if (checkSetup(display, sprite, mImage, background, planet, box, timer, q, bodyFont, titleFont) != 0)
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

	//additional setup
    al_start_timer(timer);
    bool paused = false;
    int counter = 0; //counts loops for meteors to spawn once a second
    int iFlash = 0; //for text on start screen
    float score = 0.0;
    bool running = true;
    int highscores[10] = {0};
    Screen scr = START;

    //game loop
    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        //start screen
        if (scr == START){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                drawLayout(background, box, START, bodyFont, score);
                drawStart(titleFont, bodyFont, iFlash);
                al_flip_display();
                iFlash = (iFlash + 1) % FPS;
            }
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
                scr = GAME;
        }

        //gameplay
        if (scr == GAME){
            if (ev.type == ALLEGRO_EVENT_TIMER){
                //spawn a meteor once a second
                if (counter == 0)
                    createMeteor(m, mImage);
                //apply gravity
                gravity(s, m, a);

                //movement
                if (al_key_down(&kState, ALLEGRO_KEY_SPACE))
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
                        /*** will change ***/
                        togglePause(timer, paused);
                        //scr = GAMEOVER;
                    }
                }

                //update new object locations and draw
                getNewCoordinates(s, m);
                drawLayout(background, box, GAME, bodyFont, score);
                drawObjects(planet, a, s, sprite, m, mImage);
                al_flip_display();

                counter = (counter + 1) % FPS;
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

            /*if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                //for testing
                s.xPos = ev.mouse.x;
                s.xVel = 0;
                s.yPos = ev.mouse.y;
                s.yVel = 0;
            }*/
        }

        //game over screen
        if (scr == GAMEOVER){

        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;
    }

    //game over screen
    //getHighscores(display);

    //destroy and release data
    for (int i = 0; i < 8; i++)
        al_destroy_bitmap(sprite[i]);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(bodyFont);

    return 0;
}
