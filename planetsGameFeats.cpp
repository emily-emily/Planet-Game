#include <allegro5/allegro.h>
#include"planets.h"

#include <stdio.h>

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
    while (!m[i].available && i < maxMeteors - 1){ //need to fix logic why do i need -1 smh
        i++;
    }
    if (m[i].available){
        m[i].xPos = rand() % SCREEN_W;
        m[i].yPos = rand() % SCREEN_H;
        m[i].available = false;
    }
    printf("create meteor!");
}
