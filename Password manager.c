
#include "headers/password-manager.h" //header files
#include "external/key-generator.c"


int main () //This function asks the user to select a choice and then invokes corresponding functions based on choice
{

	int choice;
	puts("Welcome to the password manager\n Enter your choice");
	do
	{
		puts("\n1. Save a new password\n2. Open a saved password\n3. Display all the references\n4. Erase the vault\n5. Exit the password manager");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:save_password();break;
			case 2:open_password();break;
			case 3:display_references();break;
			case 4:erase_vault();break;
			default: puts("Enter a valid choice");break;
		}
		
	}
	while (choice!=5);

}



void cipher(char *input, char *output, char *key) //Implementation of encrypting by randomizing the values, the result is stored in *output
{
	int i;
	for(i = 0; i < strlen(input); i++) 
	{
		output[i] = input[i] ^ key[i]; 			//i % (sizeof(key)/sizeof(char))
	}
	output[i]='\0';
}



void save_password() 
{	
	puts("Enter your reference");
	scanf("%s", reference);
	puts("Enter your password");
	scanf("%s", password);

	key_generator(key,strlen(password)); //key_generator generates random string and then encrypts the data and writes to password vault.

	cipher(password, encrypted, key);


	FILE *fp; //file handling
	fp=fopen("password-vault/vault.txt","a");
	if (fp == NULL) //Throws an error if file does not exist
		puts("File failed to open");
	else
	{
		fprintf(fp,"%s %s %s",reference, encrypted, key);
		fputs("\n",fp);
	}
	fclose(fp);

	puts("Your password has been encrypted and saved");
}



void open_password () //Decrypting the encrypted string again by reverse engineering the encryption process
{
	char input_reference[40];
	puts("Enter reference word \n");
	scanf("%s", input_reference);

	FILE *fp;
	fp=fopen("password-vault/vault.txt","r");
	if (fp == NULL)
		puts("File failed to open");
	else
	{
		for (int i=0;i<40;i++)
		{	
			fscanf(fp,"%s %s %s \n", reference, encrypted, key);
			if (strcmp(reference,input_reference)==0)			
				break;
		}
	}

	cipher(encrypted, decrypted, key); //output is at decrypted

	printf("Decrypted password:%s \n", decrypted);
}

void display_references () //reads data in password vault and prints it out.
{
	puts("The references are:");
	FILE *fp;
	fp=fopen("password-vault/vault.txt","r");
	if (fp == NULL)
		puts("File failed to open");
	else
	{
		while (!feof(fp))
		{	
			fscanf(fp,"%s %s %s \n", reference, encrypted, key);
			puts(reference);
		}
	}		
}

void erase_vault () //Deletes the password vault entirely.
{
	if (remove("password-vault/vault.txt") == 0) 
    	printf("Password vault deleted successfully"); 
    else
    	printf("Unable to delete the vault"); 
}