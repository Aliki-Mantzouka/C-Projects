#ifndef UTILS_H
#define UTILS_H

// Αφαιρεί κενά από την αρχή και το τέλος ενός string
char* trim_whitespace(char* str);

// Χωρίζει ένα string σε πίνακα από strings
char** split_line(char* line, const char* delimiters);

#endif
