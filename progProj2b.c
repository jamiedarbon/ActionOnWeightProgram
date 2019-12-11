/*********************************************************************
* progProj2b.c
* 
* 218721
* Version 0.0.1, 8th December 2019
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES 7
#define KEY 8
#define NURSES 5

struct info
{
    int day;
    int month;
    int year;    
}dob;

void start(struct info);
void clearScreen(void);
void details(struct info);
void nurseAmmend(struct info dob);
void decrypt(char *filename);
char encrypt(char comment[]);
char* charArray(FILE* valid, char* array);
void comments(char *filename, char* array, struct info dob);

int main(void)
{
    char usernameInput[10], passwordInput[8];
    int loginValidate, passwordValidate;
	
    /* Get user input for usernames and passwords */
    
    printf("Please enter username: ");
    scanf("%s", &usernameInput);
    getchar();
    printf("Please enter password: ");
    scanf("%s", &passwordInput);
    getchar();
    
    char login[] = "mb357";
    char password[] = "MnasIe0e";
    
    if(strcmp(login, usernameInput) == 0)
    {
    	loginValidate = 1;
	}
	if(strcmp(password, passwordInput) == 0)
	{
		passwordValidate = 1;
	}
	
	if(loginValidate == 1 && passwordValidate == 1)
	{
		start(dob);
	}
	else
	{
		printf("ERROR: Invalid username or password.");
		exit(1);
	}
}

/* Main menu */

void start(struct info dob)
{
    int choice;
    
    clearScreen();
    
    printf("CONSULTANT PROGRAM\n");
    printf("==================\n");
    printf("1 - View patient details\n");
    printf("2 - Change nurse login details\n");
    printf("3 - Exit program\n\n");
    printf("Which program would you like to access?\n");
    
    scanf("%d", &choice);
    switch(choice)
    {
        case 1:
            details(dob);
            break;
            
        case 2:
            nurseAmmend(dob);
            break;
            
        case 3:
            exit(1);
            
        default:
            printf("ERROR: Please enter a valid input");
            exit(1);
    }
}

/* Fetch patient details from a file */

void details(struct info dob)
{
    char surname[50], filename[75], c[1000];
    FILE* valid;
    
    clearScreen();
    
    printf("Please enter the patient's surname: ");
    scanf("%s", &surname);
    getchar();
    printf("Please enter the patient's date of birth(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &dob.day, &dob.month, &dob.year);
    getchar();
    
    snprintf(filename, 20, "%s%d%d%d.aow", surname, dob.day, dob.month,
    dob.year);
    
    valid = fopen(filename, "r");
    
    if(valid == 0)
    {
        printf("ERROR: file %s could not be opened.", filename);
        exit(2);
    }
    else
    {
        printf("Opening %s...\n", filename);
        decrypt(filename);
    }
}

/* Decrypt patient details into a readable format */

void decrypt(char *filename)
{
    char c, array[8][200];
    int i = 0, j = 0, k, l;
    
    FILE* valid;
    
    clearScreen();
    
    valid = fopen(filename, "r");
    
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
    printf("Forname: %s\n", array[0]);
    printf("Surname: %s\n", array[1]);
    printf("Date of birth: %s\n", array[2]);
    printf("Height: %s cm\n", array[3]);
    printf("Weight: %s lbs\n", array[4]);
    printf("Waist measurement: %s cm\n", array[5]);
    printf("Comments: %s\n", array[6]);   
    
    comments(filename, *array, dob);
}

/* Allow comments to be appended onto the file */

void comments(char * filename, char* array, struct info dob)
{
    FILE *file;
    char confirm, comment[200];
    
    printf("Would you like to enter any additional comments?\n");
    scanf("%c", &confirm);
    getchar();
    if(confirm == 'y')
    {
        file = fopen(filename, "a");
        printf("Please enter a comment: ");
        scanf("%s", &comment);
        *comment = encrypt(comment);
        fprintf(file, "%s", comment);
        start(dob);
    }
    else
    {
        start(dob);
    }
}

/* Encrypt the user comment to write to the end of the file */

char encrypt(char comment[])
{
	int i;
	for(i = 0; (i < 100 && comment[i] != '\0'); i++)
	{
		if(comment[i] != ' ')
		{
		    if(comment[i] >= 'a' && comment[i] <='z')
            {      
    			if(comment[i] >= 's' && comment[i] <='z')
    			{
    	    		comment[i] = comment[i] - 18;
    	    	}
    	    	else
    			{
    				comment[i] = comment[i] + KEY;
    			}
		    }	
			
			if(comment[i] >= 'A' && comment[i] <='Z')
            {
    			if(comment[i] >= 'S' && comment[i] <='Z')
    			{
    	    		comment[i] = comment[i] - 18;
    	    	}
    	    	else
    			{
    				comment[i] = comment[i] + KEY;
    			}
		    }
		}
	}
	return comment;
}

/* Allow the nurse's usernames and passwords to be changed */

void nurseAmmend(struct info dob)
{
    FILE* loginUsername;
    FILE* loginPassword;
    
    char *names[] = {"Jeremy Corbyn", "Boris Johnson", "Caroline Lucas",
    "Nicola Sturgeon", "Jo Swinson"};
    char username[NURSES][100];
    char password[NURSES][100];
    char inputChoice[1];
    int i, c, nurseNumber, userNum = 0, passNum = 0;
    
    clearScreen();
    
    loginPassword = fopen("passwords.txt", "r");
    loginUsername = fopen("usernames.txt", "r");
    
    fscanf(loginUsername, "%s", username[userNum]);
    c = fgetc(loginUsername);
    while (c != EOF) 
    { 
        if(c == '\n')
        {
            userNum++;
        }
        fscanf(loginUsername, "%s", username[userNum]);
        c = fgetc(loginUsername);
    } 
    
	fscanf(loginPassword, "%s", password[passNum]);
    c = fgetc(loginPassword);
    while (c != EOF) 
    { 
        if(c == '\n')
        {
            passNum++;
        }
        fscanf(loginPassword, "%s", password[passNum]);
        c = fgetc(loginPassword);
    } 
    
	for(i = 0; i < NURSES; i++)
	{
        printf("Nurse %d - %s\n", i + 1, names[i]);
        printf("===============\n");
        printf("Username : %s\n", username[i]);
        printf("Password : %s\n\n", password[i]);
    }
    
    printf("Which nurse's details would you like to change?(Input the"
    " nurse's number)\n");
    scanf("%d", &nurseNumber);
    nurseNumber -= 1;
    printf("Would you like to change %s's username or password?\n", 
    names[nurseNumber]);
    scanf("%s", inputChoice);
    if(strcmp(inputChoice, "username") == 0)
    {
        printf("Please enter the new username: ");
        scanf("%s", &username[nurseNumber]);
        fprintf("usernames.txt", "%s", &username[nurseNumber]);
    }
    if(strcmp(inputChoice, "password") == 0)
    {
        char null;
        printf("Please enter the new password: ");
        scanf("%s", &password[nurseNumber]);
        fprintf("passwords.txt", "%s", &password[nurseNumber]);
        printf("Username successfully changed, press any key to continue.");
        scanf("%s", &null);
    }
    else
    {
        printf("ERROR: Incorrect entry, please consult user guide for "
        "formatting.");
    }
    fclose(loginUsername);
    fclose(loginPassword);
    start(dob);
}

/* Clear the screen */

void clearScreen()
{
  system("cls");
}
