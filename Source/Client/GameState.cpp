#include "GameState.h"

GameState::GameState()
{
	m_bHasStateRequest = false;
	m_bHasCloseRequest = false;
}

GameState::~GameState()
{
}

void GameState::requestState(std::string sStateName)
{
	m_sRequestedState = sStateName;
	m_bHasStateRequest = true;
}

void GameState::requestClose()
{
	m_bHasCloseRequest = true;
}

bool GameState::hasStateRequest()
{
	return m_bHasStateRequest;
}

bool GameState::hasCloseRequest()
{
	return m_bHasCloseRequest;
}

std::string GameState::getStateRequest()
{
	m_bHasStateRequest = false;
	return m_sRequestedState;
}