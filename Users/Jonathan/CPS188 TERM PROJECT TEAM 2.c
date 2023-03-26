#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 1000

/* DESCRIPTION: This is the alternative to Talha's code that I made, so far I am able to retrieve the string each line and store the terms seperated by "comma"
	  I was also able to convert the VALUE (percentage of each value from data) to a float number so we can use for our calculation. NOTE we dont have to use my
	  code. I was just playing around how to read csv files and put each term into its string and convert pointer into floats. There are also extra commented out codes
	  there that can be removed later. Encase we ever need the other columns, this code retrieves it all :)*/ 

int main(void) 
{
	FILE *statistic; //opens file 
	char data[size]; //array that stores the contents of the file	
	statistic = fopen("statscan_diabetes.csv", "r"); 
	
	char *sp; //string pointer
	
	char REF_DATE[size], GEO[size], DGUID[size], Age_Group[size], Sex[size], Indicators[size], Characteristics[size], UOM[size], UOM_ID[size], SCALAR_FACTOR[size],
			SCALAR_ID[size], VECTOR[size], COORDINATE[size], /*VALUE[size],*/ STATUS[size], SYMBOL[size], TERMINATED[size], DECIMALS[size];		
	
	//char REF_DATE[size], GEO[size], DGUID[size], Age_Group[size], Sex[size];
	float VALUE; 

	while (fgets(data, size, statistic) != NULL) //while loop that reads the statistic file and store it into a data array
	{
		//int counter = 0;
	    //counter++;
		//printf("%s\n", data);
		//puts("");

		sp  = strtok(data, "," ); //takes the first term (token) before it hits the first ,
		strcpy(REF_DATE, sp); //copy contents of pointer into REF_DATE Array

		sp  = strtok(NULL, "," ); //read till it hits the next ,
		strcpy(GEO, sp);
				
		sp  = strtok(NULL, "," );
		strcpy(DGUID, sp);

		sp  = strtok(NULL, "," );		
		strcpy(Age_Group, sp);
				
		sp  = strtok(NULL, "," );	
		strcpy(Sex, sp);

		sp  = strtok(NULL, "," );	
		strcpy(Indicators, sp);
		
		sp  = strtok(NULL, "," );		
		strcpy(Characteristics, sp);
		
		sp  = strtok(NULL, "," );	
		strcpy(UOM, sp);
		
		sp  = strtok(NULL, "," );	
		strcpy(UOM_ID, sp);
		
		sp  = strtok(NULL, "," );		
		strcpy(SCALAR_FACTOR, sp);

		sp  = strtok(NULL, "," );	
		strcpy(SCALAR_ID, sp);
		
		sp  = strtok(NULL, "," );	
		strcpy(VECTOR, sp);
		
		sp  = strtok(NULL, "," );	
		strcpy(COORDINATE, sp);
		
		sp  = strtok(NULL, "\"" ); //removes the "
		VALUE = atof(sp); //converts the pointer into a float number to be used (THIS IS WHAT we need the percentage)
		//strcpy(VALUE, sp);

		sp  = strtok(NULL, "," );	
		strcpy(STATUS, sp);
		
		sp  = strtok(NULL, "," );	
		strcpy(SYMBOL, sp);
		
		sp  = strtok(NULL, "," );		
		strcpy(TERMINATED, sp);
		
		sp  = strtok(NULL, "," );		
		strcpy(DECIMALS, sp);
		
		printf("%s  %s  %s  %s  %s  %0.1lf  \n", 
		REF_DATE, GEO, DGUID, Age_Group, Sex, VALUE); //prints out the important stuff
		
	//printf("%s  %s  %s  %s  %s  %s  %s  %s  %s  %s %s  %s  %s %0.2lf  %s %s  %s  %s\n", REF_DATE, GEO, DGUID, Age_Group, Sex, Indicators, Characteristics, UOM, UOM_ID, SCALAR_FACTOR,
	//	SCALAR_ID, VECTOR, COORDINATE, VALUE, STATUS, SYMBOL, TERMINATED, DECIMALS);
	}
		
	fclose(statistic);
	
	return 0;
}
