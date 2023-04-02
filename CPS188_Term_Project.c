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
 * Important Details:
 * 
 * For any blank entries in the CSV file, an '@' symbol is used to denote it as empty; 
 * this will be substituted with a '-1' value in arrayStatsData[][]. All data 
 * for the questions will be retrieved from arrayStatsData[][] variable to
 * simplify casting double values. String data will be given specific 
 * designation digits which will be run through switch-case statements in 
 * order to match the original string to the data.
 * 
 * The following information cover the digit designations for all string statements:
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
 * 
 * The following information is about the storage behaviour of the constructed arrays:
 * 
 * arrayStatsData[row][col] -> {Year, Location, Age Group, Sex, (%) Value}
 * avgData[LOCATIONS] -> {Quebec (%), Ontario (%), Alberta (%), British Columbia (%), Canada (%)}
 * 
 * avgYearData[LOCATIONS][END_YEAR-START_YEAR]--
 * [0. Canada] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [1. Quebec] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [2. Ontario] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [3. Alberta] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [4. British Columbia] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * 
 * avgAgeData[LOCATIONS][AGE_GROUPS]--
 * [0. Canada] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [1. Quebec] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [2. Ontario] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [3. Alberta] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [4. British Columbia] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * 
 * NOTE: This important details section can be referred to at both the beginning and end of the program!
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
#define LOCATIONS 5
#define AGE_GROUPS 3

void programHeader() {
	puts("Term Project: Winter 2023");
	puts("Course: CPS188");
	puts("Section: 12");
	puts("Instructor: Dr. Sadaf Mustafiz");
	puts("TA: Reginald McLean");
	puts("\nBy: Fergus Chui, Talha Khalid, Jonathan Ly, Evangelos Shizas");
	puts("");
}

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

//Sets array values to "empty"...
void arrayReset(double percentData[LOCATIONS][END_YEAR-START_YEAR+1]) {
	for (int i = 0; i < LOCATIONS; i++) {
		for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
			percentData[i][j] = -1;
		}
	}
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

//Gives a specific location name based on designated value given...
char *locationStringFill(int digitLocation) {
	char *selectName;
	
	switch (digitLocation) {
		case -1:
			selectName = "Canada";
			break;
		
		case 0:
			selectName = "Quebec";
			break;
			
		case 1:
			selectName = "Ontario";
			break;
		
		case 2:
			selectName = "Alberta";
			break;
		
		case 3:
			selectName = "British Columbia";
			break;
		
		case 21:
			selectName = "National Average (excluding Territories)";
			break;
			
		case 22:
			selectName = "Quebec Average";
			break;
			
		case 23:
			selectName = "Ontario Average";
			break;
			
		case 24:
			selectName = "Alberta Average";
			break;
			
		case 25:
			selectName = "British Columbia Average";
			break;
			
		default:
			selectName = "#######";
			break;
	}
	
	return selectName;
}

