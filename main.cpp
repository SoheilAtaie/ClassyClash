#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

// window dimensions 
const int windowWidth = 384;
const int windowHeight = 384;

int main()
{
	InitWindow(windowWidth, windowHeight, "ClassyClash");

	// Load the world map 
	Texture2D WorldMap = LoadTexture("nature_tileset/WorldMap.png");
	Vector2 wmPos{ 0.0, 0.0 };
	const float mapScale{ 4.0 };

	Character knight(windowWidth, windowHeight);

	Prop props[2]{
		Prop{ Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
		Prop{ Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}

	};

	Enemy goblin{
		Vector2{800.f, 300.f},
		LoadTexture("Characters/goblin_idle_spritesheet.png"),
		LoadTexture("Characters/goblin_run_spritesheet.png")
	};

	Enemy slime{
		Vector2{500.f, 700.f},
		LoadTexture("Characters/slime_idle_spritesheet.png"),
		LoadTexture("Characters/slime_run_spritesheet.png")
	};

	Enemy* enemies[]{
		&goblin,
		&slime
	};

	for (auto enemy : enemies)
	{
		enemy->setTarget(&knight);
	}

    


	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		wmPos = Vector2Scale(knight.getWorldPos(), -1.f);
		// Draw the map
		DrawTextureEx(WorldMap, wmPos, 0.0, mapScale, WHITE);

		// Loop through the array and draw the props 
		for (auto prop : props)
		{
			prop.render(knight.getWorldPos());
		}

		if (!knight.getAlive())
		{
			// if the character is dead
			DrawText("Game Over!", 55.f, 45.f, 40, RED);
			EndDrawing();
			continue;
		}
		else
		{
			// if the character is alive
			std::string knightHealth = "Health: ";
			knightHealth.append(std::to_string(knight.getHealth()), 0, 5);
			DrawText(knightHealth.c_str(), 55.f, 45.f, 40, RED);
		}

		knight.tick(GetFrameTime());

		// Check map bounds
		if (knight.getWorldPos().x < 0.f ||
			knight.getWorldPos().y < 0.f ||
			knight.getWorldPos().x + windowHeight > WorldMap.width * mapScale ||
			knight.getWorldPos().y + windowHeight > WorldMap.height * mapScale)
		{
			knight.undoMovement();
		}

		// Check for Prop collisions by looping through all props
		for (auto prop : props)
		{
			if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisonRec()))
				knight.undoMovement();
		}

		for (auto enemy : enemies)
		{
			enemy->tick(GetFrameTime());
		}

		// Check for attack function
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			for (auto enemy : enemies)
			{
				if (CheckCollisionRecs(goblin.getCollisonRec(), knight.getWeaponCollisionRec()))
				{
					enemy->setAlive(false);
				}
			}
		}

		

		EndDrawing();
	}
	UnloadTexture(WorldMap);
	CloseWindow();

	return 0;
}
