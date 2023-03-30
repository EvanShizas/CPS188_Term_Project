#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 100
#define BUFF_MAX 200

//Finds the number of data entry rows
int dataArrayInit() 
{
	int rowCount = 0;
	char in;
	FILE *rowCheck = fopen("statscan_diabetes.csv", "r");
	
	//File validation...
	if (rowCheck == NULL) 
	{
		printf("Error: File could not be opened! Try again!");
		return 1;
	}
	
	for (in = getc(rowCheck); in != EOF; in = getc(rowCheck)) 
	{
		if (in == '\n') 
		{
			rowCount++;
		}
	}
	
	fclose(rowCheck);	
	return rowCount;
}

//Removes "junk" characters from arrays
void arrayCharClean(int row, char dataArray[row][MAX_CHAR]) {
	bool cleaningCharacters = false;
	
	//Removes the quotation marks from the entries...
	for (int i = 1; i < row; i++) 
	{
		for (int j = 0; j < MAX_CHAR; j++) 
			{
				if (dataArray[i][j] == '\"' && cleaningCharacters) 
				{
					cleaningCharacters = false;
					dataArray[i][j-1] = '\0';
					dataArray[i][j] = '\0';
					break;
				}
			
				else if (dataArray[i][j] == '\"' || cleaningCharacters) 
				{
					cleaningCharacters = true;
					dataArray[i][j] = dataArray[i][j+1];
				}
		}
	}
	
	//Marks the entries left blank with a "special" identifying character...
	for (int i = 1; i < row; i++) 
	{
		if (dataArray[i][0] == '\0') 
		{
			dataArray[i][0] = '@';
		}
	}
}

//Collects cell entries for each column needed
void dataArrayCollect(int row, int colCell, int indexCell, char dataArray[row][MAX_CHAR]) 
{
    int colCellDefault = colCell, indexCellDefault = indexCell, count = 0;
    char *token, lineBuffer[BUFF_MAX];
	FILE *input = fopen("statscan_diabetes.csv", "r"); //Reads each data row of the CSV file and stores the specified column in the array...
	
	while (fgets(lineBuffer, BUFF_MAX, input) != NULL) 
	{
		colCell = colCellDefault, indexCell = indexCellDefault;	
		token = strtok(lineBuffer, ","); //Parse the row into fields separated by commas...
		
		while (token != NULL) 
		{
			indexCell++;
			
			if (indexCell == colCell) 
			{
				strncpy(dataArray[count], token, BUFF_MAX);
				count++;
				break;
			}
			
			token = strtok(NULL, ",");
		}
	}
    
    fclose(input);
    arrayCharClean(row, dataArray);
}

//Gives designated value to a specific string statement...
double locationCondition(int row, int i, char dataArray[row][MAX_CHAR]) 
{
	int r = -1;
	
	if (strcmp(dataArray[i], "Canada (excluding territories)") == 0)
		r = 21;
	
	else if (strcmp(dataArray[i], "Quebec") == 0)
		r = 22;
	
	else if (strcmp(dataArray[i], "Ontario") == 0)
		r = 23;
	
	else if (strcmp(dataArray[i], "Alberta") == 0)
		r = 24;
	
	else if (strcmp(dataArray[i], "British Columbia") == 0)
		r = 25;
		
	return r;
}

//Gives designated value to a specific string statement...
double ageGroupCondition(int row, int i, char dataArray[row][MAX_CHAR]) 
{
	int r = -1;
	
	if (strcmp(dataArray[i], "35 to 49 years") == 0)
		r = 31;
		
	else if (strcmp(dataArray[i], "50 to 64 years") == 0)
		r = 32;
	
	else if (strcmp(dataArray[i], "65 years and over") == 0)
		r = 33;
	
	return r;
}

//Gives designated value to a specific string statement...
double genderCondition(int row, int i, char dataArray[row][MAX_CHAR]) 
{
	int r = -1;
	
	if (strcmp(dataArray[i], "Males") == 0)
		r = 41;
		
	else if (strcmp(dataArray[i], "Females") == 0)
		r = 42;
	
	return r;
}

