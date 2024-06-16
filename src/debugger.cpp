#include "debugger.hpp"
#include "registers.hpp"

#include <vector>
#include <iomanip>
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

  void Debugger::set_breakpoint_at_address(std::intptr_t addr)
  {
    std::cout << "Set breakpoint at address 0x" << std::hex << addr << std::endl;
    Breakpoint bp {m_Pid, addr};
    bp.enable();
    m_Breakpoints[addr] = bp;
  }

  void Debugger::dump_registers()
  {
    for (const auto& rd : g_register_descriptors)
    {
      std::cout << rd.name << "0x" << std::setfill('0') << std::setw(16) << std::hex << get_register_value(m_Pid, rd.r) << std::endl;
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
    else if (is_prefix(command, "break"))
    {
      std::string addr { args[1], 2 }; // Natively we assume that the user has written 0xADDRESS
      set_breakpoint_at_address(std::stol(addr, 0, 16));
    }
    else if (is_prefix(command, "register"))
    {
      if (is_prefix(args[1], "dump"))
      {
        dump_registers();
      }
      else if (is_prefix(args[1], "read"))
      {
        std::cout << get_register_value(m_Pid, get_register_from_name(args[2])) << std::endl;
      }
      else if (is_prefix(args[1], "write"))
      {
        std::string val { args[3], 2 };
        set_register_value(m_Pid, get_register_from_name(args[2]), std::stol(val, 0, 16));
      }
    }
    else if (is_prefix(command, "memory"))
    {
      std::string addr { args[2], 2 };

      if (is_prefix(args[1], "read"))
      {
        std::cout << std::hex << read_memory(std::stol(addr, 0, 16)) << std::endl;
      }
      else if (is_prefix(args[1], "write"))
      {
        std::string val { args[3], 2 };
        write_memory(std::stol(addr, 0, 16), std::stol(val, 0, 16));
      }
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

  uint64_t Debugger::read_memory(uint64_t address)
  {
    return ptrace(PTRACE_PEEKDATA, m_Pid, address, nullptr);
  }

  void Debugger::write_memory(uint64_t address, uint64_t value)
  {
    ptrace(PTRACE_POKEDATA, m_Pid, address, value);
  }
}