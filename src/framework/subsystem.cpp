#include "subsystem.h"

#include "subsystemThread.h"

#include <cstdio>

namespace tots {
  /**
   * This constructs a Subsystem object.
   *
   * The Subsystem constructor is typically called from the main thread. Derived
   * classes should also assume that their constructors will also be called from
   * the main thread.
   */
  Subsystem::Subsystem() : m_lastThread(NULL), m_hoggedThread(NULL) {
  }

  /**
   * This destroys a Subsystem object.
   *
   * Most of the real cleanup might actually happen in close() and m_close().
   */
  Subsystem::~Subsystem() {
    if(m_hoggedThread != NULL) {
      /**
       * Normally, m_readySemaphore is owned by the ThreadPool class. With a
       * hogged thread, the Subsystem object effectively owns the semaphore and
       * must destroy it here.
       */
      SDL_DestroySemaphore(m_hoggedThread->m_readySemaphore);
      m_hoggedThread->m_readySemaphore = NULL;
      delete m_hoggedThread;
    }
  }

  /**
   * The init() method is called early from the GameLoop to initialize this
   * Subsystem object in a thread. The init() method does some Subsystem upkeep
   * and then immediately calls m_init() on itself.
   *
   * Derived Subsystem classes that wish to initialize themselves in a thread
   * (as opposed to in their constructor, which is likely run in the main
   * thread) must implement m_init().
   *
   * \sa m_init()
   */
  void Subsystem::init(const GameState *state) {
    // TODO: initialize something I guess
    m_init(state);
  }

  /**
   * The update() method is called regularly from the GameLoop according to the
   * Subsystem object's period, among other factors such as thread availability.
   * This method performs some per-subsystem-tick upkeep and then immediately
   * calls the m_update() method on itself.
   *
   * Derived Subsystem classes must implement m_update() to update themselves.
   *
   * \sa m_update()
   */
  void Subsystem::update(const GameState *state) {
    // TODO: determine and update the delta time in m_dt
    m_update(state);
  }

  /**
   * The close() method is counterpart to the init() method. close() is called
   * late in the GameLoop object's lifetime and is intended to clean us anything
   * that was initialized in init() or any other time during the Subsystem
   * object's lifetime. close() does some general Subsystem clean up and then
   * immediately calles m_close() on itself.
   *
   * Derived Subsystem classes must implement m_close() to clean themselves up.
   *
   * close() is run in a subsystem thread, as opposed to the destructor, which
   * is run in the main thread. Subsystems should try to clean up most of their
   * resources in m_close() rather than in the destructor.
   *
   * \sa m_close()
   */
  void Subsystem::close(const GameState *state) {
    // TODO: close stuff I guess
    m_close(state);
  }
}
