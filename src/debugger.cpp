#include "debugger.hpp"

#include <vector>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#include "linenoise.h"

namespace Fdbg
{
  void Debugger::run()
  {
    int wait_status;
    auto options = 0;
    waitpid(m_Pid, &wait_status, options);

    char* line = nullptr;
    while ((line = linenoise("fodogdb> ")) != nullptr)
    {
      // handle_command(line);
      linenoiseHistoryAdd(line);
      linenoiseFree(line);
    }
  }
}