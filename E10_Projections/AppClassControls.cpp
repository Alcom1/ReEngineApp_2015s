#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	//Flag for the modifier
	bool bModifier = false;

	//ON PRESS/RELEASE
#pragma region ON PRESS/RELEASE DEFINITION
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
				bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
				bLastEscape = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

	//F1 Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		m_m4Projection = glm::perspective(90.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
		m_m4View = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 14.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//F2 Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
	}

	//F3 Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
	{
	}

	//F4 Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
	{
	}

	//F5 Controllers
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
	}

	//Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveForward(0.1);
	}

	//Back
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveSideways(0.1);
	}

	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveForward(-0.1);
	}

	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveSideways(-0.1);
	}

	//Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveVertical(0.1);
	}

	//Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		Camera* cam = Camera::GetInstance();
		cam->MoveVertical(-0.1);
	}

	//x-axis rot
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		Camera* cam = Camera::GetInstance();
		if (!bModifier)
		{
			cam->ChangePitch(5);
		}
		else
		{
			cam->ChangePitch(-5);
		}
	}

	//y-axis rot
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		Camera* cam = Camera::GetInstance();
		if (!bModifier)
		{
			cam->ChangeYaw(5);
		}
		else
		{
			cam->ChangeYaw(-5);
		}
	}

	//z-axis rot
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		if (!bModifier)
		{
			cam->ChangeRoll(5);
		}
		else
		{
			cam->ChangeRoll(5);
		}
	}

	//Exit the program
#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL))
#pragma endregion
}