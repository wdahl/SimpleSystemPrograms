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

//holds direcotry entries
struct node{
	struct dirent *dp;
	struct node *next;
};

//protoypes
struct node* add(struct dirent *dp, struct node* head);

//ls -al program that can only take one argument
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
	struct node *head = NULL;
	char real_path[1000];
	char * abs_path;

	// checks if no arguments were given
	if(argc == 1){
		path = "."; // selts the path to the current directory
	}

	// sets path to the argument givin
	else{
		path = argv[1];
	}


	//checks if the path given was a directory
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

	//checks if the path given was a file
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

	//error message if file or directory could not be found
	else{
		fprintf(stderr, "lsal: cannot access \'%s\': No such file or directory\n", path);
	}

	return 0;//end of program
}

//adds entry to list in alphabetical order
//returns pointer to beging of list
struct node* add(struct dirent *dp, struct node *head){
	//allocates memory and sets filds
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	new_node->dp = dp;
	new_node->next = NULL;
	//checks if list is empty
	if(head == NULL){
		head = new_node;
	}

	//compares new_node and head 
	else if(strcmp(head->dp->d_name, new_node->dp->d_name) > 0){
		new_node->next = head;
		head = new_node;
	}

	//loops through list to find where the new node should be placed
	else{
		struct node* temp = head;
		int added = 0; // checks if the node was added
		while(temp->next != NULL){
			//adds node before temp next
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
		//adds node to the ned of the list
		if(added == 0){
			temp->next = new_node;
		}
	}

	return head; // returns head
}