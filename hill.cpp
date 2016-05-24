// Name: Ben Wilfong
// File Name: hill.cpp
// Due Date: 3/8/16
// Program Description:
/*   This program will encrypt or decrypt a given text based on
 *   the user's input on the command line. The user will provide
 *   an input file name and a destination file name, along with
 *   an encryption matrix if applicable. 
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <sstream>

using namespace std;

void encryptionMode();
void decryptionMode();
string getFileText(string);
string transform(string,vector<int>);
string decrypt(string,vector<int>);
void writeToFile(string,string);
bool hasAnInverse(vector<int>);
int findReciprocal(int);
vector<int> findInverse(vector<int>,int);


int main(int argc, char** argv)
{
     if(argc == 1)
     {
          cout << "To use this program, please provide a command line argument"
               << " (either -e or -d) to enter encryption or decryption mode. "
               << endl;
          
          return 0;
     }

     else if(argc == 2)
     {
          if(strcmp(argv[1], "-e") == 0)
          {
               cout << "Entering encryption mode..." << endl;
               encryptionMode(); // start encryption process
          }

          else if(strcmp(argv[1], "-d") == 0)
          {
               cout << "Entering decryption mode..." << endl;
               decryptionMode(); // start decryption process
          }

          else
          {
               cout << "Invalid command line arguments! Please try again with"
                       " ./hill [-e | -d]" << endl;
               return 0;
          }
     }
}

//===============================================================================

void encryptionMode()
{
     string plainText, matrixEle, inputFile, cipherText, outputFile;
     vector<int> encryptionMatrix;

     cout << "Please enter the name of your input (plaintext) file:" << endl;
     getline(cin, inputFile);

     cout << "Please provide a name for the output (ciphertext) file:" << endl;
     getline(cin, outputFile);

     cout << "Please enter a 4-element encryption matrix, "
          << "separating elements by commas.\n"
          << "EX: 1,15,13,5 = |1 15|\n"
          << "                |13 5|" << endl;
     
     getline(cin,matrixEle);

     istringstream iss(matrixEle);

     while(iss)
     {
          if (!getline(iss, matrixEle, ',')) break;
          encryptionMatrix.push_back(atoi(matrixEle.c_str()));
     }
     
 
     plainText = getFileText(inputFile); // pull input from specified file

     cout << "Plaintext acquired:\n" << plainText << endl;

     cipherText = transform(plainText, encryptionMatrix); // encrypt the plaintext

     cout << "Ciphertext after encryption:\n"
          << cipherText << endl;

     cout << "Writing results to file... " << endl;

     writeToFile(outputFile, cipherText);
}

//===============================================================================

string transform(string originalText, vector<int>matrix)
{    // both encryption and decryption are done in the same manner

     string newText = "";
     char oldChar1, oldChar2, newChar;

     for (int i = 0; i < originalText.length(); i += 2) // go in intervals of 2
     {
          oldChar1 = originalText.at(i) - 97;
               // convert character to position in alphabet
          oldChar2 = originalText.at(i+1) - 97;

          newChar = ((oldChar1 * matrix.at(0) +
                       oldChar2 * matrix.at(1)) % 26) + 65;
               // transform first char of digraph and convert to uppercase

          newText = newText + newChar; // append to new text

          newChar = ((oldChar1 * matrix.at(2) +
                       oldChar2 * matrix.at(3)) % 26) + 65;
               // transform second char of digraph and convert to uppercase

          newText = newText + newChar; // append to ciphertext
     }    

     return newText;
}

//===============================================================================

string decrypt(string cipherText, vector<int> matrix)
{
     int determinant;
     
     determinant = matrix.at(3) * matrix.at(0) - matrix.at(1) * matrix.at(2);
          // m22 * m11 - m12 * m21 = |m|

     cout << "Calculated Determinant: " << determinant;

     while(determinant < 0)
          determinant += 26; // because c++ can't handle negative mods

     cout << " = " << determinant << "(mod 26)." << endl;

     if(((determinant % 2) == 1) && ((determinant % 13) != 0))
          // if the determinant is odd and isn't a multiple of 13
     {
          cout << "Matrix has an inverse." << endl;
          
          return transform(cipherText, 
                         findInverse(matrix, findReciprocal(determinant)));
                         // transform the text with the inverse matrix, found in 
                         // findInverse() with the reciprocal of the 
                         // determinant in findReciprocal()          
     }   
     
     else
          return "Matrix determinant is even or divisible by 13. Cannot "
                 "be used to find an inverse";
}

//===============================================================================

vector<int> findInverse(vector<int> encryptionMatrix, int reciprocal)
{
     vector<int> decryptionMatrix;

     decryptionMatrix.push_back(((encryptionMatrix.at(3) * reciprocal) % 26));
          // M22/|M|

     decryptionMatrix.push_back((((-1 * encryptionMatrix.at(1) + 26) * reciprocal) % 26));
          // -(M12/|M|)

     decryptionMatrix.push_back((((-1 * encryptionMatrix.at(2) + 26) * reciprocal) % 26));
          // -(M21/|M|)

     decryptionMatrix.push_back(((encryptionMatrix.at(0) * reciprocal) % 26));
          // M11/|M|

     cout << "Decryption Matrix = |" << decryptionMatrix.at(0) << "  "  
                                     << decryptionMatrix.at(1) << "|\n"
             "                    |" << decryptionMatrix.at(2) << "  "
                                     << decryptionMatrix.at(3) << "|\n";

     return decryptionMatrix;
}

//===============================================================================

int findReciprocal(int determinant)
{
     int reciprocal = 1;

     while((determinant * reciprocal) % 26 != 1)
          reciprocal++;

     cout << "Reciprocal of " << determinant << "(mod 26) is " << reciprocal << endl;

     return reciprocal;
}

//===============================================================================

void decryptionMode()
{
     string plainText, matrixEle, inputFile, cipherText, outputFile;
     vector<int> encryptionMatrix;

     cout << "Please enter the name of your input (ciphertext) file:" << endl;
     getline(cin, inputFile);

     cout << "Please provide a name for the output (plaintext) file:" << endl;
     getline(cin, outputFile);

     cout << "Please enter the 4-element encryption matrix used, "
          << "for encryption, separating elements by commas.\n"
          << "EX: 1,15,13,5 = |1 15|\n"
          << "                |13 5|" << endl;
     
     getline(cin,matrixEle);

     istringstream iss(matrixEle);

     while(iss)
     {
          if (!getline(iss, matrixEle, ',')) break;
          encryptionMatrix.push_back(atoi(matrixEle.c_str()));
     }

     cipherText = getFileText(inputFile);

     cout << "Ciphertext acquired:\n" << cipherText << endl;

     plainText = decrypt(cipherText,encryptionMatrix);

     cout << plainText << endl;

     writeToFile(outputFile, plainText);

     return;
}

//===============================================================================

string getFileText(string inputFileName)
{
     string input = "";
     char buffer;

     ifstream inputFile;
     inputFile.open(inputFileName.c_str());

     if(!inputFile)
     {
          cout << "Bad file name. Please try again." << endl;
          exit(0); // quit
     }

     while(inputFile >> buffer)
     {
          if(buffer >= 'a' && buffer <= 'z') // do character filtering
               input = input + buffer;

          else if(buffer >= 'A' && buffer <= 'Z')
          {
               buffer += 32; // convert to lowercase
               input = input + buffer; 
          }
     }

     if(input.length() % 2 == 1) // plaintext is odd
     {
          input = input + 'x';  
          cout << "Appended one 'x' to make character count even. " << endl;
     }
     
     inputFile.close();

     return input;         
}

//===============================================================================

void writeToFile(string outputFileName, string text)
{
     ofstream outputFile;
     outputFile.open(outputFileName.c_str());

     if(!outputFile)
     {
          cout << "Bad file name. Please try again." << endl;
          exit(0); // quit
     }

     outputFile << text; // write to file

     outputFile.close();

     cout << "File write to " << outputFileName << " complete." << endl;

     return;
}
