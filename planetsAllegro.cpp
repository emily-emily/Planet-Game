/*Emily Yu
ICS3U P4
May-June 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "planets.h"

void initializeAllegro(){
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();
}

int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *sprite[], ALLEGRO_BITMAP *mImage, ALLEGRO_BITMAP *background,
               ALLEGRO_BITMAP *planet, ALLEGRO_BITMAP *box, ALLEGRO_BITMAP *icon, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q, ALLEGRO_FONT *font[],
               ALLEGRO_SAMPLE *music[], ALLEGRO_SAMPLE *SFX){
    if (!display){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    //images
    for (int i = 0; i < 8; i++){
        if (!sprite[i] || !mImage || !background || !planet || !box || !icon){
            al_show_native_message_box(display, "Error", "Error", "Failed to load image!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return -1;
        }
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
	for (int i = 0; i < 3; i++)
        if (!font[i]) {
            al_show_native_message_box(display, "Error", "Error", "Failed to load font!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return -1;
        }
    for (int i = 0; i < numMusTracks; i++)
    if (!music[i]) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load soundtrack!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}
    if (!SFX) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load soundtrack!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

	return 0;
}