//Writes all calculated data onto a file for GNUPlot (Questions 5 and 6)...
void dataWrite(double avgYearData[LOCATIONS][END_YEAR-START_YEAR+1], double avgAgeData[LOCATIONS][AGE_GROUPS]) {
	int digitYear = START_YEAR;
	char *ageGroup;
	FILE *write;
	
	//Question 5:
	for (int i = 0; i < LOCATIONS; i++) {
		digitYear = START_YEAR;
		
		switch (i) {
			case 0:
				write = fopen("Q.5 Canada AVG.txt", "w");
				break;
			case 1:
				write = fopen("Q.5 Quebec AVG.txt", "w");
				break;
			case 2:
				write = fopen("Q.5 Ontario AVG.txt", "w");
				break;
			case 3:
				write = fopen("Q.5 Alberta AVG.txt", "w");
				break;
			case 4:
				write = fopen("Q.5 British Columbia AVG.txt", "w");
				break;
			default:
				write = fopen("extra_data.txt", "w");
				break;
		}
		
		for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
			fprintf(write, "%d, %0.2lf", digitYear, avgYearData[i][j]);
			
			if (j+1 != END_YEAR-START_YEAR+1)
				fprintf(write, "\n");
			
			digitYear++;
		}
	}
	
	//Question 6:
	write = fopen("Q.6 Canada Age Group AVG.txt", "w");
		
	for (int i = 0; i < AGE_GROUPS; i++) {
		switch (i) {
			case 0:
				ageGroup = "35-49";
				break;
			case 1:
				ageGroup = "50-64"; 
				break;
			case 2:
				ageGroup = "65+";
				break;
			default:
				ageGroup = "#####";
				break;
		}
		
		fprintf(write, "%d, %s, %0.2lf", i, ageGroup, avgAgeData[0][i]);
		
		if (i+1 != AGE_GROUPS)
				fprintf(write, "\n");
	}
	
	fclose(write);
	
	printf("Success! All calculations have been performed and saved onto \n6 different files with question number followed by province as label\n ");
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
double locationAverage(int row, int col, int digitLocation, double data[row][col]) {
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
	int col = COLUMN_SIZE, row = dataArrayInit(), digitLocation, digitAgeGroup, digitYear;
	double arrayStatsData[row][col], avgData[LOCATIONS], avgYearData[LOCATIONS][END_YEAR-START_YEAR+1], 
		avgAgeData[LOCATIONS][AGE_GROUPS], percentData[LOCATIONS][END_YEAR-START_YEAR+1],
		sum = 0, count = 0, temp = 0, *countPtr = &count;
	bool clearCheck = false; //For formatting of question 4...
	char year[row][MAX_CHAR], location[row][MAX_CHAR], ageGroup[row][MAX_CHAR], gender[row][MAX_CHAR], sizePercent[row][MAX_CHAR],
		*locationName, *ageGroupString;
		
	programHeader();
	
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
	
	puts("Question 1a: ----------------------------------------------------------------------\n"); //Question 1a:
	
	digitLocation = 22; //Quebec
	
	for (int i = 0; i < LOCATIONS - 1; i++) {
		locationName = locationStringFill(digitLocation);
		
		avgData[i] = locationAverage(row, col, digitLocation, arrayStatsData);
		printf("Average for %s: %0.2lf%%\n", locationName, avgData[i]);
		digitLocation++;
	}
	
	puts("\nQuestion 1b: ----------------------------------------------------------------------\n"); //Question 1b:
	
	digitLocation = 21; //Canada (excluding territories)
	avgData[LOCATIONS-1] = locationAverage(row, col, digitLocation, arrayStatsData);
	printf("National Average (Canada Excluding Territories): %0.2lf%%\n", avgData[LOCATIONS-1]);
	
	puts("\nQuestion 1c: ----------------------------------------------------------------------\n"); //Question 1c:
	
	//Calculations for each locations average per year...
	for (int i = 0; i < LOCATIONS; i++) {
		digitYear = START_YEAR;
		
		locationName = locationStringFill(digitLocation);
		
		for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {			
			sum = yearlyAverages(row, col, countPtr, digitLocation, digitYear, arrayStatsData);
			avgYearData[i][j] = sum/count;
			printf("%s for %d: %0.2lf%%\n", locationName, digitYear, avgYearData[i][j]);
			
			count = 0;
			digitYear++;
		}
		
		puts("");
		digitLocation++;
	}
	
	puts("Question 1d: ----------------------------------------------------------------------\n"); //Question 1d:
	
	digitLocation = 21; //Canada (excluding territories)
	
	//Calculations for each locations average per age group...
	for (int i = 0; i < LOCATIONS; i++) {
		digitAgeGroup = 31;
		
		locationName = locationStringFill(digitLocation);
		
		for (int j = 0; j < AGE_GROUPS; j++) {
			switch (digitAgeGroup) {
			case 31:
				ageGroupString = "(35-49)";
				break;
				
			case 32:
				ageGroupString = "(50-64)";
				break;
				
			case 33:
				ageGroupString = "(65+)";
				break;
				
			default:
				ageGroupString = "####";
				break;
			}
			
			sum = ageGroupAverages(row, col, countPtr, digitLocation, digitAgeGroup, arrayStatsData);
			avgAgeData[i][j] = sum/count;
			printf("%s for Age Group %s: %0.2lf%%\n", locationName, ageGroupString, avgAgeData[i][j]);
			
			count = 0;
			digitAgeGroup++;
		}
		
		puts("");
		digitLocation++;
	}
	
	puts("Question 2: ----------------------------------------------------------------------\n"); //Question 2:
	
	//Calculations for highest diabetes...
	for (int i = 0; i < LOCATIONS - 1; i++) {
		if (avgData[i] > temp) {
			temp = avgData[i];
			digitLocation = i;
		}
	}
	
	locationName = locationStringFill(digitLocation);
	printf("Province with the highest diabetes rate: %s --> %.2lf%%\n", locationName, temp);
	
	//Calculations for lowest diabetes...
	for (int i = 0; i < LOCATIONS - 1; i++) {
		if (avgData[i] < temp) {
			temp = avgData[i];
			digitLocation = i;
		}
	}
	
	locationName = locationStringFill(digitLocation);
	printf("Province with the lowest diabetes rate: %s --> %.2lf%%\n", locationName, temp);
	
	puts("\nQuestion 3: ----------------------------------------------------------------------\n"); //Question 3:
	
	//Calculations for nationally higher/lower diabetes...
	for (int i = 0; i < LOCATIONS - 1; i++) {
		if (avgData[i] > avgData[LOCATIONS-1]) {
			printf("%s is above the national average!\n", locationStringFill(i));
		}
		
		else if (avgData[i] < avgData[LOCATIONS-1]) {
			printf("%s is below the national average!\n", locationStringFill(i));
		}
		
		else {
			printf("%s is equal to the national average!\n", locationStringFill(i));
		}
	}
	
	puts("\nQuestion 4: ----------------------------------------------------------------------\n"); //Question 4:
	
	//Re-initializing needed variables...
	temp = 0;
	arrayReset(percentData);
	
	//Calculations for highest diabetes in a year...
	for (int i = 1; i < LOCATIONS; i++) {
		for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
			//Resets all stored values and puts in the new corresponding data...
			if (avgYearData[i][j] > temp) {				
				temp = avgYearData[i][j];
				arrayReset(percentData);
				
				percentData[0][i-1] = i-1;
				percentData[i][j] = avgYearData[i][j];
			}
			
			//Collects like values and stores the locations and years...
			else if (avgYearData[i][j] == temp) {
				percentData[0][i-1] = i-1;
				percentData[i][j] = avgYearData[i][j];
			}
		}		
	}
	
	printf("Province(s) with the highest diabetes rate:\n"); 
	
	for (int i = 1; i < LOCATIONS; i++) {
		digitYear = START_YEAR;
		
		if (percentData[0][i-1] != -1) {
			printf("\n%s --> ", locationStringFill(percentData[0][i-1]));
		
			for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
				if (percentData[i][j] != -1) {
					//Cleans up the output format...
					switch (digitYear) {
						case START_YEAR:
							printf("%0.2lf%% (%d)", percentData[i][j], digitYear);
							clearCheck = true;
							break;
							
						default:
							if (clearCheck) //Allows for clean formatting...
								printf(" - %0.2lf%% (%d)", percentData[i][j], digitYear);
							else
								printf("%0.2lf%% (%d)", percentData[i][j], digitYear);
							break;
					}
				}
				
				digitYear++;
			}
		}
	}
	
	clearCheck = false;
	
	//Calculations for lowest diabetes in a year...
	for (int i = 1; i < LOCATIONS; i++) {
		for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
			if (avgYearData[i][j] < temp) {
				//Resets all stored values and puts in the new corresponding data...
				temp = avgYearData[i][j];
				arrayReset(percentData);
				
				percentData[0][i-1] = i-1;
				percentData[i][j] = avgYearData[i][j];
			}
			
			//Collects like values and stores the locations and years...
			else if (avgYearData[i][j] == temp) {
				percentData[0][i-1] = i-1;
				percentData[i][j] = avgYearData[i][j];
			}
		}		
	}
	
	printf("\n\nProvince(s) with the lowest diabetes rate:\n"); 
	
	for (int i = 1; i < LOCATIONS; i++) {
		digitYear = START_YEAR;
		
		if (percentData[0][i-1] != -1) {
			printf("\n%s --> ", locationStringFill(percentData[0][i-1]));
		
			for (int j = 0; j < END_YEAR-START_YEAR+1; j++) {
				if (percentData[i][j] != -1) {
					//Cleans up the output format...
					switch (digitYear) {
						case START_YEAR:
							printf("%0.2lf%% (%d)", percentData[i][j], digitYear);
							clearCheck = true;
							break;
							
						default: 
							if (clearCheck) //Allows for clean formatting...
								printf(" - %0.2lf%% (%d)", percentData[i][j], digitYear);
							else
								printf("%0.2lf%% (%d)", percentData[i][j], digitYear);
							break;
					}
				}
				
				digitYear++;
			}
		}
	}
	
	clearCheck = false;
	
	puts("\n\n----------------------------------------------------------------------\n");
	
	dataWrite(avgYearData, avgAgeData); //Writes all calculated data onto a txt file for GNUPlot graphing...
	
	printf("\nThank you for using our program!\nPlease continue by loading given scripts files onto GNUPlot to visually see the data calculated here...");
	
	return 0;
}

 /* Important Details:
 * 
 * For any blank entries in the CSV file, an '@' symbol is used to denote it as empty; 
 * this will be substituted with a '-1' value in arrayStatsData[][]. All data 
 * for the questions will be retrieved from arrayStatsData[][] variable to
 * simplify casting double values. String data will be given specific 
 * designation digits which will be run through switch-case statements in 
 * order to match the original string to the data.
 * 
 * The following information covers the digit designations for all string statements:
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
 * 
 * The following information is about the storage behaviour of the constructed arrays:
 * 
 * arrayStatsData[row][col] -> {Year, Location, Age Group, Sex, (%) Value}
 * avgData[LOCATIONS] -> {Quebec (%), Ontario (%), Alberta (%), British Columbia (%), Canada (%)}
 * 
 * avgYearData[LOCATIONS][END_YEAR-START_YEAR+1]--
 * [0. Canada] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [1. Quebec] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [2. Ontario] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [3. Alberta] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * [4. British Columbia] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
 * 
 * avgAgeData[LOCATIONS][AGE_GROUPS]--
 * [0. Canada] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [1. Quebec] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [2. Ontario] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [3. Alberta] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * [4. British Columbia] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
 * 
 * NOTE: This important details section can be referred to at both the beginning and end of the program!
 */
