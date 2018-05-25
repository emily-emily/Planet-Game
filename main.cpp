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
    ALLEGRO_BITMAP *sprite = nullptr;
    ALLEGRO_BITMAP *mImage = nullptr;
    ALLEGRO_KEYBOARD_STATE kState;
    ALLEGRO_FONT *font = nullptr;

    //create and load
    display = al_create_display(SCREEN_W, SCREEN_H);
    timer = al_create_timer(1.0/FPS);
    q = al_create_event_queue();
    sprite = al_load_bitmap("images/character.bmp");
    mImage = al_load_bitmap("images/meteor.png");
    font = al_load_ttf_font("font-Sansation/Sansation-Regular.ttf", 20, 0);

    bool running = true;
    if (checkSetup(display, sprite, mImage, timer, q, font) != 0)
        return -1;

    al_set_window_title(display, "Planet Game");
/*******************************************************************************************************/
    al_convert_mask_to_alpha(sprite, MAGENTA);

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
    float score = 0.0;

    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

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
                if (isCollision(s, al_get_bitmap_width(sprite), al_get_bitmap_height(sprite), m[i]) && !m[i].available){
                    /*** will change ***/
                    togglePause(timer, paused);
                    //gameOver(timer);
                }
            }

            //update new object locations and draw
            getNewCoordinates(s, m);
            drawLayout(font, score);
            drawObjects(a, s, sprite, m, mImage);
            al_flip_display();

            counter = (counter + 1) % FPS;
            score += 1.0 / FPS;
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

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

    //destroy and release data
    al_destroy_bitmap(sprite);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(font);

    return 0;
}
