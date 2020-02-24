#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  // check for input
  if ((argc - 1) != 1) return 0;
  char *str = argv[1];
  // get length of input
  int len = strlen(str);
  // at most, resulting string will be 2 * len (ex. input: abcd -> a1b1c1d1)
	int resLen = 2 * len;
  // allocate memory for output
  char *res = malloc((resLen + 1) * sizeof(char));
  // initialize counters
	int i, j, count = 1, resCurrLen = 0;
  // iterate through input string
	for (i = 0; i < len;) {
    // if invalid char, return error
    if (str[i] >= '0' && str[i] <= '9') {
			printf("error\n");
			return 0;
    }
    // starting from current char
    for (j = i + 1; j < len; j++) {
      // if this char is different from starting char, break
      if (str[j] != str[i]) {
        break;
      }
      count++;
    }
    // calculate max size of current group
    int remaining = (resLen + 1) - resCurrLen;
    // get size of current block, write letter & length to buffer 
    int size = snprintf(res + resCurrLen, remaining, "%c%d", str[i], count);
    // increment current length of res string
    resCurrLen += size;
    // if our resulting string exceeds original length, break
    if (resCurrLen > len) {
      res = str;
      break;
    }
    // move starting index up
    i += count;
    // reset counter
    count = 1;
	}
  // insert null char
	res[len] = '\0';
  // return converted result
	printf("%s\n", res);
	return 0;
}
