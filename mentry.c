/*
Aleksandar Irikov
2121159i
APH Exercise 1
This is my own work as defined in the Academic Ethics agreement I have signed.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mentry.h"

#define MAXLINE 500
#define SURNAMEMAX 100
#define POSTCODEMAX 10


//initialize mentry
MEntry *mentry = NULL;
/* me_get returns the next file entry, or NULL if end of file*/
MEntry *me_get(FILE *fd){
	//store values of the lines - used when reading
	char surnameline[MAXLINE+1] = {};
	char addressline[MAXLINE+1] = {};
	char postcodeline[MAXLINE+1] = {};

	//initialize stepper variables used in loops
	int i = 0;
	int j = 0;


	if ((mentry = calloc(1,sizeof(MEntry))) == NULL)
		return NULL;

	//get data from input file
	fgets(surnameline,MAXLINE,fd);
	fgets(addressline,MAXLINE,fd);
	fgets(postcodeline,MAXLINE,fd);


	//get length of all local variables (used when 'allocing)
	int surname_len = strlen(surnameline)+1;
  int address_len = strlen(addressline)+1;
	int postcode_len = strlen(postcodeline)+1;
  int full_address_len = surname_len-1 + address_len-1 +  postcode_len-1 + 1 ;

	//allocate zhe memory
	mentry->surname = calloc(1,surname_len);
  mentry->postcode = calloc(1,postcode_len);
  mentry->full_address = calloc(1,full_address_len);

	if (surnameline[0]=='\0')	//stop reading when nothing left
		return NULL;

	sprintf(mentry->full_address,"%s%s%s",surnameline,addressline,postcodeline); //concatenate everything and put it in full addressline

	//put surname in the mentry struct
	for(i=0;surnameline[i]!=',';i++)
		mentry->surname[i] = (char) tolower(surnameline[i]);

	mentry->surname[i] = '\0'; //add the EOS at the end

	//put house number in the mentry struct
	mentry->house_number = atoi(addressline);// get_house_number(address_len);

	//put postcode in the mentry struct
	for(i=0;i<postcode_len;i++){
		if(isalnum(postcodeline[i])){
			mentry->postcode[j] = tolower(postcodeline[i]);
			j++;
		}
	}
  mentry->postcode[j] = '\0'; //add EOS

	return mentry;
}


/* me_hash computes a hash of the MEntry, mod size */
unsigned long me_hash(MEntry *me, unsigned long size){
	unsigned long result = 0; //store the hash of the mail entry

	//use the lengths of these strings when iterating thorugh them
	int surname_len = strlen(me->surname);
	int postcode_len = strlen(me->postcode);

	int i = 0;//stepper variable

	//get the hash by iterating through the strings
	for(i = 0; i < surname_len; i++)
	result = (result *  17) + me->surname[i];//added 17 as a random prime number to hash with

	for(i = 0; i < postcode_len; i++)
		result = (result * 17) + me->postcode[i];

	result *= (me->house_number + 17);//add the value of the house number as well
	result = result % size;//hash the result using the modulo operator
	return result;
}

/* me_print prints the full address on fd */
void me_print(MEntry *me, FILE *fd){
	fprintf(fd,"%s\n", me->full_address);
}

/* me_compare compares two mail entries, returning <0, 0, >0 if
 * me1<me2, me1==me2, me1>me2
 */
//primitive version, looks only if the entries are equal or not
int me_compare(MEntry *me1, MEntry *me2){
	if (strcmp(me1->surname,me2->surname) ==0 && strcmp(me1->postcode,me2->postcode)==0 && (me1->house_number == me2->house_number)){
		return 0;
	}
	return 1;
}

//Have to implement a me_destroy method which returns all heap-alocated memory associated with mailing list entries
void me_destroy(MEntry *me){
	if (me != NULL){
		free(me->surname);
		free(me->full_address);
		free(me->postcode);
	}
	free(me);
}


// }
