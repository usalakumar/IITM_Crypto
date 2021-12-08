
#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
	mpz_t q, p;
	gmp_randstate_t r_state;

	mpz_inits(p, q, NULL);
	gmp_randinit_mt(r_state);

	//generate seed based off of time
	srand(time(0));
	int seed = rand();
	gmp_randseed_ui(r_state, seed);

	//Find random number q
	mpz_urandomb(q, r_state, 1000);
	mpz_nextprime(q, q);
	mpz_mul_ui(p, q, 2);
	mpz_add_ui(p, q, 1);

	//Check whether p is prime, if not, keep trying to find prime q and then p
	while (mpz_probab_prime_p(p, 25) != 1) {
		mpz_nextprime(q, q);
		mpz_mul_ui(p, q, 2);
		mpz_add_ui(p, p, 1);
	}

	gmp_printf("p: %Zd\n", p);
	gmp_printf("q: %Zd\n", q);
	
	gmp_randclear(r_state);
	mpz_clears(p, q, NULL);
}