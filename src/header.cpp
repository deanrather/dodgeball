//header.cpp

//Header files required for Dodgeball Program.


//Standrad Template Libary
#include <stdlib.h>

//Standard Input/Output header file
#include <stdio.h>

//So I can have stings!
#include <string>

//For couting stuff to stdout.txt
#include <iostream>

//for time
#include <ctime>

//Vectors are used to store ball instances
#include <vector>

//Maps are used to store image and sound data
#include <map>

//Maths Functions
#include <math.h>

//Main SDL Functions
#include "SDL.h"

//Used for SDL Audio
#include "SDL_mixer.h"

//Used to write text w/ SDL
#include "SDL_ttf.h"

//Used for text functions
#include <sstream>

// for reading txt files
#include <fstream> 


//string stuff
template <class T>
inline std::string toString(T data){
	std::ostringstream s;
	s << data;
	return s.str();
}

//my own includes

//Global Variables
#include "globals.cpp"
