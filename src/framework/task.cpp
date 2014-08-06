#include "task.h"

namespace tots { namespace framework {
  Task::Task(Subsystem *subsystem, Subsystem::Command command) : m_subsystem(subsystem), m_command(command) {
  }

  Task::~Task() {
  }
} }