int main(void)
{
	int col = 5, row = dataArrayInit();
	double arrayStatsData[row][col];
	char year[row][MAX_CHAR], location[row][MAX_CHAR], ageGroup[row][MAX_CHAR], gender[row][MAX_CHAR], sizePercent[row][MAX_CHAR];
	
	//Seems to have fixed itself. Keep incase all goes to shit again!
	//char *ptr[MAX_CHAR]; //Using this keeps a strange memory crapshit from happening apparently...
	
	//Reads and collects the respective data for each category...
	dataArrayCollect(row, 1, 0, year);
	dataArrayCollect(row, 2, 0, location);
	dataArrayCollect(row, 4, 0, ageGroup);
	dataArrayCollect(row, 5, 0, gender);
	dataArrayCollect(row, 14, 0, sizePercent);
	
	//Transfers data into one multidimensional array...
	for (int i = 0; i < row; i++) 
	{
		if (strcmp(year[i], "@") == 0)
			arrayStatsData[i][0] = -1;
		else
			arrayStatsData[i][0] = atof(year[i]);
			
		arrayStatsData[i][1] = locationCondition(row, i, location);
		arrayStatsData[i][2] = ageGroupCondition(row, i, ageGroup);
		arrayStatsData[i][3] = genderCondition(row, i, gender);
		
		if (strcmp(sizePercent[i], "@") == 0)
		{
			arrayStatsData[i][4] = -1;
		}
		else
			arrayStatsData[i][4] = atof(sizePercent[i]);
		
	}

//CALCULATION FOR PROVINCIAL AVG - By Jonathan 
	puts("QUESTION 1a:");
//Printing Quebec AVG		 
	double sumQC = 0,  cQC = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 22.0)
        {		   
			 if (arrayStatsData[i][4] != -1)
			{
				sumQC += arrayStatsData[i][4];
				cQC++;
			}
        }    
    }    		
    printf("The Percent Average for Quebec: %0.2lf\n", sumQC/ cQC);
    //printf("%0.0lf", cQC);

//Printing Ontario AVG
	double sumON = 0,  cON = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 23.0)
        {		
			if (arrayStatsData[i][4] != -1)
			{
				sumON += arrayStatsData[i][4];
				cON++;
			}  
        }        
    }    		
    printf("The Percent Average for Ontario : %0.2lf\n", sumON/ cON);
    //printf("%0.0lf", cON);
    
//Printing Alberta AVG
	double sumAB = 0, cAB = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 24.0)
        {		
			if (arrayStatsData[i][4] != -1)
			{
				sumAB += arrayStatsData[i][4];
				cAB++;
			} 
        }            
    }    		
    printf("The Percent Average for Alberta: %0.2lf\n", sumAB/ cAB);
    //printf("%0.0lf", cAB);	
	
//Printing British Columbia AVG
    double sumBC = 0, cBC = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 25.0)
        {		
			if (arrayStatsData[i][4] != -1)
			{
				//printf("%lf", arrayStatsData[i][4]);
				sumBC += arrayStatsData[i][4];
				cBC++;
			} 

        }            
    }    		
    printf("The Percent Average for British Columbia: %0.2lf\n", sumBC/ cBC);
    puts("");
    //printf("%0.0lf", cBC);
    
    //Printing National AVG (Canada Excluding Territores)    
	puts("Question 1b: "); 	
    double sumNA = 0, cNA = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 21.0)
        {		
			if (arrayStatsData[i][4] != -1)
			{
				sumNA += arrayStatsData[i][4];
				cNA++;
			} 
        }            
    }   
	
    printf("The Percent Average for National (Canada Excluding Territores): %0.2lf\n", sumNA/ cNA);
    puts(" ");
    //printf("%0.0lf", cNA);
    
//CANADA National AVG 
    puts("Question 1c: "); 	
    double sumNAYr15 = 0, cNAYr15 = 0,sumNAYr16 = 0, cNAYr16 = 0, sumNAYr17 = 0, cNAYr17 = 0,
		   sumNAYr18 = 0, cNAYr18 = 0, sumNAYr19 = 0, cNAYr19 = 0, sumNAYr20 = 0, cNAYr20 = 0,
		   sumNAYr21 = 0, cNAYr21 = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2015)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr15 += arrayStatsData[i][4];
				cNAYr15++;
				} 
			} 
		}
		
		if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2016)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr16 += arrayStatsData[i][4];
				cNAYr16++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2017)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr17 += arrayStatsData[i][4];
				cNAYr17++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2018)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr18 += arrayStatsData[i][4];
				cNAYr18++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2019)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr19 += arrayStatsData[i][4];
				cNAYr19++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2020)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr20 += arrayStatsData[i][4];
				cNAYr20++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 21.0)
        {
			if (arrayStatsData[i][0] == 2021)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumNAYr21 += arrayStatsData[i][4];
				cNAYr21++;
				} 
			} 
		}
			           
    }   
	
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2015: %0.2lf\n", sumNAYr15/ cNAYr15);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2016: %0.2lf\n", sumNAYr16/ cNAYr16);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2017: %0.2lf\n", sumNAYr17/ cNAYr17);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2018: %0.2lf\n", sumNAYr18/ cNAYr18);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2019: %0.2lf\n", sumNAYr19/ cNAYr19);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2020: %0.2lf\n", sumNAYr20/ cNAYr20);
    printf("The Percent Average for National(Canada Excluding Territores)in the year 2021: %0.2lf\n", sumNAYr21/ cNAYr21);
    
