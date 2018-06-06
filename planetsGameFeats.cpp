/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include "planets.h"

//find coordinates for a button based on text and y-position on the screen
void setupBtn(Button &btn, ALLEGRO_FONT *f[], int y1){
    btn.x1 = (SCREEN_W - al_get_text_width(f[5], btn.text)) / 2 - 10;
    btn.x2 = (SCREEN_W + al_get_text_width(f[5], btn.text)) / 2 + 10;
    btn.y1 = y1;
    btn.y2 = y1 + 50;
}

//returns true or false depending on if the user clicked a button
bool btnIsClicked(Button btn, int mouseX, int mouseY){
    if (mouseX < btn.x1 || mouseX > btn.x2 || mouseY < btn.y1 || mouseY > btn.y2)
        return false;
    else return true;
}

//draws a button using the coordinates
void drawBtn(Button btn, ALLEGRO_FONT *f[]){
    al_draw_rounded_rectangle(btn.x1, btn.y1, btn.x2, btn.y2, 5, 5, WHITE, 2);
    al_draw_text(f[5], WHITE, btn.x1 + 10, btn.y1 + 10, 0, btn.text);
}

//pauses or unpauses game
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

void createMeteor(Meteor m[], Planet a, ALLEGRO_BITMAP *image){
    int i = 0;
    //look for next available meteor
    while (!m[i].available && i < maxMeteors - 1){
        i++;
    }
    //create meteor if available
    if (m[i].available){
        do{
            m[i].xPos = rand() % SCREEN_W;
            m[i].yPos = rand() % SCREEN_H;
        }
        while (sqrt(pow(m[i].xPos - a.x, 2) + pow(m[i].yPos - a.y, 2)) <= a.r + minMeteorDistance);

        //overwrite destroyed meteor info
        m[i].xVel = 0;
        m[i].yVel = 0;
        m[i].available = false;
    }
}

void destroyMeteor(Meteor m[], int i){
    m[i].available = true;
}

//resets values for a new game
void reset(Meteor m[], float &score){
    for (int i = 0; i < maxMeteors; i++)
        m[i].available = true;
    score = 0.0;
}

//read in previous scores
int getHighscores(ALLEGRO_DISPLAY *display, char name[][maxNameLength], int score[]){
    FILE *fptr;
    fptr = fopen("highscores.txt", "r");

    if (!fptr){
        al_show_native_message_box(display, "ERROR", "Error loading file", "\"highscores.txt\" could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    //get data
    for (int i = 0; i < 10; i++){
        fscanf(fptr, "%s", name[i]);
        fscanf(fptr, "%d", &score[i]);
    }

    //check for blank names-- there may not be ten scores in the list
    for (int i = 0; i < 10; i++){
        if (strcmp(name[i], "") == 0)
            strcpy(name[i], "NONAME");
    }

    fclose(fptr);
    return 0;
}

//returns the ranking of the player's score on the leaderboard as an int
int ranking(int highscores[], float score){
    for (int i = 9; i >= 0; i--)
        if (score < highscores[i])
            return i + 2;
    return 1;
}

//writes new placements into file
void submitScore(char name[][maxNameLength], int highscore[], const char newName[], float newScore, ALLEGRO_DISPLAY *display){
    int rankingIndex = ranking(highscore, newScore) - 1;
    FILE *fptr;
    fptr = fopen("highscores.txt", "w");

    //not necessary...
    //if (!fptr)
    //    al_show_native_message_box(display, "ERROR", "Error loading file", "\"highscores.txt\" could not be opened.", NULL, ALLEGRO_MESSAGEBOX_ERROR);

    //move all existing highscores down to make space for the new highscore
    for (int i = 9; i > rankingIndex; i--){
        strcpy(name[i], name[i - 1]);
        highscore[i] = highscore[i - 1];
    }
    //insert new highscore
    strcpy(name[rankingIndex], newName);
    highscore[rankingIndex] = newScore;

    //print new data into file
    for (int i = 0; i < 10; i++)
        fprintf(fptr, "%s %d\n", name[i], highscore[i]);

    fclose(fptr);
    //let user know the score was submitted
    al_show_native_message_box(display, "HIGHSCORE SUBMISSION", "Highscore submitted", "Your highscore has successfully been submitted.", NULL, 0);
}
