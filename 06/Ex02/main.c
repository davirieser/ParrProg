
#include <stdio.h>
#include <stdlib.h>

#define SERIAL 0
#define OPTIMIZED 1
#define PARALLEL 2

#ifndef VERSION 
#define VERSION SERIAL
#endif

// https://rosettacode.org/wiki/N-queens_problem#C
int solve_serial(int n, int col, int * hist) {
	if (col == n) {
		return 1;
	}

	int count = 0;

#	define attack(i, j) (hist[j] == i || abs(hist[j] - i) == col - j)
	for (int i = 0, j = 0; i < n; i++) {
		for (j = 0; j < col && !attack(i, j); j++);
		if (j < col) continue;

		hist[col] = i;
		count += solve_serial(n, col + 1, hist);
	}

	return count;
}

#define MAXN 31

// https://rosettacode.org/wiki/N-queens_problem#C
int solve_optimized(int n)
{
  int q0,q1;
  int cols[MAXN], diagl[MAXN], diagr[MAXN], posibs[MAXN]; // Our backtracking 'stack' 
  int num=0;
  //
  // The top level is two fors, to save one bit of symmetry in the enumeration by forcing second queen to
  // be AFTER the first queen.
  //
  for (q0=0; q0<n-2; q0++) {
    for (q1=q0+2; q1<n; q1++){
      int bit0 = 1<<q0;
      int bit1 = 1<<q1;
      int d=0; // d is our depth in the backtrack stack 
      cols[0] = bit0 | bit1 | (-1<<n); // The -1 here is used to fill all 'coloumn' bits after n ...
      diagl[0]= (bit0<<1 | bit1)<<1;
      diagr[0]= (bit0>>1 | bit1)>>1;

      //  The variable posib contains the bitmask of possibilities we still have to try in a given row ...
      int posib = ~(cols[0] | diagl[0] | diagr[0]);

      while (d >= 0) {
        while(posib) {
          int bit = posib & -posib; // The standard trick for getting the rightmost bit in the mask
          int ncols= cols[d] | bit;
          int ndiagl = (diagl[d] | bit) << 1;
          int ndiagr = (diagr[d] | bit) >> 1;
          int nposib = ~(ncols | ndiagl | ndiagr);
          posib^=bit; // Eliminate the tried possibility.

          // The following is the main additional trick here, as recognizing solution can not be done using stack level (d),
          // since we save the depth+backtrack time at the end of the enumeration loop. However by noticing all coloumns are
          // filled (comparison to -1) we know a solution was reached ...
          // Notice also that avoiding an if on the ncols==-1 comparison is more efficient!
          num += ncols==-1; 

          if (nposib) {
            if (posib) { // This if saves stack depth + backtrack operations when we passed the last possibility in a row.
              posibs[d++] = posib; // Go lower in stack ..
            }
            cols[d] = ncols;
            diagl[d] = ndiagl;
            diagr[d] = ndiagr;
            posib = nposib;
          }
        }
        posib = posibs[--d]; // backtrack ...
      }
    }
  }
  return num*2;
}

int solve_parallel(int n) {
	return 0;
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Usage <%s> [n]\n", argv[0]);
		return EXIT_FAILURE;
	}

	char * ptr;
	long n = strtol(argv[1], &ptr, 10);
	if (*ptr != 0) {
		printf("Could not parse N");
	}

	if (n > 1) {
		int count = -1;
#if VERSION == SERIAL
		int hist[n];
		count = solve_serial(n, 0, hist);
#elif VERSION == OPTIMIZED
		count = solve_optimized(n);
#elif VERSION == PARALLEL
		count = solve_serial(n, 0, hist);
#endif

		printf("Count: %d\n", count);
	}

	return 0;
}

