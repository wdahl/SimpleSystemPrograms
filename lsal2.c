#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <ctype.h>

//node to hold directory entry
struct node{
	struct dirent *dp;
	struct node *next;
};

//holds file name
struct file_node{
	char * name;
	struct file_node *next;
};

//holds directory arguments
struct dir_node{
	char * name;
	DIR *dir;
	struct node *dp;
	struct dir_node *next;
};

int count = 0;//line count

//protoypres
struct node* add(struct dirent *dp, struct node* head);
int print_files(struct file_node *head);
void print_directories(struct dir_node* head, int argc);
struct file_node* add_file(char* name, struct file_node *head);
struct dir_node* add_dir(DIR *dir, struct dir_node *dir_head, char * name);

//ls -al program that takes multpule arguments
int main(int argc, char *argv[]){
	char *path; // holds the path name
	DIR *dir; // pointer to the directory
	FILE *file; // file pointer
	struct dirent *dp; // holds the pointer to the current entry in the directory
	struct stat stat; // holds the statuses of the file and directories
	struct passwd *pw; // pointes to the password uid
	struct group *gr; // points to the group id
	struct tm *p; // points to a time structure for formating
	char s[1000]; // holds the time as a string
	struct node *head = NULL; // head of directory entry nodes
	struct file_node *file_head = NULL; //head of file nodes
	struct dir_node *dir_head = NULL; //head of directoyr list
	char real_path[1000];
	char * abs_path;

	// checks if no arguments were given
	if(argc == 1){
		path = "."; // selts the path to the current directory
		//checks if the path given was a directory
		if((dir = opendir(path)) != NULL){
			//loops through the contents of the directory and puts them into linked list
			while((dp = readdir(dir)) != NULL){
				head = add(dp, head);
			}

			struct node *temp = head; // temp node for looping
			struct node *size = temp;
			int total_size = 0;
			while(size != NULL){
				memset(real_path, 0, sizeof real_path);
				abs_path = strcpy(real_path, path);
				strcat(abs_path, "/");
				strcat(abs_path, temp->dp->d_name);
				lstat(abs_path, &stat);
				total_size += stat.st_blocks;
				size = size->next;
			}

			printf("total %d\n", total_size);
			while(temp != NULL){
				lstat(temp->dp->d_name, &stat); // retrives the staues of the file in the directory and puts them into stat
				// checks if the entry is a directory or a file
				if(S_ISDIR(stat.st_mode) || S_ISREG(stat.st_mode)){
					//formates the mode of the file to be in rwx form
					printf( (S_ISDIR(stat.st_mode)) ? "d" : "-");
   					printf( (stat.st_mode & S_IRUSR) ? "r" : "-");
   					printf( (stat.st_mode & S_IWUSR) ? "w" : "-");
  					printf( (stat.st_mode & S_IXUSR) ? "x" : "-");
    				printf( (stat.st_mode & S_IRGRP) ? "r" : "-");
    				printf( (stat.st_mode & S_IWGRP) ? "w" : "-");
    				printf( (stat.st_mode & S_IXGRP) ? "x" : "-");
   					printf( (stat.st_mode & S_IROTH) ? "r" : "-");
    				printf( (stat.st_mode & S_IWOTH) ? "w" : "-");
    				printf( (stat.st_mode & S_IXOTH) ? "x" : "-");
    				printf(" %d", stat.st_nlink); // prints link number
  					pw = getpwuid(stat.st_uid); // sets the password struct to the uid of the files password
    				printf(" %s", pw->pw_name); // prints user name
    				gr = getgrgid(stat.st_gid); // sets gr to the group id
    				printf(" %s", gr->gr_name); // prints group name
    				printf(" %ld", stat.st_size); // prints size of file
    				p = localtime(&stat.st_mtime); // gets the time of modification of the file
    				strftime(s, 1000, "%b %d %H:%M", p); // formates the time
    				printf(" %s %s\n", s, temp->dp->d_name); // prints time and name of file
    			}

    			temp = temp->next;
			}
		}
	}

	//checks if argument count is 2
	else if(argc == 2){
		path = argv[1];
		if((dir = opendir(path)) != NULL){
			//loops through the contents of the directory
			while((dp = readdir(dir)) != NULL){
				head = add(dp, head);
			}

			struct node *temp = head;
			struct node *size = temp;
			int total_size = 0;
			while(size != NULL){
				memset(real_path, 0, sizeof real_path);
				abs_path = strcpy(real_path, path);
				strcat(abs_path, "/");
				strcat(abs_path, temp->dp->d_name);
				lstat(abs_path, &stat);
				total_size += stat.st_blocks;
				size = size->next;
			}

			printf("total %d\n", total_size);
			while(temp != NULL){
				memset(real_path, 0, sizeof real_path);
				abs_path = strcpy(real_path, path);
				strcat(abs_path, "/");
				strcat(abs_path, temp->dp->d_name);
				lstat(abs_path, &stat); // retrives the staues of the file in the directory and puts them into stat
				// checks if the entry is a directory or a file
				if(S_ISDIR(stat.st_mode) || S_ISREG(stat.st_mode)){
					//formates the mode of the file to be in rwx form
					printf( (S_ISDIR(stat.st_mode)) ? "d" : "-");
   					printf( (stat.st_mode & S_IRUSR) ? "r" : "-");
   					printf( (stat.st_mode & S_IWUSR) ? "w" : "-");
  					printf( (stat.st_mode & S_IXUSR) ? "x" : "-");
    				printf( (stat.st_mode & S_IRGRP) ? "r" : "-");
    				printf( (stat.st_mode & S_IWGRP) ? "w" : "-");
    				printf( (stat.st_mode & S_IXGRP) ? "x" : "-");
   					printf( (stat.st_mode & S_IROTH) ? "r" : "-");
    				printf( (stat.st_mode & S_IWOTH) ? "w" : "-");
    				printf( (stat.st_mode & S_IXOTH) ? "x" : "-");
    				printf(" %d", stat.st_nlink); // prints link number
  					pw = getpwuid(stat.st_uid); // sets the password struct to the uid of the files password
    				printf(" %s", pw->pw_name); // prints user name
    				gr = getgrgid(stat.st_gid); // sets gr to the group id
    				printf(" %s", gr->gr_name); // prints group name
    				printf(" %ld", stat.st_size); // prints size of file
    				p = localtime(&stat.st_mtime); // gets the time of modification of the file
    				strftime(s, 1000, "%b %d %H:%M", p); // formates the time
    				printf(" %s %s\n", s, temp->dp->d_name); // prints time and name of file
    			}

    			temp = temp->next;
			}
		}

		else if((file = fopen(path, "r")) != NULL){
			lstat(path, &stat);
			printf( (S_ISDIR(stat.st_mode)) ? "d" : "-");
    		printf( (stat.st_mode & S_IRUSR) ? "r" : "-");
    		printf( (stat.st_mode & S_IWUSR) ? "w" : "-");
   			printf( (stat.st_mode & S_IXUSR) ? "x" : "-");
    		printf( (stat.st_mode & S_IRGRP) ? "r" : "-");
    		printf( (stat.st_mode & S_IWGRP) ? "w" : "-");
    		printf( (stat.st_mode & S_IXGRP) ? "x" : "-");
    		printf( (stat.st_mode & S_IROTH) ? "r" : "-");
    		printf( (stat.st_mode & S_IWOTH) ? "w" : "-");
    		printf( (stat.st_mode & S_IXOTH) ? "x" : "-");
    		printf(" %d", stat.st_nlink);
    		pw = getpwuid(stat.st_uid);
    		printf(" %s", pw->pw_name);
    		gr = getgrgid(stat.st_gid);
    		printf(" %s", gr->gr_name);
    		printf(" %ld", stat.st_size);
    		p = localtime(&stat.st_mtime);
    		strftime(s, 1000, "%b %d %H:%M", p);
    		printf(" %s %s\n", s, path);
    	}
	}

	// sets path to the argument givin
	else{
		int i;//counter
		//loops through the arguments given
		for(i=1;i<argc;i++){
			if((dir = opendir(argv[i])) != NULL){
				dir_head = add_dir(dir, dir_head, argv[i]);
			}

			else if((file = fopen(argv[i], "r")) != NULL){
				file_head = add_file(argv[i], file_head);
			}

			//error message if file or directory could not be found
			else{
				fprintf(stderr, "lsal2: cannot access \'%s\': No such file or directory\n", path);
			}
		}

		int printed = print_files(file_head);
		//checks if a new line should be printed
		if(printed == 1){
			printf("\n");
		}

		print_directories(dir_head, argc);
	}

	return 0;//end of program
}

