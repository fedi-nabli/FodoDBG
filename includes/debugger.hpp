#pragma once

#include <string>
#include <utility>
#include <linux/types.h>

namespace Fdbg
{
  class Debugger
  {
    public:
      Debugger(std::string prog_name, pid_t pid)
        : m_ProgName{std::move(prog_name)}, m_Pid{pid} {}

      void run();

    private:
      std::string m_ProgName;
      pid_t m_Pid;
  };
}