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
      void dump_registers();
      void set_breakpoint_at_address(std::intptr_t addr);

    private:
      void handle_command(const std::string& line);
      void continue_execution();
      auto get_pc() -> uint64_t;
      void set_pc(uint64_t pc);
      void step_over_breakpoint();
      void wait_for_signal();

      auto read_memory(uint64_t address) -> uint64_t;
      auto write_memory(uint64_t address, uint64_t value) -> void;

    private:
      std::string m_ProgName;
      pid_t m_Pid;
      std::unordered_map<std::intptr_t, Breakpoint> m_Breakpoints;
  };
}