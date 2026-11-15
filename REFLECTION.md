# PA2 Story Tree Reflection

## Question 1: Explain what each function you wrote (TODO 1 to 19) does and give its time complexity.

**TODO 1 - Scene Constructor**

My constructor creates a new scene and makes enough space for however many choices that scene has. It also sets every choice to nullptr so there aren't any random pointers. Since a scene can only have up to 3 choices, the time complexity is O(1).

**TODO 2 - Scene Destructor**

My destructor deletes all the child scenes, the choices array, and the scene's data. This makes sure the memory gets cleaned up when a scene is deleted. The time complexity is O(n) because it can end up deleting the whole tree.

**TODO 3 - isEnding**

This function checks if a scene has zero choices. If it does, that means the player reached an ending. The time complexity is O(1) because it only checks one number.

**TODO 4 - getChoice**

This function gets the next scene based on the choice number. It also checks if the index is valid before returning anything. The time complexity is O(1) because it accesses one spot in the array.

**TODO 5 - setChoice**

This function puts a child scene into one of the choice slots. It checks the index first so it doesn't try to use a spot outside the array. The time complexity is O(1).

**TODO 6 - countScenes**

This function counts how many scenes are in the tree. It uses recursion to go through every child and adds them all together. The time complexity is O(n) because every scene gets visited once.

**TODO 7 - countEndings**

This function counts how many endings the story has. It goes through the tree and adds one whenever it finds a scene with no choices. The time complexity is O(n) because it checks every scene.

**TODO 8 - height**

This function finds the longest path through the story. It checks the height of each child and keeps the biggest one. The time complexity is O(n) because it has to go through the tree.

**TODO 9 - clear**

This function deletes the root scene and sets it back to nullptr. Since deleting the root also deletes its children, it clears the entire story. The time complexity is O(n).

**TODO 10 - load**

This function opens the story file, reads the starting HP, and starts building the story tree. It returns false if the file can't be opened or loaded correctly. The time complexity is O(n) because it loads all the scenes.

**TODO 11 - readScene**

This function reads the information for one scene and then uses recursion to load its children. It also makes sure a scene doesn't have more than 3 choices. The time complexity is O(n) when loading the whole story because every scene gets created once.

**TODO 12 - askChoice**

This function asks the player to enter a choice and makes sure the number is valid. If they enter something wrong, it keeps asking until they enter a valid number. Each check is O(1), but the total time depends on how many times the player enters something wrong.

**TODO 13 - play**

This function runs the actual game. It keeps track of the player's HP, loot, and inventory while moving through the story based on their choices. A playthrough visits at most n scenes, so the main path takes O(n), although searching the inventory adds extra work.

**TODO 14 - printStats**

This function prints the number of scenes, endings, and the longest playthrough. It gets these numbers from the StoryTree functions I already made. The time complexity is O(n) because those functions go through the tree.

**TODO 15 - printSpoilers**

This function starts printing all the possible endings in the story. It calls printEndings starting at the root. The time complexity is O(n) for visiting the scenes, not counting the extra work of building and printing the path strings.

**TODO 16 - printEndings**

This function uses recursion to find every ending and print the choices needed to reach it. It goes through each child until it reaches a scene with no choices. It visits every scene once, so the tree traversal is O(n), plus the work of handling the path strings.

**TODO 17 - greedyRun**

This function picks whichever next scene gives the most loot without looking at what happens later. It keeps doing that until it reaches an ending or dies. The time complexity is O(n) in the worst case because it follows one path and checks at most 3 choices at each scene.

**TODO 18 - bestLoot**

This function uses recursion to find the path with the most loot while making sure the player survives. It checks all the possible routes and ignores the ones where HP reaches zero. The tree traversal is O(n) because each scene is visited once, although copying path strings adds some extra work.

**TODO 19 - optimalRun**

This function calls bestLoot and prints the best surviving path and its total loot. Unlike greedy, it looks at all the possible routes before deciding which one is best. Its main tree traversal takes O(n) because it uses bestLoot.

## Question 2: Why does each scene allocate exactly numChoices slots instead of declaring Scene<T>* choices[3]? What does an ending hold in choices_, and why?

I used numChoices because not every scene needs 3 choices. Some scenes only have 1 or 2, so making the array the exact size avoids wasting space. If a scene is an ending, choices_ stays nullptr because there are no more scenes for the player to choose from.

## Question 3: Walk through your Scene destructor. What gets deleted, and in what order? What would go wrong if choices_ were deleted first?

My destructor first goes through the choices array and deletes each child scene. After that, it deletes the choices array and then deletes the scene's data. This is important because deleting a child also deletes everything below it. If I deleted the choices array first, I wouldn't be able to access the child pointers anymore, which could cause memory leaks.

## Question 4: Show the path greedy took and the path optimal took on your story, with the loot for each. Explain why greedy lost.

In my zombie story, greedy takes the path 1-1-1 and collects 19 loot, but ends up dying. It chooses to raid the pharmacy, break into the supply room, and carry the heavy crate outside. The problem is that it keeps choosing the options with the most immediate loot without thinking about HP.

Optimal takes the path 2-1-2-1-1 and collects 20 loot while surviving. This route involves finding Maya, going to the police station, finding the harbor tunnel, and escaping on the rescue boat. I made the pharmacy route dangerous on purpose to show how choosing the most loot right away isn't always the best decision.

## Question 5: bestLoot tries every path, yet it runs in O(n) time. Explain why it is not exponential, using what you know about paths and leaves in a tree.

Even though bestLoot checks every possible path, each scene in the tree only gets visited once. The different paths share some scenes, so the function doesn't have to restart from the root every time it checks another ending. The number of scenes already includes all the branches and endings, which is why the tree traversal is O(n) instead of exponential. My code also copies path strings, so that adds some extra time beyond just visiting the scenes.

## Question 6: Which List methods does your game use, and why? What does one search cost as the bag grows?

My game uses addFront, search, size, and print from the List class. I use addFront to put items into the player's inventory, search to make sure they don't already have an item, size to show how many items they have, and print to display everything at the end. Since the inventory is a linked list, search has to go through the items one at a time. This makes searching O(b), where b is the number of items in the bag.