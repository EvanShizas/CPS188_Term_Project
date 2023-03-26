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

double genderCondition(int row, int i, char dataArray[row][MAX_CHAR]) {
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
	for (int i = 0; i < row; i++) {
		arrayStatsData[i][0] = atof(year[i]);		
		arrayStatsData[i][1] = locationCondition(row, i, location);
		arrayStatsData[i][2] = ageGroupCondition(row, i, ageGroup);
		arrayStatsData[i][3] = genderCondition(row, i, gender);
		arrayStatsData[i][4] = atof(sizePercent[i]);
	}
	
	//debug junk -> delete later!
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
 */
