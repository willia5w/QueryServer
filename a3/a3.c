#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "a3.h"

const int kSize = 50;

// Print full name sequentially
void Big(Name *name) {
  char big[kSize];
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  } 
  new_first[strlen(name->first)] = '\0';
  strcpy(big, new_first);
  strcat(big, " ");
  char new_middle[kSize];
  if (name->middle != NULL) {
    new_middle[0] = toupper(name->middle[0]);
    for (i = 1; i < strlen(name->middle); i++) {
      new_middle[i] = tolower(name->middle[i]);
    }
  new_middle[strlen(name->middle)] = '\0';
  strcat(big, new_middle);
  strcat(big, " ");
  }
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(big, new_last);
  printf("%s", big);
}

// Last name first
void Last(Name *name) {
  char last[kSize];
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  int i = 0;
  for (i = 1; i < strlen(name->last); i++) {
     new_last[i] = tolower(name->last[i]);
  }  
  new_last[strlen(name->last)] = '\0';
  strcpy(last, new_last);
  strcat(last, ",");
  strcat(last, " ");
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  for (i = 1; i < strlen(name->first); i++) {
     new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcat(last, new_first);
  printf("%s", last);
}

void Reg(Name *name) {
  char reg[kSize];
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(reg, new_first);
  strcat(reg, " ");
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(reg, new_last);
  printf("%s", reg);
}

void Mid(Name *name) {
  char mid[kSize];
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(mid, new_first);
  strcat(mid, " ");
  // Middle name (optional)
  char mid_init[1];
  if (name->middle != NULL) {
    mid_init[0] = toupper(name->middle[0]);
    strcat(mid, mid_init);
    strcat(mid, ".");
    strcat(mid, " ");
  }
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(mid, new_last);
  printf("%s", mid);
}
// First name only
void Small(Name *name) {
  char small[kSize];
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(small, new_first);
  printf("%s",small);
}

// Handles invalid format inputs such as "w"
void FormatName(Name *name, char format) {
  format = tolower(format);

  switch (format)
  {
  case 'b': Big(name);
          break;
  case 'l': Last(name);
          break;
  case 'r': Reg(name);
          break;
  case 'm': Mid(name);
          break;
  case 's': Small(name);
          break;    
  default: printf("Invalid format");
;          break;
  }
}

void FillName(Name *name, char format, char *dest) {
  format = tolower(format);
  switch (format)
  {
  case 'b': 
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  } 
  new_first[strlen(name->first)] = '\0';
  strcpy(dest, new_first);
  strcat(dest, " ");
  char new_middle[kSize];
  if (name->middle != NULL) {
    new_middle[0] = toupper(name->middle[0]);
    for (i = 1; i < strlen(name->middle); i++) {
      new_middle[i] = tolower(name->middle[i]);
    }
  new_middle[strlen(name->middle)] = '\0';
  strcat(dest, new_middle);
  strcat(dest, " ");
  }
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(dest, new_last);
  break;

  case 'l': 
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  int i = 0;
  for (i = 1; i < strlen(name->last); i++) {
     new_last[i] = tolower(name->last[i]);
  }  
  new_last[strlen(name->last)] = '\0';
  strcpy(dest, new_last);
  strcat(dest, ",");
  strcat(dest, " ");
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  for (i = 1; i < strlen(name->first); i++) {
     new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcat(dest, new_first);
  break;

  case 'r': 
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(dest, new_first);
  strcat(dest, " ");
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(dest, new_last);
  break;

  case 'm':
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(dest, new_first);
  strcat(dest, " ");
  // Middle name (optional)
  char mid_init[1];
  if (name->middle != NULL) {
    mid_init[0] = toupper(name->middle[0]);
    strcat(dest, mid_init);
    strcat(dest, ".");
    strcat(dest, " ");
  }
  char new_last[kSize];
  new_last[0] = toupper(name->last[0]);
  for (i = 1; i < strlen(name->last); i++) {
    new_last[i] = tolower(name->last[i]);
  }
  new_last[strlen(name->last)] = '\0';
  strcat(dest, new_last);
  break;

  case 's':
  char new_first[kSize];
  new_first[0] = toupper(name->first[0]);
  int i = 0;
  for (i = 1; i < strlen(name->first); i++) {
    new_first[i] = tolower(name->first[i]);
  }
  new_first[strlen(name->first)] = '\0';
  strcpy(dest, new_first);
  break;
  default: printf("Invalid format");
            break;
  }
}
// Use qsort, also a switcher would work considering all ordering possibilities
/* USE strcompare()
void ListByName(Name *n1, Name *n2, Name *n3, char format) {
  char new_n1;
  char new_n2;
  char new_n3;
  char format = tolower(format);

  switch (format)
  {
  
  }
}

