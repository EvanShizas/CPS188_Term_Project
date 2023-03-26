#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 1000

int main(void) 
{
	FILE *statistic; //opens file 
	char data[size]; //array that stores the contents of the file	
	statistic = fopen("statscan_diabetes.csv", "r"); 
	
	char *sp; //string pointer
	
	//char REF_DATE[size], GEO[size], DGUID[size], Age_Group[size], Sex[size], Indicators[size], Characteristics[size], UOM[size], UOM_ID[size], SCALAR_FACTOR[size],
	//		SCALAR_ID[size], VECTOR[size], COORDINATE[size], VALUE[size], STATUS[size], SYMBOL[size], TERMINATED[size], DECIMALS[size];		
	
	char REF_DATE[size], GEO[size], DGUID[size], Age_Group[size], Sex[size], VALUE[size];

	//float VALUE; 

	while (fgets(data, size, statistic) != NULL)
	{
		int counter = 0;
		counter++;
	//printf("%s\n", data);
	//puts("");

		sp  = strtok(data, "," );	
		strcpy(REF_DATE, sp);

		sp  = strtok(NULL, "," );	
		strcpy(GEO, sp);
				
		sp  = strtok(NULL, "," );	
		strcpy(DGUID, sp);

		sp  = strtok(NULL, "," );	
		strcpy(Age_Group, sp);
				
		sp  = strtok(NULL, "," );	
		strcpy(Sex, sp);

		sp  = strtok(NULL, "," );	
		//strcpy(Indicators, sp);
		
		sp  = strtok(NULL, "," );	
	//	strcpy(Characteristics, sp);
		
		sp  = strtok(NULL, "," );	
	//	strcpy(UOM, sp);
		
		sp  = strtok(NULL, "," );	
	//	strcpy(UOM_ID, sp);
		
		sp  = strtok(NULL, "," );	
	//	strcpy(SCALAR_FACTOR, sp);

		sp  = strtok(NULL, "," );	
	//	strcpy(SCALAR_ID, sp);
		
		sp  = strtok(NULL, "," );	
		//strcpy(VECTOR, sp);
		
		sp  = strtok(NULL, "," );	
		//strcpy(COORDINATE, sp);

		sp  = strtok(NULL,  "," );	
		//VALUE = atoi(sp);
		strcpy(VALUE,sp);

		sp  = strtok(NULL, "," );	

		//strcpy(STATUS, sp);
		
		sp  = strtok(NULL, "," );	
		//strcpy(SYMBOL, sp);
		
		sp  = strtok(NULL, "," );	
		//strcpy(TERMINATED, sp);
		
		sp  = strtok(NULL, " , " );	
		//strcpy(DECIMALS, sp);
		

		printf("%s  %s  %s  %s  %s  %s\n", 
		REF_DATE, GEO, DGUID, Age_Group, Sex, VALUE);


	}
	

	
	fclose(statistic);
	
	return 0;
}
