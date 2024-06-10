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
  std::vector<std::string> split(const std::string& s, char delim)
  {
    std::vector<std::string> out{};
    std::stringstream ss {s};
    std::string item;

    while (std::getline(ss, item, delim))
    {
      out.push_back(item);
    }

    return out;
  }

  bool is_prefix(const std::string& s, const std::string& of)
  {
    if (s.size() > of.size()) return false;
    return std::equal(s.begin(), s.end(), of.begin());
  }

  void Debugger::run()
  {
    int wait_status;
    auto options = 0;
    waitpid(m_Pid, &wait_status, options);

    char* line = nullptr;
    while ((line = linenoise("fodogdb> ")) != nullptr)
    {
      handle_command(line);
      linenoiseHistoryAdd(line);
      linenoiseFree(line);
    }
  }

  void Debugger::handle_command(const std::string& line)
  {
    auto args = split(line, ' ');
    auto command = args[0];

    if (is_prefix(command, "continue"))
    {
      continue_execution();
    }
    else
    {
      std::cerr << "Unknown command" << std::endl;
    }
  }

  void Debugger::continue_execution()
  {
    ptrace(PTRACE_CONT, m_Pid, nullptr, nullptr);

    int wait_status;
    auto options = 0;
    waitpid(m_Pid, &wait_status, options);
  }
}