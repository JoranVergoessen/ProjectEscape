#include "mge/core/World.hpp"
#include "mge/Lights/Light.hpp"

using namespace std;

vector<GameObject*> World::_worldChildren;
vector<Light*> World::_lights;

World::World():GameObject("root"), _mainCamera(0)
{
	//ctor
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

void World::add(GameObject* pChild)
{
	std::cout << "\tAdding " << pChild->getName() << " to world" << std::endl;
	pChild->setParent(this);
	_worldChildren.push_back(pChild);	//for gameobject.find

	//World::AddLight((Light*)pChild);
}

void World::remove(GameObject* pChild)
{
	pChild->setParent(NULL);
	for (auto i = _worldChildren.begin(); i != _worldChildren.end(); ++i) {
		if (*i == pChild) {
			_worldChildren.erase(i);
			return;
		}
	}
}

GameObject* World::findGameObjectByName(string pName) 
{
	for (size_t i = 0; i < _worldChildren.size(); i++)
	{
		if (_worldChildren[i]->getName() == pName)
		{
			cout << "Found object with name [" << pName << "]" << endl;
			return _worldChildren[i];
		}
	}
}

void World::AddLight(Light* pLight)
{
	try 
	{
		//if the light is already in the list dont add second one
		for (auto i = _lights.begin(); i < _lights.end(); ++i)
		{
			if (*i == pLight)
			{
				cout << pLight->getName() << " is already in the list. AddLight will be ignored" << endl;
				return;
			}
		}

		_lights.push_back(pLight);
		cout << "Successfully added " << pLight->getName() << " to the scene" << endl;
	}
	catch(exception e)
	{
		cout << "Failed to add " << pLight->getName() << " in the lights list" << endl;
	}
}

void World::RemoveLight(Light* pLight)
{
	try
	{
		for (auto i = _lights.begin(); i < _lights.end(); ++i)
		{
			if (*i == pLight)
			{
				_lights.erase(i);
				cout << "Removed " << pLight->getName() << " from the lights list" << endl;
				return;
			}
		}

		cout << "Could not remove " << pLight->getName() << " from the list because it wasn't found" << endl;
	}
	catch (exception e)
	{
		cout << "Something went wrong with RemoveLight(" << pLight << ")" << endl;
	}
}

vector<Light*> World::GetLights()
{
	return _lights;
}
