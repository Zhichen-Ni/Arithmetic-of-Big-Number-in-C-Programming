///////////////////////////////////////////////////////////////////////////// 
// INTEGRITY STATEMENT (v3)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement:
// https://student.cs.uwaterloo.ca/~cs136/current/assignments/integrity.shtml
/////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with: 

// None 
//  
// Name: Zhichen Ni 
// login ID: z29ni 
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "biggie.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "cs136-trace.h"

/////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE THIS STRUCTURE
/////////////////////////////////////////////////////////////////////////////
struct biggie {
  bool negative;
  char *digits;
};
// notes: digits is a properly null-terminated dynamic string
//        digits contains the number reversed, so 1203 => "3021"
//        digits cannot have leading zeros, so 1203 cannot be "30210"
//        zero is stored as "0" and cannot be negative (negative is false)
/////////////////////////////////////////////////////////////////////////////



// see biggie.h for details
struct biggie *biggie_create(const char *s) {
  assert(s);
  int len = strlen(s);
  if (((s[0] == '-') && (s[1] == '0')) || 
      ((s[0] == '0') && (len > 1)) || (len == 0)) {
    return NULL;
  }
  if (((!(s[0] == '-')) && (s[0] < '0')) ||
      ((!(s[0] == '-')) && (s[0] > '9')))  {
    return NULL;
  }
  for (int i = 1; i < len; i++) {
    if ((s[i] > '9') || (s[i] < '0')) {
      return NULL;
    }
  }
  struct biggie *big = malloc(sizeof(struct biggie));
  if (s[0] == '-') {
    big->negative = true;
    big->digits = malloc(sizeof(char) * len);
    big->digits[len - 1] = '\0';
    for (int i = 1; i < len; i++) {
      big->digits[len - 1 - i] = s[i];
    }
  } else {
    big->negative = false;
    big->digits = malloc(sizeof(char) * (1 + len));
    for (int i = 0; i < len; i++) {
      big->digits[len - i - 1] = s[i];
    }
    big->digits[len] = '\0';
  }
  return big;
}

// see biggie.h for details
void biggie_destroy(struct biggie *big) {
  assert(big);
  free(big->digits);
  free(big);
}

// see biggie.h for details
void biggie_print(const struct biggie *big, bool newline) {
  assert(big);
  if (big->negative == true) {
    printf("-");
  }
  int len = strlen(big->digits);
  for (int i = (len - 1); i >= 0; i--) {
    int current_num = big->digits[i] - '0';
    printf("%d", current_num);
  }
  if (newline == true) {
    printf("\n");
  }
}

