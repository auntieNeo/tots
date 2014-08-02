#ifndef TASK_H_
#define TASK_H_

#include "subsystem.h"

namespace tots {
  class Task {
    public:
      Task(Subsystem *subsystem, Subsystem::Command command);
      ~Task();

      Subsystem *subsystem() { return m_subsystem; }
      Subsystem::Command command() { return m_command; }

    private:
      Subsystem *m_subsystem;
      Subsystem::Command m_command;
  };
}

#endif
