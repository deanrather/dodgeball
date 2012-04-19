//   int to string source:
//   http://www.wlug.org.nz/ConvertingAnIntegerToaStringInCpp


#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

template<class T>
std::string any2string(T i) {
    std::ostringstream buffer;
    buffer << i;
    return buffer.str();
}


int numberOfDigits(int number)
{
    if (number == 0)
       return 1;
    
    int result = 1;
    for (int i = 1; i < number; i*=10)
        result++;
    return result;
}


int str2int(string strNumber)
{
    int intResult = 0;
    for (int i = 0; i < strNumber.length(); i++)
        intResult += numberOfDigits(i)*(strNumber[i]);
    
    return intResult;
}
 
   
int main()
{
    int intNumber = 567;
    string strNumber = "A String!";
    
    cout << "Put in a number: ";
    cin >> intNumber;
    
    /*
    cout << "Proof it's a int: " << intNumber << " X2 = " << intNumber + intNumber << endl;
        
    
    
    cout << "Converted: " << strNumber << endl;
    cout << "Proof it's a str: " << strNumber << " X2 = " << strNumber + strNumber << endl;
    */
    
    strNumber = any2string(intNumber);
    
    string strResult = "";
    for (int i = strNumber.length(); i >= 0; --i)
         strResult += strNumber[i];  
    
    cout << "Result = " << strResult << endl;
    
    int intResult = str2int(strResult);
    cout << "Int result = " << intResult << endl;
    
    int x = 1;
    for (int i = 0; i < 9; i++)
        cout << "numeber of digits in " << x << ": " << numberOfDigits(x*=10) << endl;
    
    system("pause");
    return 0;
}
