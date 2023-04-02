# Project Repository Information
CPS188 - Term Project

This is going to be the project folder where all the work for this term project will be completed.
Make sure that all the work done at an individual-level is saved in the respective member's folder, allowing for commits to be made to the main program upon all members being in agreement on the changes/additions made.

# Important Details:

For any blank entries in the CSV file, an '@' symbol is used to denote it as empty; this will be substituted with a '-1' value in arrayStatsData[][]. All data for the questions will be retrieved from arrayStatsData[][] variable tosimplify casting double values. String data will be given specific designation digits which will be run through switch-case statements in order to match the original string to the data.
 
The following information covers the digit designations for all string statements:

- 21.0 -> "Canada (excluding territories)"
- 22.0 -> "Quebec"
- 23.0 -> "Ontario"
- 24.0 -> "Alberta"
- 25.0 -> "British Columbia"

- 31.0 -> "35 to 49 years"
- 32.0 -> "50 to 64 years"
- 33.0 -> "65 years and over"

- 41.0 -> "Males"
- 42.0 -> "Females"

The first digit represents the column in which the digit should be located in
in arrayStatsData[][], with the second digit being the differentiator between 
the possible string statements for that column.

The columns are organized as follows:

- 1 -> Reference Year (2015 - 2021)
- 2 -> Location (21.0 - 25.0)
- 3 -> Age Group (31.0 - 33.0)
- 4 -> Gender (41.0 - 42.0)
- 5 -> Percent of Population (%)

The following information is about the storage behaviour of the constructed arrays:

arrayStatsData[row][col]--
- {Year, Location, Age Group, Sex, (%) Value}

avgData[LOCATIONS]--
- {Quebec (%), Ontario (%), Alberta (%), British Columbia (%), Canada (%)}

avgYearData[LOCATIONS][END_YEAR-START_YEAR+1]--
- [0. Canada] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
- [1. Quebec] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
- [2. Ontario] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
- [3. Alberta] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}
- [4. British Columbia] -> {2015 (%), 2016 (%), 2017 (%), 2018 (%), 2019 (%), 2020 (%), 2021 (%)}

avgAgeData[LOCATIONS][AGE_GROUPS]--
- [0. Canada] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
- [1. Quebec] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
- [2. Ontario] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
- [3. Alberta] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}
- [4. British Columbia] -> {35 to 49 years (%), 50 to 64 years (%), 65 years and over (%)}

NOTE: This important details section can be referred to at both the beginning and end of the program!

This README.md will be modified as the project moves along; make sure to stay updated on any new additions!

- Evan Shizas
