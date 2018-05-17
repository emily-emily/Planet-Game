#include <allegro5/allegro.h>
#include <stdio.h>
#include"planets.h"

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

void createMeteor(Meteor m[]){
    int x = rand() % FPS + 1;
    if (x > FPS - 1)
        printf("create meteor!");
}
