#include "my_strings.h"

char* substring(char* text, int start, int end) {
  char* substr = (char*)malloc((end-start) * sizeof(char));

  for(int i = start; i <= end ; ++i) {
    substr[i-start] = text[i];
  }

  return substr;
}

char** split(char* text, int len, char delimiter) {
  int prevIndex = (text[0] == delimiter)?1:0;
  int segmentsNum = 0;
  char** segments = (char**)malloc(segmentsNum * sizeof(char*));

  for(int i = (text[0] == delimiter)?1:0 ; i < len ; ++i) {
    if(text[i] == delimiter) {
      segments = (char**)realloc(segments, ++segmentsNum * sizeof(char*));
      segments[segmentsNum - 1] = substring(text, prevIndex, i-1);
      prevIndex = i+1;
    }
  }

  if(text[len-1] != delimiter) {
    segments = (char**)realloc(segments, ++segmentsNum * sizeof(char*));
    segments[segmentsNum - 1] = substring(text, prevIndex, len-1);
  }

  return segments;
}
