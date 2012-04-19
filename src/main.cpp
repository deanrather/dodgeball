//main.cpp

//  Dodgeball
//by Dean Rather.

//a seperate file with all of my includes in it
#include "header.cpp"

//By using this namespace, we don't need std:: before most of the lines.
using namespace std;


//----------------------------------Global Variables----------------------------


//Global integers.
int STATE         = MENU,
    mouseX        = 0,
    mouseY        = 0,
    origTime      = 0,
    Timer         = 0,
    GameTimer     = 0,
    LastUpdate    = 0,
    Seconds       = 0,
    GameSeconds   = 0,
    OldFrames     = 0,
    Frames        = 0,
    Hit           = 0,
    lastBallSpawn = 0,
    middleness    = 0,
    score         = 0,
    Games         = 0,
    Balls         = 0;

//Global Boolean Values
bool isRunning  = true,
     paused     = false,
     keyUp      = false,
     keyDown    = false,
     keyLeft    = false,
     keyRight   = false,
     keyReturn  = false,
     keyAlt     = false,
     keySpace   = false,
     keyM       = false,
     keyEsc     = false,
     mouseDown  = false,
     slowBalls  = false,
     toggler    = false,
     mute       = false,
     XBUTTON    = false;

// modified in extractTxtData() fn
bool FULLSCREEN = false; 
int MAXSPEED    = 9;
int MINSPEED    = 7;
          
//global variables about keyboard input and player names for scores.
   int KEYBOARDINT = 0;
string KEYBOARDSTR = "";
string playerName = "Player";


//----------------------------------SDL Surfaces--------------------------------


// a map called img. compares string names to sdl surfaces.
map <string, SDL_Surface*> img;
//creates a surface called screen. is the buffer stuff is written to.
SDL_Surface *screen;
//this is the array of strings all of the image names (and eventually mapped
//sdl surfaces) are stored in.
//in order to add a new image, all that's neccesary is to add it to this array.
const string imageNames[] = {"menu",
                             "cursor",
                             "paused",
                             "arena",
                             "man",
                             "ball",
                             "quit",
                             "icon",
                             "credits2",
                             "learn2",
                             "play2",
                             "x2",
                             "gameover",
                             "tutorial",
                             "score",
                             "menuback2",
                             "topten",
                             "scorewinheading"};
//this const int will be generated to keep track of how many images are in the game.
const int imageCount = sizeof(imageNames) / sizeof(string);

//string array for high score names. these are the default names, and the names
//reverted back to in case of file corruption.
string hiScoreNames[10] = {
"Dean Rather",
"Cornflakes",
"Bob",
"Frederick",
"Temelcoff",
"Roderigo",
"John",
"Pumpernickle",
"Ruddiger",
"Moku Jin"};

/* temp names


string hiScoreNames[10] = {
"Dean Rather",
"BBBBBBBBBBBB",
"WWWWWWWWWWWW",
"MMMMMMMMMMMM",
"ABCDEFGHIJOK",
"QWEWREERTGFF",
"OSIDHFDMNGDF",
"Eighth",
"Ninth",
"Tenth"};

*/

//these are the corresponding actual high scores for the same indexed high score
//names. deault names are again supplied, for the same reasons.
int hiScores[10] = {10000,
                     7500,
                     5000,
                     4000,
                     3000,
                     2000,
                     1000,
                      500,
                      100,
                      50};

//sounds
Mix_Music *music;
//Mix_Chunk *spawn;

//fonts
TTF_Font* font;


//-------------------------------fn prototypes----------------------------------
    
    
void beginSDL(), // sets up SDL. loads images, all the SDL stuff.
     endSDL(), //unloads SDL, giving memory back to the computer.
     drawScreen(), //draws all the images to the screen.
     drawImage(SDL_Surface *img,
               SDL_Surface *dest,
               int x, int y), // draws a specific image to a specific area.
     game(), // the actual game function
     learn(), // the tutorial
     credits(), // runs the credits
     endGame(), // runs on end game to delete the ball instances and stuff.
     gameOver(), // brings up the gameover / enter hiscore name screen
     newBall(), // summons a new ball.
     couter(), // couts data to a txt file for debugging purposes. 
     printDetails(), // prints details such as score and lives to screen.
     deleteBall(), // destroys a ball instance
     updateScore(), //updates the score
     extractTxtData(), // opens txt databases and extracts variable information
     DisplayText(string text, int x, int y), // writes text to the screen.
     setupGame(); // sets up the game, resets variables, etc.
int  menu(), // calls the menu, returns next gamestate
     getUI(), // gets all of the user input from the keyboard/mouse.
     stringToInt(string num), // converts a string: "123" into an int: 123
     toThePowerOf(int num, int power = 2); // gets 'num' to the power of 'power'
     
void newLine(int lines); // couts 'lines' number of new lines.
string extractString(string text, int x, int y); // extracts a string from another

inline int Random(int X, int Y), // returns a random number between x and y
           abs(int x), // returnes the absolute value of x
           distance(int X1, int Y1, int X2, int Y2), // returns the distance 
           about(int num, int tolerance); // 100 would return 99, 100 or 101.
bool outOfBounds(int x, int y), // see's if that xy coord is out of arena bounds
     toggle(bool option); // flag = toggle(flag) would change true / false.
inline void updateTimer(); // updates the game timers corresponding to sdl time
string wordNum(int num); //converts a int: 12, into a string "twelve"


//----------------------------------classes-------------------------------------


// this is to group all the functions about the man. only one instane.
struct Man 
{
public:
       bool visible, // wether the man is to be displayed this frame
            invulnerable; //wether the man isnt able to be hit
       int health, // amount of men
           invulStart, // when the invulnerability began
           invulTimer, // how long to be invulnerable for
           slow, // number of slow seconds to have
           inverters, // number of inverters to have
           emp; // number of emp's you have.
           
       Man() // class constructor. runs each time an instance is made.
       {
            reset(); //calls the reset fn, because it sets all the vars to default.
       }
       
       void invul(int time) //makes the man invulnerable for time seconds.
       {
            invulStart = GameSeconds; 
            invulTimer = time;
            visible = false;
            invulnerable = true;
       }
       
       void draw() // draws the man to the screen.
       {
          //if the man should no longer be invulnerable.
          if(invulTimer < GameSeconds - invulStart)
          {
            visible = true;
            invulnerable = false;
          }
          
          //if the man was ment to be drawn this frame, draw him, then tell it
          //not to draw him next frame.
          //otherwise, if it didn't tell it to draw him this frame, tell it to
          // draw him next frame.
          if(visible)
          {
            drawImage(img["man"], screen,
                      mouseX - (MANW/2),
                      mouseY - (MANH/2));
            visible = false;
          }
          else
            visible = true;
       }
       
