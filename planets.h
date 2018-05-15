//defined values
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define MAGENTA al_map_rgb(240, 50, 230)

//constant values
const int SCREEN_W = 640;
const int SCREEN_H = 480;

const int FPS = 30;
const int jumpVel = 400;
const int gVel = 20;
const int moveSpd = 20;

//structures
struct Planet{
    int x;
    int y;
    int r;
};

struct Sprite{
    float xPos, yPos;
    float xVel, yVel;
};

//prototypes
void initializeAllegro();
int checkSetup(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *character, ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *q);
void draw(Planet a, Sprite s, ALLEGRO_BITMAP *bitmap);
bool isGrounded(Sprite s, Planet a);
void getNewCoordinates(Sprite &s);
void jump(Sprite &s, Planet a);
void gravity(Sprite &s, Planet a);
float rotateAngle(Sprite s, Planet a);

