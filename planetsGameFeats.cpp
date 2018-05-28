/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include "planets.h"

#include <stdio.h>

//draws background etc
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_FONT *font, float score){
    //background
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, SCREEN_W, SCREEN_H, 0);

    //score
    al_draw_textf(font, WHITE, SCREEN_W - 150, 0, 0, "SCORE: %d", (int) score);
}

void togglePause(ALLEGRO_TIMER *timer, bool &paused){
    if (paused){
        al_start_timer(timer);
        paused = false;
    }
    else{
        al_stop_timer(timer);
        paused = true;
    }
}

void createMeteor(Meteor m[], ALLEGRO_BITMAP *image){
    int i = 0;
    while (!m[i].available && i < maxMeteors - 1){
        i++;
    }
    if (m[i].available){
        m[i].xPos = rand() % SCREEN_W;
        m[i].yPos = rand() % SCREEN_H;
        //overwrite destroyed meteor info
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].available = false;
    }
}

void destroyMeteor(Meteor m[], int i){
    m[i].available = true;
}

void gameOver(ALLEGRO_TIMER *timer){
    al_stop_timer(timer);
}
