#include <errno.h>
#include <string.h>

#include "acutest.h"
#include "s21_string.h"
// #define TEST_NO_MAIN

static void test_s21_memchr(void) {
  {
    const char str[] = "Hello, World!";
    int c = 'o';
    s21_size_t n = 13;

    void *result = s21_memchr(str, c, n);
    void *expected = memchr(str, c, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str[] = "Hello, World!";
    int c = 'z';  // Character not present in the string
    s21_size_t n = 13;

    void *result = s21_memchr(str, c, n);
    void *expected = memchr(str, c, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int c = 'X';
    s21_size_t n = 26;

    void *result = s21_memchr(str, c, n);
    void *expected = memchr(str, c, n);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_memcmp(void) {
  {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    s21_size_t n = 5;

    int result = s21_memcmp(str1, str2, n);
    int expected = memcmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello";
    const char str2[] = "World";
    s21_size_t n = 5;

    int result = s21_memcmp(str1, str2, n);
    int expected = memcmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char str2[] = "abcdefghijklmnopqrstuvwxyz";
    s21_size_t n = 26;

    int result = s21_memcmp(str1, str2, n);
    int expected = memcmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_memset(void) {
  {
    char str1[] = "Hello, World!";
    char str2[] = "Hello, World!";
    s21_size_t n = 13;

    s21_memset(str1, '*', n);
    memset(str2, '*', n);

    TEST_CHECK(s21_memcmp(str1, str2, n) == 0);
  }

  {
    char str1[1000];
    char str2[1000];
    s21_size_t n = 1000;

    for (s21_size_t i = 0; i < n; ++i) {
      str1[i] = 'A';
      str2[i] = 'A';
    }

    s21_memset(str1, 'B', n);
    memset(str2, 'B', n);

    TEST_CHECK(s21_memcmp(str1, str2, n) == 0);
  }
}

static void test_s21_strncat(void) {
  {
    char dest1[20] = "Hello, ";
    char dest2[20] = "Hello, ";
    const char src[] = "World!";
    s21_size_t n = 5;

    char *result = s21_strncat(dest1, src, n);
    char *expected = strncat(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
  }

  {
    char dest1[20] = "Hello, ";
    char dest2[20] = "Hello, ";
    const char src[] = "World!";
    s21_size_t n = 10;

    char *result = s21_strncat(dest1, src, n);
    char *expected = strncat(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
  }

  {
    char dest1[20] = "";
    char dest2[20] = "";
    const char src[] = "World!";
    s21_size_t n = 5;

    char *result = s21_strncat(dest1, src, n);
    char *expected = strncat(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
  }
  {
    char dest1[20] = "Hello, ";
    char dest2[20] = "Hello, ";
    const char src[] = "World!";
    s21_size_t n = 10;

    char *result = s21_strncat(dest1, src, n);
    char *expected = strncat(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
  }
  {
    char dest1[20] = "Hello, ";
    char dest2[20] = "Hello, ";
    const char src[] = "World!";
    s21_size_t n = 15;

    char *result = s21_strncat(dest1, src, n);
    char *expected = strncat(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) == 0);
  }
}

static void test_s21_strncmp(void) {
  {
    const char str1[] = "Hello";
    const char str2[] = "Hello";
    s21_size_t n = 5;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello";
    const char str2[] = "World";
    s21_size_t n = 5;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char str2[] = "abcdefghijklmnopqrstuvwxyz";
    s21_size_t n = 26;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello";
    const char str2[] = "Hell";
    s21_size_t n = 10;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hell";
    const char str2[] = "Hello";
    s21_size_t n = 10;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello";
    const char str2[] = "World";
    s21_size_t n = 10;

    int result = s21_strncmp(str1, str2, n);
    int expected = strncmp(str1, str2, n);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_strncpy(void) {
  {
    char dest1[10] = "Hello";
    char dest2[10] = "Hello";
    const char src[] = "World!";
    s21_size_t n = 5;

    char *result = s21_strncpy(dest1, src, n);
    char *expected = strncpy(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) == 0);
  }
  {
    char dest1[10] = "Hello";
    char dest2[10] = "Hello";
    const char src[] = "World!";
    s21_size_t n = 10;

    char *result = s21_strncpy(dest1, src, n);
    char *expected = strncpy(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) == 0);
  }

  {
    char dest1[10] = "";
    char dest2[10] = "";
    const char src[] = "World!";
    s21_size_t n = 5;

    char *result = s21_strncpy(dest1, src, n);
    char *expected = strncpy(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) == 0);
  }
  {
    char dest1[10] = "Hello";
    char dest2[10] = "Hello";
    const char src[] = "World!";
    s21_size_t n = 10;

    char *result = s21_strncpy(dest1, src, n);
    char *expected = strncpy(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) == 0);
  }
  {
    char dest1[10] = "Hello";
    char dest2[10] = "Hello";
    const char src[] = "World!";
    s21_size_t n = 3;

    char *result = s21_strncpy(dest1, src, n);
    char *expected = strncpy(dest2, src, n);

    TEST_CHECK(result == dest1);
    TEST_CHECK(expected == dest2);
    TEST_CHECK(strcmp(dest1, dest2) != 0);
  }
}

static void test_s21_strtok(void) {
  char str1[] = "Hello,World!";
  char delim1[] = ",";
  char *result1 = s21_strtok(str1, delim1);
  char *expected1 = strtok(str1, delim1);

  if (result1 != NULL && expected1 != NULL) {
    TEST_CHECK(strcmp(result1, expected1) == 0);
  }

  char str2[] = "Hello,,World!";
  char delim2[] = ",";
  char *result2 = s21_strtok(str2, delim2);
  char *expected2 = strtok(str2, delim2);

  if (result2 != NULL && expected2 != NULL) {
    TEST_CHECK(strcmp(result2, expected2) == 0);
  }

  char str3[] = ",,Hello,World!";
  char delim3[] = ",";
  char *result3 = s21_strtok(str3, delim3);
  char *expected3 = strtok(str3, delim3);

  if (result3 != NULL && expected3 != NULL) {
    TEST_CHECK(strcmp(result3, expected3) == 0);
  }

  char str4[] = "HelloWorld!";
  char delim4[] = ",";
  char *result4 = s21_strtok(str4, delim4);
  char *expected4 = strtok(str4, delim4);

  if (result4 != NULL && expected4 != NULL) {
    TEST_CHECK(strcmp(result4, expected4) == 0);
  }
}

static void test_s21_strrchr(void) {
  {
    const char str[] = "Hello, World!";
    int c = 'o';

    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);
    TEST_CHECK(result - str + 1 == expected - str + 1);
  }
  {
    const char str[] = "Hello, World!";
    int c = 'z';  // Character not present in the string

    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }
  {
    const char str[] = "Hello, World!";
    int c = 'H';

    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }

  {
    const char str[] = "Hello, World!";
    int c = '!';

    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }
  {
    const char str[] = "Hello, World!";
    int c = 'l';

    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }

  {
    const char str[] = "";
    int c = 'z';
    char *result = s21_strrchr(str, c);
    char *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }

  {
    const char str[] = "Hello, World!";
    int c = 'W';

    void *result = s21_strrchr(str, c);
    void *expected = strrchr(str, c);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_strpbrk(void) {
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "aeiou";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello, World!";
    const char str2[] = "1234567890";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello, World!";
    const char str2[] = "!1234567890";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello, World!";
    const char str2[] = "aeiou!";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "";
    const char str2[] = "aeiou";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }

  {
    const char str1[] = "Hello, World!";
    const char str2[] = "1234567890";

    void *result = s21_strpbrk(str1, str2);
    void *expected = strpbrk(str1, str2);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_memcpy() {
  {
    char src[] = "Hello";
    char dest[10];
    s21_size_t n = 5;

    void *result = s21_memcpy(dest, src, n);
    void *expected = memcpy(dest, src, n);

    TEST_CHECK(result == expected);
  }

  {
    char src[] = "Crock of oooo";
    char dest[10];
    s21_size_t n = 1;

    void *result = s21_memcpy(dest, src, n);
    void *expected = memcpy(dest, src, n);

    TEST_CHECK(result == expected);
  }

  {
    char src[1000];
    char dest[1000];
    s21_size_t n = 1000;

    for (s21_size_t i = 0; i < n; ++i) {
      src[i] = i % 256;
    }

    void *result = s21_memcpy(dest, src, n);
    void *expected = memcpy(dest, src, n);

    TEST_CHECK(result == expected);
  }
}

static void test_s21_strstr() {
  {
    const char haystack[] = "Hello, World!";
    const char needle[] = "World";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }
  {
    const char haystack[] = "Hello, World!";
    const char needle[] = "Hello";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  {
    const char haystack[] = "Hello, World!";
    const char needle[] = "World!";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  {
    const char haystack[] = "Hello, World!";
    const char needle[] = ", ";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  {
    const char haystack[] = "";
    const char needle[] = "World";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  {
    const char haystack[] = "Hello, World!";
    const char needle[] = "";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  // Test s21_strstr - Needle not found
  {
    const char haystack[] = "Hello, World!";
    const char needle[] = "Universe";

    void *result = s21_strstr(haystack, needle);
    void *expected = strstr(haystack, needle);

    TEST_CHECK(result == expected);
  }

  // Test s21_strstr - Negative tests
  // {
  //     const char haystack[] = "Hello, World!";
  //     const char needle[] = "Hello, Universe!";

  //     void *result = s21_strstr(haystack, needle);
  //     void *expected = strstr(haystack, needle);

  //     TEST_CHECK(result == expected);
  // }
}
static void test_s21_strerror() {
  {
    int errnum = EINVAL;
    const char *result = s21_strerror(errnum);
    const char *expected = strerror(errnum);

    TEST_CHECK(result != NULL);
    TEST_CHECK(expected != NULL);
    TEST_CHECK(strcmp(result, expected) == 0);
  }
  {
    int errnum = 9999;
    const char *result = s21_strerror(errnum);
    const char *expected = strerror(errnum);

    TEST_CHECK(result != NULL);
    TEST_CHECK(expected != NULL);
    TEST_CHECK(strcmp(result, expected) == 0);
  }

  {
    int errnum = -EINVAL;
    const char *result = s21_strerror(errnum);
    const char *expected = strerror(errnum);

    TEST_CHECK(result != NULL);
    TEST_CHECK(expected != NULL);
    TEST_CHECK(strcmp(result, expected) == 0);
  }

  {
    int errnum = 0;  // No error
    const char *result = s21_strerror(errnum);
    const char *expected = strerror(errnum);

    TEST_CHECK(result != NULL);
    TEST_CHECK(expected != NULL);
    TEST_CHECK(strcmp(result, expected) == 0);
  }

  {
    int errnum = EINVAL;
    const char *result = s21_strerror(errnum);

    TEST_CHECK(result != NULL);
    TEST_CHECK(strlen(result) > 0);
  }
}
static void test_s21_strcspn() {
  {
    const char str1[] = "Hello, World!";
    const char str2[] = ", ";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }

  // Test s21_strcspn - No common characters
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "1234567890";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }

  // Test s21_strcspn - Common characters at the beginning
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "Hello";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }

  // Test s21_strcspn - Common characters at the end
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "World!";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }

  // Test s21_strcspn - Empty str1
  {
    const char str1[] = "";
    const char str2[] = ", ";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }

  // Test s21_strcspn - Empty str2
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }
  {
    const char str1[] = "Hello, World!";
    const char str2[] = "!1234567890";

    s21_size_t result = s21_strcspn(str1, str2);
    s21_size_t expected = strcspn(str1, str2);

    TEST_CHECK(result == expected);
  }
}
static void test_s21_strlen() {
  {
    const char str[] = "Hello, World!";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - Empty string
  {
    const char str[] = "";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - Null string

  // Test s21_strlen - String with spaces
  {
    const char str[] = "   ";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - String with newline characters
  {
    const char str[] = "Hello,\nWorld!";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - String with special characters
  {
    const char str[] = "!@#$%^&*()";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - String with multiple null terminators
  {
    const char str[] = "Hello\0\0\0World!";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen(str);

    TEST_CHECK(result == expected);
  }

  // Test s21_strlen - Negative tests
  {
    const char str[] = "Hello, World!";
    s21_size_t result = s21_strlen(str);
    s21_size_t expected = strlen("Hello, Universe!");

    TEST_CHECK(result != expected);
  }
}

static void test_s21_strchr() {
  {
    const char str[] = "Hello, World!";
    int ch = 'o';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Character not found
  {
    const char str[] = "Hello, World!";
    int ch = 'z';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Null character
  {
    const char str[] = "Hello, World!";
    int ch = '\0';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Empty string
  {
    const char str[] = "";
    int ch = 'o';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Character at the beginning
  {
    const char str[] = "Hello, World!";
    int ch = 'H';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Character at the end
  {
    const char str[] = "Hello, World!";
    int ch = '!';
    char *result = s21_strchr(str, ch);
    char *expected = strchr(str, ch);

    TEST_CHECK(result == expected);
  }

  // Test s21_strchr - Negative tests
  {
    const char str[] = "Hello, World!";
    int ch = 'o';
    char *result = s21_strchr(str, ch);
    char *expected = strchr("Hello, Universe!", ch);

    TEST_CHECK(result != expected);
  }
}
// void *trim(const char *src, const char *trim_chars);
// void *insert(const char *src, const char *str, s21_size_t start_index);
// void *to_lower(const char *str);
// void *to_upper(const char *str);

static void test_to_upper(void) {
  const char str1[] = "Hello, World!";
  char *result1 = to_upper(str1);
  char *expected1 = "HELLO, WORLD!";

  TEST_CHECK(strcmp(result1, expected1) == 0);
  free(result1);

  const char str2[] = "1a";
  char *result2 = to_upper(str2);
  char *expected2 = "1A";

  TEST_CHECK(strncmp(result2, expected2, 2) == 0);
  free(result2);

  const char str3[] = "abc123!@#";
  char *result3 = to_upper(str3);
  char *expected3 = "ABC123!@#";

  TEST_CHECK(strncmp(result3, expected3, 9) == 0);
  // TEST_MSG("s21 Produced: %d", strncmp(result3, expected3,9));
  // TEST_MSG("Produced: %d", strcmp(result3, expected3));
  free(result3);
}
static void test_to_lower(void) {
  // Test Case 1: Basic conversion
  const char str1[] = "Hello, World!";
  char *result1 = to_lower(str1);
  char *expected1 = "hello, world!";

  TEST_CHECK(result1 != NULL);
  TEST_CHECK(strcmp(result1, expected1) == 0);
  free(result1);

  // Test Case 2: Empty string
  const char str2[] = "OS1";
  char *result2 = to_lower(str2);

  TEST_CHECK(result2 != NULL);
  TEST_CHECK(strncmp(result2, "os1", 3) == 0);
  free(result2);

  // Test Case 3: String with digits and symbols
  const char str3[] = "ABC123!@#";
  char *result3 = to_lower(str3);
  char *expected3 = "abc123!@#";

  TEST_CHECK(result3 != NULL);
  TEST_CHECK(strncmp(result3, expected3, 9) == 0);
  free(result3);
}
static void test_trim(void) {
  // Test Case 1: Basic trimming
  const char src1[] = " H e l l o ,   W o r l d !    ";
  const char trim_chars1[] = " ";
  char *result1 = trim(src1, trim_chars1);
  char *expected1 = "Hello,World!";

  TEST_CHECK(strcmp(result1, expected1) == 0);
  free(result1);

  // Test Case 2: Empty string
  const char src2[] = "D--O--N--A-L-N-D-TRUM---P";
  const char trim_chars2[] = "--";
  char *result2 = trim(src2, trim_chars2);

  TEST_CHECK(strcmp(result2, "DONALNDTRUMP") == 0);
  free(result2);

  const char src4[] = "--Hello, World!--";
  const char trim_chars4[] = "-";
  char *result4 = trim(src4, trim_chars4);
  char *expected4 = "Hello, World!";

  TEST_CHECK(strcmp(result4, expected4) == 0);
  free(result4);

  const char *src5 = NULL;
  const char trim_chars5[] = " ";
  char *result5 = trim(src5, trim_chars5);

  TEST_CHECK(result5 == NULL);
}

static void test_insert(void) {
  // Test Case 1: Basic insertion
  const char src1[] = "Hello, World!";
  const char str1[] = "Beautiful ";
  s21_size_t start_index1 = 7;
  char *result1 = insert(src1, str1, start_index1);
  char *expected1 = "Hello, Beautiful World!";

  TEST_CHECK(result1 != NULL);
  TEST_CHECK(strcmp(result1, expected1) == 0);
  free(result1);

  // Test Case 2: Inserting at the beginning
  const char src2[] = "World!";
  const char str2[] = "Hello, ";
  s21_size_t start_index2 = 0;
  char *result2 = insert(src2, str2, start_index2);
  char *expected2 = "Hello, World!";

  TEST_CHECK(result2 != NULL);
  TEST_CHECK(strcmp(result2, expected2) == 0);
  free(result2);

  // Test Case 3: Inserting at the end
  const char src3[] = "Hello, ";
  const char str3[] = "World!";
  s21_size_t start_index3 = 7;
  char *result3 = insert(src3, str3, start_index3);
  char *expected3 = "Hello, World!";

  TEST_CHECK(result3 != NULL);
  TEST_CHECK(strcmp(result3, expected3) == 0);
  free(result3);

  // Test Case 4: Inserting an empty string
  const char src4[] = "Hello, World!";
  const char str4[] = "";
  s21_size_t start_index4 = 7;
  char *result4 = insert(src4, str4, start_index4);
  char *expected4 = "Hello, World!";

  TEST_CHECK(result4 != NULL);
  TEST_CHECK(strcmp(result4, expected4) == 0);
  free(result4);
}

static void test_sprintf(void) {
  char buffer[1000];
  char test_buffer[1000];

  int result =
      s21_sprintf(buffer, "%s %d %f %c %u %o", "Hello", 5, 2.5, 'i', -10000, 5);
  int expected = sprintf(test_buffer, "%s %d %f %c %u %o", "Hello", 5, 2.5, 'i',
                         -10000, 5);

  TEST_CHECK(result == expected);
  if (result == expected) {
    TEST_CHECK(strcmp(buffer, test_buffer) == 0);
  }
  result = s21_sprintf(buffer, "%6s %-5d %2f %c %-u %#o", "Hello", 5, 2.5, 'i',
                       -10000, 5);
  expected = sprintf(test_buffer, "%6s %-5d %2f %c %-u %#o", "Hello", 5, 2.5,
                     'i', -10000, 5);

  TEST_CHECK(result == expected);
  if (result == expected) {
    TEST_CHECK(strcmp(buffer, test_buffer) == 0);
  }
  const char *str = "Hello";
  result = s21_sprintf(buffer, "%x %p %% %#010Xabc", 15, str, 15);
  expected = sprintf(test_buffer, "%x %p %% %#010Xabc", 15, str, 15);
  
  TEST_CHECK(result == expected);
  if (result == expected) {
    TEST_CHECK(strcmp(buffer, test_buffer) == 0);
  }
  short int num = 10000;
  result = s21_sprintf(buffer, "%+lf %+hd", 12345.6789, num);
  expected = sprintf(test_buffer, "%+lf %+hd", 12345.6789, num);

  TEST_CHECK(result == expected);
  if (result == expected) {
    TEST_CHECK(strcmp(buffer, test_buffer) == 0);
  }

#define TCASE(_name) \
  { #_name, _name }

TEST_LIST = {
    TCASE(test_s21_memchr),  TCASE(test_s21_memcmp),  TCASE(test_s21_memset),
    TCASE(test_s21_strncat), TCASE(test_s21_strncmp), TCASE(test_s21_strncpy),
    TCASE(test_s21_strtok),  TCASE(test_s21_strrchr), TCASE(test_s21_strpbrk),
    TCASE(test_s21_memcpy),  TCASE(test_s21_strstr),  TCASE(test_s21_strerror),
    TCASE(test_s21_strcspn), TCASE(test_s21_strlen),  TCASE(test_s21_strchr),
    TCASE(test_to_upper),    TCASE(test_to_lower),    TCASE(test_trim),
    TCASE(test_insert),      TCASE(test_sprintf),     {NULL, NULL}};
