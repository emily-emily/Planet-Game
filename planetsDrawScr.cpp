/*Emily Yu
ICS3U P4
May-June 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include "planets.h"

//switch screens and play appropriate music
void switchScr(Screen &prevScr, Screen &currentScr, ALLEGRO_SAMPLE *tracks[], bool music, int volume, int &track, Screen newScr){
    prevScr = currentScr;
    currentScr = newScr;

    //audio
    if (music){
        switch (newScr){
            case START:
            case INSTRUCTIONS:
            case SETTINGS:
            case HIGHSCORES:
            case CREDITS:
            case GAMEOVER:
                if (track != 0){
                    al_stop_samples();
                    al_play_sample(tracks[0], (float) volume / 100, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
                    track = 0;
                }
                break;
            case GAME:
                al_stop_samples();
                al_play_sample(tracks[1], (float) volume / 100, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
                track = 1;
                break;
            case NEWHIGHSCORE:
                al_stop_samples();
                al_play_sample(tracks[2], (float) volume / 100, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                track = 2;
        }
        if (newScr == START && prevScr == SETTINGS){
            al_stop_samples();
            al_play_sample(tracks[0], (float) volume / 100, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            track = 0;
        }
    }
    else{
        al_stop_samples();
        track = -1;
    }
}

//draws background etc
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *box, Screen scr, ALLEGRO_FONT *f[], float score){
    //background
    al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, SCREEN_W, SCREEN_H, 0);

    //score
    if (scr == GAME)
        al_draw_textf(f[6], WHITE, SCREEN_W - 150, 0, 0, "SCORE: %d", (int) score);
    else
        al_draw_scaled_bitmap(box, 0, 0, al_get_bitmap_width(box), al_get_bitmap_height(box), 50, 50, SCREEN_W - 100, SCREEN_H - 100, 0);
}

void drawStart(ALLEGRO_FONT *f[], Button settings, ALLEGRO_BITMAP *icon, Button btn1, Button btn2, Button btn3, int iFlash){
    al_draw_text(f[0], WHITE, SCREEN_W / 2, 190, ALLEGRO_ALIGN_CENTER, "THE PLANET GAME");
    drawBtn(settings, f, true);
    al_draw_scaled_bitmap(icon, 0, 0, al_get_bitmap_width(icon), al_get_bitmap_height(icon), settings.x1 + 5, settings.y1 + 5,
                        settings.x2 - settings.x1 - 10, settings.y2 - settings.y1 - 10, 0);
    drawBtn(btn1, f, true);
    drawBtn(btn2, f, true);
    drawBtn(btn3, f, true);
    //flashing text
    if (iFlash > 20 * FPS / 60)
        al_draw_text(f[6], WHITE, SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, "- Press space to continue -");
}

int drawInstructions(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn){
    FILE *fptr;
    fptr = fopen("instructions.txt", "r");
    char text[100] = "";

    if (!fptr){
        al_show_native_message_box(display, "ERROR", "Error loading file", "\"instructions.txt\" could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_draw_text(f[3], WHITE, SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "INSTRUCTIONS");

    int i = 0;
    while (fgets(text, 100, fptr) != NULL){
        for (int i = 0; i < 100; i++)
            if (text[i] == '\n')
                text[i] = '\0';
        al_draw_text(f[6], WHITE, 150, 180 + 25 * i, 0, text);
        i++;
    }

    drawBtn(btn, f, true);
    fclose(fptr);
    return 0;
}

void drawSettings(ALLEGRO_FONT *f[], Button M1, Button M2, Button S1, Button S2, Button btnBack, bool music, int mVol, bool SFXOn, int sVol){
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 125, ALLEGRO_ALIGN_CENTER, "SETTINGS");

    //toggle music
    al_draw_text(f[4], WHITE, 200, 200, 0, "Music:");
    drawBtn(M1, f, music);
    drawBtn(M2, f, !music);

    al_draw_text(f[4], WHITE, 200, 260, 0, "Music volume");
    //volume slider
    al_draw_line(200, 320, SCREEN_W - 200, 320, WHITE, 2);
    al_draw_filled_circle(mVol * 8 + 200, 320, 10, WHITE);

    //note
    al_draw_text(f[6], WHITE, 200, 340, 0, "*music changes will apply when settings is closed");

    //toggle SFX
    al_draw_text(f[4], WHITE, 200, 390, 0, "SFX:");
    drawBtn(S1, f, SFXOn);
    drawBtn(S2, f, !SFXOn);

    al_draw_text(f[4], WHITE, 200, 450, 0, "SFX volume");
    //volume slider
    al_draw_line(200, 500, SCREEN_W - 200, 500, WHITE, 2);
    al_draw_filled_circle(sVol * 8 + 200, 500, 10, WHITE);

    drawBtn(btnBack, f, true);
}

void drawGameOver(ALLEGRO_FONT *f[], float score, Button btn1, Button btn2, Button btn3){
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 270, ALLEGRO_ALIGN_CENTER, "Score: %d", (int)score);
    drawBtn(btn1, f, true);
    drawBtn(btn2, f, true);
    drawBtn(btn3, f, true);
}

void drawNewHighscore(ALLEGRO_FONT *f[], char name[][maxNameLength], int scores[], int newScore, ALLEGRO_BITMAP *box,
                      ALLEGRO_USTR *text, int counter, Button btnSubmit, Button btnNo){
    //draw title and info
    al_draw_text(f[2], WHITE, SCREEN_W / 2, 175, ALLEGRO_ALIGN_CENTER, "NEW HIGHSCORE!");
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 275, ALLEGRO_ALIGN_CENTER, "Your score: %d", newScore);
    al_draw_textf(f[5], WHITE, SCREEN_W / 2, 315, ALLEGRO_ALIGN_CENTER, "Ranking: %d", ranking(scores, newScore));

    al_draw_text(f[5], WHITE, SCREEN_W / 2, 375, ALLEGRO_ALIGN_CENTER, "Enter a name for your highscore:");

    //text box background
    al_draw_scaled_bitmap(box, 0, 0, al_get_bitmap_width(box), al_get_bitmap_height(box), SCREEN_W / 2 - 150, 420, 300, 50, 0);

    al_draw_ustr(f[5], WHITE, SCREEN_W / 2 - 140, 430, 0, text);

    //draw the flashing thing in textboxes
    if (counter < 30)
        al_draw_text(f[5], WHITE, SCREEN_W / 2 - 140 + al_get_ustr_width(f[5], text), 430, 0, "|");

    //buttons
    drawBtn(btnSubmit, f, true);
    drawBtn(btnNo, f, true);
}

int drawHighscores(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn, char name[][maxNameLength], int score[]){
    if (getHighscores(display, name, score) != 0)
        return -1;

    al_draw_text(f[3], WHITE,SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "HIGHSCORES");

    //draw boxes?
    al_draw_rectangle(100, 180, SCREEN_W / 2 - 30, SCREEN_H - 150, WHITE, 2);
    al_draw_rectangle(SCREEN_W / 2 + 30, 180, SCREEN_W - 100, SCREEN_H - 150, WHITE, 2);

    for (int i = 0; i < 5; i++){
        al_draw_textf(f[5], WHITE, 125, 200 + 72 * i, 0, "%s", name[i]);
        al_draw_textf(f[5], WHITE, 375, 200 + 72 * i, 0, "%d", score[i]);
    }

    for (int i = 5; i < 10; i++){
        al_draw_textf(f[5], WHITE, SCREEN_W / 2 + 55, 200 + 72 * (i - 5), 0, "%s", name[i]);
        al_draw_textf(f[5], WHITE, SCREEN_W / 2 + 305, 200 + 72 * (i - 5), 0, "%d", score[i]);
    }

    drawBtn(btn, f, true);
    return 0;
}

int drawCredits(ALLEGRO_DISPLAY *display, ALLEGRO_FONT *f[], Button btn){
    FILE *fptr;
    fptr = fopen("credits.txt", "r");
    char text[100];

    if (!fptr){
        al_show_native_message_box(display, "ERROR", "Error loading file", "\"credits.txt\" could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    al_draw_text(f[2], WHITE, SCREEN_W / 2, 125, ALLEGRO_ALIGN_CENTER, "CREDITS");

    int i = 0;
    while (fgets(text, 100, fptr) != NULL){
        for (int i = 0; i < 100; i++)
            if (text[i] == '\n')
                text[i] = '\0';
        al_draw_text(f[5], WHITE, SCREEN_W / 2, 250 + 50 * i, ALLEGRO_ALIGN_CENTER, text);
        i++;
    }

    drawBtn(btn, f, true);
    fclose(fptr);
    return 0;
}
