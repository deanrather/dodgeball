//globals.cpp

//Constant global variables:
const int WINDOW_WIDTH  = 800,
          WINDOW_HEIGHT = 600,
          WINDOW_DEPTH  = 32,
          FPS           = 60,
          FRAME_RATE    = 1000/FPS,
          BALLD         = 30,
          CURSORD       = 64,
          ARENAX        = 47,
          ARENAY        = 53,
          ARENAW        = 583,
          ARENAH        = 522,
          RBARX         = 665,
          RBARY         = 17,
          RBARW         = 787-RBARX,
          RBARH         = 585-RBARY,
          MANW          = 36,
          MANH          = 54,
          FONTSIZE      = 20,
          STARTBALLS    = 3,
          GAMESPEED     = 1,
          NEWBALLDELAY  = 4;

//FULLSCREEN, MINSPEED, and MAXSPEED are defined in the main.cpp

//a char array for reference of 123, abc, 24,25,26, xyz. etc.
const char alphabet[] = {'a','b','c','d','e','f','g','h','i',
                         'j','k','l','m','n','o','p','q','r',
                         's','t','u','v','w','x','y','z'};
                         
//these are the gamestates that STATE is compared to.
enum States
{
     MENU,
     GAME,
     GAMEOVER,
     LEARN,
     CREDITS,
     QUIT
};
