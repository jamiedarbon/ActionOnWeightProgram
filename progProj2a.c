/*********************************************************************
* progProj2a.c
* Allows verified users to input patient data.
* Candidate number: 218721
* 6th December 2019, Version 1.0.0
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NURSES 5
#define KEY 8

struct info
{
	int day;
	int month;
	int year;
	
	char name1[50];
	char name2[50];
	char file[500];
	
	int weight;
	int height;
	int waist;
	
	char comment[100];
} patient;

int login(FILE *loginUsername, FILE* loginPassword, char array[]);
int menu(FILE *loginUsername , FILE *loginPassword, 
struct info patient);
int enterDetails(struct info patient);
struct info encrypt(struct info patient);
void clearScreen();
int validation(struct info patient);
struct info inputDetails(struct info patient);
void outputDetails(struct info patient);
struct info fileEncrypt(struct info patient);
char decrypt(char* detailArray);
char* decryptPasswords(void);

int main(void)
{	
	struct info patient;

	FILE *loginUsername;
	FILE *loginPassword;
	int verify;
	
	char c, array[8][200];
    int i = 0, j = 0, k, l;
    
    FILE* valid;
    
    valid = fopen("passwords.txt", "r");
    
    fscanf(valid, "%[^\n]", &array[i]);
    c = fgetc(valid);
    while (c != EOF) 
    { 
        if(c == '\n')
        {
            i++;
        }
        fscanf(valid, "%[^\n]", &array[i]);
        c = fgetc(valid);
        j++;
    } 
    
    for(j = 0; j < i; j++)
    {
        for(k = 0; array[j][k] != '\0'; k++)
        {
            if(array[j][k] >= 'a' && array[j][k] <='z')
		    {
			if(array[j][k] <= 'g' && array[j][k] >='a')
			{
	    		array[j][k] = array[j][k] + 18;
	    	}
	    	else
			{
				array[j][k] = array[j][k] - 8;
			}
		    }
		    
		    if(array[j][k] >= 'A' && array[j][k] <='Z')
		    {
			if(array[j][k] <= 'G' && array[j][k] >='A')
			{
	    		array[j][k] = array[j][k] + 18;
	    	}
	    	else
			{
				array[j][k] = array[j][k] - 8;
			}
   		 }
		             
        }
    }
    
    printf("%s", array[0]);
	
	verify = login(loginUsername, loginPassword, *array);
	clearScreen();
	switch(verify)
	{
		case 0:
			printf("ERROR: Invalid username or password.");
			exit(1);
		
		case 1:
			menu(loginUsername, loginPassword, patient);
			break;
			
	}
}

int login(FILE *loginUsername, FILE* loginPassword, char* array)
{
	char usernames[NURSES][100];
    char passwords[NURSES][100]; 
	char inputUsername[100];
	char inputPassword[8];
	char input;
	int i, j, userNumber;
	int usernameVerify = 0;
	int passwordVerify = 0;
	int passNum = 0;
	int userNum = 0;
	
	fileCheck(loginUsername, loginPassword);
	loginUsername = fopen("usernames.txt", "r");
	
	strcpy(*passwords, array);
	
	printf("Please enter username: ");
	scanf("%s", &inputUsername);
	while(!feof(loginUsername))
	{
		fscanf(loginUsername, "%s", &usernames[userNum]);
		userNum++;
	}
    for(i = 0; i < NURSES; i++)
	{
		if(strcmp(usernames[i], inputUsername) == 0)
		{
			usernameVerify = 1;
			userNumber = i;
			printf("1");
			break;
		}
	}
	
	printf("Please enter password: \n");
	scanf("%s", &inputPassword);
    	if(strcmp(passwords[userNumber], inputPassword) == 0)
    	{
    		passwordVerify = 1;
    		printf("!");
    	}
	if((usernameVerify == 1) && (passwordVerify == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	fclose(loginUsername);
	fclose(loginPassword);
}

int fileCheck(FILE *loginUsername, FILE *loginPassword)
{
	loginUsername = fopen("usernames.txt", "r");
	loginPassword = fopen("passwords.txt", "r");
	if(!loginUsername)
	{
		printf("Unable to open usernames.txt\n");
		exit(1);
	}
	
	if(!loginPassword)
	{
		printf("Unable to open passwords.txt");
		exit(1);
	}
}

int menu(FILE *loginUsername , FILE *loginPassword, struct info patient)
{
    clearScreen();
	int selection;
	
	printf("ACTION ON WEIGHT PATIENT PROGRAM\n");
	printf("================================\n");
	printf("1 - Enter patient details\n");
	printf("2 - Exit program\n");
	printf("\n\nWhich function would you like to access?\n");
	scanf("%d", &selection);
	switch(selection)
	{
		case 1:
			enterDetails(patient);
			break;
		
		case 2:
			exit(2);
	}
}

int enterDetails(struct info patient)
{
    char null;
    int validate;
    
	clearScreen();
	
	FILE* output;
	
	patient = inputDetails(patient);
	
	validate = validation(patient);
	if(validate == 1)
	{
		printf("ERROR: Invalid input");
		exit(3);
	}
	
	snprintf(patient.file, 20, "%s%d%d%d.aow", patient.name2, 
    patient.day, patient.month, patient.year);
    
	outputDetails(patient);

	printf("Information written to %s", patient.file);
	
	menu(0, 0, patient);
}

struct info inputDetails(struct info patient)
{
	printf("Please enter the new patient's first name: ");
	scanf("%s", &patient.name1);
	getchar();
	printf("Please enter the new patient's last name: ");
	scanf("%s", &patient.name2);
	getchar();
	printf("Please enter the new patient's date of birth "
	"(dd/mm/yyyy): ");
	scanf("%d/%d/%d", &patient.day, &patient.month, &patient.year);
	getchar();
	printf("Please enter the new patient's height in cm: ");
	scanf("%d", &patient.height);
	getchar();
	printf("Please enter the new patient's weight in lbs: ");
	scanf("%d", &patient.weight);
	getchar();
	printf("Please enter the new patient's waist measurement in cm: ");
	scanf("%d", &patient.waist);
	getchar();
	printf("Please enter any additional comments about the patient: ");
	scanf("%[^\n]", &patient.comment);
	getchar();
	
	return patient;
}

void outputDetails(struct info patient)
{
	FILE* output;
	
	output = fopen(patient.file, "w");
	
	patient = encrypt(patient);
	
	fprintf(output, "%s\n", patient.name1);
	fprintf(output, "%s\n", patient.name2);
	fprintf(output, "%d/%d/%d\n", patient.day, 
    patient.month, patient.year);
    fprintf(output, "%d\n", patient.height);
    fprintf(output, "%d\n", 
    patient.weight);
    fprintf(output, "%d\n", 
    patient.waist);
    fprintf(output, "%s\n", patient.comment);
}

struct info encrypt(struct info patient)
{
	int i;
	
	/* First Name*/
	for(i = 0; (i < 100 && patient.name1[i] != '\0'); i++)
	{
		if(patient.name1[i] != ' ')
		{
		    if(patient.name1[i] >= 'a' && patient.name1[i] <='z')
            {      
			if(patient.name1[i] >= 's' && patient.name1[i] <='z')
			{
	    		patient.name1[i] = patient.name1[i] - 18;
	    	}
	    	else
			{
				patient.name1[i] = patient.name1[i] + KEY;
			}
		    }	
			
			if(patient.name1[i] >= 'A' && patient.name1[i] <='Z')
            {
			if(patient.name1[i] >= 'S' && patient.name1[i] <='Z')
			{
	    		patient.name1[i] = patient.name1[i] - 18;
	    	}
	    	else
			{
				patient.name1[i] = patient.name1[i] + KEY;
			}
		    }
		}
	}
	
	/* Second Name*/
	for(i = 0; (i < 100 && patient.name2[i] != '\0'); i++)
	{
		if(patient.name2[i] != ' ')
		{
		    if(patient.name2[i] >= 'a' && patient.name2[i] <='z')
            {
			if(patient.name2[i] >= 's' && patient.name2[i] <='z')
			{
	    		patient.name2[i] = patient.name2[i] - 18;
	    	}
	    	else
			{
				patient.name2[i] = patient.name2[i] + KEY;
			}	
		    }
			
			if(patient.name2[i] >= 'A' && patient.name2[i] <='Z')
            {
			if(patient.name2[i] >= 'S' && patient.name2[i] <='Z')
			{
	    		patient.name2[i] = patient.name2[i] - 18;
	    	}
	    	else
			{
				patient.name2[i] = patient.name2[i] + KEY;
			}
		    }
		}
	}
	
	/* Comment */
	for(i = 0; (i < 100 && patient.comment[i] != '\0'); i++)
	{
		if(patient.comment[i] != ' ')
		{
		    if(patient.comment[i] >= 'a' && patient.comment[i] <='z')
		    {
			if(patient.comment[i] >= 's' && patient.comment[i] <='z')
			{
	    		patient.comment[i] = patient.comment[i] - 18;
	    	}
	    	else
			{
				patient.comment[i] = patient.comment[i] + KEY;
			}
		    }
      
            if(patient.comment[i] >= 'A' && patient.comment[i] <='Z')
            {
			if(patient.comment[i] >= 'S' && patient.comment[i] <='Z')
			{
	    		patient.comment[i] = patient.comment[i] - 18;
	    	}
	    	else
			{
				patient.comment[i] = patient.comment[i] + KEY;
			}
		    }
		}
	}
	return patient;
}

