#include <stdio.h>
#define THE_BILL 272

/*
 * This function prints a "VICTORY!" three times.
 *
 */
void Victory(int n) {
  for (int i = 0; i <  n; i++) {
    printf("VICTORY!\n");
		  }
}
/*
 * This function should print n numbers all in one line
 * where each number i should repeat i times.
 *

}

/*
 * This function prints a 20% tip on the provided input. 
 *
 * For example, Tip(5) -> $1.00. 
 *              Tip(272) -> $54.40.
 */
void Tip(int n) {
  double t = n * 0.2;
	printf("$%.2f\n", t);

}

int main() {
	Victory(3);
	PrintNumbers(5);
	Tip(THE_BILL);
	return 0;
}
