/*Emily Yu
ICS3U P4
May 2018*/

#include <allegro5/allegro_font.h>
//defined values
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define MAGENTA al_map_rgb(240, 50, 230)
#define RED al_map_rgb(255, 0, 0)

//constant values
const int SCREEN_W = 1200;
const int SCREEN_H = 700;

const int FPS = 60;
const int jumpVel = 800;
const int gVel = 40;
const int moveSpd = 200;

const int maxMeteors = 10;
const float imageScale = 0.1;

//structures
struct Planet{
    int x;
    int y;
    int r;
};

struct Sprite{
    float xPos, yPos;
    float xVel, yVel;
    float shiftX;
    float shiftY;
    int frame;
    int dir; //0 left, 1 right
    bool airborne;
};

struct Meteor{
    float xPos, yPos;
    float xVel, yVel;
    int w, h;
    bool available;
};

enum Direction {NONE, LEFT, RIGHT};
//enum Difficulty {BEGINNER = 8, EASY = 4, NORMAL = 3, HARD = 1};
//Difficulty gameDiff = BEGINNER;

//prototypes

//planetsAllegro
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *sprite[], ALLEGRO_BITMAP *mImage, ALLEGRO_BITMAP *background,
               ALLEGRO_BITMAP *planet, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q, ALLEGRO_FONT *f);

//planetsMovement
void drawObjects(ALLEGRO_BITMAP *planet, Planet a, Sprite s, ALLEGRO_BITMAP *sprite[], Meteor m[], ALLEGRO_BITMAP *mImage);
void getNewCoordinates(Sprite &s, Meteor m[]);
void jump(Sprite &s, Planet a);
void shift(Sprite &s, Planet a, Direction spriteD);
bool isGrounded(Sprite s, Planet a);
bool misGrounded(Meteor m[], int i, Planet a);
void gravity(Sprite &s, Meteor m[], Planet a);
float rotateAngle(Sprite s, Planet a);
bool isCollision(Sprite p, int sw, int sh, Meteor m, int mw, int mh);

//planetsGameFeats
void drawLayout(ALLEGRO_BITMAP *background, ALLEGRO_FONT *font, float score);
void togglePause(ALLEGRO_TIMER *timer, bool &paused);
void createMeteor(Meteor m[], ALLEGRO_BITMAP *image);
void destroyMeteor(Meteor m[], int i);
void gameOver(ALLEGRO_TIMER *timer, bool &running);
void getHighscores(ALLEGRO_DISPLAY *display);