       //this fn resets all of his variables to default beginning variables.
       void reset()
       {            
            visible      = true;
            invulnerable = false;
            health       = 3;
            invulStart   = 0;// recorded in seconds/1000
            invulTimer   = 0;// recorded in seconds/1000
            slow         = 5000; // recorded in seconds/1000
            inverters    = 3;
            emp          = 3;
       }
};
Man man;//create the one instance of man that is to be used thruought the game.

struct Ball
{
   //X & Y Pos, Speed, creation Time, & dx & dy directions.
   int X, Y, S, T, dx, dy;
   bool flashing,
        visible,
        beenHit;
   
   Ball()
   {
       X = Random(ARENAX + BALLD, ARENAX + ARENAW - BALLD);
       Y = Random(ARENAY + BALLD, ARENAY + ARENAH - BALLD);
       S = Random(MINSPEED, MAXSPEED);
       T = GameSeconds;
       flashing = true;
       visible  = false;
       beenHit  = false;
       
       int tolerance = 70;
       if (Random(0,1))
           dx = about(S, tolerance);
       else
           dx = about(-S, tolerance);
           
       if (Random(0,1))
           dy = about(S, tolerance);
       else
           dy = about(-S, tolerance);
           
       if (dy == dx)
       {
          if (Random(0,1))
             dy++;
          else
             dx++;
       }
       
       Balls++;
   }

   void moveBall()
   {
       //create temp dir x and y
       int tdx = dx,
           tdy = dy;

       //toggle slowBalls
       if (keySpace && man.slow > 0)
         slowBalls = true;
       else
         slowBalls = false;
       
       if (slowBalls)
       {
         tdx /= 4;
         tdy /= 4;
       }
       
       tdx *= GAMESPEED;
       tdy *= GAMESPEED;
       
       //create temp X and Y's
       int tX = X + tdx;
       int tY = Y + tdy;
       
       //test if it's a bounce.
       if(outOfBounds(tX + (BALLD/2), tY + (BALLD/2)) ||
          outOfBounds(tX - (BALLD/2), tY - (BALLD/2)) )
       {
         //test wether it's vertically or horizontally OOB.
         if(outOfBounds(tX + (BALLD/2), ARENAW/2) ||
            outOfBounds(tX - (BALLD/2), ARENAW/2) ) // horz
           dx *= -1;
         if(outOfBounds(ARENAH/2, tY + (BALLD/2)) ||
            outOfBounds(ARENAH/2, tY - (BALLD/2)) ) // vert
           dy *= -1;
       }
       else
       {
         X = tX;
         Y = tY;
       }
       
       if (GameSeconds - T > 1 && !man.invulnerable || GameSeconds < 1)
       {
           flashing = false;
           //get distance from man
           /*
           int d = distance(mouseX, mouseY, X, Y);
           if (d <= (BALLD/2)+(MANW/2))
           */
           if (X+BALLD > mouseX && X < mouseX + MANW &&
               Y+BALLD > mouseY && Y < mouseY + MANH)
           {
                 Hit ++;
                 man.health --;
                 man.invul(1);
                 beenHit = true;
           }
       }
              
       if(!flashing || visible )
       {
           drawImage(img["ball"], screen, X - (BALLD/2), Y - (BALLD/2));
           visible = false;
       }
       else
           visible = true;
       
   }
   
};
vector <Ball*> BallVector;


class Button
{
public:
    int x, y, w, h;
        
    Button(int x, int y, int w, int h):
           x(x), y(y), w(w), h(h) { }
    
    bool inBounds(int mx, int my)
    {
         if(mx > x && mx < x+w &&
            my > y && my < y+h)
            return true;
         else
            return false;
    }
    
};

Button Quit(700,    0,   100, 100),
       Play(370,    220, 361, 81), 
       Learn(412,   317, 324, 77), 
       Credits(436, 433, 302, 106); 

//main fn
int main(int argc, char *argv[])
{
    srand(time(0));
    //start up
    extractTxtData();
    beginSDL();
    //loop game
    while(STATE != QUIT)
    {
        STATE = menu(); // call the menu and find out where to go.
                
        switch(STATE)
        {
        case GAME:
            isRunning = true;
            setupGame();
            while(isRunning)
            {
                getUI();
                game();
                drawScreen();
            }
            endGame();
            break;
                                     
        case LEARN:
             learn();
             STATE=MENU;
             break;
             
        case CREDITS:
             credits();
             break;
        
        case QUIT:
             
             break;
        }//end switch
    }//end while not quit
    endSDL();
    return 0;
}

void game()
{
    if (keyEsc || XBUTTON)
    {
      isRunning = false;
      STATE = GAMEOVER;
    }
        
    if (man.health < 0)
    {
       isRunning = false;
       STATE = GAMEOVER;
    }
        
    if (paused)
    {
      Mix_FadeOutMusic(0);
      if(!Mix_PausedMusic())
          Mix_PauseMusic();
      Mix_HaltMusic();
    }
    else //not paused
    {           
        if (mute)
        {
           if(!Mix_PausedMusic())
               Mix_PauseMusic();
        }
        else
        {
           if(Mix_PausedMusic())
               Mix_ResumeMusic();
           if (!Mix_PlayingMusic())
             Mix_PlayMusic(music, -1);
        }
        
        //make a new ball if need be
        if (GameSeconds >= lastBallSpawn + NEWBALLDELAY)
        {
          lastBallSpawn += NEWBALLDELAY;
          newBall();
        }
        
        if(mouseDown)
        {
            getUI();
            if (!mouseDown)
                newBall();
        }
        
    }
}

int menu()
{    
    while (true)
    {
        getUI();
        
        if ((Quit.inBounds(mouseX, mouseY) && mouseDown) || XBUTTON)
           return QUIT;
           
        if (Play.inBounds(mouseX, mouseY) && mouseDown || keyReturn)
            return GAME;
            
        if (Learn.inBounds(mouseX, mouseY) && mouseDown)
            return LEARN;
                
        drawScreen();
    }
}

