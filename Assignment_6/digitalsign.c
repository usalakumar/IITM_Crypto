#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio_ext.h>

mpz_t p, m, q, e0, e1, e2, d, r, s1, s2, p_minus_1_div_q, hm, ds1, v, s2_inv, e1_p, e2_p, r_inv, s_1;
unsigned char obuf[200];
char input[100];

unsigned char* fun()
{
  memset(obuf, 0, 100);
  SHA1(input, strlen(input), obuf);
}

void signing()
{
  unsigned char *hm_str;

  /* Signing Algorithm */
  mpz_set_ui(p, 8081);
  mpz_set_ui(q, 101);
  mpz_set_ui(e0, 3);
  mpz_sub_ui(p_minus_1_div_q, p, 1);
  mpz_divexact(p_minus_1_div_q, p_minus_1_div_q, q);

  mpz_powm(e1, e0, p_minus_1_div_q, p);

  mpz_set_ui(d, 61);
  mpz_powm(e2, e1, d, p);

  mpz_set_ui(r, 61);
  mpz_powm(s1, e1, r, p);
  mpz_mod(s1, s1, q);

  gmp_printf("Enter a msg(number)  to signature\n");
  fgets(input, 100, stdin);
  mpz_set_str(hm, input, 10);
  mpz_mul(ds1, d, s1);
  fun();

  mpz_set_str(hm, obuf, 10);
  mpz_add(s2, ds1, hm);
  mpz_invert(r_inv, r, q);
  mpz_mul(s2, s2, r_inv);
  mpz_mod(s2, s2, q);
  gmp_printf("\nsignature  S1: %Zd\n", s1);
  gmp_printf("\nsignature S2: %Zd\n", s2);
}

void verifying()
{
  mpz_inits(s_1, NULL);
  gmp_printf("\nEnter S1,S2\n");
  gmp_scanf("%Zd,%Zd,%Zd", s_1,s2);
  gmp_printf("\nEnter the message(number) to be verified\n");
  memset(input, 0, 100);
  __fpurge(stdin);
  fgets(input, 100, stdin);
  mpz_set_str(hm, input, 10);
  fun();

  /* Verifying */
  mpz_invert(s2_inv, s2, q);
  mpz_mul(e1_p, hm, s2_inv);
  mpz_mod(e1_p, e1_p, q);
  mpz_pow_ui(e1_p, e1, mpz_get_ui(e1_p));

  mpz_mul(e2_p, s_1, s2_inv);
  mpz_mod(e2_p, e2_p, q);
  mpz_pow_ui(e2_p, e2, mpz_get_ui(e2_p));
  mpz_mul(v, e1_p, e2_p);
  mpz_mod(v, v, p);
  mpz_mod(v, v, q);
  gmp_printf("\nv: %Zd\n", v);

  if(mpz_cmp(s_1, v) == 0) {
    gmp_printf("\n Verification has done successfully!. The Receiver accepted the message\n");
  } else
  {
    gmp_printf("\n Verification has been failed !. The Receiver Rejected the message\n");
  }

  mpz_clears(s_1, NULL);
}

int main()
{
  mpz_inits(p, m, q, e0, e1, e2, d, r, s1, s2, p_minus_1_div_q, hm, ds1, v, s2_inv, e1_p, e2_p, r_inv, NULL);
  char input[100] = {0};

  while (1) {
    __fpurge(stdin);
    printf("\nEnter 1 to sign and 0 to verify:");
    memset(input, 0, 100);
    fgets(input, 100, stdin);
    input[strlen(input)-1] = '\0';
    int is_sign = atoi(input);

    if(!( (is_sign == 1) ^ ((is_sign == 0) && !strncmp(input, "0", 1))))
    {
      printf("\n Invalid sign/verify value. Enter 1 to sign 0 to verify.\n");
      continue;
    }

    if (is_sign == 1) {
      printf("\nSigning!!!!!!!!!!!!!!!!!\n");
      signing();
    }

    if (is_sign == 0) {
      printf("\nVerifying!!!!!!!!!!!!!!!!!\n");
      verifying();
    }

  }

  mpz_clears(p, m, q, e0, e1, e2, d, r, s1, s2, p_minus_1_div_q, hm, ds1, v, s2_inv, e1_p, e2_p, r_inv, NULL);
  return 0;
}