int validation(struct info patient)
{
	if((patient.day > 31) || (patient.day < 1))
	{
		return 1;
	}
	if(patient.month > 12 || patient.month < 0)
	{
	    return 1;
    }
    if(patient.year > 2100 || patient.year < 1900)
    {
        return 1;
    }
    if(patient.height > 250 || patient.height < 0)
    {
        return 1;
    }
    if(patient.weight > 300 || patient.weight < 0)
    {
        return 1;
    }
    if(patient.waist > 300 || patient.waist < 0)
    {
        return 1;
    }
    
	return 0;
}

char decrypt(char* detailArray)
{
    char c, array[8][200];
    int i = 0, j = 0, k, l;
    
    for(j = 0; j < i; j++)
    {
        for(k = 0; array[j][k] != '\0'; k++)
        {
            if(array[j][k] >= 'a' && array[j][k] <='z')
		    {
			if(array[j][k] <= 'g' && array[j][k] >='a')
			{
	    		array[j][k] = array[j][k] + 18;
	    	}
	    	else
			{
				array[j][k] = array[j][k] - 8;
			}
		    }
		    
		    if(array[j][k] >= 'A' && array[j][k] <='Z')
		    {
			if(array[j][k] <= 'G' && array[j][k] >='A')
			{
	    		array[j][k] = array[j][k] + 18;
	    	}
	    	else
			{
				array[j][k] = array[j][k] - 8;
			}
   		 }
		             
        }
    }
    printf("%s", array);
    return detailArray;
}

void clearScreen()
{
  system("cls");
}
