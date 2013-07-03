#ifndef CCTWCOMMANDLINE_H
#define CCTWCOMMANDLINE_H

class CctwCommandLine
{
public:
  CctwCommandLine();

public:
  static int integerValue(int argc, char *argv[], const char *flag, int defval);
  static double doubleValue(int argc, char *argv[], const char *flag, double defval);
  static const char* stringValue(int argc, char *argv[], const char *flag, const char *defval);
};

#endif // CCTWCOMMANDLINE_H
