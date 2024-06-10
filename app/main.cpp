#include <iostream>
#include <unistd.h>
#include <sys/ptrace.h>

int main(int argc, char* argv[])
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
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execl(prog, prog, nullptr);
  }
  else if (pid >= 1)
  {
    // We're in the parent process
    // execute debugger
  }

  return 0;
}