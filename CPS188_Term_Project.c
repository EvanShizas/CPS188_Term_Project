/*
 * CPS188_Term_Project.c
 * 
 * Copyright 2023 
 * 
 * Fergus Chui <fergus.chui@torontomu.ca>
 * Talha Khalid <talha.khalid@torontomu.ca>
 * Jonathan Ly <jonathan.tri.ly@torontomu.ca>
 * Evangelos Shizas <eshizas@torontomu.ca>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 100
#define BUFF_MAX 200
#define COLUMN_SIZE 5
#define START_YEAR 2015
#define END_YEAR 2021

//Finds the number of data entry rows
int dataArrayInit() {
	int rowCount = 0;
	char in;
	FILE *rowCheck = fopen("statscan_diabetes.csv", "r");
	
	//File validation...
	if (rowCheck == NULL) {
		printf("Error: File could not be opened! Try again!");
		return 1;
	}
	
	for (in = getc(rowCheck); in != EOF; in = getc(rowCheck)) {
		if (in == '\n') {
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
	for (int i = 1; i < row; i++) {
		for (int j = 0; j < MAX_CHAR; j++) {
			if (dataArray[i][j] == '\"' && cleaningCharacters) {
				cleaningCharacters = false;
				dataArray[i][j-1] = '\0';
				dataArray[i][j] = '\0';
				break;
			}
			
			else if (dataArray[i][j] == '\"' || cleaningCharacters) {
				cleaningCharacters = true;
				dataArray[i][j] = dataArray[i][j+1];
			}
		}
	}
	
	//Marks the entries left blank with a "special" identifying character...
	for (int i = 1; i < row; i++) {
		if (dataArray[i][0] == '\0') {
			dataArray[i][0] = '@';
		}
	}
}

//Collects cell entries for each column needed
void dataArrayCollect(int row, int colCell, int indexCell, char dataArray[row][MAX_CHAR]) {
    int colCellDefault = colCell, indexCellDefault = indexCell, count = 0;
    char *token, lineBuffer[BUFF_MAX];
	FILE *input = fopen("statscan_diabetes.csv", "r"); //Reads each data row of the CSV file and stores the specified column in the array...
	
	while (fgets(lineBuffer, BUFF_MAX, input) != NULL) {
		colCell = colCellDefault, indexCell = indexCellDefault;	
		token = strtok(lineBuffer, ","); //Parse the row into fields separated by commas...
		
		while (token != NULL) {
			indexCell++;
			
			if (indexCell == colCell) {
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
double locationCondition(int row, int i, char dataArray[row][MAX_CHAR]) {
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
double ageGroupCondition(int row, int i, char dataArray[row][MAX_CHAR]) {
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
double genderCondition(int row, int i, char dataArray[row][MAX_CHAR]) {
	int r = -1;
	
	if (strcmp(dataArray[i], "Males") == 0)
		r = 41;
		
	else if (strcmp(dataArray[i], "Females") == 0)
		r = 42;
	
	return r;
}

//Calculates provincial averages for Questions 1a) & 1b)...
double provinceAverage(int row, int col, int digitLocation, double data[row][col]) {
	double sum = 0, count = 0;
	
	for (int i = 1; i < row; i++) {
		if(data[i][1] == digitLocation) {		   
			 if (data[i][4] != -1) {
				sum += data[i][4];
				count++;
				}
			}
		}
	
	return (sum/count);
}

//Calculates provincial averages for Question 1c)...
double yearlyAverages(int row, int col, double *countPtr, int digitLocation, int year, double data[row][col]) {
	double sumYr = 0;
	
	for (int i = 1; i < row; i++) {
		if (data[i][1] == digitLocation) {
			if (data[i][0] == year) {
				if (data[i][4] != -1) {
					sumYr += data[i][4];
					*countPtr += 1;
				}
			}
		}
	}
	
	return sumYr;
}

//Calculates age-group averages for Question 1d)...
double ageGroupAverages(int row, int col, double *countPtr, int digitLocation, int digitAgeGroup, double data[row][col]) {
	double sum = 0;
	
	for (int i = 1; i < row; i++) {
		if (data[i][2] == digitAgeGroup) {
			if (data[i][1] == digitLocation) {
				if (data[i][4] != -1) {
					sum += data[i][4];
					*countPtr += 1;
				}
			}
		}
	}
	
	return sum;
}

int main(void)
{
	int col = COLUMN_SIZE, row = dataArrayInit(), digitLocation, digitAgeGroup;
	double arrayStatsData[row][col], avgStatsData[8][END_YEAR-START_YEAR+1], sum = 0, count = 0, *countPtr = &count,
		avgNAYr15, avgNAYr16, avgNAYr17, avgNAYr18, avgNAYr19, avgNAYr20, avgNAYr21,
		avgQCYr15, avgQCYr16, avgQCYr17, avgQCYr18, avgQCYr19, avgQCYr20, avgQCYr21,
		avgONYr15, avgONYr16, avgONYr17, avgONYr18, avgONYr19, avgONYr20, avgONYr21,
		avgABYr15, avgABYr16, avgABYr17, avgABYr18, avgABYr19, avgABYr20, avgABYr21,
		avgBCYr15, avgBCYr16, avgBCYr17, avgBCYr18, avgBCYr19, avgBCYr20, avgBCYr21,
		avgAge31NA, avgAge32NA, avgAge33NA, avgAge31QC, avgAge32QC, avgAge33QC,
		avgAge31ON, avgAge32ON, avgAge33ON, avgAge31AB, avgAge32AB, avgAge33AB,
		avgAge31BC, avgAge32BC, avgAge33BC;
	char year[row][MAX_CHAR], location[row][MAX_CHAR], ageGroup[row][MAX_CHAR], gender[row][MAX_CHAR], sizePercent[row][MAX_CHAR];
	
	//Reads and collects the respective data for each category...
	dataArrayCollect(row, 1, 0, year);
	dataArrayCollect(row, 2, 0, location);
	dataArrayCollect(row, 4, 0, ageGroup);
	dataArrayCollect(row, 5, 0, gender);
	dataArrayCollect(row, 14, 0, sizePercent);
	
	//Transfers data into one multidimensional array...
	for (int i = 0; i < row; i++) {
		if (strcmp(year[i], "@") == 0)
			arrayStatsData[i][0] = -1;
		else
			arrayStatsData[i][0] = atof(year[i]);
			
		arrayStatsData[i][1] = locationCondition(row, i, location);
		arrayStatsData[i][2] = ageGroupCondition(row, i, ageGroup);
		arrayStatsData[i][3] = genderCondition(row, i, gender);
		
		if (strcmp(sizePercent[i], "@") == 0)
			arrayStatsData[i][4] = -1;
		else
			arrayStatsData[i][4] = atof(sizePercent[i]);
	}
	
	puts("Question 1a:\n"); //Question 1a:
    
    digitLocation = 22; //Quebec
    printf("Average for Quebec: %0.2lf%%\n", provinceAverage(row, col, digitLocation, arrayStatsData));
    
    digitLocation = 23; //Ontario
    printf("Average for Ontario: %0.2lf%%\n", provinceAverage(row, col, digitLocation, arrayStatsData));
    
    digitLocation = 24; //Alberta
    printf("Average for Alberta: %0.2lf%%\n", provinceAverage(row, col, digitLocation, arrayStatsData));
    
    digitLocation = 25; //British Columbia
    printf("Average for British Columbia: %0.2lf%%\n", provinceAverage(row, col, digitLocation, arrayStatsData));
    
    puts("\nQuestion 1b:\n"); //Question 1b:
    
    digitLocation = 21; //Canada (excluding territories)
    printf("National Average (Canada Excluding Territores): %0.2lf%%\n", provinceAverage(row, col, digitLocation, arrayStatsData));
    
    puts("\nQuestion 1c:\n"); //Question 1c:
    
    //Calculations for National Average...
    
    for (int i = START_YEAR; i < END_YEAR + 1; i++) {
		sum = yearlyAverages(row, col, countPtr, digitLocation, i, arrayStatsData);
		
		if (i == 2015)
			avgNAYr15 = sum/count;
		else if (i == 2016)
			avgNAYr16 = sum/count;
		else if (i == 2017)
			avgNAYr17 = sum/count;
		else if (i == 2018)
			avgNAYr18 = sum/count;
		else if (i == 2019)
			avgNAYr19 = sum/count;
		else if (i == 2020)
			avgNAYr20 = sum/count;
		else if (i == 2021)
			avgNAYr21 = sum/count;
			
		count = 0;
	}
	
    printf("National Average (Canada Excluding Territores) for 2015: %0.2lf%%\n", avgNAYr15);
    printf("National Average (Canada Excluding Territores) for 2016: %0.2lf%%\n", avgNAYr16);
    printf("National Average (Canada Excluding Territores) for 2017: %0.2lf%%\n", avgNAYr17);
    printf("National Average (Canada Excluding Territores) for 2018: %0.2lf%%\n", avgNAYr18);
    printf("National Average (Canada Excluding Territores) for 2019: %0.2lf%%\n", avgNAYr19);
    printf("National Average (Canada Excluding Territores) for 2020: %0.2lf%%\n", avgNAYr20);
    printf("National Average (Canada Excluding Territores) for 2021: %0.2lf%%\n\n", avgNAYr21);
    
    //Calculations for Quebec Average...
    
    digitLocation = 22;
    
    for (int i = START_YEAR; i < END_YEAR + 1; i++) {
		sum = yearlyAverages(row, col, countPtr, digitLocation, i, arrayStatsData);
		
		if (i == 2015)
			avgQCYr15 = sum/count;
		else if (i == 2016)
			avgQCYr16 = sum/count;
		else if (i == 2017)
			avgQCYr17 = sum/count;
		else if (i == 2018)
			avgQCYr18 = sum/count;
		else if (i == 2019)
			avgQCYr19 = sum/count;
		else if (i == 2020)
			avgQCYr20 = sum/count;
		else if (i == 2021)
			avgQCYr21 = sum/count;
			
		count = 0;
	}
    
    printf("Quebec Average (Canada Excluding Territores) for 2015: %0.2lf%%\n", avgQCYr15);
    printf("Quebec Average (Canada Excluding Territores) for 2016: %0.2lf%%\n", avgQCYr16);
    printf("Quebec Average (Canada Excluding Territores) for 2017: %0.2lf%%\n", avgQCYr17);
    printf("Quebec Average (Canada Excluding Territores) for 2018: %0.2lf%%\n", avgQCYr18);
    printf("Quebec Average (Canada Excluding Territores) for 2019: %0.2lf%%\n", avgQCYr19);
    printf("Quebec Average (Canada Excluding Territores) for 2020: %0.2lf%%\n", avgQCYr20);
    printf("Quebec Average (Canada Excluding Territores) for 2021: %0.2lf%%\n\n", avgQCYr21);
    
    //Calculations for Ontario Average...
    
    digitLocation = 23;
    
    for (int i = START_YEAR; i < END_YEAR + 1; i++) {
		sum = yearlyAverages(row, col, countPtr, digitLocation, i, arrayStatsData);
		
		if (i == 2015)
			avgONYr15 = sum/count;
		else if (i == 2016)
			avgONYr16 = sum/count;
		else if (i == 2017)
			avgONYr17 = sum/count;
		else if (i == 2018)
			avgONYr18 = sum/count;
		else if (i == 2019)
			avgONYr19 = sum/count;
		else if (i == 2020)
			avgONYr20 = sum/count;
		else if (i == 2021)
			avgONYr21 = sum/count;
			
		count = 0;
	}
    
    printf("Ontario Average (Canada Excluding Territores) for 2015: %0.2lf%%\n", avgONYr15);
    printf("Ontario Average (Canada Excluding Territores) for 2016: %0.2lf%%\n", avgONYr16);
    printf("Ontario Average (Canada Excluding Territores) for 2017: %0.2lf%%\n", avgONYr17);
    printf("Ontario Average (Canada Excluding Territores) for 2018: %0.2lf%%\n", avgONYr18);
    printf("Ontario Average (Canada Excluding Territores) for 2019: %0.2lf%%\n", avgONYr19);
    printf("Ontario Average (Canada Excluding Territores) for 2020: %0.2lf%%\n", avgONYr20);
    printf("Ontario Average (Canada Excluding Territores) for 2021: %0.2lf%%\n\n", avgONYr21);
    
    //Calculations for Alberta Average...
    
    digitLocation = 24;
    
    for (int i = START_YEAR; i < END_YEAR + 1; i++) {
		sum = yearlyAverages(row, col, countPtr, digitLocation, i, arrayStatsData);
		
		if (i == 2015)
			avgABYr15 = sum/count;
		else if (i == 2016)
			avgABYr16 = sum/count;
		else if (i == 2017)
			avgABYr17 = sum/count;
		else if (i == 2018)
			avgABYr18 = sum/count;
		else if (i == 2019)
			avgABYr19 = sum/count;
		else if (i == 2020)
			avgABYr20 = sum/count;
		else if (i == 2021)
			avgABYr21 = sum/count;
			
		count = 0;
	}
    
    printf("Alberta Average (Canada Excluding Territores) for 2015: %0.2lf%%\n", avgABYr15);
    printf("Alberta Average (Canada Excluding Territores) for 2016: %0.2lf%%\n", avgABYr16);
    printf("Alberta Average (Canada Excluding Territores) for 2017: %0.2lf%%\n", avgABYr17);
    printf("Alberta Average (Canada Excluding Territores) for 2018: %0.2lf%%\n", avgABYr18);
    printf("Alberta Average (Canada Excluding Territores) for 2019: %0.2lf%%\n", avgABYr19);
    printf("Alberta Average (Canada Excluding Territores) for 2020: %0.2lf%%\n", avgABYr20);
    printf("Alberta Average (Canada Excluding Territores) for 2021: %0.2lf%%\n\n", avgABYr21);
    
    //Calculations for British Columbia Average...
	
	digitLocation = 25;
	
	for (int i = START_YEAR; i < END_YEAR + 1; i++) {
		sum = yearlyAverages(row, col, countPtr, digitLocation, i, arrayStatsData);
		
		if (i == 2015)
			avgBCYr15 = sum/count;
		else if (i == 2016)
			avgBCYr16 = sum/count;
		else if (i == 2017)
			avgBCYr17 = sum/count;
		else if (i == 2018)
			avgBCYr18 = sum/count;
		else if (i == 2019)
			avgBCYr19 = sum/count;
		else if (i == 2020)
			avgBCYr20 = sum/count;
		else if (i == 2021)
			avgBCYr21 = sum/count;
			
		count = 0;
	}
	
	printf("British Columbia Average (Canada Excluding Territores) for 2015: %0.2lf%%\n", avgBCYr15);
    printf("British Columbia Average (Canada Excluding Territores) for 2016: %0.2lf%%\n", avgBCYr16);
    printf("British Columbia Average (Canada Excluding Territores) for 2017: %0.2lf%%\n", avgBCYr17);
    printf("British Columbia Average (Canada Excluding Territores) for 2018: %0.2lf%%\n", avgBCYr18);
    printf("British Columbia Average (Canada Excluding Territores) for 2019: %0.2lf%%\n", avgBCYr19);
    printf("British Columbia Average (Canada Excluding Territores) for 2020: %0.2lf%%\n", avgBCYr20);
    printf("British Columbia Average (Canada Excluding Territores) for 2021: %0.2lf%%\n", avgBCYr21);
	
	puts("\nQuestion 1d:\n"); //Question 1d:
	
	//Calculations for National Average...
	
	digitLocation = 21; //Canada (excluding territories)
	
	digitAgeGroup = 31; //35 to 49 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge31NA = sum/count;
    
    digitAgeGroup = 32; //50 to 64 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge32NA = sum/count;
    
    digitAgeGroup = 33; //65 years and over
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge33NA = sum/count;
    
    printf("National Average (Canada Excluding Territores) for Age Group (35-49): %0.2lf%%\n", avgAge31NA);
    printf("National Average (Canada Excluding Territores) for Age Group (50-64): %0.2lf%%\n", avgAge32NA);
    printf("National Average (Canada Excluding Territores) for Age Group (65+): %0.2lf%%\n\n", avgAge33NA);
	
	//Calculations for Quebec Average...
	
	digitLocation = 22; //Quebec
	
	digitAgeGroup = 31; //35 to 49 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge31QC = sum/count;
    
    digitAgeGroup = 32; //50 to 64 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge32QC = sum/count;
    
    digitAgeGroup = 33; //65 years and over
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge33QC = sum/count;
    
    printf("Quebec Average for Age Group (35-49): %0.2lf%%\n", avgAge31QC);
    printf("Quebec Average for Age Group (50-64): %0.2lf%%\n", avgAge32QC);
    printf("Quebec Average for Age Group (65+): %0.2lf%%\n\n", avgAge33QC);
	
	//Calculations for Ontario Average...
	
	digitLocation = 23; //Ontario
	
	digitAgeGroup = 31; //35 to 49 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge31ON = sum/count;
    
    digitAgeGroup = 32; //50 to 64 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge32ON = sum/count;
    
    digitAgeGroup = 33; //65 years and over
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge33ON = sum/count;
    
    printf("Ontario Average for Age Group (35-49): %0.2lf%%\n", avgAge31ON);
    printf("Ontario Average for Age Group (50-64): %0.2lf%%\n", avgAge32ON);
    printf("Ontario Average for Age Group (65+): %0.2lf%%\n\n", avgAge33ON);
	
	//Calculations for Alberta Average...
	
	digitLocation = 24; //Alberta
	
	digitAgeGroup = 31; //35 to 49 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge31AB = sum/count;
    
    digitAgeGroup = 32; //50 to 64 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge32AB = sum/count;
    
    digitAgeGroup = 33; //65 years and over
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge33AB = sum/count;
    
    printf("Alberta Average for Age Group (35-49): %0.2lf%%\n", avgAge31AB);
    printf("Alberta Average for Age Group (50-64): %0.2lf%%\n", avgAge32AB);
    printf("Alberta Average for Age Group (65+): %0.2lf%%\n\n", avgAge33AB);
	
	//Calculations for British Columbia Average...
	
	digitLocation = 25; //British Columbia
	
	digitAgeGroup = 31; //35 to 49 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge31BC = sum/count;
    
    digitAgeGroup = 32; //50 to 64 years
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge32BC = sum/count;
    
    digitAgeGroup = 33; //65 years and over
	count = 0;
    sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
    avgAge33BC = sum/count;
    
    printf("British Columbia Average for Age Group (35-49): %0.2lf%%\n", avgAge31BC);
    printf("British Columbia Average for Age Group (50-64): %0.2lf%%\n", avgAge32BC);
    printf("British Columbia Average for Age Group (65+): %0.2lf%%\n\n", avgAge33BC);
	
	puts("\nQuestion 2:\n"); //Question 2:
	
	//PLACE CODE HERE>>>
	
	puts("\nQuestion 3:\n"); //Question 3:
	
	//PLACE CODE HERE>>>
	
	puts("\nQuestion 4:\n"); //Question 4:
	
	//PLACE CODE HERE>>> Evan will finish this part!
	
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
 * 0 -> Reference Year (2015 - 2021)
 * 1 -> Location (21.0 - 25.0)
 * 2 -> Age Group (31.0 - 33.0)
 * 3 -> Gender (41.0 - 42.0)
 * 4 -> Percent of Population (%)
 */
