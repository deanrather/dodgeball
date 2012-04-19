// a function to convert string numbers: "123" into int numbers: 123.
// by Dean Rather

#include <iostream> // for cin and cout
#include <string> // for strings

using namespace std;

int stringToInt(string num);
int toThePowerOf(int num, int power = 2);

int main()
{
    cout << "This program should convert a string number: \"123\"" << endl
         << "into a int number: 123." << endl << endl;
    string one = "1";
    string oneTwoThree = "123";
    string nineEightSeven = "987";
    string bigNumber = "0123465789";
    
    cout << "String one\t\t= \"" << one
         << "\"\t\tShould be int " << stringToInt(one) << endl;
    cout << "String oneTwoThree\t= \"" << oneTwoThree
         << "\"\t\tShould be int " << stringToInt(oneTwoThree) << endl;
    cout << "String nineEightSeven\t= \"" << nineEightSeven
         << "\"\t\tShould be int " << stringToInt(nineEightSeven) << endl;
    cout << "String bigNumber\t= \"" << bigNumber
         << "\"\tShould be int " << stringToInt(bigNumber) << endl;
        
    system("pause");
    return 0;
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
