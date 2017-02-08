#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

//		core
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"
//		materials
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/WobblingMaterial.hpp"
#include "mge/materials/LitMaterial.hpp"
#include "mge/materials/LitTextureMaterial.hpp"
#include "mge/materials/TerrainMaterial.hpp"
//		behaviours
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "mge/behaviours/OrbitBehaviour.h"
//		lights
#include "mge/Lights/Light.hpp"
#include "mge/Lights/DirectionalLight.hpp"
//util
#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
	//COLORS
	glm::vec3 white = glm::vec3(1, 1, 1);
	glm::vec3 cyan = glm::vec3(0, 1, 1);
	glm::vec3 red = glm::vec3(1,0,0);
	glm::vec3 green = glm::vec3(0,1,0);
	glm::vec3 blue = glm::vec3(0,0,1);
	glm::vec3 yellow = glm::vec3(1,1,0);
	glm::vec3 purple = glm::vec3(1,0,1);
	glm::vec3 black = glm::vec3(0);


    _renderer->setClearColor(1,0,1);

    //add camera first (it will be updated last)
    Camera* camera = new Camera ("mainCamera", glm::vec3(0,2,5));
    world->add(camera);
    world->setMainCamera(camera);

	//TEXTURES
	Texture* terrainGrass = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg");
	Texture* terrainWater = Texture::load(config::MGE_TEXTURE_PATH + "terrain/water.jpg");
	Texture* terrainSand = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse2.jpg");
	Texture* terrainStone = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse3.jpg");
	Texture* terrainSnow = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg");
	Texture* terrainHeightMap = Texture::load(config::MGE_TEXTURE_PATH + "terrain/heightmap.png");
	Texture* customHeightMap = Texture::load(config::MGE_TEXTURE_PATH + "terrain/customHeightMap.png");
	Texture* terrainSplatMap = Texture::load(config::MGE_TEXTURE_PATH + "terrain/splatmap.png");
	Texture* customSplatMap = Texture::load(config::MGE_TEXTURE_PATH + "terrain/customSplatMap.png");


    //MESHES
    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
	Mesh* terrainMesh = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
	Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");

    //MATERIALS
	AbstractMaterial* terrainMaterial = new TerrainMaterial(terrainWater, terrainGrass, terrainStone, terrainSnow, customHeightMap, customSplatMap);
	
	Light* dirLight = new Light("dirLight", white, glm::vec3(-5, 10, 0), 3, glm::vec3(0.1, -1, 0));
	dirLight->setMesh(cubeMesh);
	dirLight->setMaterial(new ColorMaterial(yellow));
	dirLight->setBehaviour(new RotatingBehaviour());
	world->add(dirLight);


	GameObject* terrain = new GameObject("terrain", glm::vec3(0,0,-2));
	terrain->scale(glm::vec3(5));
	terrain->setMesh(terrainMesh);
	terrain->setMaterial(terrainMaterial);
	terrain->setBehaviour(new KeysBehaviour());
	world->add(terrain);


	camera->setBehaviour(new OrbitBehaviour(terrain));

	cout << "Number of lights in the scene: " << World::GetLights().size() << endl;
}

void MGEDemo::_render() {
    AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
