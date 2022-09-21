// this is a partial solution for biggie_add that only works
// if n and m have the same sign

// We thought we would share this to help you develop your own solutions
// and inspire you to make some helper functions

// You may use this code in your solution but add the following comment
// (in your code, not in your integrity statement)

// some code based on partial solution released as part of the second chance


// get_digit(pos, s, s_len) retrieves s[pos] as an integer instead of its
//   ASCII value, and returns 0 if pos is too big for s
// requires: s is a valid string corresponding to biggie digits
//           pos >= 0
static int get_digit(int pos, const char *s, int s_len) {
  assert(s);
  assert(pos >= 0);
  if (pos >= s_len) {
    return 0;
  }
  return s[pos] - '0';
}

// abs_add(a, b) returns a new string calculating |a| + |b|
// requires: a, b are valid strings corresponding to biggie digits
// effects: returns a new string (allocates memory)
static char *abs_add(const char *a, const char *b) {
  assert(a);
  assert(b);
  int a_len = strlen(a);
  int b_len = strlen(b);
  int max_len = max(a_len, b_len) + 1; // extra digit for carry over
  char *digits = malloc((max_len + 1) * sizeof(char)); // + 1 for '\0'
  int carry = 0;
  for (int i = 0; i <= max_len; ++i) {
    int sum = carry + get_digit(i, a, a_len) + get_digit(i, b, b_len);
    digits[i] = (sum % 10) + '0';
    carry = sum / 10;
  }
  // note above it loops to <= max_len so there's always an extra trailing '0'
  // at the end of the array: we need to replace all trailing '0's with '\0's
  while (digits[max_len] == '0' && max_len) {
    digits[max_len] = '\0';
    --max_len;
  }
  return digits;
}

void biggie_add(struct biggie *n, const struct biggie *m) {
  assert(n);
  assert(m);
  if (n->negative == m->negative) {
    char *sum = abs_add(n->digits, m->digits);
    free(n->digits);
    n->digits = sum;
  } else {
    // TODO
  }
}
