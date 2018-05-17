#include<allegro5/allegro.h>
#include <time.h>
#include "planets.h"

#include<stdio.h>

int main(int argc, char *argv[]){
    initializeAllegro();
    srand(time(0));

    //setup
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    ALLEGRO_EVENT_QUEUE *q = nullptr;
    ALLEGRO_BITMAP *character = nullptr;
    ALLEGRO_KEYBOARD_STATE kState;

    display = al_create_display(SCREEN_W, SCREEN_H);
    character = al_load_bitmap("character.bmp");
    timer = al_create_timer(1.0/FPS);
    q = al_create_event_queue();

    bool running = true;
    if (checkSetup(display, character, timer, q) != 0)
        return -1;

    al_set_window_title(display, "Planet Game");

	al_register_event_source(q, al_get_timer_event_source(timer));
	al_register_event_source(q, al_get_display_event_source(display));
	al_register_event_source(q, al_get_keyboard_event_source());
	al_register_event_source(q, al_get_mouse_event_source());

    al_start_timer(timer);
    bool paused = false;

    Planet a;
    a.r = 200;
    a.x = SCREEN_W / 2;
    a.y = SCREEN_H / 2;

    Sprite s;
    s.xPos = a.x + a.r;
    s.yPos = a.y - a.r;
    s.xVel = 0;
    s.yVel = 0;

    Meteor m[maxMeteors];
    for (int i = 0; i < maxMeteors; i++){
        m[i].xPos = 0;
        m[i].yPos = 0;
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].available = true;
    }

    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER){
            createMeteor(m);
            gravity(s, a);

            if (al_key_down(&kState, ALLEGRO_KEY_SPACE))
                jump(s, a);

            if (al_key_down(&kState, ALLEGRO_KEY_LEFT))
                shift(s, a, LEFT);

            if (al_key_down(&kState, ALLEGRO_KEY_RIGHT))
                shift(s, a, RIGHT);

            getNewCoordinates(s);
            draw(a, s, character);
            al_flip_display();
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
            if (ev.keyboard.keycode == ALLEGRO_KEY_P){
                printf("Pause!");
                togglePause(timer, paused);
            }
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            //for testing
            s.xPos = ev.mouse.x;
            s.xVel = 0;
            s.yPos = ev.mouse.y;
            s.yVel = 0;
        }
    }

    al_destroy_bitmap(character);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}

