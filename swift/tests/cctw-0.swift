
import assert;
import io;

import cctw;

main
{
  string result = cctw("42");
  printf("result: %s", result);
  assert(result == "42", "ERROR");
}