void learn()
{
     const int H = FONTSIZE + 1;
     
     const int tutSteps = 7;
     const int tutLines = 9;
     
     //-----------------  Lesson 1  ------------------------------|
     string tutLessons[tutSteps][tutLines]={
     {"Welome to Dodgeball!",
     "Your goal is to beat " + hiScoreNames[0] + "'s high-score of:",
     wordNum(hiScores[0]) + ".",
     "You play as Gimmace. A little blue man who hates getting hit!",
     "Grimmace will follow your mouse aound. if he leave's the ",
     "arena, the game is paused. If he get's hit by a dodgeball,",
     "you lose a Grimmace. Once your out of Grimmaces,",
     "it's game over.",
     " "},
     
     //-----------------  Lesson 2  ------------------------------|
     {"Dodgeball's are red balls.",
     "There's " + wordNum(STARTBALLS) + " of them to begin with,",
     "and another one appears every " + wordNum(NEWBALLDELAY) + " seconds.",
     "A ball will flash for a little while as soon as it appears, ",
     "so it can't hurt you straight away. If you get hit, ",
     "you flash for a while, this means you can't get hit in this",
     "time either. Dodgeball's go in all different directions, at ",
     "all different speeds. Try not to focus. use your perpipheral",
     "vision to keep track of where their all going."},
     
     //-----------------  Lesson 3  ------------------------------|
     {"You can see your high-score at the top-right of the screen,",
     "You can also see the high-score your trying to beat to be ",
     "the winner. At the moment, " + hiScoreNames[0] + " is the ",
     "high scorer, with " + toString(hiScores[0]) + " points. to win,",
     "get more than " + toString(hiScores[0]) + " points, and claim your",
     "place on the high score board! Maybe even on the top of it!",
     " ",
     "Note: Alpha version doesn't record names or high scores yet.",
     "Sorry."},
     
     //-----------------  Lesson 4  ------------------------------|
     {"To win, you have to beat the high score, if you die before ",
     "then, you lose. in order to make a high score, you need to ",
     "live for a really long time, and make lots of points along ",
     "the way. Hiding in the corners doesn't make you very many ",
     "points. You get about ten times as many points in the middle ",
     "of the arena.",
     "There are also various power-ups and weapons that will randomly",
     "appear to help you on your quest. Note: Alpha version only has ",
     "one of these."},
     
     //-----------------  Lesson 5  ------------------------------|
     {"SLOW Power-Up.",
     "WHAT IT DOES: While in action, this power-up makes all of",
     "the balls move at only half their original speed.",
     "SUGGESTED USE: When you are in a bit of a bind, and could get",
     "out with a little extra time. Good for when your surrounded.",
     "HOW TO USE IT: Hold the Space Bar.",
     "AMMUNITION: There is 5 seconds of SLOW that you can use. It",
     "will replenish itself slowly.",
     " "},
     
     //-----------------  Lesson 6  ------------------------------|
     {"POWER-UPS THAT ARENT IN THE GAME YET",
     "Extra Lives - Replenishes your Grimmaces.",
     "Explosives - Decreases the amount of dodgeballs.",
     "Overshield - Shield's you for one hit.",
     "Inverter - Make all of the balls turn around and go backwards.",
     "Forks - Makes dodgeballs pop when you hit them.",
     " ",
     "And other secret power-ups.",
     " "},
     
     //-----------------  Lesson 7  ------------------------------|
     {"Enjoy the Game, and Good Luck!",
     " ",
     " ",
     " ",
     " ",
     " ",
     " ",
     " ",
     " "}};
     
     /*
     ,
     //-----------------  Lesson x  ------------------------------|
     {" ",
     " ",
     " ",
     " ",
     " ",
     " ",
     " ",
     " ",
     " "}
     */
          
     for (int step = 0; step < tutSteps && !keyEsc && !XBUTTON; step++)
     {
           getUI();          
           drawImage(img["tutorial"], screen, 0, 0);
           for (int line = 0; line < tutLines; line++)
               DisplayText(tutLessons[step][line],55,208 + (H*line));
               
           switch(step)
           {
           case 0:
                drawImage(img["man"], screen, ARENAX+(ARENAW/2)-(MANW/2), 150);
                break;
                
           case 1:
                drawImage(img["ball"], screen, ARENAX+(ARENAW/2)-(MANW/2), 150);
                break;     
                   
           case 2:
                DisplayText(hiScoreNames[0] + "'s High Score: " + wordNum(hiScores[0]), 2, 2);
                //High Score and Current Score showed in display.
                break;      
                  
           case 3:
                //Lesson 4 stuff
                //pics of all of the power-ups and weapons. and displayed along menu.
                break;     
                   
           case 4:
                //Lesson 5 stuff
                //Only pic of SLOW powerup.
                break;   
           /* 
           case x:
                //Lesson x+1 stuff
                break;
                
           */         
           }
           
           SDL_Flip(screen);
           
           int tempTime = SDL_GetTicks();
           while(!keyEsc && !XBUTTON)
           {
               getUI();
               if (SDL_GetTicks() > tempTime + (1 * 1000) && mouseDown)
                  break;
           }
           
     }
}

void credits()
{}

void beginSDL()
{
    //initialise SDL, print error and quit if fail
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr,
                "Failed to Initialie SDL: %s\n",
                SDL_GetError());
        exit(1);
    }
    
    //set up audio
    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 22050) < 0)
    {
         fprintf(stderr,
                 "Failed to Initialie SDL Audio: %s\n",
                 SDL_GetError());
         //exit(1);
    }
   
    // Load in the music
    music = Mix_LoadMUS("data/sounds/gameBGM.mid");
    // And sound effects
    //spawn = Mix_LoadWAV("data/sounds/spawn.wav");
    
    //make it auto quit SDL upon program close
    atexit(SDL_Quit);
    
    //set up video. print error and quit if fail.
    if (FULLSCREEN)    
        screen = SDL_SetVideoMode(WINDOW_WIDTH,
                                  WINDOW_HEIGHT,
                                  WINDOW_DEPTH,
                                  SDL_HWSURFACE|
                                  SDL_DOUBLEBUF|
                                  SDL_FULLSCREEN|
                                  SDL_RLEACCEL);
    else
        screen = SDL_SetVideoMode(WINDOW_WIDTH,
                                  WINDOW_HEIGHT,
                                  WINDOW_DEPTH,
                                  SDL_HWSURFACE|
                                  SDL_DOUBLEBUF|
                                  SDL_RLEACCEL); //SDL_NOFRAME to hide menu bar
    if(screen == NULL)
    {
        fprintf(stderr,
                "Unable to set video mode: %s\n",
                SDL_GetError());
        exit(1);          
    }
            
    //set caption
    SDL_WM_SetCaption("Dodgeball", NULL);
    
    //quickly put up loading screen while images load.
    SDL_Surface* loading =  SDL_LoadBMP("data/images/loading.bmp");
    drawImage(loading, screen,0,0);
    SDL_Flip(screen);
    
    //update timer to time we stated
    origTime = SDL_GetTicks();

    int r = 255,
        g = 0,
        b = 255;
    for (int i = 0; i < imageCount; i++)
    {
        string location = "data/images/" + imageNames[i] + ".bmp";
        img[imageNames[i]] = SDL_LoadBMP(location.c_str());
        SDL_SetColorKey(img[imageNames[i]], SDL_SRCCOLORKEY,
                        SDL_MapRGB(img[imageNames[i]]->format, r, g, b));
    }
    
    //hide mouse
    SDL_ShowCursor(0);  
    
    //set top icon
    SDL_WM_SetIcon(img["icon"], NULL);
    
    //open TTF
    TTF_Init();
    
    // Open our font and set its size to the given parameter. //
    font = TTF_OpenFont("data/comic.ttf", FONTSIZE);
}