// Annual Quebec AVG

    double sumQCYr15 = 0, cQCYr15 = 0,sumQCYr16 = 0, cQCYr16 = 0, sumQCYr17 = 0, cQCYr17 = 0,
		   sumQCYr18 = 0, cQCYr18 = 0, sumQCYr19 = 0, cQCYr19 = 0, sumQCYr20 = 0, cQCYr20 = 0,
		   sumQCYr21 = 0, cQCYr21 = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2015)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr15 += arrayStatsData[i][4];
				cQCYr15++;
				} 
			} 
		}
		
		if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2016)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr16 += arrayStatsData[i][4];
				cQCYr16++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2017)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr17 += arrayStatsData[i][4];
				cQCYr17++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2018)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr18 += arrayStatsData[i][4];
				cQCYr18++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2019)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr19 += arrayStatsData[i][4];
				cQCYr19++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2020)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr20 += arrayStatsData[i][4];
				cQCYr20++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 22.0)
        {
			if (arrayStatsData[i][0] == 2021)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumQCYr21 += arrayStatsData[i][4];
				cQCYr21++;
				} 
			} 
		}
			           
    }   
	puts(" ");
    printf("The Percent Average for Quebec in the year 2015: %0.2lf\n", sumQCYr15/ cQCYr15);
    printf("The Percent Average for Quebec in the year 2016: %0.2lf\n", sumQCYr16/ cQCYr16);
    printf("The Percent Average for Quebec in the year 2017: %0.2lf\n", sumQCYr17/ cQCYr17);
    printf("The Percent Average for Quebec in the year 2018: %0.2lf\n", sumQCYr18/ cQCYr18);
    printf("The Percent Average for Quebec in the year 2019: %0.2lf\n", sumQCYr19/ cQCYr19);
    printf("The Percent Average for Quebec in the year 2020: %0.2lf\n", sumQCYr20/ cQCYr20);
    printf("The Percent Average for Quebec in the year 2021: %0.2lf\n", sumQCYr21/ cQCYr21);
    
//ONTARIO ANNUAL AVG
    double sumONYr15 = 0, cONYr15 = 0,sumONYr16 = 0, cONYr16 = 0, sumONYr17 = 0, cONYr17 = 0,
		   sumONYr18 = 0, cONYr18 = 0, sumONYr19 = 0, cONYr19 = 0, sumONYr20 = 0, cONYr20 = 0,
		   sumONYr21 = 0, cONYr21 = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2015)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr15 += arrayStatsData[i][4];
				cONYr15++;
				} 
			} 
		}
		
		if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2016)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr16 += arrayStatsData[i][4];
				cONYr16++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2017)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr17 += arrayStatsData[i][4];
				cONYr17++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2018)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr18 += arrayStatsData[i][4];
				cONYr18++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2019)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr19 += arrayStatsData[i][4];
				cONYr19++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2020)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr20 += arrayStatsData[i][4];
				cONYr20++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 23.0)
        {
			if (arrayStatsData[i][0] == 2021)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumONYr21 += arrayStatsData[i][4];
				cONYr21++;
				} 
			} 
		}
			           
    }   
	puts(" ");
    printf("The Percent Average for Ontario in the year 2015: %0.2lf\n", sumONYr15/ cONYr15);
    printf("The Percent Average for Ontario in the year 2016: %0.2lf\n", sumONYr16/ cONYr16);
    printf("The Percent Average for Ontario in the year 2017: %0.2lf\n", sumONYr17/ cONYr17);
    printf("The Percent Average for Ontario in the year 2018: %0.2lf\n", sumONYr18/ cONYr18);
    printf("The Percent Average for Ontario in the year 2019: %0.2lf\n", sumONYr19/ cONYr19);
    printf("The Percent Average for Ontario in the year 2020: %0.2lf\n", sumONYr20/ cONYr20);
    printf("The Percent Average for Ontario in the year 2021: %0.2lf\n", sumONYr21/ cONYr21);
    
