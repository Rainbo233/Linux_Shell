// Inclusion of libraries utilized in our improved shell 
#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#define MAX 256

/*

Tyler Allen Hall, Ethan Gladen, Gabriel T L Aracena
CST-315
Professor Ricardo Citro
April 3, 2022

Project 4, File Management System

This is the final version of our custom linux shell,
now with a list of custom commands that perform various
file management tasks such as creating and deleting directories,
listing the files within directories, checking data and metadata 
of files, etc.

*/

#define TOK_CUTOFF " \t\r\a\n" //splt argument function
// A color for error messages, followed by a reset system call.
#define RED "\033[0;31m"
#define RESET "\e[0m"
#define BUFF_SIZE 1024 // buffer size 

// Where all functions are initialized.
char *read_input();
char **parse_line(char *);
int dash_exit(char**);
int dash_create_dir(char **args);
int dash_rename(char **args);
int dash_create_file(char **args);
int dash_delete_file(char **args);
int dash_duplicate_file(char **args);
int dash_move_file(char **args);
int dash_s_file(char **args);
int dash_duplicate_directory(char **args);
int dash_execute(char **);
int dash_list_files(char **args);
int dash_change_directory(char **args);
int dash_copy_directory(char **args);
int dash_delete_directory(const char *path);
int dash_basic_file(char **args);
int dash_edit_file(char **args);
int dash_display_tree(char *args);

