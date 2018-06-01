/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "planets.h"

#include <stdio.h>

//draws background etc
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *box, Screen scr, ALLEGRO_FONT *font, float score){
    //background
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, SCREEN_W, SCREEN_H, 0);

    //score
    if (scr == GAME)
        al_draw_textf(font, WHITE, SCREEN_W - 150, 0, 0, "SCORE: %d", (int) score);
    else
        al_draw_scaled_bitmap(box, 0, 0, al_get_bitmap_width(box), al_get_bitmap_height(box), 50, 50, SCREEN_W - 100, SCREEN_H - 100, 0);
}

void drawStart(ALLEGRO_FONT *tf, ALLEGRO_FONT *bf, int iFlash){
    al_draw_text(tf, WHITE, (SCREEN_W - al_get_text_width(tf, "GAME TITLE")) / 2, 250, 0, "GAME TITLE");
    if (iFlash > 20)
        al_draw_text(bf, WHITE, (SCREEN_W - al_get_text_width(bf, "- Press space to continue -")) / 2, SCREEN_H - 100, 0, "- Press space to continue -");
}

void drawGameOver(ALLEGRO_FONT *tf){
    al_draw_text(tf, WHITE, (SCREEN_W - al_get_text_width(tf, "GAME OVER")) / 2, 200, 0, "GAME OVER");
}

void drawHighscores(ALLEGRO_FONT *font[]){
    char name[10][10];
    int score[10] = {NULL};
    FILE *fptr;
    fptr = fopen("highscores.txt", "r");

    if (!fptr)
        printf("error");
    al_draw_text(font[3], WHITE,(SCREEN_W - al_get_text_width(font[3], "HIGHSCORES")) / 2, 100, 0, "HIGHSCORES");

    //draw boxes?
    al_draw_rectangle(100, 180, SCREEN_W / 2 - 30, SCREEN_H - 150, WHITE, 2);
    al_draw_rectangle(SCREEN_W / 2 + 30, 180, SCREEN_W - 100, SCREEN_H - 150, WHITE, 2);

    for (int i = 0; i < 10; i++){
        fscanf(fptr, "%s", name[i]);
        fscanf(fptr, "%d", &score[i]);
    }

    for (int i = 0; i < 5; i++){
        al_draw_textf(font[5], WHITE, 125, 200 + 72 * i, 0, "%s", name[i]);
        al_draw_textf(font[5], WHITE, 375, 200 + 72 * i, 0, "%d", score[i]);
    }

    for (int i = 5; i < 10; i++){
        al_draw_textf(font[5], WHITE, SCREEN_W / 2 + 55, 200 + 72 * (i - 5), 0, "%s", name[i]);
        al_draw_textf(font[5], WHITE, SCREEN_W / 2 + 305, 200 + 72 * (i - 5), 0, "%d", score[i]);
    }

    fclose(fptr);
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

void getHighscores(ALLEGRO_DISPLAY *display){
    FILE *fptr;
    fptr = fopen("highscores.txt", "r");

    if (fptr == NULL)
        al_show_native_message_box(display, "Error", "Error loading file", "Highscores file could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);

    fclose(fptr);
}