// see biggie.h for details
void biggie_add(struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  int len_m = strlen(m->digits);
  int len_n = strlen(n->digits);
  int len_small = ((len_m < len_n) ? len_m : len_n);
  int len_big = ((len_m > len_n) ? len_m : len_n);
  if (m->negative == n->negative) {
    n->digits = realloc(n->digits, sizeof(char) * (len_big + 2));
    n->digits[len_big] = '\0';
    int great = 0;
    for (int i = 0; i < len_small; i++) {
      int m_current = m->digits[i] - '0';
      int n_current = n->digits[i] - '0';
      int current = m_current + n_current + great;
      if (current < 10) {
        n->digits[i] = current + '0';
        great = 0;
      } else {
        n->digits[i] = current - 10 + '0';
        great = 1;
      }
    }
    if (len_n < len_m) {
      for (int i = len_small; i < len_big; i++) {
        n->digits[i] = m->digits[i];
      }
    }
    if (great == 1) {
      for (int i = len_small; i < len_big; i++) {
        int small_current = n->digits[i] - '0';
        if (small_current == 9) {
          n->digits[i] = '0';
        } else {
          n->digits[i]++;
          great = 0;
          break;
        }
      }
    }
    if (great == 1) {
      n->digits[len_big] = '1';
      n->digits[len_big + 1] = '\0';
    }
  } else {
    int large = 0;
    if (len_m > len_n) {
      large = -1;
    } else if (len_m < len_n) {
      large = 1;
    } else {
      for (int i = len_m - 1; i >= 0; i--) {
        if (n->digits[i] > m->digits[i]) {
          large = 1;
          break;
        } else if (n->digits[i] < m->digits[i]) {
          large = -1;
          break;
        }
      }
    }
    if (large == 0) {
      n->negative = false;
      n->digits[0] = '0';
      n->digits[1] = '\0';
      return;
    }
    if (large == -1) {
      if (n->negative == false) {
        n->negative = true;
      } else if (n->negative == true) {
        n->negative = false;
      }
    }
    if (large == -1) {
      char *m_temp = malloc(sizeof(char) * (len_n + 1));
      strcpy(m_temp, n->digits);
      n->digits = realloc(n->digits, sizeof(char) * (len_m + 1));
      n->digits[len_m] = '\0';
      strcpy(n->digits, m->digits);
      for (int i = 0; i < len_small; i++) {
        if (n->digits[i] >= m_temp[i]) {
          n->digits[i] = n->digits[i] - m_temp[i] + '0';
        } else {
          n->digits[i] = n->digits[i] + 10 - m_temp[i] + '0';
          for (int j = i + 1; j < len_big; j++) {
            if (n->digits[j] > '0') {
              n->digits[j]--;
              break;
            } else {
              n->digits[j] = '9';
            }
          }
        }
      }
      for (int i = len_big - 1; i >= 0; i--) {
        if (n->digits[i] != '0') {
          break;
        }
        if (n->digits[i] == '0') {
          n->digits[i] = '\0';
        }
      }
      free(m_temp);
    } else if (large == 1) {
      for (int i = 0; i < len_small; i++) {
        if (n->digits[i] >= m->digits[i]) {
          n->digits[i] = n->digits[i] - m->digits[i] + '0';
        } else {
          n->digits[i] = n->digits[i] + 10 - m->digits[i] + '0';
          for (int j = i + 1; j < len_big; j++) {
            if (n->digits[j] > '0') {
              n->digits[j]--;
              break;
            } else {
              n->digits[j] = '9';
            }
          }
        }
      }
      for (int i = len_big - 1; i >= 0; i--) {
        if (n->digits[i] != '0') {
          break;
        }
        if (n->digits[i] == '0') {
          n->digits[i] = '\0';
        }
      }
    }
  }
}

// see biggie.h for details
void biggie_sub(struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  struct biggie *m_temporary = malloc(sizeof(struct biggie));
  int len_m = strlen(m->digits);
  m_temporary->digits = malloc(sizeof(char) * (1 + len_m));
  strcpy(m_temporary->digits, m->digits);
  if (m->negative == true) {
    m_temporary->negative = false;
  } else {
    m_temporary->negative = true;
  }
  biggie_add(n, m_temporary);
  biggie_destroy(m_temporary);
}

// see biggie.h for details
struct biggie *biggie_copy(const struct biggie *n) {
  assert(n);
  struct biggie *big2 = malloc(sizeof(struct biggie));
  big2->negative = n->negative;
  big2->digits = malloc(sizeof(char) * (1 + strlen(n->digits)));
  strcpy(big2->digits, n->digits);
  return big2;
}

