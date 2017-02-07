#pragma once
#include <mge\core\AbstractGame.hpp>

class DebugHud;

class Project3 : public AbstractGame
{
public:
	Project3();
	virtual ~Project3();

	virtual void initialize();

protected:
	virtual void _initializeScene();

	virtual void _render();

private:
	DebugHud* _hud;

	void _updateHud();

	Project3(const Project3&);
	Project3& operator =(const Project3&);

};