//adds the diretory entry into the linked list in alphabetical order
//returns pointer to begining of the list
struct node* add(struct dirent *dp, struct node *head){
	struct node* new_node = (struct node*)malloc(sizeof(struct node)); // allocates memory
	//sets fileds
	new_node->dp = dp;
	new_node->next = NULL;
	//checks if list is empty
	if(head == NULL){
		head = new_node;
	}

	//checks if the new_node should be infromt of the current head
	else if(strcmp(head->dp->d_name, new_node->dp->d_name) > 0){
		new_node->next = head;
		head = new_node;
	}

	//loops through the list and finds where to pput the new node
	else{
		struct node* temp = head;
		int added = 0; //checks if the node was added
		while(temp->next != NULL){
			int i;
			char str[strlen(temp->next->dp->d_name)+1];
			memset(str, 0, sizeof str);
			for(i = 0; i < strlen(temp->next->dp->d_name); i++){
				str[i] = tolower(temp->next->dp->d_name[i]);
			}
			char str2[strlen(new_node->dp->d_name)+1];
			memset(str2, 0, sizeof str2);
			for(i = 0; i < strlen(new_node->dp->d_name); i++){
				str2[i] = tolower(new_node->dp->d_name[i]);
			}
			if(strcmp(str, str2) > 0){
				new_node->next = temp->next;
				temp->next = new_node;
				added = 1;
				break;
			}

			else{
				temp = temp->next;
			}
		}

		if(added == 0){
			temp->next = new_node; // adds new node to the front of the list
		}
	}

