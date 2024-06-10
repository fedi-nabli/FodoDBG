#include "fdbg.hpp"
#include "debugger.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/personality.h>

namespace Fdbg
{
  int fdbg(int argc, char* argv[])
  {
    if (argc < 2)
    {
      std::cerr << "Program name not specified" << std::endl;
      return -1;
    }

    auto prog = argv[1];

    auto pid = fork();
    if (pid == 0)
    {
      // We're in the child process
      // execute debugee
      personality(ADDR_NO_RANDOMIZE);
      ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
      execl(prog, prog, nullptr);
    }
    else if (pid >= 1)
    {
      // We're in the parent process
      // execute debugger
      std::cout << "Started debugging process " << pid << std::endl;
      Debugger dbg {prog, pid};
      dbg.run();
    }

    return 0;
  }
}