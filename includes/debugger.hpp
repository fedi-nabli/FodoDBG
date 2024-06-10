#pragma once

#include "breakpoint.hpp"

#include <string>
#include <utility>
#include <unordered_map>
#include <linux/types.h>

namespace Fdbg
{
  class Debugger
  {
    public:
      Debugger(std::string prog_name, pid_t pid)
        : m_ProgName{std::move(prog_name)}, m_Pid{pid} {}

      void run();
      void set_breakpoint_at_address(std::intptr_t addr);

    private:
      void handle_command(const std::string& line);
      void continue_execution();

    private:
      std::string m_ProgName;
      pid_t m_Pid;
      std::unordered_map<std::intptr_t, Breakpoint> m_Breakpoints;
  };
}