//ALBERTA ANNUAL AVG
    double sumABYr15 = 0, cABYr15 = 0,sumABYr16 = 0, cABYr16 = 0, sumABYr17 = 0, cABYr17 = 0,
		   sumABYr18 = 0, cABYr18 = 0, sumABYr19 = 0, cABYr19 = 0, sumABYr20 = 0, cABYr20 = 0,
		   sumABYr21 = 0, cABYr21 = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2015)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr15 += arrayStatsData[i][4];
				cABYr15++;
				} 
			} 
		}
		
		if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2016)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr16 += arrayStatsData[i][4];
				cABYr16++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2017)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr17 += arrayStatsData[i][4];
				cABYr17++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2018)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr18 += arrayStatsData[i][4];
				cABYr18++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2019)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr19 += arrayStatsData[i][4];
				cABYr19++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2020)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr20 += arrayStatsData[i][4];
				cABYr20++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 24.0)
        {
			if (arrayStatsData[i][0] == 2021)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumABYr21 += arrayStatsData[i][4];
				cABYr21++;
				} 
			} 
		}
			           
    }   
	puts(" ");
    printf("The Percent Average for Alberta in the year 2015: %0.2lf\n", sumABYr15/ cABYr15);
    printf("The Percent Average for Alberta in the year 2016: %0.2lf\n", sumABYr16/ cABYr16);
    printf("The Percent Average for Alberta in the year 2017: %0.2lf\n", sumABYr17/ cABYr17);
    printf("The Percent Average for Alberta in the year 2018: %0.2lf\n", sumABYr18/ cABYr18);
    printf("The Percent Average for Alberta in the year 2019: %0.2lf\n", sumABYr19/ cABYr19);
    printf("The Percent Average for Alberta in the year 2020: %0.2lf\n", sumABYr20/ cABYr20);
    printf("The Percent Average for Alberta in the year 2021: %0.2lf\n", sumABYr21/ cABYr21);
 
 //BRITISH COLUMBIA ANNUAL PERCENTAGE
    double sumBCYr15 = 0, cBCYr15 = 0,sumBCYr16 = 0, cBCYr16 = 0, sumBCYr17 = 0, cBCYr17 = 0,
		   sumBCYr18 = 0, cBCYr18 = 0, sumBCYr19 = 0, cBCYr19 = 0, sumBCYr20 = 0, cBCYr20 = 0,
		   sumBCYr21 = 0, cBCYr21 = 0;
    for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2015)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr15 += arrayStatsData[i][4];
				cBCYr15++;
				} 
			} 
		}
		
		if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2016)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr16 += arrayStatsData[i][4];
				cBCYr16++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2017)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr17 += arrayStatsData[i][4];
				cBCYr17++;
				} 
			} 
		}

        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2018)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr18 += arrayStatsData[i][4];
				cBCYr18++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2019)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr19 += arrayStatsData[i][4];
				cBCYr19++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2020)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr20 += arrayStatsData[i][4];
				cBCYr20++;
				} 
			} 
		}
		
        if(arrayStatsData[i][1] == 25.0)
        {
			if (arrayStatsData[i][0] == 2021)
			{
				if (arrayStatsData[i][4] != -1)
				{
				sumBCYr21 += arrayStatsData[i][4];
				cBCYr21++;
				} 
			} 
		}
			           
    }   
	puts(" ");
    printf("The Percent Average for British Columbia in the year 2015: %0.2lf\n", sumBCYr15/ cBCYr15);
    printf("The Percent Average for British Columbia in the year 2016: %0.2lf\n", sumBCYr16/ cBCYr16);
    printf("The Percent Average for British Columbia in the year 2017: %0.2lf\n", sumBCYr17/ cBCYr17);
    printf("The Percent Average for British Columbia in the year 2018: %0.2lf\n", sumBCYr18/ cBCYr18);
    printf("The Percent Average for British Columbia in the year 2019: %0.2lf\n", sumBCYr19/ cBCYr19);
    printf("The Percent Average for British Columbia in the year 2020: %0.2lf\n", sumBCYr20/ cBCYr20);
    printf("The Percent Average for British Columbia in the year 2021: %0.2lf\n", sumBCYr21/ cBCYr21);
    
    puts(" ");
    puts("Question 1d:"); 