// see biggie.h for details
void biggie_mult(struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  struct biggie *m_temp = malloc(sizeof(struct biggie));
  int len_m = strlen(m->digits);
  m_temp->digits = malloc(sizeof(char) * (1 + len_m));
  strcpy(m_temp->digits, m->digits);
  m_temp->negative = m->negative;
  struct biggie *n_temp = malloc(sizeof(struct biggie));
  int len_n = strlen(n->digits);
  n_temp->digits = malloc(sizeof(char) * (1 + len_n));
  strcpy(n_temp->digits, n->digits);
  n_temp->negative = n->negative;
  trace_string(m_temp->digits);
  trace_string(n_temp->digits);
  if (((m_temp->digits[0] == '0') && (m_temp->digits[1] == '\0')) ||
      ((n_temp->digits[0] == '0') && (n_temp->digits[1] == '\0'))) {
    trace_string(n_temp->digits);
    trace_string(m_temp->digits);
    n->negative = false;
    n->digits[0] = '0';
    n->digits[1] = '\0';
    biggie_destroy(m_temp);
    biggie_destroy(n_temp);
    return;
  }
  trace_string(m_temp->digits);
  trace_string(n_temp->digits);
  if (m->negative == true) {
    if (n->negative == false) {
      n->negative = true;
    } else {
      n->negative = false;
    }
  }
  int len_max = len_m + len_n;
  n->digits = realloc(n->digits, sizeof(char) * (len_max + 1));
  n->digits[len_m + len_n] = '\0';
  for (int i = 0; i < len_max; i++) {
    n->digits[i] = '0';
  }
  for (int i = 0; i < len_n; i++) {
    for (int j = 0; j < len_m; j++) {
      int current = n->digits[i + j] - '0';
      current = current + 
        ((n_temp->digits[i] - '0') * (m_temp->digits[j] - '0'));
      if (current <= 9) {
        n->digits[i + j] = current + '0';
      } else {
        n->digits[i + j] = current % 10 + '0';
        int left = current / 10;
        for (int k = i + j + 1; k < len_max; k++) {
          int this = n->digits[k] - '0';
          this = this + left;
          if (this <= 9) {
            n->digits[k] = this + '0';
            break;
          } else {
            n->digits[k] = this % 10 + '0';
            left = this / 10;
          }
        }
      }
    }
  }
  for (int i = len_max - 1; i > 0; i--) {
    if (n->digits[i] != '0') {
      break;
    }
    if (n->digits[i] == '0') {
      n->digits[i] = '\0';
    }
  }
  biggie_destroy(m_temp);
  biggie_destroy(n_temp);
}

// see biggie.h for details
bool biggie_eq(const struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  if (m->negative != n->negative) {
    return false;
  }
  if (0 != (strcmp(m->digits, n->digits))) {
    return false;
  }
  return true;
}

// biggie_gt(n, m) determines if n > m
// time: O(logn + logm)
bool biggie_gt(const struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  int len_m = strlen(m->digits);
  int len_n = strlen(n->digits);
  if (biggie_eq(n, m) == true) {
    return false;
  }
  if ((false == n->negative) && (true == m->negative)) {
    return true;
  } else if ((false == m->negative) && (true == n->negative)) {
    return false;
  } else {
    if (false == m->negative) {
      if (len_m > len_n) {
        return false;
      } else if (len_m < len_n) {
        return true;
      } else {
        for (int i = len_m - 1; i >= 0; i--) {
          if (n->digits[i] > m->digits[i]) {
            return true;
          } else if (n->digits[i] < m->digits[i]) {
            return false;
          }
        }
      }
    }
    if (true == m->negative) {
      if (len_m < len_n) {
        return false;
      } else if (len_m > len_n) {
        return true;
      } else {
        for (int i = len_m - 1; i >= 0; i--) {
          if (n->digits[i] > m->digits[i]) {
            return false;
          } else if (n->digits[i] < m->digits[i]) {
            return true;
          }
        }
      }
    }
  }
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// BONUS:
/////////////////////////////////////////////////////////////////////////////

void biggie_div(struct biggie *n, const struct biggie *m) {
  assert(m);
  assert(n);
  int len_m = strlen(m->digits);
  int len_n = strlen(n->digits);
  trace_string(m->digits);
  assert(m->digits[len_m - 1] != '0');
  if ((n->digits[0] == '0') && (n->digits[1] == '\0')) {
    n->negative = false;
    n->digits[0] = '0';
    n->digits[1] = '\0';
    return;
  }
  int large = 0;
  if (len_m > len_n) {
    large = -1;
  } else if (len_m < len_n) {
    large = 1;
  } else {
    for (int i = len_m - 1; i >= 0; i--) {
      if (n->digits[i] > m->digits[i]) {
        large = 1;
        break;
      } else if (n->digits[i] < m->digits[i]) {
        large = -1;
        break;
      }
    }
  }
  if (m->negative == true) {
    if (n->negative == false) {
      n->negative = true;
    } else {
      n->negative = false;
    }
  }
  if (large == 0) {
    n->digits[0] = '1';
    n->digits[1] = '\0';
  }
}
