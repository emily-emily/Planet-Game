#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void initializeAllegro(){
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
}

int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *sprite, ALLEGRO_BITMAP *mImage, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q, ALLEGRO_FONT *f){
    if (!display){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!sprite || !mImage){
        al_show_native_message_box(display, "Error", "Error", "Failed to load image!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	al_destroy_display(display);
    	return -1;
    }

    if (!timer) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create timer!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

    if (!q) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create event queue!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

	/*if (!f) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load font!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}*/

	return 0;
}
