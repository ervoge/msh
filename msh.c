//**************************
//msh.c
//Emily Vogelsperger
//Date: 2/23/18
//mini shell program
//**************************

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "stringlist.h"
#include "stringlist.c"
#include "lexer.h"
#include "lexer.c"

char assignCode(char *command);
int closeFile(char *command);
int extnlCmd();
int fetchID(char code);
int giveTime();
int help();
int isFileExecutable(const char *filename);
int listDirectories(char *command);
int openFile(char *command);
int readFile(char *command);
char *readLine(int fd);
int repeatString(char *command);
char *searchPath();
int sleepTime(char *command);
int startUp();
char **commandWords;
int quit = 1; //0: false, 1: true
struct timespec start, finish;

int main(int argc, char *argv[]){
  setbuf(stdout, NULL);

  startUp(); //calls first set of outputs

  clock_gettime(CLOCK_REALTIME, &start); //Silas is born

  while(quit == 0){
    printf("What next?: ");
    char *command;
    command = readLine(0);

    //if -x is passed, repeat command
    if(argc != 1){
      printf("About to execute: %s\n", command);
    }

  //*****PROCESSING COMMANDS*****//
    char code = assignCode(command);

    switch(code){
      case 'a': //how are you
        printf("I'm doing quite well, thank you!\n");
        break;
      case 'b': //tell me the time
        giveTime();
        break;
      case 'c': //tell me your name
        printf("My name is Silas\n");
        break;
      case 'd': //where did you come from
        printf("Emily V. made me after a lengthy few hours learning C.\n");
        break;
      case 'e': //tell me your ID
        printf("My program ID is %d\n", fetchID(code));
        break;
      case 'f': //tell me your parent's ID
        printf("My parent program ID is %d\n", fetchID(code));
        break;
      case 'g': //tell me your age
        clock_gettime(CLOCK_REALTIME, &finish);
        double age = finish.tv_sec - start.tv_sec;
        printf("I am about %f seconds old.\n", age);
        break;
      case 'h': //who am I
        printf("You are %s of course!\n", getenv("USER"));
        break;
      case 'i': //"say" command
        repeatString(command);
        break;
      case 'j': //sleep command
        sleepTime(command);
        break;
      case 'k': //listing directories command
        listDirectories(command);
        break;
      case 'l': //file open
        openFile(command);
        break;
      case 'm': //file read
        readFile(command);
        break;
      case 'n': //file close
        closeFile(command);
        break;
      case 'o': //help
        help();
        break;
      case 'p': //I/O redirection
        extnlCmd(command);
        break;
      case 'q': //quit
        printf("Let me know if you need anything later! See ya!\n");
        quit = 1;
        break;
      case 'r': //fork() exec()
      printf("On to extnlCmd()\n");
        extnlCmd(command);
        break;
    }
    free(command);
  }//end of while(quit = false)
  return 0;
}//end of main()

