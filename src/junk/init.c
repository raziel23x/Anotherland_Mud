#include <stdio.h>
#include <limits.h>

int main(int argc, char **argv)
{
  int i;
  long int l;
  
  printf("size of int: %d\n", sizeof(i));
  printf("max size of int: %d\nmin size of int: %d\n",INT_MAX,INT_MIN);
  return 0;
}