void endSDL()
{ 
    drawImage(img["quit"], screen,0,0);
    SDL_Flip(screen);
    
    //clear up images    
    for(int i = 0; i < imageCount; i++)
    {
        SDL_FreeSurface(img[imageNames[i]]);
    }
    
    // Free the sounds
    Mix_FreeMusic(music);
    //Mix_FreeChunk(spawn);
    
    // Close the font.
    TTF_CloseFont(font);
    
    //free TTF fonts.
    TTF_Quit();
}

int getUI()
{
    //check user input
    
    mouseDown = false;
    XBUTTON = false;
        
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT://if they hit the X
                 //isRunning = false;
                 //STATE = QUIT;
                 XBUTTON = true;
                 break;
                                  
            case SDL_MOUSEBUTTONDOWN:
                 mouseDown = true;
                 break;
                 
			case SDL_KEYUP:
				//DisplayState(&event.key);
				//DisplayModifiers(&event.key);
				//DisplayKey(&event.key);
				
				KEYBOARDINT = (&event.key)->keysym.sym;
				KEYBOARDSTR = SDL_GetKeyName((&event.key)->keysym.sym);
				
				/*
				int keyboardIn = (&event.key)->keysym.sym;
				if ((keyboardIn >= 97  && keyboardIn <= 122) || //a - z
                    (keyboardIn >= 48  && keyboardIn <= 57 ) || //0 - 9
                    (keyboardIn >= 256 && keyboardIn <= 265) || //[0] - [9]
                     keyboardIn == 301 || //caps lock
                     keyboardIn == 303 || //right shift
                     keyboardIn == 304 || //left shift
                     keyboardIn == 32  || //space
                     keyboardIn == 13) //return
                {
				    cout << (&event.key)->keysym.sym
                         << "\t= " << SDL_GetKeyName((&event.key)->keysym.sym) << endl;
                }
                */
				break;
        }
       
        Uint8* keys = SDL_GetKeyState(NULL);
        if (keys[SDLK_UP])     {keyUp     = true;} else {keyUp     = false;}
        if (keys[SDLK_DOWN])   {keyDown   = true;} else {keyDown   = false;}
        if (keys[SDLK_LEFT])   {keyLeft   = true;} else {keyLeft   = false;}
        if (keys[SDLK_RIGHT])  {keyRight  = true;} else {keyRight  = false;}
        if (keys[SDLK_RETURN]) {keyReturn = true;} else {keyReturn = false;}
        if (keys[SDLK_RALT])   {keyAlt    = true;} else {keyAlt    = false;}
        if (keys[SDLK_SPACE])  {keySpace  = true;} else {keySpace  = false;}
        if (keys[SDLK_m])      {keyM      = true;} else {keyM      = false;}
        if (keys[SDLK_ESCAPE]) {keyEsc    = true;} else {keyEsc    = false;}
        
        // set mouseX and mouseY
        SDL_GetMouseState(&mouseX, &mouseY);
        
        if (keyM)
           mute = toggle(mute);
    }
}

void drawImage(SDL_Surface *img,
               SDL_Surface *dest,
               int x,
               int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = img->w;
    rect.h = img->h;
    SDL_BlitSurface(img, NULL, dest, &rect);
}

void drawScreen()
{
    //only bother if it is time.
    updateTimer();
    
    /*
    if(Timer <= LastUpdate + FRAME_RATE)
       SDL_Delay((LastUpdate + FRAME_RATE) - Timer);
    LastUpdate = Timer;
    */
    
    //if we're still in...
    //couter(); // cout some data.
        
    //draw scene
    switch(STATE)
    {
        case MENU:
             drawImage(img["menu"], screen, 0, 0);
             
             if (!Play.inBounds(mouseX, mouseY) &&
                 !Quit.inBounds(mouseX, mouseY) &&
                 !Learn.inBounds(mouseX, mouseY) &&
                 !Credits.inBounds(mouseX, mouseY))
             {
                 DisplayText("     HIGH SCORES", 74, 230);
                 
                 for(int i = 0; i < 10; i++)
                    DisplayText("     " + toString(i+1) + ". " +
                    hiScoreNames[i] + ": " +
                    toString(hiScores[i]), 50, 270+17*(i));
             }
             
             if(Play.inBounds(mouseX, mouseY))
             {
                 drawImage(img["play2"], screen, Play.x-18, Play.y-26);
                 DisplayText("     CLICK TO PLAY!", 54, 210);
             }
                 
             if(Quit.inBounds(mouseX, mouseY))
             {
                 drawImage(img["x2"], screen, Quit.x-20, Quit.y);
                 DisplayText("     Click to Quit!", 54, 210);
             }
                 
             if(Learn.inBounds(mouseX, mouseY))
             {
                 drawImage(img["learn2"], screen, Learn.x-11, Learn.y-5);
                 //DisplayText("Learn how to play the game!", 54, 210);
                 DisplayText("     How to play the game:", 54, 210);
                 DisplayText("     MOUSE: Move", 54, 210+17*1);
                 DisplayText("     SPACE: Slow Motion", 54, 210+17*2);
                 DisplayText("     ESCAPE: Quit", 54, 210+17*3);
                 DisplayText("     GOAL: Make a new High ", 54, 210+17*4);
                 DisplayText("     Score", 54, 210+17*5);
                 DisplayText("     Click for Tutorial.", 54, 210+17*7);
                 
             }
             
             if(Credits.inBounds(mouseX, mouseY))
             {
                 drawImage(img["credits2"], screen, Credits.x-13, Credits.y+6);
                 //DisplayText("A List of Dean and his friends!", 54, 210);
                 DisplayText("     CREDITS", 54, 210);
                 
                 DisplayText("     Game:", 54, 210+17*3);
                 DisplayText("     Dean Rather:", 54, 210+17*4);
                 
                 DisplayText("     Music:", 54, 210+17*7);
                 DisplayText("     Robbie Gifford", 54, 210+17*8);
                 
                 DisplayText("     Person who named the main ", 54, 210+17*10);
                 DisplayText("     character, and fixed some", 54, 210+17*11);
                 DisplayText("     internal documention typos:", 54, 210+17*12);
                 DisplayText("     Nathan Leong", 54, 210+17*13);
             }
             
             drawImage(img["cursor"], screen,
                       mouseX - (CURSORD/2),
                       mouseY - (CURSORD/2));
             break;
            
       case GAME:
            //draw arena
            drawImage(img["arena"], screen, 0, 0);
            
            //draw either cursor or man
            if (outOfBounds(mouseX, mouseY))
            {
                drawImage(img["cursor"], screen,
                          mouseX - (CURSORD/2),
                          mouseY - (CURSORD/2));
                paused = true;
            }
            else
            {
                man.draw();
                paused = false;
            }
            
            //draw balls
            if (paused)
            {
                drawImage(img["paused"], screen,80,217);
            }
            else
            {
                for (vector<Ball*>::iterator pos = BallVector.begin();
                     pos != BallVector.end();
                     pos ++)
                {
                     (*pos)->moveBall();
                     if ((*pos)->beenHit)
                     {
                         BallVector.erase((pos));
                         pos = BallVector.begin();
                         Balls--;
                     }
                }
            }
            
            printDetails();
            
            break;
            
       case GAMEOVER:
       
            drawImage(img["arena"], screen, 0, 0);
            drawImage(img["gameover"], screen, 80-2, 217-2);
            printDetails();
   
            break;
            
   case LEARN:
       
            drawImage(img["arena"], screen, 0, 0);
            drawImage(img["tutorial"], screen, 206, 30);
            drawImage(img["cursor"], screen,
                      mouseX - (CURSORD/2),
                      mouseY - (CURSORD/2));
            break;
    }
    
    //output to screen
    SDL_Flip(screen);
}

