// Typing of the Stars
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "common.h"
#include "level.h"

#include <iostream>
using namespace std;

/*!
 * \namespace Tots
 * The namespace used by Typing of the Stars.
 */

int main(int argc, char **argv)
{
  if(al_init() != true)
    return 1;

  if(al_install_keyboard() != true)
    return 1;

  if(al_init_primitives_addon() != true)
    return 1;

  if(al_init_image_addon() != true)
    return 1;

  ALLEGRO_DISPLAY *display = al_create_display(500, 768);

  Tots::Level *level;
  level = new Tots::Level("level01.lua");

  ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

  ALLEGRO_TIMER *timer = al_install_timer(1.0 / FRAMES_PER_SECOND);
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_start_timer(timer);

  ALLEGRO_EVENT event;

  bool needDraw = false;
  while(true)
  {
    al_wait_for_event(queue, &event);

    // TODO: handle input events

    if(event.type == ALLEGRO_EVENT_TIMER)
    {
      level->tick();
      needDraw = true;
    }
    if(needDraw && al_event_queue_is_empty(queue))
    {
      level->draw();
      al_flip_display();
      needDraw = false;
    }
  }

  al_uninstall_timer(timer);

  al_destroy_event_queue(queue);

  al_destroy_display(display);

  return 0;
}
