/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <algorithm>

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	brd( gfx ),
	rng( std::random_device()() ),
	snake({2,2})
{
	InitializeBoard();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
	deltaTime = ft.Mark();
}

void Game::UpdateModel()
{
	if (gameIsStarted)
	{
		ManageInput();
		SnakeMovement();
	}
	else if (gameIsOver)
	{
		if (wnd.kbd.KeyIsPressed(VK_CONTROL) || wnd.kbd.KeyIsPressed(VK_RCONTROL) || wnd.kbd.KeyIsPressed(VK_RBUTTON))
		{
			ResetGame();
		}
	}
	else
	{
		gameIsStarted = wnd.kbd.KeyIsPressed(VK_RETURN);
	}
}

void Game::ManageInput()
{
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		nextDeltaLocation = { 0,-1 };
	}
	else if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		nextDeltaLocation = { 0,1 };
	}
	else if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		nextDeltaLocation = { -1,0 };
	}
	else if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		nextDeltaLocation = { 1,0 };
	}	
}

void Game::SnakeMovement()
{
	snakeMoveCounter += deltaTime;
	if (snakeMoveCounter >= snakeMovePeriod)
	{
		if (nextDeltaLocation != (deltaLocation * - 1))
		{
			deltaLocation = nextDeltaLocation;
		}

		snakeMoveCounter -= snakeMovePeriod;
		const Location next = snake.GetNextHeadLocation(deltaLocation);

		if (!brd.IsInsideBoard(next) ||	snake.IsInTileExceptEnd(next) || brd.CheckForObstacle(next))
		{
			gameIsOver = true;
			gameIsStarted = false;
		}
		else
		{
			if (brd.CheckForPoison(next))
			{
				brd.ConsumePoison(next);
				snakeMovePeriod = std::max(snakeMovePeriod - snakeSpeedUp * deltaTime, minSnakeMovePeriod);
			}

			snake.MoveBy(deltaLocation);

			if (brd.CheckForGoal(next))
			{
				score++;
				snake.Grow();
				brd.RespawnGoal(next, rng, snake);
				brd.SpawnObstacle(rng, snake);
			}
		}
	}
}

void Game::ResetGame()
{
	snake.Reset({ 2,2 });
	score = 0;
	deltaLocation = { 1,0 };
	gameIsOver = false;
	gameIsStarted = true;
	brd.ResetBoard();
	InitializeBoard();
}

void Game::InitializeBoard()
{
	for (int i{ 0 }; i < goalNumber; i++)
	{
		brd.SpawnGoal(rng, snake);
	}

	for (int i{ 0 }; i < poisonPools; i++)
	{
		brd.SpawnPoison(rng, snake);
	}
}

void Game::ComposeFrame()
{
	int xPos{ Graphics::ScreenWidth / 2 - 50 },
		yPos{ Graphics::ScreenHeight / 2 - 50 };
	
	if (gameIsStarted)
	{
		brd.DrawBorder();
		brd.DrawContent();
		snake.Draw(brd);
	}
	else if (gameIsOver)
	{
		SpriteCodex::DrawGameOver(xPos, yPos, gfx);
	}
	else
	{
		SpriteCodex::DrawTitle(xPos - 50, yPos - 50, gfx);
	}

}
