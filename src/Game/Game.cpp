#include "Game.h"
#include "../Resources/ResourceManager.h"

#include<GLFW/glfw3.h>

#include <iostream>

Game::Game(const glm::ivec2& windowSize)
	: m_eCurrentGameState(EGameState::Active)
	, m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game() {

}

void Game::render() {
		
	if (m_pTank) {
		m_pTank->render();
	}
	if (m_pLevel) {
		m_pLevel->render();
	}
}

void Game::update(const double delta) {
	
	if (m_pLevel) {
		m_pLevel->update(delta);
	}

	if (m_pTank) {
		if (m_keys[GLFW_KEY_W])
		{
			m_pTank->setOrientation(Tank::EOrientation::Top);
			m_pTank->move(true);
		}
		else if (m_keys[GLFW_KEY_A]) {
			m_pTank->setOrientation(Tank::EOrientation::Left);
			m_pTank->move(true);
		}
		else if (m_keys[GLFW_KEY_D]) {
			m_pTank->setOrientation(Tank::EOrientation::Right);
			m_pTank->move(true);
		}
		else if (m_keys[GLFW_KEY_S]) {
			m_pTank->setOrientation(Tank::EOrientation::Bottom);
			m_pTank->move(true);
		}
		else {
			m_pTank->move(false);
		}

		m_pTank->update(delta);
	}

}

void Game::setKey(const int key, int action) {
	m_keys[key] = action;
}

bool Game::init() {
	ResourceManager::loadJSONResources("res/resources.json");

	auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
	if (!pSpriteShaderProgram) {
		std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
		return false;
	}	
	m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);
	m_windowSize.x = static_cast<int>(m_pLevel->gelLevelWidth());
	m_windowSize.y = static_cast<int>(m_pLevel->gelLevelHeight());

	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

	pSpriteShaderProgram->use();
	pSpriteShaderProgram->setInt("tex", 0);//������������� ��������� �� �������� �����
	pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	m_pTank = std::make_unique<Tank>(0.05, m_pLevel->getPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, 16.f), 0.f);

	return true;
}

size_t Game::getCurrentLevelWidth() const {
	return m_pLevel->gelLevelWidth();
}

size_t Game::getCurrentLevelHeight() const {
	return m_pLevel->gelLevelHeight();
}