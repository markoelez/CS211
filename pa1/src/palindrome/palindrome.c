#include <stdio.h> 
#include <string.h> 
#include <ctype.h>
  
int main(int argc, char **argv) {
  // check for input
  if ((argc - 1) != 1) return 0;
  char *str = argv[1];
  // initialize window
  int lo = 0;
  int hi = strlen(str) - 1;
  // convert input to lowercase
  for (int i = 0; i < hi; i++) str[i] = tolower(str[i]); 
  // iterate through string
  while (hi > lo) {
    // if lower index is not a letter, move up
    if (!(str[lo] >= 'a' && str[lo] <= 'z')) {
      lo++;
    // if upper index is not a letter, move down
    } else if (!(str[hi] >= 'a' && str[hi] <= 'z')) {
      hi--;
    // if equal, continue
    } else if (str[lo] == str[hi]) {
      lo++, hi--;
    // otherwise, this is not a palindrome, return
    } else {
      printf("no");
      return 0;
    }
  }
  printf("yes");
  return 0;
}