//assigns each command a
//character code to use in
//the switch statement in main
char assignCode(char *command){
  //error checking to make sure
  //user doesn't just input a blank line
  if(strcmp(command, "") == 0){
    printf("You didn't input anything! Type help if you're confused.\n");
    return 'z';
  }
  //asks how are you
  if(strcmp(command, "how are you") == 0){
    return 'a';
  }
  //asks for the time
  else if(strcmp(command, "tell me the time") == 0){
    return 'b';
  }
  //asks for chat's name
  else if(strcmp(command, "tell me your name") == 0){
    return 'c';
  }
  //secret answer
  else if(strcmp(command, "where did you come from") == 0){
    return 'd';
  }
  //asks for ID
  else if(strcmp(command, "tell me your id") == 0){
    return 'e';
  }
  //asks for parent's ID
  else if(strcmp(command, "tell me your parent's id") == 0){
    return 'f';
  }
  //asks for age
  else if(strcmp(command, "tell me your age") == 0){
    return 'g';
  }
  //asks who the user is
  else if(strcmp(command, "who am i") == 0){
    return 'h';
  }
  //parses the command into seperate words
  commandWords = split_words(command);
  //asks chat to repeat something
  if(strcmp(commandWords[0], "say") == 0){
    stringlist_free(&commandWords);
    return 'i';
  }
  //asks chat to sleep
  else if(strcmp(commandWords[0], "sleep") == 0){
    stringlist_free(&commandWords);
    return 'j';
  }
  //asks to close a file
  else if(strcmp(commandWords[0], "close") == 0){
    stringlist_free(&commandWords);
    return 'n';
  }
  //asks to list directories
  else if(strcmp(commandWords[0], "list") == 0){
    stringlist_free(&commandWords);
    return 'k';
  }
  //asks to open a file
  else if(strcmp(commandWords[0], "open") == 0){
    stringlist_free(&commandWords);
    return 'l';
  }
  //asks to read a file
  else if(strcmp(commandWords[0], "read") == 0){
    stringlist_free(&commandWords);
    return 'm';
  }
  //asks for help
  else if(strcmp(commandWords[0], "help") == 0){
    stringlist_free(&commandWords);
    return 'o';
  }
  //quits
  else if(strcmp(commandWords[0], "quit") == 0){
    stringlist_free(&commandWords);
    return 'q';
  }
  //finding how many words are in command
  int num = stringlist_len(commandWords);
  //testing for I/O Redirection '>' '>>' '<'
  if(num > 1){ //double check so no segmentation faults occur
    if((strcmp(commandWords[num-2], ">") == 0) || (strcmp(commandWords[num-2], ">>") == 0) || (strcmp(commandWords[num-2], "<")) == 0){
      stringlist_free(&commandWords);
      return 'p';
    }
  }

  //return extnlCmd() for anything else
  stringlist_free(&commandWords);
  return 'r';
}//end of assignCode()

//closes the file descriptor
//given by the user input
int closeFile(char *command){
  //parse command into separate words
  commandWords = split_words(command);
  char *s = commandWords[1];
  int fileDscpt = atoi(s);

  if(close(fileDscpt) == 0){
    printf("The file %s has been successfully closed!\n", s);
    stringlist_free(&commandWords);
    return 0;
  }
  else {
    printf("Something went wrong! The file %s was not closed properly or does not exist.\n", s);
    stringlist_free(&commandWords);
    return 0;
  }
  stringlist_free(&commandWords);
  return 0;
}//end of closeFile()

