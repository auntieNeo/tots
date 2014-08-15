-- TODO: a list of funny words
dictionary = {
  "A better pill",
  "A dime a dozen",
  "Ace in the hole",
  "Achilles' heel",
  "Add insult to injury",
  "All ears",
  "All thumbs",
  "Apple of discord",
  "At the drop of a hat",
  "Barking up the wrong tree",
  "Basket case",
  "Beat around the bush",
  "Bite off more than one can chew",
  "Bite the bullet",
  "Bite the dust",
  "Break a leg",
  "Burn the midnight oil",
  "Bust one's chops",
  "By the seat of one's pants",
  "By the skin of one's teeth",
  "Call it a day",
  "Cat nap   A nap",
  "Champ at the bit",
  "Chomp at the bit",
  "Chew the fat",
  "Chink in one's armor",
  "Clam up",
  "Cold shoulder",
  "Couch potato",
  "Cut a rug",
  "Cut the cheese",
  "Cut the mustard",
  "Don't have a cow ",
  "Drop a dime ",
  "Fit as a fiddle",
  "For a song",
  "From A to Z",
  "From scratch",
  "Get bent out of shape",
  "Have a blast",
  "Have eyes in the back of one's head ",
  "Hit the road ",
  "Hit the sack ",
  "Let the cat out of the bag ",
  "Kick the bucket",
  "Off one's trolley",
  "Off one's rocker",
  "Off the hook",
  "Pop one's clogs",
  "Piece of cake ",
  "Pull somebody's leg",
  "Pushing up daisies",
  "Put the cat among the pigeons",
  "Right as rain",
  "Screw the pooch",
  "Shoot the breeze",
  "Sleep with the fishes",
  "Spill the beans",
  "Split the whistle",
  "Take the biscuit"
}

function spawnEnemy()
  enemy = makeEnemy()
  -- make sure it isn't the same as something that exists
  duplicate = false
  for j, existing in ipairs(enemies) do
    if string.sub(existing["remainingText"], 1, 1) == string.sub(enemy["remainingText"], 1, 1) then
      duplicate = true
      break
    end
  end
  if not duplicate then
    enemies[table.getn(enemies) + 1] = enemy
  end
end

function makeEnemy()
  enemy = {
    text=dictionary[love.math.random(1, table.getn(dictionary))],
    x=love.math.random(0, love.graphics.getWidth()),
    y=love.math.random(0, love.graphics.getHeight())
  }
  enemy["remainingText"] = enemy["text"]

  return enemy
end

enemies = {}
g_missileAccel = 0.1
g_engineAccel = 2
g_maxPlayerSpeed = 15
g_playerFriction = 1
g_frictionThreshold = 0.5
g_collisionRadius = 90
g_enemyWanderRate = 2
g_worldMargin = 100
missiles = {}
player = {
  target=0,
  x=100,
  y=100,
  x_vel=0,
  y_vel=0,
  mode="flying",
  x_engine = 0,
  y_engine = 0
}

function love.load()
  -- create a bunch of "enemy" objects at random positions with random words
  for i = 1, 50 do
    spawnEnemy()
  end
end

function fireMissile(target)
  i = table.getn(missiles) + 1
  vector = {
    x=enemies[target]["x"] - player["x"],
    y=enemies[target]["y"] - player["y"]
  }
  vector = normalizeVector(vector)
  missiles[i] = {
    target=target,
    x=player["x"],
    y=player["y"],
    x_vel=(vector["x"] * 20) + love.math.random(0, 5),  -- random is more interesting
    y_vel=(vector["y"] * 20) + love.math.random(0, 5),
  }
end

function love.keypressed(key)
  if player["mode"] == "flying" then
    -- flying around mode with .oeu keys
    if key == "." then
      player["y_engine"] = -1
    end
    if key == "e" then
      player["y_engine"] = 1
    end
    if key == "o" then
      player["x_engine"] = -1
    end
    if key == "u" then
      player["x_engine"] = 1
    end
    if key == "tab" then
      player["x_engine"] = 0
      player["y_engine"] = 0
      player["mode"] = "shooting"
    end
  else
    -- use tab key to un-target anything
    if key == "tab" then
      player["mode"] = "flying"
      return
    end
    -- spawn a rocket that targets an enemy, if we have a target
    -- check if the player has a current target
    target = 0
    if player["target"] ~= 0 then
      if key == string.lower(string.sub(enemies[player["target"]]["remainingText"], 1, 1)) then
        target = player["target"]
      end
    end
    if target == 0 then
      -- find the first enemy that matches the key
      for i, enemy in ipairs(enemies) do
        if key == string.lower(string.sub(enemy["remainingText"], 1, 1)) then
          target = i
          player["target"] = target
          break
        end
      end
      if target == 0 then
        return  -- must not have spelled it correctly
      end
    end
    enemies[target]["remainingText"] = string.sub(enemies[target]["remainingText"], 2)
    fireMissile(target)
    if string.len(enemies[target]["remainingText"]) == 0 then
      player["mode"] = "flying"
    end
  end