//AGE GROUP NATIONAL AVG
 double sumAge31NA = 0,  cAge31NA = 0, sumAge32NA = 0,  cAge32NA = 0, sumAge33NA = 0,  cAge33NA = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][2] == 31)
        {
			if(arrayStatsData[i][1] == 21.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge31NA += arrayStatsData[i][4];
					cAge31NA++;
				}  
			}
		}
		if(arrayStatsData[i][2] == 32)
        {
			if(arrayStatsData[i][1] == 21.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge32NA += arrayStatsData[i][4];
					cAge32NA++;
				}  
			}
		}    
		if(arrayStatsData[i][2] == 33)
        {
			if(arrayStatsData[i][1] == 21.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge33NA += arrayStatsData[i][4];
					cAge33NA++;
				}  
			}	
		}	   	    
	}	
    	
    printf("The Percent Average for National (Excluding Territories) with Age (35-49): %0.2lf\n", sumAge31NA/ cAge31NA);
    printf("The Percent Average for National (Excluding Territories) with Age (60-64): %0.2lf\n", sumAge32NA/ cAge32NA);
    printf("The Percent Average for National (Excluding Territories) with Age (65+): %0.2lf\n", sumAge33NA/ cAge33NA);
    puts(" ");
    
 //AGE GROUP QUEBEC AVG
  double sumAge31QC = 0,  cAge31QC = 0, sumAge32QC = 0,  cAge32QC = 0, sumAge33QC = 0,  cAge33QC = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][2] == 31)
        {
			if(arrayStatsData[i][1] == 22.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge31QC += arrayStatsData[i][4];
					cAge31QC++;
				}  
			}
		}
		if(arrayStatsData[i][2] == 32)
        {
			if(arrayStatsData[i][1] == 22.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge32QC += arrayStatsData[i][4];
					cAge32QC++;
				}  
			}
		}    
		if(arrayStatsData[i][2] == 33)
        {
			if(arrayStatsData[i][1] == 22.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge33QC += arrayStatsData[i][4];
					cAge33QC++;
				}  
			}	
		}	   	    
	}	
    	
    printf("The Percent Average for Quebec with Age (35-49): %0.2lf\n", sumAge31QC/ cAge31QC);
    printf("The Percent Average for Quebec with Age (60-64): %0.2lf\n", sumAge32QC/ cAge32QC);
    printf("The Percent Average for Quebec with Age (65+): %0.2lf\n", sumAge33QC/ cAge33QC);
    puts(" ");
       
 //AGE GROUP ONTARIO AVG
  double sumAge31ON = 0,  cAge31ON = 0, sumAge32ON = 0,  cAge32ON = 0, sumAge33ON = 0,  cAge33ON = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][2] == 31)
        {
			if(arrayStatsData[i][1] == 23.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge31ON += arrayStatsData[i][4];
					cAge31ON++;
				}  
			}
		}
		if(arrayStatsData[i][2] == 32)
        {
			if(arrayStatsData[i][1] == 23.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge32ON += arrayStatsData[i][4];
					cAge32ON++;
				}  
			}
		}    
		if(arrayStatsData[i][2] == 33)
        {
			if(arrayStatsData[i][1] == 23.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge33ON += arrayStatsData[i][4];
					cAge33ON++;
				}  
			}	
		}	   	    
	}	
    	
    printf("The Percent Average for Ontario with Age (35-49): %0.2lf\n", sumAge31ON/ cAge31ON);
    printf("The Percent Average for Ontario with Age (60-64): %0.2lf\n", sumAge32ON/ cAge32ON);
    printf("The Percent Average for Ontario with Age (65+): %0.2lf\n", sumAge33ON/ cAge33ON);
    puts(" ");
    
 //AGE GROUP ALBERTA AVG
   double sumAge31AB = 0,  cAge31AB = 0, sumAge32AB = 0,  cAge32AB = 0, sumAge33AB = 0,  cAge33AB = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][2] == 31)
        {
			if(arrayStatsData[i][1] == 24.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge31AB += arrayStatsData[i][4];
					cAge31AB++;
				}  
			}
		}
		if(arrayStatsData[i][2] == 32)
        {
			if(arrayStatsData[i][1] == 24.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge32AB += arrayStatsData[i][4];
					cAge32AB++;
				}  
			}
		}    
		if(arrayStatsData[i][2] == 33)
        {
			if(arrayStatsData[i][1] == 24.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge33AB += arrayStatsData[i][4];
					cAge33AB++;
				}  
			}	
		}	   	    
	}	
    	
    printf("The Percent Average for Alberta with Age (35-49): %0.2lf\n", sumAge31AB/ cAge31AB);
    printf("The Percent Average for Alberta with Age (60-64): %0.2lf\n", sumAge32AB/ cAge32AB);
    printf("The Percent Average for Alberta with Age (65+): %0.2lf\n", sumAge33AB/ cAge33AB);
    puts(" ");
    
