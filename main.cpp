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

    Circle a;
    a.r = 100;
    a.x = 300;
    a.y = 250;

    Person p;
    p.xPos = 400;
    p.yPos = 350;
    p.xVel = 0;
    p.yVel = 0;

    while (running){
        ALLEGRO_EVENT ev;

        al_get_keyboard_state(&kState);
        al_wait_for_event(q, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER){
            gravity(p, a);

            if (al_key_down(&kState, ALLEGRO_KEY_SPACE))
                jump(p, a);

            getNewCoordinates(p);
            draw(a, p, character);
            al_flip_display();
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            running = false;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
            //jump(p, a);
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            p.xPos = ev.mouse.x;
            p.xVel = 0;
            p.yPos = ev.mouse.y;
            p.yVel = 0;
        }

    }

    al_destroy_bitmap(character);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}

void draw(Circle c, Person p, ALLEGRO_BITMAP *bitmap){
    al_clear_to_color(BLACK);

    //planets
    al_draw_circle(c.x, c.y, c.r, WHITE, 2);
    //gravity fields
    al_draw_circle(c.x, c.y, c.r + 0.5 * c.r, MAGENTA, 1);

    //draw person
    al_draw_rotated_bitmap(bitmap, al_get_bitmap_width(bitmap) / 2, al_get_bitmap_height(bitmap), p.xPos, p.yPos, rotateAngle(p, c), 0);

    al_draw_line(c.x, c.y, p.xPos, p.yPos, WHITE, 1.0);
}
