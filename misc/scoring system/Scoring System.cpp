//Dean's High Score File.

#include <fstream> // for reading files
#include <iostream> // for cin and cout
#include <ctype.h> // for toupper etc
#include <iostream> // for cin and cout
#include <string> // for strings
#include <sstream> //Used for text functions

//string stuff
template <class T>
inline std::string toString(T data){
	std::ostringstream s;
	s << data;
	return s.str();
}

using namespace std;

void newLine(int lines);
string extractString(string text, int x, int y);
int stringToInt(string num);
int toThePowerOf(int num, int power = 2);//Used for text functions

string hiScoreNames[10];
int hiScores[10];

int main()
{
    //Create inFile
    ifstream inFile;
    inFile.open("Scores.txt");
    
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
        string orig = ">Dean Rather, 10000\n";
               orig+= ">Second Placer, 5000\n";
               orig+= ">Third Placer, 2500\n";
               orig+= ">Fourth Placer, 2000\n";
               orig+= ">Fifth Placer, 1500\n";
               orig+= ">Sixth Placer, 1400\n";
               orig+= ">Seventh Placer, 1300\n";
               orig+= ">Eighth Placer, 1200\n";
               orig+= ">Ninth Placer, 1100\n";
               orig+= ">Tenth Placer, 1000.";// this dot is because if the 
               //scores are read from this reconstruction as opposed to the 
               //source file, then the last character is discarded.
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

    //It's In!!!-----------------------------------------------------------------

    //display all of the hiscore data
    for (int i = 0; i < 10; i++)
        cout << hiScoreNames[i] << ":\t" << hiScores[i] << endl;
    newLine(2);
    
    //attain a new score
    cout << "Enter a new Score:";
    int score;
    cin >> score;
    cout << "\nYou Entered :" << score;
    newLine(2);
        
    //check wether it should be discarded
    if (score < hiScores[9])
        cout << "Sorry, " << score << " doesn't make the scoreboard.";
    else
    {    
        // starting from the bottom up, compare the score to the recorded scores
        int position = 0;
        for (int i = 10; i > 0; i--)
        {
            if (score >= hiScores[i-1])//if it's higher, set pos to take over it
                position = i;
        }
        
        cout << "You rank number " << position << " on the scoreboard!\n\n";
        
        if (score > hiScores[0])
           cout << "And made a new HIGH SCORE!!!\n\n";
           
        //attain name
        cout << "Please enter your name!\n>";
        string name;
        cin >> name;
        
        //regulate length of name
        while(name.length() > 12)
        {
            cout << "\nSorry, " << name
                 << " is too long. Please enter a name less than 12 characters.\n>";
            cin >> name;
        }
        
        cout << "\nYour name is " << name << " and your score is " << score << ".\n\n";
        
        // starting from the second from the bottom, store the score into the slot
        // below it, untill we've opened a spot to put in the new score
        for (int i = 9; i > position-1; i--)
        {
                 hiScores[i] = hiScores[i-1];
                 hiScoreNames[i] = hiScoreNames[i-1];
        } 
        //put the new score into the slot.
        hiScores[position-1] = score;
        hiScoreNames[position-1] = name;
        
    }
       
       
    //display all of the hiscore data
    newLine(2); 
    for (int i = 0; i < 10; i++)
        cout << hiScoreNames[i] << ":\t" << hiScores[i] << endl;
    newLine(2);
    
    
    //Let's Put it back!!--------------------------------------------------------
    
    temp = "";
    for (int i = 0; i < 10; i ++)
        temp += ">" + hiScoreNames[i] + ", " + toString(hiScores[i]) + "\n";
    Text = temp;
    
    //open outFile
    ofstream outFile;
    outFile.open("Scores.txt");    
    
    //write Text to outfile
    for(int i = 0; i < Text.length(); i++)
        outFile.put(Text[i]);
    outFile.close();

    //End Program
    newLine(1);
    system("pause");
    return 0;
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
        //answer += (((length-i)+1)*10)*answers[i-1];
    
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
