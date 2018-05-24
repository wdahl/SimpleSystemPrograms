#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//implementaion of a simplified version of the grep program in C called filegrep
int main(int argc, char *argv[]){
	//checks for the correct number of arguments
	if(argc != 3){
		//usage pattern for the program
		fprintf(stderr, "Usage: filegrep PATTERN FILE\n");
		return 0; //exits the program
	}

	FILE *file = fopen(argv[2], "r");//file pointer to file given in command line arguments
	char line[255]; // buffer for the line in the file
	int line_num = 0; // line count

	//checks if file was opened successfully
	if(file == 0){
		//error mesage
		fprintf(stderr, "filegrep: %s: No such file or directory\n", argv[2]);
		return 0;
	}

	//lopps through file line by line
	while(fgets(line, sizeof line, file) != NULL){
		line_num++; // increases line count
		//checks if the pattren is in the line of the file
		if((strstr(line, argv[1])) != NULL){
			// rpints the line conating the searched for pattern
			printf("%d:%s", line_num, line);
		}
	}

	fclose(file); // closes file
	return 0;
}