	return head; // returns head
}

//adds file to list and returns pointer to fromt of list in alphabetical order
struct file_node* add_file(char* name, struct file_node *head){
	struct file_node *new_node = (struct file_node*)malloc(sizeof(struct file_node));
	new_node->name = name;
	new_node->next = NULL;
	if(head == NULL){
		head = new_node;
	}

	else if(strcmp(head->name, new_node->name) > 0){
		new_node->next = head;
		head = new_node;
	}

	else{
		struct file_node* temp = head;
		int added = 0;
		while(temp->next != NULL){
			int i;
			char str[strlen(temp->next->name)+1];
			memset(str, 0, sizeof str);
			for(i = 0; i < strlen(temp->next->name); i++){
				str[i] = tolower(temp->next->name[i]);
			}
			char str2[strlen(new_node->name)+1];
			memset(str2, 0, sizeof str2);
			for(i = 0; i < strlen(new_node->name); i++){
				str2[i] = tolower(new_node->name[i]);
			}
			if(strcmp(str, str2) > 0){
				new_node->next = temp->next;
				temp->next = new_node;
				added = 1;
				break;
			}

			else{
				temp = temp->next;
			}
		}

		if(added == 0){
			temp->next = new_node;
		}
	}

	return head;
}

//adds directory to list in alphabetical order 
//returns pointer to beging of the list
struct dir_node* add_dir(DIR *dir, struct dir_node *dir_head, char * name){
	struct dir_node *new_node = (struct dir_node*)malloc(sizeof(struct dir_node));
	new_node->dir = dir;
	new_node->name = name;
	new_node->next = NULL;
	new_node->dp = NULL;
	if(dir_head == NULL){
		dir_head = new_node;
	}

	else if(strcmp(dir_head->name, new_node->name) > 0){
		new_node->next = dir_head;
		dir_head = new_node;
	}

	else{
		struct dir_node* temp = dir_head;
		int added = 0;
		while(temp->next != NULL){
			int i;
			char str[strlen(temp->next->name)+1];
			memset(str, 0, sizeof str);
			for(i = 0; i < strlen(temp->next->name); i++){
				str[i] = tolower(temp->next->name[i]);
			}
			char str2[strlen(new_node->name)+1];
			memset(str2, 0, sizeof str2);
			for(i = 0; i < strlen(new_node->name); i++){
				str2[i] = tolower(new_node->name[i]);
			}
			if(strcmp(str, str2) > 0){
				new_node->next = temp->next;
				temp->next = new_node;
				added = 1;
				break;
			}

			else{
				temp = temp->next;
			}
		}

		if(added == 0){
			temp->next = new_node;
		}
	}

	return dir_head;
}

