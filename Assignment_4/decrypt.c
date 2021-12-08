
#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define X "40622201812345"

int main() {
	//Decryption Check
	mpz_t c1_powx, c1_powx_inv, c1, c2, p, x, m2;
	mpz_inits(c1, c2, p, x, c1_powx, c1_powx_inv, m2, NULL);

	mpz_set_str(x, X, 10);

	printf("Enter a value of c1,c2,p: ");
	gmp_scanf("%Zd,%Zd,%Zd", c1, c2, p);
	//Compute c1^x
	mpz_powm(c1_powx, c1, x, p);

	//Compute (c1^x)^-1
	mpz_invert(c1_powx_inv, c1_powx, p);

	//Compute c2 * (c1^x)^-1
	mpz_mul(m2, c2, c1_powx_inv);
	mpz_mod(m2, m2, p);

	gmp_printf("Decrpyted message: %Zd\n", m2);

	mpz_clears(m2, c1, c2, p, x, c1_powx, c1_powx_inv, NULL);
}