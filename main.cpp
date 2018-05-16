#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>
#include "planets.h"

#include<stdio.h>

int main(int argc, char *argv[]){
    initializeAllegro();

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

    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER){
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
            printf("Pause!");
            togglePause(timer, paused);
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

void draw(Planet a, Sprite s, ALLEGRO_BITMAP *bitmap){
    al_clear_to_color(BLACK);

    //planets
    al_draw_circle(a.x, a.y, a.r, WHITE, 2);
    //gravity fields
    //al_draw_circle(a.x, a.y, a.r + 0.5 * a.r, MAGENTA, 1);

    //draw person
    al_draw_rotated_bitmap(bitmap, al_get_bitmap_width(bitmap) / 2, al_get_bitmap_height(bitmap), s.xPos, s.yPos, rotateAngle(s, a), 0);

    al_draw_line(a.x, a.y, s.xPos, s.yPos, WHITE, 1.0);
}