end

function love.keyreleased(key)
    if key == "." then
      player["y_engine"] = 0
    end
    if key == "e" then
      player["y_engine"] = 0
    end
    if key == "o" then
      player["x_engine"] = 0
    end
    if key == "u" then
      player["x_engine"] = 0
    end
end

function vectorLength(vector)
  return math.sqrt(vector["x"]^2 + vector["y"]^2)
end

function normalizeVector(vector)
  length = vectorLength(vector)
  normalized = {
    x = vector["x"] / length,
    y = vector["y"] / length
  }
  return normalized
end

function hitEnemy(index)
  enemies[index]["text"] = string.sub(enemies[index]["text"], 2)
  if string.len(enemies[index]["text"]) == 0 then
    popArrayElement(enemies, index)
    if player["target"] == index then
      player["target"] = 0
    end
  end
end

function destroyMissile(index)
  popArrayElement(missiles, index)
end

function popArrayElement(array, index)
  i = index
  while array[i+1] ~= nil do
    array[i] = array[i+1]
    i = i + 1
  end
  array[i] = nil
end

function checkOffScreen(object)
  if (object["x"] < -g_worldMargin) or (object["x"] > love.graphics.getWidth() + g_worldMargin) then
    return true
  end
  if (object["y"] < -g_worldMargin) or (object["y"] > love.graphics.getHeight() + g_worldMargin) then
    return true
  end
  return false
end

function applyFriction(friction, object)
  vector = {
    x=object["x_vel"],
    y=object["y_vel"]
  }
  vector = normalizeVector(vector)
  if math.abs(object["x_vel"]) > g_frictionThreshold then
    object["x_vel"] = object["x_vel"] - (friction * vector["x"])
  end
  if math.abs(object["y_vel"]) > g_frictionThreshold then
  object["y_vel"] = object["y_vel"] - (friction * vector["y"])
  end
  if math.abs(object["x_vel"]) < g_frictionThreshold * 1.5 then
    object["x_vel"] = 0
  end
  if math.abs(object["y_vel"]) < g_frictionThreshold * 1.5 then
    object["y_vel"] = 0
  end
end

function love.update(dt)
  -- apply physics to the missiles
  for i, missile in ipairs(missiles) do
    target = enemies[missile["target"]]
    vector = {
      x = target["x"] - missile["x"],
      y = target["y"] - missile["y"]
    }
    length = vectorLength(vector)
    if(length < g_collisionRadius) then
      -- simple proximity collision detection
      hitEnemy(missile["target"])
      destroyMissile(i)
      return
    end
    vector = normalizeVector(vector)
    -- accelerate the missile
    missile["x_vel"] = missile["x_vel"] + (vector["x"] * g_missileAccel)
    missile["y_vel"] = missile["y_vel"] + (vector["y"] * g_missileAccel)
    -- move the missile
    missile["x"] = missile["x"] + missile["x_vel"]
    missile["y"] = missile["y"] + missile["y_vel"]
    -- check for missiles way off the screen
    if(checkOffScreen(missile)) then
      -- missile AI isn't working well, so we cheat here ;P
      hitEnemy(missile["target"])
      destroyMissile(i)
    end
  end

  -- enemies wander
  for i, enemy in ipairs(enemies) do
    enemy["x"] = enemy["x"] + (love.math.random(0, g_enemyWanderRate) - g_enemyWanderRate / 2)
    enemy["y"] = enemy["y"] + (love.math.random(0, g_enemyWanderRate) - g_enemyWanderRate / 2)
  end

  -- acceleration from player engine
  vector = {
    x=player["x_vel"],
    y=player["y_vel"],
  }
  if(vectorLength(vector) < g_maxPlayerSpeed) then
    player["x_vel"] = player["x_vel"] + (g_engineAccel * player["x_engine"])
    player["y_vel"] = player["y_vel"] + (g_engineAccel * player["y_engine"])
  end
  -- friction on the player
  applyFriction(g_playerFriction, player)
  -- move the player
  player["x"] = player["x"] + player["x_vel"]
  player["y"] = player["y"] + player["y_vel"]
end

function love.draw()
  -- draw each enemy
  for i, enemy in ipairs(enemies) do
    if player["target"] == i then
      love.graphics.setColor(255,0,0)
    end
    love.graphics.print(enemy["text"], enemy["x"], enemy["y"])
    love.graphics.setColor(255,255,255)
  end
  -- draw each missile
  for i, missile in ipairs(missiles) do
    love.graphics.print("x", missile["x"], missile["y"])
  end
  -- draw the player
  if player["mode"] == "shooting" then
    love.graphics.setColor(255,0,0)
  else
    love.graphics.setColor(0,255,0)
  end
  love.graphics.print("X", player["x"], player["y"])
  love.graphics.setColor(255,255,255)
end
