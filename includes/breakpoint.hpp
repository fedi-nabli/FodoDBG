#pragma once

#include <cstdint>
#include <unistd.h>
#include <linux/types.h>
#include <sys/ptrace.h>

namespace Fdbg
{
  class Breakpoint
  {
    public:
      Breakpoint(pid_t pid, std::intptr_t addr)
        : m_Pid{pid}, m_Addr{addr}, m_Enabled{false}, m_SavedData{}
      {}

      inline void enable()
      {
        auto data = ptrace(PTRACE_PEEKDATA, m_Pid, m_Addr, nullptr);
        m_SavedData = static_cast<uint8_t>(data & 0xff); // Save bottom byte
        uint64_t int3 = 0xcc;
        uint64_t data_with_int3 = ((data & ~0xff) | int3);
        ptrace(PTRACE_POKEDATA, m_Pid, m_Addr, data_with_int3);

        m_Enabled = true;
      }

      inline void disable()
      {
        auto data = ptrace(PTRACE_PEEKDATA, m_Pid, m_Addr, nullptr);
        auto restored_data = ((data & ~0xff) | m_SavedData);
        ptrace(PTRACE_POKEDATA, m_Pid, m_Addr, restored_data);

        m_Enabled = false;
      }

      auto is_enabled() const -> bool { return m_Enabled; }
      auto get_address() const -> std::intptr_t { return m_Addr; }

    private:
      pid_t m_Pid;
      bool m_Enabled;
      uint8_t m_SavedData;
      std::intptr_t m_Addr;
  };
}