//AGE GROUP BRITISH COLUMBIA AVG
   double sumAge31BC = 0,  cAge31BC = 0, sumAge32BC = 0,  cAge32BC = 0, sumAge33BC = 0,  cAge33BC = 0;
	for (int i = 1; i < row; i++) 
	{
        if(arrayStatsData[i][2] == 31)
        {
			if(arrayStatsData[i][1] == 25.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge31BC += arrayStatsData[i][4];
					cAge31BC++;
				}  
			}
		}
		if(arrayStatsData[i][2] == 32)
        {
			if(arrayStatsData[i][1] == 25.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge32BC += arrayStatsData[i][4];
					cAge32BC++;
				}  
			}
		}    
		if(arrayStatsData[i][2] == 33)
        {
			if(arrayStatsData[i][1] == 25.0)
			{		
				if (arrayStatsData[i][4] != -1)
				{
					sumAge33BC += arrayStatsData[i][4];
					cAge33BC++;
				}  
			}	
		}	   	    
	}	
    	
    printf("The Percent Average for British Columbia with Age (35-49): %0.2lf\n", sumAge31BC/ cAge31BC);
    printf("The Percent Average for British Columbia with Age (60-64): %0.2lf\n", sumAge32BC/ cAge32BC);
    printf("The Percent Average for British Columbia with Age (65+): %0.2lf\n", sumAge33BC/ cAge33BC);
    puts(" ");

//End of Jonathan's Work
    
 //TALHA PUT YOUR CODE HERE FOR QUESTION 2



    
    

    
    


    


    
	//DEBUG JUNK - DELETE LATER
	/*	
	printf("%d\n\n", row);
	
	for (int i = 1; i < row; i++) {
		printf("%s\n", sizePercent[i]);
		
	} 
	printf("----------------------------------------\n\n");
	for (int i = 1; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%lf || ", arrayStatsData[i][j]);
		}
		printf("\n");
	}
	*/
	return 0;
}

/* Important Details:
 * 
 * For any blank entries in the CSV have been denoted with an '@' symbol; 
 * this will be substituted with a '-1' value in arrayStatsData[][]. All data 
 * for the questions will be retrieved from arrayStatsData[][] variable to
 * simplify casting double values. String data will be given specific 
 * designation digits which will be run through switch-case statements in 
 * order to match the original string to the data.
 * 
 * The following information the digit designation for all string statements:
 * 
 * 21.0 -> "Canada (excluding territories)"
 * 22.0 -> "Quebec"
 * 23.0 -> "Ontario"
 * 24.0 -> "Alberta"
 * 25.0 -> "British Columbia"
 * 
 * 31.0 -> "35 to 49 years"
 * 32.0 -> "50 to 64 years"
 * 33.0 -> "65 years and over"
 * 
 * 41.0 -> "Males"
 * 42.0 -> "Females"
 * 
 * The first digit represents the column in which the digit should be located in
 * in arrayStatsData[][], with the second digit being the differentiator between 
 * the possible string statements for that column.
 * 
 * The columns are organized as follows:
 * 
 * 1 -> Reference Year (2015 - 2021)
 * 2 -> Location (21.0 - 25.0)
 * 3 -> Age Group (31.0 - 33.0)
 * 4 -> Gender (41.0 - 42.0)
 * 5 -> Percent of Population (%)
 * 
 * //JONATHAN'S CORRECTION FOR INDEX NUMBER FOR EACH COLUMN
 * 0 -> Reference Year (2015 - 2021)
 * 1 -> Location (21.0 - 25.0)
 * 2 -> Age Group (31.0 - 33.0)
 * 3 -> Gender (41.0 - 42.0)
 * 4 -> Percent of Population (%)
 */

