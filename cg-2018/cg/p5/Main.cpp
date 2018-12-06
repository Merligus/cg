#include "P5.h"

int
main(int argc, char** argv)
{
  return cg::Application{new P5{1280, 720}}.run(argc, argv);
}