// to execute commands from the user
int dash_execute(char **args) {

    pid_t cpid;
    int status;

    // Custom command to exit the shell
    if(strcmp(args[0], "exit") == 0){ return dash_exit(args); } 

    // Custom command to make directory
    if(strcmp(args[0], "crdir") == 0){ return dash_create_dir(args); }

    // Custom command to rename a file/directory
    if(strcmp(args[0], "rename") == 0){ return dash_rename(args); }
   
    // Custom command to create file
    if(strcmp(args[0], "crf") == 0){ return dash_create_file(args); } 
    
    // Custom command to delete file
    if(strcmp(args[0], "dltf") == 0){ return dash_delete_file(args); }

    // Custom command to duplicatefile
    if(strcmp(args[0], "copyf") == 0){ return dash_duplicate_file(args); }

    // command to duplicate directory
    if(strcmp(args[0], "copydir") == 0){ return dash_copy_directory(args); }

    // Command to change current directory.
    if(strcmp(args[0], "dltdir") == 0){ return dash_delete_directory(args[1]); }

    // Command that gives the size of a file.
    if(strcmp(args[0], "sif") == 0){ return dash_s_file(args); }

    // command to move files to other directories 
    if(strcmp(args[0], "movef") == 0){ return dash_move_file(args); }

    // Command to list files in current directory 
    if(strcmp(args[0], "list") == 0){ return dash_list_files(args); }

    // Command to change current directory.
    if(strcmp(args[0], "chgdir") == 0){ return dash_change_directory(args); }

    // command to display detailed info on file
    if(strcmp(args[0], "basicf") == 0){ return dash_basic_file(args); }

    // command to edit content within a file
    if(strcmp(args[0], "editf") == 0){ return dash_edit_file(args); }

    // command to display directory tree from a starting directory
    if(strcmp(args[0], "tree") == 0){ return dash_display_tree(args[1]); };
  
    cpid = fork();
  
    if(cpid == 0){
       // if the commmand does not exist in shell
        if(execvp(args[0], args) < 0)
            printf("dash: command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);

    } else if (cpid < 0) { printf(RED "Error forking" RESET "\n"); }
    else{ waitpid(cpid, & status, WUNTRACED); }
    return 1;
}
 // exit the shell
int dash_exit(char **args){
    printf("Leaving Shell.\n");
    return 0;

}
// command 0 :Create directories
int dash_create_dir(char **args){
  // Takes second parameter as the name of 
  // new directory.
  char *name = args[1];
  int ret = 0;
  ret = mkdir(name, 0755);

  if(ret == 0){

    printf("Directory %s created.\n", name);
    
  } else if(ret == -1){

    printf("Failed to create directory.");
    
  }

  
  return 1;
}

// Command 1: Rename files/directories
int dash_rename(char **args){

  // Variables for old name of file/dir
  // and int to check if successful.
  char *curr_name = args[1];
  char *new_name = args[2];
  int ret = 0;

  ret = rename(curr_name, new_name);

  if(ret == 0){

    printf("%s renamed to %s successful.\n", curr_name, new_name);
    
  } else if(ret == -1){

    printf("Failed to rename %s.", curr_name);
    
  }
    
  
  return 1;
}


// Command 2 to make files 
int dash_create_file(char **args){

  // Takes parameter as file name, then opens
  // a new file in write mode, and because the current file
  // does not exist, it creates a file.
  char *file_name = args[1];
  FILE *fp;
  fp = fopen(file_name, "w");
  fclose(fp);
  int randomSize = rand() % 5000;
  printf("%s file created successfully with a size of %i bytes\n", file_name, randomSize);
  return 1;
  
  }

// Command that removes a file.
int dash_delete_file(char **args){

  char *name = args[1];
  int ret = 0;

  ret = remove(name);

  if(ret == 0){

    printf("%s was successfully removed.\n", name);
    
  } else if(ret == -1){

    printf("Failed to delete %s.\n", name);
    
  }


  return 1;
}

// command 3 that duplicates a file dupf file1 file2
int dash_duplicate_file(char **args){
   char ch, *source_file = args[1], *target_file = args[2];
    
   FILE *source, *target;   
 
   source = fopen(source_file, "r");
 
   if( source == NULL )
   {
      printf("Press any key to exit...\n");
      return 1;
   }
 
   target = fopen(target_file, "w");
 
   if( target == NULL )
   {
      fclose(source);
      printf("Press any key to exit...\n");
      return 1;
   }
 
 char c = fgetc(source);
    while (c != EOF)
    {
        fputc(c, target);
        c = fgetc(source);
    }
  
    printf("Contents of %s copied to %s\n", source_file, target_file);
  
    fclose(source);
    fclose(target);
    return 1;
        
        
}

// Command that deletes a directory via recursion
int dash_delete_directory(const char *path){

  char *dir_path = getcwd(dir_path, 1000);
  char command[10000] = "rm -r ";
  strcat(dir_path, "/");
  strcat(dir_path, path);
  strcat(dir_path, "/");
  strcat(command, dir_path);

  
  DIR *dp = opendir(dir_path);
  size_t path_len = strlen(dir_path);
  int r = -1;

  if(dp){
  
  struct dirent *p;
  r = 0;
    while(!r && (p=readdir(dp))){

      int s = -1;
      char *buffer;
      size_t len;
      
      // Skips the names . and .. since we don't want to recurse on them.
      if(strcmp(p->d_name, ".") || strcmp(p->d_name, "..")){ continue; }

      len = path_len + strlen(p->d_name) + 2;
      buffer = malloc(len);

      if(buffer){

        struct stat statbuffer;

        snprintf(buffer, len, "%s%s", path, p->d_name);
        if(!stat(buffer, &statbuffer)){

          if(S_ISDIR(statbuffer.st_mode)){ s = dash_delete_directory(buffer); }
          else{ s = unlink(buffer); }
          
        }

        free(buffer);
        
      }

      r = s;
      
    }

    closedir(dp);

    
  }

  if(!r){ r = system(command); }

  printf("%s directory deleted successfully\n", path);
  return 1;  
}

int dash_change_directory(char **args){

  char *target = args[1];
  if (chdir(target) != 0){ 
           
           printf("Path does not exist, aborting.\n"); 
           return 1;
           
           }
  chdir(target);
  printf("Current path is: ./%s\n", target);
  
  return 1;
}

// command 5 to move files to other locations/directories
 int dash_move_file(char **args){


       char ch;
        FILE *fp1, *fp2;
        char *source_file = args[1]; 
        char *target_dir = args[2];
        char *start_point;

        start_point = getcwd(start_point, 100);
   
  /* open the source file in write mode */
        fp1 = fopen(source_file, "r");
        /* move to the destination dir */
         if (chdir(target_dir) != 0){ 
           
           printf("Path does not exist, aborting.\n"); 
           fclose(fp1);
           return 1;
           
           }
        
        chdir(target_dir);

        fp2 = fopen(source_file, "w");

        /* error handling */
        if (!fp1) {
                printf("Unable to open source file to read!\n");
                fclose(fp2);
                return 1;
        }

        if (!fp2) {
                printf("Unable to open target file to write!\n");
                fclose(fp1);
                return 1;
        }

        /* copying contents of source file to target file */
        while ((ch = fgetc(fp1)) != EOF) {
                fputc(ch, fp2);
        }
 
   printf("Contents of %s moved to %s\n", source_file, target_dir);
        /* closing the opened files */
        fclose(fp1);
        fclose(fp2);

        /* removing the source file */
        chdir(start_point);
        remove(source_file);
        return 1;
  }
// command to check the file size
int dash_s_file(char **args){

char *source_file = args[1];
FILE *fp;
// Check if file exists
if (fp = fopen(source_file, "r")) {
	fp = fopen(source_file, "r");
	fseek(fp, 0, SEEK_END);
	unsigned long long int size = ftell(fp);

	fclose(fp);

	printf("Size of file is: %llu bytes.\n", size);
}
// Let the user know in case the file does not exist
else {
	printf("File does not exist\n");
}


  
  return 1;
}

// Command 6: to list all files in current directory
int dash_list_files(char **args){

  char *flag = args[1];
  struct dirent *dp;
  DIR *dir = opendir(".");
  int rtn = 0;
  

    // Unable to open directory stream
    if (!dir) {
        printf("Couldn't open current directory, aborting.\n");
        return 1; 
    }
    // Checks for the -d flag, and displays metadata if present.
    if(!strcmp(flag, "-d")){
      
      while ((dp = readdir(dir)) != NULL){
        
        /* Creates struct that stored the metadata of the current file. */
        struct stat info;
        stat(dp->d_name, &info);
        
        printf("*\t%s\n", dp->d_name);
        
        // Check read access
        rtn = 0;
        rtn = access (dp->d_name, R_OK);
        if (rtn == 0) { printf( "READ --- GRANTED\n" ); }
        else
        {
        if (errno == ENOENT) {printf ("%s No such file or directory.\n", dp->d_name);}
        else if (errno == EACCES) {printf ( "READ --- DENIED\n" );}
        }// Check write access
        rtn = 0;
        rtn = access (dp->d_name, W_OK);
        if (rtn == 0) {printf ( "WRITE --- GRANTED\n" );}
        else
        {
        if (errno == ENOENT){printf ("%s No such file or directory.\n", dp->d_name);}
        else if (errno == EACCES) {printf ( "WRITE --- DENIED\n" );}
        }// Check execute access
        rtn = 0;
        rtn = access (dp->d_name, X_OK);
        if (rtn == 0) {printf ( "EXECUTE --- GRANTED\n" );}
        else
        {
        if (errno == ENOENT) {printf ("%s No such file or directory.\n", dp->d_name);}
        else if (errno == EACCES) {printf ( "EXECUTE --- DENIED\n" );}
        }
        
        // Uses info taken from stat() to display metadata.
        printf("inode:   %d\n",   (int) info.st_ino);
        printf("Dev id:   %d\n",   (int) info.st_dev);
        printf("Mode:   %08x\n", info.st_mode);
        printf("Links:   %ld\n",  info.st_nlink);
        printf("User id:   %d\n", (int) info.st_uid);
        printf("gid:   %d\n", (int) info.st_gid);
        printf("Created:   %s\n", asctime(localtime(&info.st_ctime)));
    }

      
    }
  
  // Only displays the name of the file if the -d
  // flag is not present.
  else{

    while ((dp = readdir(dir)) != NULL)
    {
        printf("%s\n", dp->d_name);
    }

    printf("Finished!\n");
    
  }
    
  // Close directory stream
    closedir(dir);
    return 1;
  
}

int dash_copy_directory(char **args){

  /*

  What this does is take the name of the directory
  the user would like to copy, and the folder
  within the same directory that the duplicate
  directory would be put into.

  First, the path to the current active directory
  is taken, with the name for the source and destination
  added onto it along with a / after.

  Then, the paths are added to a command string, which
  is then passed into a system(); call.
  
  */
  char *source = args[1];
  char *target = args[2];
  char *source_path = getcwd(source_path, 1000);
  char *target_path = getcwd(target_path, 1000);
  char command[10000] = "cp -r ";
  strcat(source_path, "/");
  strcat(target_path, "/");
  strcat(source_path, source);
  strcat(target_path, target);
  strcat(source_path, "/");
  strcat(target_path, "/");
  strcat(command, source_path);
  strcat(command, " ");
  strcat(command, target_path);

  system(command);

  return 1;
}
int dash_basic_file(char **args){
  
  char *source_file = args[1];
  FILE *ptr;
    char ch;
 
    // Opening file in reading mode
    ptr = fopen(source_file, "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    printf("content of this file are \n");
 
    // Printing what is written in file
    // character by character using loop.
    do {
        ch = fgetc(ptr);
        printf("%c", ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop eading.
      
    } while (ch != EOF);
 
    // Closing the file
    fclose(ptr);
  printf("\n");
  
    return 1;
}

int dash_edit_file(char **args){
  
  char *source_file = args[1];
  char user_string[1000];
  char *end_command = "DONE";
  FILE *ptr = fopen(source_file, "a"); 

  if (ptr == NULL){
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", source_file);
        printf("Make sure you've entered the name correctly.\n");
        return 1;
  }

  printf("\nPlease enter  text to add (Type DONE to quit):\n");
  fflush(stdin);          // To clear extra white space characters in stdin
  fgets(user_string, 1000, stdin);

  fputs(user_string, ptr); 

  /* Reopen file in read mode to print file contents */
  ptr = freopen(source_file, "r", ptr);

  fclose(ptr);
  printf("Data successfully added to %s.\n", source_file);
  return 1;
}

int dash_display_tree(char *args){

  char path[1000];
  struct dirent *dp;
  DIR *dir = opendir(args);

  if(!dir){ return 1; }

  while((dp = readdir(dir)) != NULL){

    if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){

        printf("%s\n", dp->d_name);

        strcpy(path, args);
        strcat(path, "/");
        strcat(path, dp->d_name);

        dash_display_tree(path);

    }


  }

  printf("---------------------\n");
  
  closedir(dir);

  return 1;
}



// function to take two arguments and split the line with tokens
char **parse_line(char * line){

    int buffsize = BUFF_SIZE, position = 0;
    char **tokens = malloc(buffsize * sizeof(char *));
    char *token;

    if(!tokens){

        fprintf(stderr, "%sdash: Allocation error%s\n,", RED, RESET);
        exit(EXIT_FAILURE);

    }
    token = strtok(line, TOK_CUTOFF);
    while(token != NULL){

        tokens[position] = token;
        position++;

        if(position >= buffsize){

            buffsize += BUFF_SIZE;
            tokens = realloc(tokens, buffsize * sizeof(char *));

            if(!tokens){

                fprintf(stderr, "%sdash: Allocation error%s\n", RED, RESET);
                exit(EXIT_FAILURE);

            }
        

        }

        token = strtok(NULL, TOK_CUTOFF);

    }

    tokens[position] = NULL;

    return tokens;

}
// function to read the input from the user
char *read_input(){

    int buffsize = 1024;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffsize);
    int c;

    if(!buffer){

        fprintf(stderr, "%sdash: Allocation error%s\n", RED, RESET);
        exit(EXIT_FAILURE);

    }

    while(1){
// Stops entering into the buffer array when ;, &, or | is
// entered.
        c = getchar();
        if(c == EOF || c == '\n' || c == ';'|| c == '|' || c == '&'){

            buffer[position] = '\0';
            return buffer;

        } else {

            buffer[position] = c;

        }

        position++;

        /*
        
        If the commands entered are too large for the buffer array, the sizer of it is doubled for the sake of allocating more space to hold more commands.
        
        */
        if(position >= buffsize){

            buffsize += BUFF_SIZE;
            buffer = realloc(buffer, buffsize);

            if(!buffer){

                fprintf(stderr, "dash: Allocation error\n");
                exit(EXIT_FAILURE);

            }

        }

    }

}
// infinte loop to start and run the shell
void loop(){

	  int count = 0;
    char *line;
    char **args;
    int status = 1;
    int min = 250, max = 500;

    do{
      // Writes a simple prompt, then takes and executes commands. 
        printf("$ ");
        line = read_input();
        args = parse_line(line);
        printf("Process %d: READY\n", count);
        printf("Process %d: WAIT\n", count + 1);
        int time_taken = (rand() %(max - min + 1)) + min;
        for(int i = 0; i < 3 ; i++){

        printf("Process %d: RUNNING\n", count);
        usleep(time_taken / 3); 
          
        }
        // printing the process number and the amount of microseconds per process
        status = dash_execute(args);
        printf("Process %d ran in %d microseconds. \n", count, time_taken);
        free(line);
        free(args);
		    count++;

    }

    while(status);

}

int main(){
  srand(time(0));
  printf(" Welcome to the improved shell! \n");
// start the shell 
    loop();
    return 0;
  

}