inline int Random(int X, int Y)// returns a random number beteen X and Y
{
   return X + rand()%(Y-X+1);
}

bool outOfBounds(int x, int y)
{
    if (x < ARENAX || x > ARENAX+ARENAW || y < ARENAY || y > ARENAY+ARENAH)
        return true;
    else
        return false;
}

void setupGame()
{    
    //reset some vars
    Hit           = 0;
    GameSeconds   = 0;
    lastBallSpawn = 0;
    score         = 0;
    Balls         = 0;
    GameTimer     = 0;
    Games ++;
    
    if(!Mix_PlayingMusic())
        Mix_PlayMusic(music, -1);
            
    man.reset();
    
    man.invul(1);
        
    //make some balls
    for (int i = 0; i < STARTBALLS; i ++)
      newBall();
}

void newBall()
{    
  BallVector.push_back(new Ball);
  //cout << "New Ball!" << endl;
}

void deleteBall()
{
   delete BallVector.back();
   BallVector.pop_back(); 
}

void endGame()
{    
    // Fade out all the audio channels in 1 second
    Mix_FadeOutChannel(-1, 1000);
    // and the music in 1 second
    Mix_FadeOutMusic(0);
    
    //delete the balls
    while(!BallVector.empty())
    {
      deleteBall();
    }
    
    //Game over things.
    gameOver();
}

inline int about(int num, int tolerance = 10) // Function to randomise a given number
{
   int fraction = num * tolerance/100; // find 'tolerance' percent of 'num' 
   int min = num - fraction/2; // create min and max ints, of 'tolerance'
   int max = num + fraction/2; // distance apart, w/ 'num' in the middle
   return Random(min, max);
}

inline int distance(int X1, int Y1, int X2, int Y2)
{
  int distance = (int)sqrt( (double)((toThePowerOf(X2-X1,2)) + (toThePowerOf(Y2-Y1,2))) );
  return distance;
}

inline int abs(int x)
{
  if (x < 0)
    x *= -1;
  return x;
}

inline void updateTimer()
{
    Timer = SDL_GetTicks() - origTime;
    updateScore();
    if(slowBalls)
        man.slow--;
    
    
   if (!paused)
   {
       GameTimer ++;
   }
    
    int tempSec = Seconds;
    Seconds = Timer / 1000;
    
    if (tempSec != Seconds)
    {
       OldFrames = Frames;
       Frames = 0;
       if (!paused)
       {
           GameSeconds ++;
       }
    }
}

bool toggle(bool option)
{
     //toggles a bool true/false     
     if (option)
        option = false;
     else
         option = true;
     return option;
}

void couter()
{            
    //couts data to stdout.txt
    if (Frames == 0)
       cout << "\nIt is now second " << Seconds << endl;
    
    cout << "Frame " << ++Frames;
    
cout << " | ";

    cout << "STATE: ";
    switch(STATE)
    {
    case GAME:
         cout << "GAME";
         break;
    case MENU:
         cout << "MENU";
         break;
    case LEARN:
         cout << "LEARN";
         break;
    case CREDITS:
         cout << "CREDITS";
         break;
    case QUIT:
         cout << "QUIT";
         break;
    }
    
cout << " | ";

    
cout << endl;

}


