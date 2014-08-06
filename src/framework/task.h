#ifndef TASK_H_
#define TASK_H_

#include "subsystem.h"

namespace tots { namespace framework {
  class SubsystemThread;
  class Task {
    public:
      Task(Subsystem *subsystem, Subsystem::Command command);
      ~Task();

      Subsystem *subsystem() const { return m_subsystem; }
      Subsystem::Command command() const { return m_command; }

      SubsystemThread *lastThread() const { return m_subsystem->lastThread(); }
      void setLastThread(SubsystemThread *thread) { m_subsystem->setLastThread(thread); }

      void run() { }

    private:
      Subsystem *m_subsystem;
      Subsystem::Command m_command;
  };
} }

#endif
