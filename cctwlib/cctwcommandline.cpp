#include "cctwcommandline.h"
#include <stdlib.h>
#include <string.h>

CctwCommandLine::CctwCommandLine()
{
}

int CctwCommandLine::integerValue(int argc, char *argv[], const char *flag, int defval)
{
  for (int i=1; i<(argc-1); i++) {
    if (strcmp(flag, argv[i]) == 0) {
      return atoi(argv[i+1]);
    }
  }

  return defval;
}

double CctwCommandLine::doubleValue(int argc, char *argv[], const char *flag, double defval)
{
  for (int i=1; i<(argc-1); i++) {
    if (strcmp(flag, argv[i]) == 0) {
      return atof(argv[i+1]);
    }
  }

  return defval;
}

const char* CctwCommandLine::stringValue(int argc, char *argv[], const char *flag, const char *defval)
{
  for (int i=1; i<(argc-1); i++) {
    if (strcmp(flag, argv[i]) == 0) {
      return argv[i+1];
    }
  }

  return defval;
}
