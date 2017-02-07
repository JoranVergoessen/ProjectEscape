#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include "mge/Lights/Light.hpp"

class Camera;

using namespace std;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
		static GameObject* findGameObjectByName(std::string pName);
		virtual void add(GameObject* pChild) override;
		virtual void remove(GameObject* pChild) override;
		static void AddLight(Light* pLight);
		static void RemoveLight(Light* pLight);
		static vector<Light*> GetLights();

	private:
	    Camera* _mainCamera;
		static vector<Light*> _lights;

		static vector<GameObject*> _worldChildren;
        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
