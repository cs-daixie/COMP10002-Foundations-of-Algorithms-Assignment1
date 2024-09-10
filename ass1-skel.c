/* Program to generate hierarchical reports from TSV structured data

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2024

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'	     /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50       /* maximum field length */

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];


/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);


/**************************************************************/

/* main program provides traffic control
*/
int
main(int argc, char *argv[]) {


	/* all done, time to go home */
	printf("ta daa!\n");
	return 0;
}

/**************************************************************/

/* read characters and build a string, stopping when a tab or newline
   as encountered, with the return value indicating what that
   terminating character was
*/
int
getfield(field_t dest) {

	int ch, nchs=0;
	int status=STATUS_NML;

	dest[0] = '\0';
	while ((ch=mygetchar())
		&& (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

		if (nchs < MAXFIELDLEN) {
			/* ok to save this character */
			dest[nchs++] = ch;
			dest[nchs] = '\0';
		} else {
			/* silently discard extra characters if present */
		}
	}

	/* return status is defined by what character stopped the loop */
	if (ch == EOF) {
		status = STATUS_EOF;
	} else if (ch == CHAR_NL) {
		status = STATUS_EOL;
	} else if (ch == CHAR_TB) {
		status = STATUS_NML;
	}
	return status;
}

/**************************************************************/

/* read a single character, bypassing any CR characters encountered,
   so as to work correctly with either PC-type or Unix-type input
 */
int
mygetchar(void) {
	int c;
	while ((c=getchar()) == CHAR_CR) {
		/* empty loop body */
	}
	return c;
}

/**************************************************************/

