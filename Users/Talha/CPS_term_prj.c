#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int lineSize = 211;
    int fieldSize = 500;
    char line[lineSize];





    FILE *ref_file;
    char ref_dates[1000][fieldSize]; //array for refrence dates
    ref_file = fopen("statscan_diabetes.csv", "r");
    // Read each data row of the CSV file and store the specified column in the array
    int ref_counter = 0;
    char *ref_ptr;
    while (fgets(line, lineSize, ref_file) != NULL) {
		int column_num = 1;
        int field_num = 0;

        // Parse the row into fields separated by commas
        ref_ptr = strtok(line, ",");
        while (ref_ptr != NULL) {
            field_num++;
            if (field_num == column_num) {
                strncpy(ref_dates[ref_counter], ref_ptr, fieldSize);
                ref_counter++;
                break;
            }
            ref_ptr = strtok(NULL, ",");
        }
    }
    fclose(ref_file);
    
    
    
    
    
    
    
    
    FILE *geo_file;
    geo_file = fopen("statscan_diabetes.csv", "r");
    char geo[1000][fieldSize];
    int geo_counter = 0;
    char *geo_ptr;
    while (fgets(line, lineSize, geo_file) != NULL) {
		int column_num = 2;
        int field_num = 0;

        // Parse the row into fields separated by commas
        geo_ptr = strtok(line, ",");
        while (geo_ptr != NULL) {
            field_num++;
            if (field_num == column_num) {
                strncpy(geo[geo_counter], geo_ptr, fieldSize);
                geo_counter++;
                break;
            }
            geo_ptr = strtok(NULL, ",");
        }
    }
    fclose(geo_file);
    
    
    
    
    
    
    
    
    FILE *age_file;
    age_file = fopen("statscan_diabetes.csv", "r");
    char age[1000][fieldSize];
    int age_counter = 0;
    char *age_ptr;
    while (fgets(line, lineSize, age_file) != NULL) {
		int column_num = 4;
        int field_num = 0;

        // Parse the row into fields separated by commas
        age_ptr = strtok(line, ",");
        while (age_ptr != NULL) {
            field_num++;
            if (field_num == column_num) {
                strncpy(age[age_counter], age_ptr, fieldSize);
                age_counter++;
                break;
            }
            age_ptr = strtok(NULL, ",");
        }
    }
    fclose(age_file);
     
    
    
    
    
    
    FILE *sex_file;
    sex_file = fopen("statscan_diabetes.csv", "r");
    char sex[1000][fieldSize];
    int sex_counter = 0;
    char *sex_ptr;
    while (fgets(line, lineSize, sex_file) != NULL) {
		int column_num = 5;
        int field_num = 0;

        // Parse the row into fields separated by commas
        sex_ptr = strtok(line, ",");
        while (sex_ptr != NULL) {
            field_num++;
            if (field_num == column_num) {
                strncpy(sex[sex_counter], sex_ptr, fieldSize);
                sex_counter++;
                break;
            }
            sex_ptr = strtok(NULL, ",");
        }
    }
    fclose(sex_file);
    
    
     
    
    /*
    
    
    FILE *val_file;
    val_file = fopen("statscan_diabetes.csv", "r");
    char val[1000][fieldSize];
    int val_counter = 0;
    char *val_ptr;
    while (fgets(line, lineSize, val_file) != NULL) {
		int column_num = 14;
        int field_num = 0;

        // Parse the row into fields separated by commas
        val_ptr = strtok(line, ",");
        while (val_ptr != NULL) {
            field_num++;
            if (field_num == column_num) {
                strncpy(val[val_counter], val_ptr, fieldSize);
                val_counter++;
                break;
            }
            val_ptr = strtok(NULL, ",");
        }
    }
    fclose(val_file);
    
    
    */
    
    
    
    
    for (int i = 1; i < ref_counter; i++) {
        printf("%s\n", ref_dates[i]);
    }
    printf("\n");
    printf("\n");
    for (int i = 1; i < geo_counter; i++) {
        printf("%s\n", geo[i]);
    }
    printf("\n");
    printf("\n");
    for (int i = 1; i < age_counter; i++) {
        printf("%s\n", age[i]);
    }
    printf("\n");
    printf("\n");
    for (int i = 1; i < sex_counter; i++) {
        printf("%s\n", sex[i]);
    }
    /*
    printf("\n");
    printf("\n");
    for (int i = 1; i < val_counter; i++) {
        printf("%s\n", val[i]);
    }
    */
    return 0;
}
