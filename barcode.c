#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BARCODE_LENGTH 16
#define SIZE_OF_FILES 100000
//default barcode length

#ifdef USE_MALLOC_WRAPPERS
#  include "malloc_wrap.h"
#endif

int main_barcode(int argc, char *argv[]){
	//assume lines are never longer than 512 char
	char line1[512],line2[512];
	char *inputfiles[2]={0,0},*outputfile=0;
	FILE *inputfp1=0,*inputfp2=0,*outputfp=stdout;
	int count = 1;
	if(argc !=3 && argc !=5){
		fprintf(stderr,"The command is bwa barcode <-o outputFile> R1file R2file\n");
		return 1;
	} 
	
	//***********************************************************************
	//parse the command line
	//
	//check for case when there are 3 arguments (i.e. no -o flag)
	//
	//check the case when there are 5 arguments
	//check that there actually is a -o flag and an argument following it
	//********************************************************************
	if(argc == 3){
		inputfiles[0] = argv[1];
		inputfiles[1] = argv[2];
	}else if(argc == 5){
		fprintf(stdout, "1: %s, 2: %s, 3: %s, 4: %s", argv[1],argv[2],argv[3],argv[4]);
		if(strcmp(argv[1],"-o")==0){
			fprintf (stdout, "true");
			outputfile = argv[2];
			inputfiles[0] = argv[3];
			inputfiles[1] = argv[4];
		}else{
			fprintf(stderr,"The command is bwa barcode <-o outputFile> R1file R2file\n");
			return 1;
		}
	}
	
	if(inputfiles[0])fprintf(stderr,"reading in from %s\n",inputfiles[0]);
	else fprintf(stderr,"reading from stdin\n");
	if(inputfiles[1])fprintf(stderr,"reading in from %s\n",inputfiles[1]);
	else fprintf(stderr,"writing to stdout\n");
	//**********************************************************************
	//open the files
	//use fopen to open streams to the two input files and the output file if one is given
	//check that the open is successful	(i.e. that the file pointers are not 0)			
	//**********************************************************************
	if(inputfiles[0] && inputfiles[1]){
		inputfp1 = fopen(inputfiles[0],"r");
		if(!inputfp1) exit(0);
		inputfp2 = fopen(inputfiles[1], "r");
		if(!inputfp1) exit(0);
	}
	if(outputfile) outputfp = fopen(outputfile, "w");
	//**********************************************************************
	//now print out a new file which is the same as file2 with the exception that
	//the header lines have a colon and the first BARCODE_LENGTH characters of the 
	//sequence line from file 1
	//
	//HINT: this can be done with one while loop using fgets, strlen and fprintf
	//HINT:  use the modulo % operator to figure out which lines are sequence lines
	//**********************************************************************
	fgets(line1, SIZE_OF_FILES, inputfp1);
	while(fgets(line2, SIZE_OF_FILES, inputfp2)){
		fgets(line1, SIZE_OF_FILES, inputfp1);
		if ((count%4 == 1)) {
			line2[strlen(line2) -1] = ':';
			line1[16] = '\n';
			line1[17] = '\0';
			strcat(line2, line1);
			fprintf(outputfp, "%s", line2);
		} else {
			fprintf(outputfp,"%s",line2);
		}
		count++;
	}
	fclose(inputfp1);
	fclose(inputfp2);
	if (outputfp != stdout) fclose(outputfp);
	return 0;
}