string wordNum(int num)
{
//converts the passed integer into a string.

//just in case we need it later, back up num
const int origNum = num;

//Create strings that will represent the number
string sSign = "";
string sNumber = "";
string sHundred = "";
string sThousand = "";
string sMillion = "";
string sBillion = "";
//create an array w/ base numbers
string saNums[10] = {"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
string saTens[8] = {"Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};


//if it's a neg, set a value for later use
if(num<0)
{
 num = 0-num; //  make absolute value
 sSign = "Negative ";
}
//num is hereby a positive

//if it's a basic number, return that
if(num==1000)
 return sSign + "One Thousand";

if(num==10000)
 return sSign + "Ten Thousand";

if(num==100000)
 return sSign + "One Hundred Thousand";

if(num==1000000)
 return sSign + "One Million";
 
if(num==10000000)
 return sSign + "Ten Million";
 
if(num==100000000)
 return sSign + "One Hundred Million";

if(num==1000000000)
 return sSign + "One Billion";

/*
//if it's out of range return that (impliment basic conversion here)
if (origNum>1000000000)
 return "More than One Billion!";

if (origNum<-1000000000)
 return "Less than Negative One Billion!";
*/

if (num > 1000000000)
{
    if (num < 1000000100)
       sBillion = "One Billion and ";
    else
       sBillion = "One Billion, ";
    num -= 1000000000;
}

//If it's between one and nine (inclusive), set it.
if(num>=0 && num<10)
 sNumber = saNums[num];

//if it's exactly one, two, or ninety nine million, return that.
for (int i = 1; i < 1001; i++)
{
    if (num == 1000000 * i)
    return sSign + wordNum(num/1000000) + " Million";
}

//if it's between one million and one hundred million (non inclusive),
//then set sThousand to be wordNum of the thousands, + " Thousand ". 
if (num > 1000000 && num < 1000000000)
 sMillion = wordNum(num/1000000) + " Million, ";

for (int i = 1; i < 1000001; i++)
{
    if (num >= 1000000 * i && num < (1000000 * i) + 100)
    sMillion = wordNum(num/1000000) + " Million and ";
}

//chop millions
if(num>=1000000)//if it's one million or more, 
   for(int i = 1; i<1000; i++) // for i = 1 thru 99
      if (num>=1000000*i && num < (1000000*i) + 1000000) //if it's between eg, 200000 & 300000
         num -= 1000000*i;//chop off the millions

//if it's between one thousand and one million (non inclusive),
//then set sThousand to be wordNum of the thousands, + " Thousand ". 
if (num > 1000 && num < 1000000)
 sThousand = wordNum(num/1000) + " Thousand, ";

//BUT if it's between one thousand, and one thousand, one hundred
//for example: one thousand and one, or one thousand and ninety nine
//then make it say " thousand and " instead of the old " Thousand, ".
//"One Thousand, One" sounds silly.
for (int i = 1; i < 1001; i++)
{
    if (num >= 1000 * i && num < (1000 * i) + 100)
    sThousand = wordNum(num/1000) + " Thousand and ";
}

//if it's exactly one, two, or ninety nine thousand, return that.
for (int i = 1; i < 1001; i++)
{
    if (num == 1000 * i)
    return sSign + wordNum(num/1000) + " Thousand";
}

//chop thoudsands
if(num>=100000)//if it's one hundred thousand or more, 
   for(int i = 1; i<10; i++) // for i = 1 thru 9
      if (num>=100000*i && num < (100000*i) + 100000) //if it's between eg, 200000 & 300000
         num -= 100000*i;//chop off the hundreds of thousands


if(num>=10000)//if it's ten thousand or more, 
  for(int i = 1; i<10; i++) // for i = 1 thru 9
    if (num>=10000*i && num < (10000*i) + 10000) //if it's between eg, 20000 & 30000
      num -= 10000*i;//chop off the tens of thousands

if(num>=1000)//if it's one thousand or more
  for(int i = 1; i<10; i++) // for i = 1 thru 9
    if (num>=1000*i && num < (1000*i) + 1000) //if it's between for eg, 2000 & 3000
      num -= 1000*i;//chop off the thousands.
//we have hereby finished with tesn of thousands, and thousadsn operations. 

//now work with the between one hundred and one thousand.
if(num>=100)
{
 for(int i=1; i<10; i++)
 {
  if(num>=(100*i) && num < ((100*i)+100))
  {
   if(num==(100*i))
   { 
    sHundred = saNums[i] + " Hundred";
    break;
   }//End if perfect equal
   else
   {
    num -= 100*i;
    sHundred = saNums[i] + " Hundred and ";
    break;
   }
  }//end if  exact hundred
 }//end loop

}

//fix if it's not a base number


if(num>=10)
{
 //if adding 'teen' to the end fixes it...
 if(num==16 || num==17 || num==19)
 {
  num-=10;
  sNumber = saNums[num] + "teen";
 }
 //Otherwise, things are a little more complex
 else
 {
  switch(num)
  {
   case 10:
    sNumber = "Ten";
    break;

   case 11:
    sNumber = "Eleven";
   break;

   case 12:
    sNumber = "Twelve";
    break;

   case 13:
    sNumber = "Thirteen";
    break;

   case 14:
    sNumber = "Fourteen";
    break;

   case 15:
    sNumber = "Fifteen";
    break;

   case 18:
    sNumber = "Eighteen";
    break;

   default:
    for(int i=2; i<10; i++)
    {
     if(num>=(10*i) && num < ((10*i)+10))
     {
      if(num==(10*i))
      { 
       i-=2;
       sNumber = saTens[i];
       break;
      }//End if perfect equal
      else
      {
       num -= 10*i;
       sNumber =  saTens[i-2] + " " + saNums[num];
       break;
      }
     }//end if between 20 and 100
    }//end loop
   break;
  }//end switch
 }//end ifelse
}//end if more than 10
else
{
 sNumber = saNums[num];
}

string sResult = sSign + sBillion + sMillion + sThousand + sHundred + sNumber;
if (sResult == "")
   return "ERROR";
else
   return sResult;
}//end wordNum fn


void DisplayText(string text, int x, int y)
{    
    SDL_Color foreground = { 0, 0, 0};  // Text color. //
    SDL_Color background = { 255, 0, 255 };  // Color of what's behind the text. //

    // This renders our text to a temporary surface. There //
    // are other text functions, but this one looks nice. //
    SDL_Surface* temp = TTF_RenderText_Shaded(font, text.c_str(), foreground, background);

    SDL_SetColorKey(temp, SDL_SRCCOLORKEY,SDL_MapRGB(temp->format, 255, 0, 255));
    // A structure storing the destination of our text. //
    SDL_Rect destination = { x, y, 0, 0 };

    // Blit the text surface to our window surface, the NULL specifies the whole surface. //
    SDL_BlitSurface(temp, NULL, screen, &destination);

    // Always free memory! //
    SDL_FreeSurface(temp);
}

void printDetails()
{
     const int H = FONTSIZE + 1;
     //DisplayText("HIGH SCORE: " + hiScoreNames[0]+ ". " + wordNum(hiScores[0]) + " (" + toString(hiScores[0]) + ").",10,10);
     
     //DisplayText("Score: " + wordNum(score) + " (" + toString(score) + ").",10,10);
     
     /*
     DisplayText(hiScoreNames[0] + "'s High-Score:" + toString(hiScores[0]) +
     ". Your Score: " + toString(score) +
     ". Spread: " + (toString(hiScores[0] - score))  ,10,10);
     */
     DisplayText(hiScoreNames[0] + "'s", 10, 10);
     DisplayText(toString(hiScores[0]), 358,10);
     DisplayText(toString(score), 677,10);
     DisplayText(toString(hiScores[0] - score), 662,99);
     DisplayText(toString(GameSeconds),675,189);
     DisplayText(toString(Timer),675,250);
     //DisplayText(,,);
     /*
     DisplayText("Score: ", RBARX, RBARY+H);
     DisplayText(toString(score), RBARX, RBARY+(H*2));
     
     DisplayText("Spread: ", RBARX, RBARY+(H*4));
     DisplayText(toString(hiScores[0] - score), RBARX, RBARY+(H*5));

     DisplayText("Time: ", RBARX, RBARY+(H*7));
     DisplayText(toString(GameSeconds), RBARX, RBARY+(H*8));
     */
     
     //DisplayText("Balls:",RBARX,RBARY + (H*10));
     DisplayText(toString(Balls),RBARX,RBARY + (H*13));
          
     int lives = man.health;
     if (lives > 3)
        lives = 3;
     
     for (int i = 0; i < lives; i ++)
     {
         drawImage(img["man"], screen, (RBARX-17 + (i*(MANW+16))), 500);
     }
     
     DisplayText(wordNum(man.slow),RBARX-9,559);
     
     /*
     const int H = FONTSIZE + 1;
     int N = 0;
     
     if (paused)
     {
      DisplayText("PAUSED",RBARX,RBARY);
      return;
     }
     
     if(true)
     {
         DisplayText("Score:",RBARX,RBARY + (H*N++));
         DisplayText(toString(score),RBARX,RBARY + (H*N++));
         N++;
         
         int lives = man.health;
         
         for (int i = 0; i < lives; i ++)
         {
             drawImage(img["man"], screen, RBARX+(i*MANW), RBARY + (H*N));
         }
         N+=3;
         
         DisplayText("Timer:",RBARX,RBARY + (H*N++));
         DisplayText(toString(Timer),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("Game Seconds:",RBARX,RBARY + (H*N++));
         DisplayText(toString(GameSeconds),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("FPS:",RBARX,RBARY + (H*N++));
         DisplayText(toString(OldFrames),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("Balls:",RBARX,RBARY + (H*N++));
         DisplayText(toString(Balls),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("Middleness:",RBARX,RBARY + (H*N++));
         DisplayText(toString(middleness),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("Hits:",RBARX,RBARY + (H*N++));
         DisplayText(toString(Hit),RBARX,RBARY + (H*N++));
         N++;
         
         DisplayText("Slow Seconds Left:",RBARX,RBARY + (H*N++));
         DisplayText(toString(man.slow),RBARX,RBARY + (H*N++));
         N++;
         
         if(slowBalls)
         { 
             DisplayText("SLOW",RBARX,RBARY + (H*N++));
             N++;
         }
     }
     */
}

void updateScore()
{
        //scoring
        /*
        each second
        your points increase by
        the number of balls on screen multiplied by your middleness
        minus hits percent
        
        eg 10 balls on screen, multiplied by 5 middlesness = 50 pts
        and you've been hit 10 times, so 10% of your points are filtered out
        so you get 45 points that second.
        */
        
        middleness = 5 - (distance(mouseX, mouseY, ARENAX + (ARENAW/2), ARENAY + (ARENAH/2)) / 50);
        if (middleness < 0)
           middleness = 0;
        int oldScore = score;
        double newScore = Balls * (middleness*2); // undocumented mod. middleness*2
        int tempHit = Hit;
        if (tempHit > 99)
           tempHit = 99;
        double fraction = 100/(100-tempHit);
        newScore *= fraction;
        score = oldScore + (int)newScore;
}

void gameOver()
{
   
   int origtime = SDL_GetTicks();
   int secondsToPause = 2;
   while (SDL_GetTicks() < origtime + secondsToPause * 1000 && !mouseDown)
   {
         //displaying game over screen for a second
         getUI();
   }
   
   
   
   string output = "";
         
    //check wether it should be discarded
    if (score < hiScores[9])
        output = "Sorry, Your score is terrible.";
    else
    {    
        // starting from the bottom up, compare the score to the recorded scores
        int position = 0;
        for (int i = 10; i > 0; i--)
        {
            if (score >= hiScores[i-1])//if it's higher, set pos to take over it
                position = i;
        }
        
        output = "You rank: " + toString(position);            
           
        // starting from the second from the bottom, store the score into the slot
        // below it, untill we've opened a spot to put in the new score
        for (int i = 9; i > position-1; i--)
        {
                 hiScores[i] = hiScores[i-1];
                 hiScoreNames[i] = hiScoreNames[i-1];
        } 
        
        //put the new score into the slot.
        hiScores[position-1] = score;
        hiScoreNames[position-1] = "Player " + toString(Games);
    }

    playerName = output;
        
   while (!keyEsc && !XBUTTON)
   {
         // here is where we do the whole score magig
         getUI();
         Button backToMenuButton(548,461,167,118);
         
         //processes
         
         //draw screen
         drawImage(img["score"], screen, 0, 0);
         
         if (backToMenuButton.inBounds(mouseX,mouseY))
            drawImage(img["menuback2"], screen, 548, 461);
            
         for(int i = 0; i < 10; i++)
            DisplayText(toString(i+1) + ". " +
            hiScoreNames[i] + ": " +
            toString(hiScores[i]), 20, 344+17*(i));
            
         DisplayText(toString(score), 435, 283);
         DisplayText(toString(hiScores[0]), 435, 321);
         
         //-------------------------------------------------------------
         //input
         //playerName = KEYBOARDSTR;
         
         
         DisplayText(playerName, 337, 432);
         
         //-------------------------------------------------------------
    
         
           
         if(score >= hiScores[9] && score < hiScores[0])
            drawImage(img["topten"], screen, 66, 228);
            
         if(score >= hiScores[0])
            drawImage(img["scorewinheading"], screen, 34, 41);
         
         
         //close database file
         
         string temp = "";
         string Text = "";
        for (int i = 0; i < 10; i ++)
            temp += ">" + hiScoreNames[i] + ", " + toString(hiScores[i]) + "\n";
        Text = temp;
        
        //open outFile
        ofstream outFile;
        outFile.open("data/data/Scores.txt");    
        
        //write Text to outfile
        for(int i = 0; i < Text.length(); i++)
            outFile.put(Text[i]);
        outFile.close();
         
         
         
         if (backToMenuButton.inBounds(mouseX,mouseY) && mouseDown)
         {
             // all of the adding you to the list stuff.
             STATE = MENU;
             return;
         }
         
         drawImage(img["cursor"], screen, mouseX - CURSORD/2, mouseY - CURSORD/2);
         SDL_Flip(screen);
   }
   
   
   
   STATE = MENU;
}

int stringToInt(string num)
{
    string numbers = "0123456789";
    int length = num.length();
    int answers[length]; // array to store each 
    int answer = 0;
    
    // x goes for each char in the string (number of digits)
    // i goes 10 times, for 0 through 9.
    
    for (int x = 0; x < length; x++)
    {
        for (int i = 0; i < 10; i ++)
        {
            if (num[x] == numbers[i])
            {
                answers[x] = i;
                break;
            }
        }
    }
        
    //if length-i = 1; need 1.
    //if = 2, need 10.
    //if = 3, need 100.
    for (int i = length; i > 0; i--)
        answer += toThePowerOf(10,(length-i))*answers[i-1];
    
    return answer;
}

int toThePowerOf(int num, int power)
{
    if (power==1)
        return num;
        
    if (power==0)
        return 1;
        
    power--;
    
    int answer = num;
    for (int i = 0; i < power; i++)
    {
        answer *= num;
    }
    return answer;
}

void newLine(int lines = 1)
{
    for (int i = 0; i < lines; i++)
      cout << "\n";
}

string extractString(string text, int x, int y)
{

    if (x < 0)
       return "Paramater Error: x < 0";
       
    if(y > text.length())
       return "Paramater Error: y > text.length()";
       
    if (y < x)
       return "Paramater Error: y < x";
    
    // need to have template char array (string) of correct length.
    string answer = "X";
    for (int i = 0; i < y-x-1; i++)
        answer += "X";
    
    for (int i = 0; i < y-x; i++)
        answer[i] = text[x+i];
    
    return answer;
}

void extractTxtData()
{
     
     
    //---------------high scores--------------------
    //open scores file
   
    //Create inFile
    ifstream inFile;
    inFile.open("data/data/Scores.txt");
    
    //Put file in string
    string temp;
    while(inFile.good())
        temp += inFile.get();
    string inText;
    // The last digit is a funny y.
    for (int i = 0; i < temp.length()-1; i++)
        inText += temp[i];

    //Close inFile
    inFile.close();

    string Text = inText;
    
    /*
    cout << Text;
    newLine(1);
    */

    //if edited.
    if(inText[0] != '>')
    {
        string orig = "";
        for (int i = 0; i < 9; i++)
            orig += ">" + hiScoreNames[i] + ", " + toString(hiScores[i]) + "\n";
        orig += ">" + hiScoreNames[9] + ", " + toString(hiScores[9]) + ".";
        Text = orig;
        
        
        //cout << "Database reconstructed.\n\n";  
    }
    else
    {
        //cout << "Database not reconstructed.\n\n";    
    }

    //find how many lines (">"'s)
    int lines = 0;
    for(int i = 0; i < Text.length(); i++)
        if(Text[i] == '>')
            lines ++;

    //cout << "Lines & Commas: " << lines << endl;

    //find position of all (">"'s)
    int newLines[lines];
    int line = 0;
    for(int i = 0; i < Text.length(); i++)
        if(Text[i] == '>')
        { 
            newLines[line] = i;
            line++;
        }
    /*
    //cout out some data about this
    for(int i = 0; i < lines; i ++)
       cout << "new line " << i << " at Text[" << newLines[i] << "].\n";
    */
    
    //find position of all (","'s)
    int commas[lines];
    line = 0;
    for(int i = 0; i < Text.length(); i++)
    {
        if(Text[i] == ',')
        {
            commas[line] = i;
            line++;
        }
    }
    
    /*
    //cout out some data about this
    for(int i = 0; i < lines; i ++)
       cout << "new comma " << i << " at Text[" << commas[i] << "].\n";
    
    newLine(2);
    */  
        
    for (int i = 0; i < lines; i++)
    {
        hiScoreNames[i] = extractString(Text, newLines[i]+1, commas[i]);
        if (i != lines-1)
           hiScores[i] = stringToInt(extractString(Text, commas[i]+2, newLines[i+1]-1));
        else
           hiScores[i] = stringToInt(extractString(Text, commas[i]+2, Text.length()-1));
    }
    
    
    //---------------preferences--------------------


    //Create inFile
    ifstream prefFile;
    prefFile.open("data/preferences.txt");
    
    //Put file in string
    string temppref;
    while(prefFile.good())
        temppref += prefFile.get();
    string prepherences;
    // The last digit is a funny y.
    for (int i = 0; i < temppref.length()-1; i++)
        prepherences += temppref[i];

    //Close inFile
    prefFile.close();
    
    string FullScreenString = "FULLSCREEN = ";
    
    int pos = prepherences.find(FullScreenString,0) + FullScreenString.length();
    
    if (toupper(prepherences[pos])   == 'T' && 
        toupper(prepherences[pos+1]) == 'R' && 
        toupper(prepherences[pos+2]) == 'U' && 
        toupper(prepherences[pos+3]) == 'E')
       FULLSCREEN = true;
    else
       FULLSCREEN = false;
       
    
    string character = "00";
    string tempString = "0";
    int value = 0;
    
    
    //MINBALLSPEED
    string MinBallSpeedString = "MINBALLSPEED = ";
    pos = prepherences.find(MinBallSpeedString,0) + MinBallSpeedString.length();
    
    tempString = prepherences[pos];
    if (stringToInt(tempString) >= 0 && stringToInt(tempString) <= 9)
        character[0] = prepherences[pos];
    else
        character[0] = 0;
    
    tempString = prepherences[pos+1];
    if (stringToInt(tempString) >= 0 && stringToInt(tempString) <= 9)
        character[1] = prepherences[pos+1];
    else
    {
        character[1] = character[0];
        character[0] = 0;
    }
    
    value = stringToInt(character);
    if(value < 2)
        MINSPEED = 2;
    else if (value > 100)
        MINSPEED = 100;
    else 
        MINSPEED = value;
    //cout << "Minspeed in  = " << character << " (" << value << ")." << endl;
    //cout << "MINSPEED var = " << MAXSPEED << endl << endl;
    
    
    // MAXBALSPEED
    string MaxBallSpeedString = "MAXBALLSPEED = ";
    pos = prepherences.find(MaxBallSpeedString,0) + MaxBallSpeedString.length();
    
    tempString = prepherences[pos];
    if (stringToInt(tempString) >= 0 && stringToInt(tempString) <= 9)
        character[0] = prepherences[pos];
    else
        character[0] = 0;
    
    tempString = prepherences[pos+1];
    if (stringToInt(tempString) >= 0 && stringToInt(tempString) <= 9)
        character[1] = prepherences[pos+1];
    else
    {
        character[1] = character[0];
        character[0] = 0;
    }
    
    value = stringToInt(character);
    if(value < 2)
        MAXSPEED = 2;
    else if (value > 100)
        MAXSPEED = 100;
    else 
        MAXSPEED = value;
        
    //cout << "Maxspeed in  = " << character << " (" << value << ")." << endl;
    //cout << "MAXSPEED var = " << MAXSPEED << endl;
}