// prints files
// returns 1 if files were printed
int print_files(struct file_node *head){
	struct stat stat; // holds the statuses of the file and directories
	struct passwd *pw; // pointes to the password uid
	struct group *gr; // points to the group id
	struct tm *p; // points to a time structure for formating
	char s[1000]; // holds the time as a string
	struct file_node *temp = head;
	if(head != NULL){
		while(temp != NULL){
			lstat(temp->name, &stat);
			printf( (S_ISDIR(stat.st_mode)) ? "d" : "-");
    		printf( (stat.st_mode & S_IRUSR) ? "r" : "-");
    		printf( (stat.st_mode & S_IWUSR) ? "w" : "-");
   			printf( (stat.st_mode & S_IXUSR) ? "x" : "-");
    		printf( (stat.st_mode & S_IRGRP) ? "r" : "-");
    		printf( (stat.st_mode & S_IWGRP) ? "w" : "-");
    		printf( (stat.st_mode & S_IXGRP) ? "x" : "-");
    		printf( (stat.st_mode & S_IROTH) ? "r" : "-");
    		printf( (stat.st_mode & S_IWOTH) ? "w" : "-");
    		printf( (stat.st_mode & S_IXOTH) ? "x" : "-");
    		printf(" %d", stat.st_nlink);
    		pw = getpwuid(stat.st_uid);
    		printf(" %s", pw->pw_name);
    		gr = getgrgid(stat.st_gid);
   			printf(" %s", gr->gr_name);
   			printf(" %ld", stat.st_size);
   			p = localtime(&stat.st_mtime);
   			strftime(s, 1000, "%b %d %H:%M", p);
    		printf(" %s %s\n", s, temp->name);
    		temp = temp->next;
    		count++;
		}

		return 1;
	}

	return 0;
}

//prints directories
void print_directories(struct dir_node *dir_head, int argc){
	struct stat stat; // holds the statuses of the file and directories
	struct passwd *pw; // pointes to the password uid
	struct group *gr; // points to the group id
	struct tm *p; // points to a time structure for formating
	char s[1000]; // holds the time as a string
	struct dirent *dp;
	char real_path[1000];
	char *abs_path;
	if(dir_head != NULL){ 
		struct dir_node *temp_dir = dir_head;
		while(temp_dir != NULL){
			struct node *head = NULL;
			while((dp = readdir(temp_dir->dir)) != NULL){
				head = add(dp, head);
			}
			temp_dir->dp = head;
			temp_dir = temp_dir->next;
		}

		temp_dir = dir_head;

		while(temp_dir != NULL){
			struct dir_node *size = temp_dir;
			int total_size = 0;
			while(size != NULL){
				memset(real_path, 0, sizeof real_path);
				abs_path = strcpy(real_path, temp_dir->name);
				strcat(abs_path, "/");
				strcat(abs_path, temp_dir->dp->dp->d_name);
				lstat(abs_path, &stat);
				int file_size = stat.st_blocks;
				total_size += file_size;
				size = size->next;
			}

			printf("%s:\n", temp_dir->name);
			printf("total %d\n", total_size);
			while(temp_dir->dp != NULL){
				memset(real_path, 0, sizeof real_path);
				abs_path = strcpy(real_path, temp_dir->name);
				strcat(abs_path, "/");
				strcat(abs_path, temp_dir->dp->dp->d_name);
				lstat(abs_path, &stat);
				if(S_ISDIR(stat.st_mode) || S_ISREG(stat.st_mode)){
					printf( (S_ISDIR(stat.st_mode)) ? "d" : "-");
    				printf( (stat.st_mode & S_IRUSR) ? "r" : "-");
    				printf( (stat.st_mode & S_IWUSR) ? "w" : "-");
   					printf( (stat.st_mode & S_IXUSR) ? "x" : "-");
    				printf( (stat.st_mode & S_IRGRP) ? "r" : "-");
    				printf( (stat.st_mode & S_IWGRP) ? "w" : "-");
    				printf( (stat.st_mode & S_IXGRP) ? "x" : "-");
    				printf( (stat.st_mode & S_IROTH) ? "r" : "-");
    				printf( (stat.st_mode & S_IWOTH) ? "w" : "-");
    				printf( (stat.st_mode & S_IXOTH) ? "x" : "-");
    				printf(" %d", stat.st_nlink);
    				pw = getpwuid(stat.st_uid);
    				printf(" %s", pw->pw_name);
    				gr = getgrgid(stat.st_gid);
   					printf(" %s", gr->gr_name);
   					printf(" %ld", stat.st_size);
   					p = localtime(&stat.st_mtime);
   					strftime(s, 1000, "%b %d %H:%M", p);
    				printf(" %s %s\n", s, temp_dir->dp->dp->d_name);
    				temp_dir->dp = temp_dir->dp->next;
    			}
			}
			count++;
			//cehecks if a new line should be printed
			if(count != (argc-1)){
				printf("\n");
			}

			temp_dir = temp_dir->next;
		}
	}
}