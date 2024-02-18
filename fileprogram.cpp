#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[]) {
   int numChar = 0;
   char message[100];
   int i = 0;
   ifstream readFile(argv[1]);
   ofstream writeFile(argv[2]);

   if (readFile.is_open()) {
      while(readFile){
         message[i] = readFile.get();
         numChar++;
         i++;
      }
   }
   int pip[2], child;
   int result;
   char instring[numChar];

   result = pipe(pip);
   if (result == -1) {
      perror("pipe");
      exit(1);
   }

   child = fork();
   if (child == -1){
      perror("Trouble creating child process");
      exit(2);
   }
   else if (child == 0) {
      close(pip[0]);
      write(pip[1], message, numChar);
      close(pip[1]);
      exit(0);

   } else {
      close(pip[1]);
      read(pip[0], instring, numChar);
      if (writeFile.is_open()){
         writeFile << instring;
         
      }

      close(pip[0]);
      exit(0);
   }

   return 0;
}