//called when the user types
//anything not immediately
//recognized by assignCode()
//or >, >>, <
//if it's a real file, this
//function fork() and exec()
//to the specified file and
//passes the correct arguments
int extnlCmd(char *command){
  int forkVal, childID, childStatus, flag;
  printf("%s\n", command);
  flag = 1;

  //parse command
  commandWords = split_words(command);
  int num = stringlist_len(commandWords);

  //vars for I/O redirect
  char *fileName;
  int fd;

  //copy the parsed words
  char **cmmdCpy = stringlist_copy(commandWords, 0, num);

  printf("This might be a fork(), exec() call!\n");

  //flag to prevent segmentation fault from short inputs
  if(num == 1){
    flag = 0;
  }

  //output redirected to fileName(commandWords[num])
  //EX. ./countdown Alice 3 > test.txt
  if(flag && (strcmp(commandWords[num-2], ">") == 0)){
    //checks and opens file
    fileName = commandWords[num-1];
    fd = openFile(fileName);

    //splits the copy of the main command so temp holds the file
    //and cmmdCpy still holds the full old command
    char **tempFile = stringlist_copy(cmmdCpy, num-1, num);

    //splits the copy of the main command so tempCmmd holds the full command before >
    char **tempCmmd = stringlist_copy(cmmdCpy, 0, num-2);

    if(isFileExecutable(tempCmmd[0]) != 0){
      //forking msh.c
      forkVal = fork();
      //error checking
      if(forkVal == -1){
        printf("Whoops! Something happened while forking. Please try again!\n");

        //freeing all stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      //error checking passes, moves to child exec()
      if(forkVal == 0){
        printf("Hello, I'm the child %d!\n", getpid());
        //redirects output to the file, fd
        if(dup2(fd, 1) < 0){
          printf("Whoops! Something happened while redirecting the output. Please try again!\n");

          //freeing all stringlists
          stringlist_free(&tempFile);
          stringlist_free(&tempCmmd);
          stringlist_free(&commandWords);
          stringlist_free(&cmmdCpy);
          return 0;
        }

        //freeing unused stringlists before execv()
        stringlist_free(&tempFile);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);

        //exec() into new program
        execv(tempCmmd[0], tempCmmd);
        //error check, will only go through if execv() doesn't work
        printf("Something went wrong with the execv() command.\n");
        exit(1);
      }
    }
    else if(isFileExecutable(tempCmmd[0]) == 0){
      //checks to see if the stub command can be found
      char *recheck = searchPath(tempCmmd[0]);
      //if searchPath returns something other than 0, call
      //extnlCmd again with that path string instead
      if(strcmp(recheck, "0") != 0){
        cmmdCpy[0] = recheck;
        char *newJoin = stringlist_join(cmmdCpy, " ");
        extnlCmd(newJoin);

        //freeing all unused stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      printf("Sorry, this file could not be executed. Maybe try again!\n");
    }
    //close the file now that we're done
    close(fd);
  }//end of ">"

  //output appended to fileName(commandWords[num])
  //EX. ./countdown Alice 3 >> test.txt
  else if(flag && (strcmp(commandWords[num-2], ">>") == 0)){
    //checks and opens file
    fileName = commandWords[num-1];
    fd = open(fileName, O_RDWR | O_APPEND, S_IXUSR);;
    printf("Successfully opened this appended file!\n");
    //print file descriptor
    printf("The file descriptor is: %d\n", fd);

    //splits the copy of the main command so temp holds the file
    //and cmmdCpy still holds the full old command
    char **tempFile = stringlist_copy(cmmdCpy, num-1, num);

    //splits the copy of the main command so tempCmmd holds the full command before >
    char **tempCmmd = stringlist_copy(cmmdCpy, 0, num-2);

    if(isFileExecutable(tempCmmd[0]) != 0){
      //forking msh.c
      forkVal = fork();
      //error checking
      if(forkVal == -1){
        printf("Whoops! Something happened while forking. Please try again!\n");

        //freeing all stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      //error checking passes, moves to child exec()
      if(forkVal == 0){
        printf("Hello, I'm the child %d!\n", getpid());
        //redirects output to the file, fd
        if(dup2(fd, 1) < 0){
          printf("Whoops! Something happened while redirecting the output. Please try again!\n");

          //freeing all stringlists
          stringlist_free(&tempFile);
          stringlist_free(&tempCmmd);
          stringlist_free(&commandWords);
          stringlist_free(&cmmdCpy);
          return 0;
        }

        //freeing all stringlists before execv()
        stringlist_free(&tempFile);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);

        //exec() into new program
        execv(tempCmmd[0], tempCmmd);
        //error check, will only go through if execv() doesn't work
        printf("Something went wrong with the execv() command.\n");
        exit(1);
      }
    }
    else if(isFileExecutable(tempCmmd[0]) == 0){
      //checks to see if the stub command can be found
      char *recheck = searchPath(tempCmmd[0]);
      //if searchPath returns something other than 0, call
      //extnlCmd again with that path string instead
      if(strcmp(recheck, "0") != 0){
        cmmdCpy[0] = recheck;
        char *newJoin = stringlist_join(cmmdCpy, " ");
        extnlCmd(newJoin);

        //freeing all unused stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      printf("Sorry, this file could not be executed. Maybe try again!\n");
    }
    //close the file now that we're done
    close(fd);
  }//end of ">>"

  //read from fileName(commandWords[num])
  //EX. ./countdown < test.txt
  else if(flag && (strcmp(commandWords[num-2], "<") == 0)){
    //checks and opens file
    fileName = commandWords[num-1];
    int fd = open(fileName, O_RDWR);

    //returns -1 if file doesn't exist
    if(fd == -1){
      printf("I'm sorry, that file doesn't exist! Please use an existing file.\n");

      //freeing all stringlists
      stringlist_free(&commandWords);
      stringlist_free(&cmmdCpy);
      return 0;
    }

    //splits the copy of the main command so temp holds the file
    //and cmmdCpy still holds the full old command
    char **tempFile = stringlist_copy(cmmdCpy, num-1, num);

    //splits the copy of the main command so tempCmmd holds the full command before >
    char **tempCmmd = stringlist_copy(cmmdCpy, 0, num-2);

    if(isFileExecutable(tempCmmd[0]) != 0){
      //forking msh.c
      forkVal = fork();
      //error checking
      if(forkVal == -1){
        printf("Whoops! Something happened while forking. Please try again!\n");

        //freeing all stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      //error checking passes, moves to child exec()
      if(forkVal == 0){
        printf("Hello, I'm the child %d!\n", getpid());
        //redirects input to the file, fd
        if(dup2(fd, 0) < 0){
          printf("Whoops! Something happened while redirecting the output. Please try again!\n");

          //freeing all stringlists
          stringlist_free(&tempFile);
          stringlist_free(&tempCmmd);
          stringlist_free(&commandWords);
          stringlist_free(&cmmdCpy);
          return 0;
        }

        //freeing all stringlists before execv()
        stringlist_free(&tempFile);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);

        //exec() into new program
        execv(tempCmmd[0], tempCmmd);
        //error check, will only go through if execv() doesn't work
        printf("Something went wrong with the execv() command.\n");
        exit(1);
      }
    }
    else if(isFileExecutable(tempCmmd[0]) == 0){
      printf("About to recheck...\n");
      //checks to see if the stub command can be found
      char *recheck = searchPath(tempCmmd[0]);
      //if searchPath returns something other than 0, call
      //extnlCmd again with that path string instead
      if(strcmp(recheck, "0") != 0){
        printf("Rechecking was successful, searchPath found the path.\n");
        tempCmmd[0] = recheck;
        char *newJoin = stringlist_join(tempCmmd, " ");
        extnlCmd(newJoin);

        //freeing all stringlists
        stringlist_free(&tempFile);
        stringlist_free(&tempCmmd);
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      printf("Sorry, this file could not be executed. Maybe try again!\n");
    }
    close(fd);
  }//end of "<"

  //normal execs without I/O pipelining
  //EX. ./countdown Alice 3
  else {
    if(isFileExecutable(commandWords[0]) != 0){
      //forking msh.c
      forkVal = fork();
      //error checking
      if(forkVal == -1){
        printf("Whoops! Something happened while forking. Please try again!\n");

        //freeing all stringlists
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      //error checking passes, moves to child exec()
      if(forkVal == 0){
        printf("Hello, I'm the child %d!\n", getpid());

        //freeing all stringlists before execv()
        stringlist_free(&cmmdCpy);

        //exec() into new program
        execv(commandWords[0], commandWords);
        //error check, will only go through if execv() doesn't work
        printf("Something went wrong with the execv() command.\n");
        exit(1);
      }
    }
    else if(isFileExecutable(commandWords[0]) == 0){
      printf("About to recheck...\n");
      //checks to see if the stub command can be found
      char *recheck = searchPath(commandWords[0]);
      //if searchPath returns something other than 0, call
      //extnlCmd again with that path string instead
      if(strcmp(recheck, "0") != 0){
        printf("Rechecking was successful, searchPath found the path.\n");
        commandWords[0] = recheck;
        char *newJoin = stringlist_join(commandWords, " ");
        extnlCmd(newJoin);

        //freeing all stringlists
        stringlist_free(&commandWords);
        stringlist_free(&cmmdCpy);
        return 0;
      }
      printf("Sorry, this file could not be executed. Maybe try again!\n");
    }
  }//end of normal exec()

  childID = wait(&childStatus);
  printf("Process %d finished with status %d.\n", childID, childStatus);

  stringlist_free(&commandWords);
  stringlist_free(&cmmdCpy);
  return 0;
}//end of extnlCmd()

//called when user asks what
//the user's name is or what
//the parent ID or chat ID is
//grabs correct ID and outputs
int fetchID(char code){
  if(code == 'e'){ //chat's ID
    return getpid();
  } else if(code == 'f'){ //parent's ID
    return getppid();
  }
  return 0;
}//end of fetchID()

//grabs current time and
//outputs time to user
int giveTime(){
 struct timespec now;
 clock_gettime(CLOCK_REALTIME, &now);
 int seconds = now.tv_sec;
 int nanoseconds = now.tv_sec;
 printf("The time is %d seconds and %d nanoseconds since midnight, January 1st, 1970.\n", seconds, nanoseconds);
 return 0;
}//end of giveTime()

//called when user types
//'help', lists commands
//recognized by chat
int help(){
  printf("I can help you answer any of these questions:\n");
  printf("- how are you\n");
  printf("- tell me the time\n");
  printf("- tell me your name\n");
  printf("- tell me your ID\n");
  printf("- tell me your parent's ID\n");
  printf("- tell me your age\n");
  printf("- who am I\n");
  printf("- sleep [number of seconds]\n");
  printf("- list [name of directory]\n");
  printf("- open [file name]\n");
  printf("- read [file descriptor]\n");
  printf("- close [file descriptor]\n");
  printf("- help\n");
  printf("- quit\n");

  return 0;
}//end of help()

// isFileExecutable() returns non-zero if the file exists and is executable,
// otherwise it returns 0.
int isFileExecutable(const char *filename) {
    struct stat statinfo;
    int result = stat(filename, &statinfo);
    if (result < 0) return 0;
    if (!S_ISREG(statinfo.st_mode)) return 0;

    if (statinfo.st_uid == geteuid()) return statinfo.st_mode & S_IXUSR;
    if (statinfo.st_gid == getegid()) return statinfo.st_mode & S_IXGRP;
    return statinfo.st_mode & S_IXOTH;
}//end of isFileExecutable()

//finds the directory requested
//and lists all files found there
int listDirectories(char *command){
  //parse command into separate words
  commandWords = split_words(command);
  char *s = commandWords[1];
  //checking to see if the directory exists
  if(s == NULL){
    printf("I'm sorry, you didn't type anything. Maybe try again!\n");
    stringlist_free(&commandWords);
    return 0;
  }
  //printf("OK, here is what I found in the %s directory:\n", s);

  DIR *d;
  struct dirent *dir;
  d = opendir(s);
  if (d != NULL){
    printf("OK, here is what I found in the %s directory:\n", s);
    while((dir = readdir(d)) != NULL){
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  else {
    printf("I'm sorry, I can't find that directory. Maybe try again!\n");
  }
  stringlist_free(&commandWords);
  return 0;
}//end of listDirectories()

//opens a file specified
//by the inputted file name
//returns fileDscpt
int openFile(char *command){
  //parse command
  commandWords = split_words(command);
  char *fileName;

  //if there is only one word passed
  //that is the fileName from extnlCmd
  if(stringlist_len(commandWords) == 1){
    fileName = command;
  }
  //anything else means its a regular command
  //so the second word should be used
  else {
    fileName = commandWords[1];
  }

  //open the file
  //***FIGURE OUT HOW TO CHOOSE BETWEEN APPEND OR NOT FOR extnlCmd()***
  int fileDscpt = open(fileName, O_RDWR);

  //returns -1 if file doesn't exist--now make that file
  if(fileDscpt == -1){
    fileDscpt = open(fileName, O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    //check if theres another error
    if(fileDscpt == -1){
      printf("I'm sorry, something went wrong with open()! Please try again.\n");
      stringlist_free(&commandWords);
      return 0;
    }
    //successful file creation
    printf("Successfully created %s!\n", fileName);
  }
  //successfully opened an existing file
  else {
    printf("Successfully opened %s!\n", fileName);
  }

  //print file descriptor
  printf("The file descriptor is: %d\n", fileDscpt);

  stringlist_free(&commandWords);
  return fileDscpt;
}//end of openFile()

//reads a file specified by the
//file descriptor given by the
//user and reads the first line
//of the file until the first return
int readFile(char *command){
  //parse command into separate words
  commandWords = split_words(command);
  //grabbing file descriptor from command
  char *s = commandWords[1];

  int fileDscpt = atof(s);
  readLine(fileDscpt);

  stringlist_free(&commandWords);
  return 0;
}//end of readFile()

//called when user types
//'say', parses input
//and outputs anything after
//'say'
char *readLine(int fd){
  char *s = malloc(256);
  int n = 0;
  while(1) {
    int r = read(fd, s + n, 1);
    if(r == 0){
      printf("I'm sorry, I don't understand that.\n");
      s[n] = '\0';
      return s;
    } else if(r < 0){
      printf("I'm sorry, I don't understand that.\n");
      s[n] = '\0';
      return s;
    } else if(s[n] == '\n'){
      s[n] = '\0';
      //if reading from anywhere other than keyboard, print first line
      if(fd != 0){
        printf("%s\n", s);
      }
      return s;
    } else if(n == 255){
      printf("I'm sorry, I don't understand that.\n");
      s[n] = '\0';
      return s;
    } else{
      n++;
    }
  }
  free(s);
  return 0;
}//end of readLine()

//called after the user inputs
//"say" the chat bot repeats
//anything said after "say"
int repeatString(char *command){
  int length = 255;
  printf("%.*s\n", length, command + 4);

  return 0;
}//end of repeatString()

//checks the user's files to see if
//the program they inputted is in
//a different path
//***FIGURE OUT HOW TO FREE MALLOC WITHOUT LOSING RETURN VAL***
char *searchPath(char *command){
  char *firstStub, *secondStub, *thirdStub;
  char *firstPath, *secondPath, *thirdPath;
  firstStub = "/usr/local/bin/";
  secondStub = "/bin/";
  thirdStub = "/usr/bin/";

  //allocating enough space for each concatonation
  firstPath = (char *) malloc(1 + strlen(firstStub) + strlen(command));
  secondPath = (char *) malloc(1 + strlen(secondStub) + strlen(command));
  thirdPath = (char *) malloc(1 + strlen(thirdStub) + strlen(command));

  //copying the paths into the new larger arrays
  strcpy(firstPath, firstStub);
  strcpy(secondPath, secondStub);
  strcpy(thirdPath, thirdStub);

  //concatonating the commands
  strcat(firstPath, command);
  strcat(secondPath, command);
  strcat(thirdPath, command);

  if(isFileExecutable(firstPath) != 0){
    printf("This file exists! Its in /usr/local/bin/\n");
    return firstPath;
  }
  else if(isFileExecutable(secondPath) != 0){
    printf("This file exists! Its in /bin/\n");
    return secondPath;
  }
  else if(isFileExecutable(thirdPath) != 0){
    printf("This file exists! Its in /usr/bin/\n");
    return thirdPath;
  }
  else {
    printf("This file doesn't exists anywhere. Maybe try again!\n");
  }

  return 0;
}


//called when the user asks
//the chat to sleep for an
//inputted amount of seconds
int sleepTime(char *command){
  char str[255];
  char *s;

  strcpy(str, command); //copying to new string to save command
  strtok_r(str, " ", &s); //parsing

  double number = atof(s); //converting string to double
  printf("OK, going to sleep for %f seconds.\n", number);
  usleep(number*1000000);

  printf("That was a good nap, I feel refreshed!\n");

  return 0;
}//end of sleepTime

//called everytime chat is
//executed for the first time
//sets quit to false
int startUp(){
  quit = 0;
  printf("Welcome to Chat Assistant!\n");
  giveTime();
  printf("My name is Silas.  How can I help you?\n");
  printf("Type 'help' if you are lost.\n");

  return 0;
}//end of startUp()
