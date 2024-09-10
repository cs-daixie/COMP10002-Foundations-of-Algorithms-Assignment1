/* Sample solution comp20005 Assignment 1, 2020.

   Alistair Moffat, April 2020
   ammoffat@unimelb.edu.au
   (c) The University of Melbourne, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/*******************************************************************/

#define MAX_WAGONS 999
#define MIN_TONNAGE 375
#define MIN_CONCENT 52.5
#define HEADER_LINES 1

#define STAGE1 1
#define STAGE2 2
#define STAGE3 3

#define DEBUG_INT(x) \
	printf("line %3d: %s = %d\n", __LINE__, #x, x)

#define DEBUG_DBL(x) \
	printf("line %3d: %s = %.2f\n", __LINE__, #x, x)

/* data structure for recording information about one wagon */
typedef struct {
	int num;
	double tonnes, percent;
} wagon_t;

int  read_data(wagon_t *wagons, int maxwagons);
void do_stage1(wagon_t *wagons, int nwagons, int stage);
void do_stage2(wagon_t *wagons, int nwagons, int stage);
void do_stage3(wagon_t *wagons, int nwagons, int stage);
wagon_t sum_wagons(wagon_t *wagons, int nwagons);
void swap_wagons(wagon_t *w1, wagon_t *w2);
void swap_weakest_last(wagon_t *active, int nactive);
void sort_by_num(wagon_t *active, int nactive);
void print_stage(int stage);
void error_and_exit(char *err, int line);
void remove_lines(int n);
void print_S1_line(wagon_t wagon);
void print_S2S3_line(wagon_t *wagons,
	int frst, int top, int consignment, int stage);
void print_wagonnums(wagon_t *wagons, int nwagons, int stage);
void blank_line(void);
void ta_daa(void);

/*******************************************************************/

/* traffic control for the whole computation
*/

int
main(int argc, char *argv[]) {

	wagon_t wagons[MAX_WAGONS];
	int nwagons;

	remove_lines(HEADER_LINES);
	nwagons = read_data(wagons, MAX_WAGONS);

	do_stage1(wagons, nwagons, STAGE1);
	do_stage2(wagons, nwagons, STAGE2);
	do_stage3(wagons, nwagons, STAGE3);

	ta_daa();
	return 0;
}

/*******************************************************************/

/* throw away the indicated number of complete lines from the start
   of the input stream
*/
void
remove_lines(int n) {
	int c;
	while ((c=getchar()) != EOF) {
		if (c=='\n') {
			n--;
			if (n==0) return;
		}
	}
	/* if get here, something is wrong */
	error_and_exit("Unexpected end of input", __LINE__);
}

/*******************************************************************/

/* read all of the two-number input rows into the supplied array
   of struct
*/
int read_data(wagon_t *wagons, int maxwagons) {
	double tonnes, percent, num=1;
	int nwagons=0;
	while (scanf("%lf%lf", &tonnes, &percent)==2) {
		if (nwagons==maxwagons) {
			error_and_exit("Too much input data", __LINE__);
		}
		/* ok to add to the array */
		wagons[nwagons].num      = num++;
		wagons[nwagons].tonnes   = tonnes;
		wagons[nwagons].percent  = percent;
		nwagons++;
	}
	/* natural end of function, all ok */
	return nwagons;
}

/*******************************************************************/

/* the required stage 1 output:
   -- number of data lines read into the array
   -- details for the first data line
   -- details for the last data line
   -- total of input data lines
*/
void
do_stage1(wagon_t *wagons, int nwagons, int stage) {
	wagon_t total;

	/* this is the only thing that needs to be computed */
	total = sum_wagons(wagons, nwagons);

	/* and can then generate required output */
	print_stage(stage);
	print_S1_line(wagons[0]);

	print_stage(stage);
	print_S1_line(wagons[nwagons-1]);

	print_stage(stage);
	printf("   whole train, ");
	print_S1_line(total);
	blank_line();

	return;
}

/*******************************************************************/

/* helper function for stage 1, to make sure the formatting of the
   three output lines is consistent
*/
void
print_S1_line(wagon_t wagon) {
	if (wagon.num>0) {
		printf("     wagon %3d, ", wagon.num);
	}
	printf("tonnes=%7.1f, %%=%5.1f\n", wagon.tonnes, wagon.percent);
	return;
}

/*******************************************************************/

/* the required stage 2 output:
   -- lists of consignments
   -- consignment weights and percentages
   -- whether accepted or rejected
   -- overall summary of ore wastage
*/
void
do_stage2(wagon_t *wagons, int nwagons, int stage) {

	int curr, frst;
	double totw;
	wagon_t total;
	int i;
	wagon_t wasted[MAX_WAGONS];
	int nwasted=0;
	int consignment=1;

	frst = 0;
	totw = 0.0;
	curr = 0;

	/* loop considers one wagon at a time */
	while (curr<nwagons) {
		/* should we add the current wagon to the consignment? */
		if (totw < MIN_TONNAGE) {
			/* yes... */
			totw += wagons[curr].tonnes;
			curr++;
		}
		/* and, with that done, have we reached the desired weight? */
		if (totw < MIN_TONNAGE) {
			/* nope, so go round the loop and get another wagon */
			continue;
		}

		/* at this point have a possible consignment using
		   wagons[frst] to wagons[curr-1]? */
		total = sum_wagons(wagons+frst, curr-frst);
		/* can it be accepted? */
		if (total.percent>=MIN_CONCENT) {
			/* yes, can accept, hooray */
			print_S2S3_line(wagons, frst, curr,
				consignment, stage);
			consignment += 1;
		} else {
			/* nope, so need to record the wasted wagons */
			for (i=frst; i<curr; i++) {
				wasted[nwasted++] = wagons[i];
			}
		}
		/* and either way, start collecting all over again */
		frst = curr;
		totw = 0.0;
	}

	/* hmm, there may be a partial consignment also wasted */
	if (frst<curr) {
		for (i=frst; i<curr; i++) {
			wasted[nwasted++] = wagons[i];
		}
	}
	if (nwasted>0) {
		total = sum_wagons(wasted, nwasted);
		print_stage(stage);
		printf("  total dumped, ");
		print_S1_line(total);
		print_wagonnums(wasted, nwasted, stage);
	}
	blank_line();

	return;
}

/*******************************************************************/

void
print_S2S3_line(wagon_t *wagons, int frst, int top,
		int consignment, int stage) {
	wagon_t total;
	total = sum_wagons(wagons+frst, top-frst);
	print_stage(stage);
	printf("consignment %2d, ", consignment);
	print_S1_line(total);
	print_wagonnums(wagons+frst, top-frst, stage);
	return;
}

/*******************************************************************/

void
print_wagonnums(wagon_t *wagons, int nwagons, int stage) {
	int i;
	sort_by_num(wagons, nwagons);
	print_stage(stage);
	printf("                wagons= ");
	for (i=0; i<nwagons; i++) {
		printf("%4d", wagons[i].num);
	}
	blank_line();
	return;
}

/*******************************************************************/

/* the required stage 3 output:
   -- list of wagons in order delivered
   -- details of wagons that get discarded
   -- details of consignments formed
   -- summary of overall wastage

*/

void
do_stage3(wagon_t *wagons, int nwagons, int stage) {

	int curr;
	double totw;
	wagon_t total;
	int i;
	wagon_t wasted[MAX_WAGONS];
	wagon_t active[MAX_WAGONS];
	int nwasted=0;
	int nactive=0;
	int consignment=1;

	totw = 0.0;
	curr = 0;

	/* loop considers one wagon at a time */
	while (curr<nwagons) {
		/* should we add the current wagon to the consignment? */
		if (totw < MIN_TONNAGE) {
			/* yes... */
			active[nactive++] = wagons[curr];
			totw += wagons[curr].tonnes;
			curr++;
		}
		/* and, with that done, have we reached the desired weight? */
		if (totw < MIN_TONNAGE) {
			/* nope, so go round the loop and get another wagon */
			continue;
		}

		/* at this point have a possible consignment using
		   active[0] to active[nactive-1] */
		total = sum_wagons(active, nactive);
		/* can it be accepted? */
		if (total.percent>=MIN_CONCENT) {
			/* yes, can accept, hooray */
			print_S2S3_line(active, 0, nactive,
				consignment, stage);
			consignment += 1;
			nactive = 0;
			totw = 0.0;
		} else {
			/* nope, so need to remove least-strength active wagon, 
			   by locating it and swapping it to last position  */
			swap_weakest_last(active, nactive);
			nactive -= 1;
			wasted[nwasted++] = active[nactive];
			totw -= active[nactive].tonnes;
		}
		/* and either way, start collecting all over again */
	}

	/* hmm, there may be a partial consignment also wasted */
	for (i=0; i<nactive; i++) {
		wasted[nwasted++] = active[i];
	}
	if (nwasted>0) {
		total = sum_wagons(wasted, nwasted);
		print_stage(stage);
		printf("  total dumped, ");
		print_S1_line(total);
		print_wagonnums(wasted, nwasted, stage);
	}
	blank_line();

	return;
}


/*******************************************************************/

/* sum up the weight and calculate overall percentage concentration
   of an array of individual wagons */
wagon_t
sum_wagons(wagon_t *wagons, int nwagons) {
	wagon_t total={0.0,0.0};
	int i;
	for (i=0; i<nwagons; i++) {
		total.tonnes  += wagons[i].tonnes;
		total.percent += wagons[i].tonnes * wagons[i].percent;
	}
	total.percent /= total.tonnes;
	return total;
}

/*******************************************************************/

/* swap over the contents of two wagons in an array
*/
void
swap_wagons(wagon_t *w1, wagon_t *w2) {
	wagon_t tmp;
	tmp = *w1;
	*w1 = *w2;
	*w2 = tmp;
	return;
}

/*******************************************************************/

/* find the weakest-concentration wagon in the active set and swap
   it to be the last one on the active set */
void
swap_weakest_last(wagon_t *active, int nactive) {
	int least=0;
	double minp;
	int i;
	assert(nactive>0);
	minp=active[0].percent;
	for (i=1; i<nactive; i++) {
		if (active[i].percent < minp) {
			least = i;
			minp = active[i].percent;
		}
	}
	swap_wagons(active+least, active+nactive-1);
	return;
}

/*******************************************************************/

/* sort an array of wagons into increasing order by wagon number
*/
void
sort_by_num(wagon_t *wagons, int nwagons) {
	int i, j;
	for (i=1; i<nwagons; i++) {
		for (j=i-1; j>0 && wagons[j-1].num > wagons[j].num; j--) {
			swap_wagons(wagons+j-1, wagons+j);
		}
	}
	return;
}

/*******************************************************************/

/* print the supplied error message and then exit the program
*/
void
error_and_exit(char *err, int line) {
	printf("Problem at line %3d: %s\n", line, err);
	exit(EXIT_FAILURE);
}

/* other helper functions */

void print_stage(int stage) {
	printf("S%d, ", stage);
	return;
}

void
blank_line(void) {
	printf("\n");
}

void
ta_daa(void) {
	printf("Ta daa!\n");
}

/*******************************************************************/
