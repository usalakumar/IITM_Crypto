
#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define P "9320483326403141908015992162623600634575383430682887743382352056156178908367184167231123764417028100440493342112733863272018608164976018482157886751640476408611107448081659328189987279478473107665608224322577609602668215373049415828076115411711894932867991657795009176799957724525869703082152176991423"
#define Q "4660241663201570954007996081311800317287691715341443871691176028078089454183592083615561882208514050220246671056366931636009304082488009241078943375820238204305553724040829664094993639739236553832804112161288804801334107686524707914038057705855947466433995828897504588399978862262934851541076088495711"
#define X "40622201812345"

int main() {
	int count = 5;

	mpz_t m;
	mpz_t q, p, c1, c2; //output and input values
	mpz_t h, g, r, x, generator;
	mpz_t h_pow_r;
	mpz_t p_sub_1_div2, g_x, g_pow_2, one;
	gmp_randstate_t g_state;
	gmp_randstate_t r_state;

	mpz_init(m);

	// Accept input value of m
	printf("Enter a value of m: ");
	gmp_scanf("%Zd", m);

	while (count != 0) {
		mpz_inits(p_sub_1_div2, g_x, g_pow_2, one, NULL);
		mpz_inits(c1, c2, q, h, p, g, generator, r, x, h_pow_r, NULL);
		gmp_randinit_mt(g_state);
		gmp_randinit_mt(r_state);
		mpz_set_str(x, X, 10);
		mpz_set_str(p, P, 10);
		mpz_set_str(q, Q, 10);

		//find random g
		srand(time(0));
		int seed = rand();
		gmp_randseed_ui(g_state, seed);
		mpz_urandomb(g, g_state, 1000);

		//find random r 
		srand(time(0));
		seed = rand();
		gmp_randseed_ui(r_state, seed);
		mpz_urandomb(r, r_state, 1000);

		mpz_set_ui(one, 1);
		mpz_set(p_sub_1_div2, p);
		mpz_submul_ui(p_sub_1_div2, one, 0.5);
		
		//calculate generator that is safe from QR/QNR attacks
		while (mpz_cmp(g, p) != 0) {
			mpz_powm(g_x, g, p_sub_1_div2, p); //g^((p-1)/2) mod p
			mpz_powm_ui(g_pow_2, g, 2, p); //g^2 mod p

			if (!(mpz_cmp_ui(g_x, 1) == 0) && !(mpz_cmp_ui(g_pow_2, 1) == 0)) {
				mpz_set(generator, g);
				break;
			}
			else {
				srand(time(0));
				int seed = rand();
				gmp_randseed_ui(g_state, seed);
				mpz_urandomb(g, g_state, 1000);
			}
		}

		//Compute c1 = g^r
		mpz_powm(c1, generator, r, p);

		//Compute h = g^x
		mpz_powm(h, generator, x, p);

		//Compute h^r
		mpz_powm(h_pow_r, h, r, p);

		//Compute c2 = m * h^r
		mpz_mul(c2, m, h_pow_r);

		gmp_printf("%Zd,%Zd,%Zd\n\n", c1, c2, p);

		count--;
		sleep(1); //since numbers are generated via system time, pause for 1 second
	}
	mpz_clear(m);
	mpz_clears(p_sub_1_div2, g_pow_2, g_x, NULL);
	mpz_clears(c1, c2, p, x, one, g, h_pow_r, h, q, generator, r, NULL);
	gmp_randclear(r_state);
	gmp_randclear(g_state);

	return 0;
}