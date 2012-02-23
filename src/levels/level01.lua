function foobar()
  blah = blah + 1
  spawnEntity(blah, 100 - (blah % 15) * 25, copyEntity("enemy"))
end

function initLevel()
  addSprite(Sprite("player", "sprites/entityTest-32x32.png", 32, 32,
      3,  -- number of states
      0, 2, 0,  -- number of frames per state
      0, 42, 0,  -- frames per second
      2))  -- frames per row

  -- Enemy
  addSprite(Sprite("enemy", "sprites/entityTest-32x32.png", 32, 32,
      3,  -- number of states
      0, 2, 0,  -- number of frames per state
      0, 42, 0,  -- frames per second
      2))  -- frames per row
  path = Path("enemy", 0.5)
  Path_quadTo(path, 150, 0, 150, 150, 0.5)
  Path_quadTo(path, 150, 350, 0, 350, 2)
  addBehaviorState(path);
  behavior = Behavior("enemy", 3)
  Behavior_addState(behavior, 1, getBehaviorState("enemy"))
  addBehavior(behavior)
  addEntity(Entity("enemy", 1, getSprite("enemy"), getBehavior("enemy")))
  
  blah = 10
  for i=1,500 do
    addTimedEvent(level, 3+(i * 0.005), "foobar")
  end
end
