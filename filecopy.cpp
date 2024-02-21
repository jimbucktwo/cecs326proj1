// Jimmy Tran, 028343131, CSULB CECS 326
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;



int main(int argc, char *argv[]) {
   int numChar = 0;
   int i = 0;

   // error handling to catch invalid files being opened
   try {
      ifstream readFile(argv[1]);
      if (readFile) {
      readFile.seekg(0, ios::end);
      numChar = readFile.tellg();
      readFile.close();
      } else {
         throw runtime_error("Unable to open source file ");
      }
      
   } catch (const exception& e) {
      cerr << "Error: " << e.what() << "'" << argv[1] << "'";
      return -1;
   }
   
   //initiate variables to read from the file and to create child process
   char message[numChar];
   int pip[2], child, result;
   char instring[numChar];

   // opens files to be read and written to
   ifstream readFile2(argv[1]);
   ofstream writeFile(argv[2]);

   // reads from input file
   if (readFile2.is_open()) {
         while(readFile2){
            message[i] = readFile2.get();
            i++;
      } 
      readFile2.close();
      }

   // creates a pipe and child process
   result = pipe(pip);
   if (result == -1) {
      perror("pipe");
      exit(1);
   }

   child = fork();

   //error handling in case a child process cannot be created
   if (child == -1){
      perror("Trouble creating child process");
      exit(2);

   // Write end of the pipe, must close other end before executing
   } else if (child == 0) {
      close(pip[0]);
      write(pip[1], message, numChar);
      close(pip[1]);
      exit(0);

   } else {
      close(pip[1]);
      read(pip[0], instring, numChar);
      // writes to the destination file
      if (writeFile.is_open()) {
         writeFile << instring;
         writeFile.close();
      }
      
      close(pip[0]);
      cout << "File successfully copied from " << argv[1] << 
   " to " << argv[2];
      exit(0);
   } 

   return 0;
}
