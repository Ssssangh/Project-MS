#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "../GameObject/Object.h"

using namespace sf;
using namespace std;

enum class Scenes
{
	Play,
	Dev,
};

class UiMgr;

class Scene
{
protected:
	Scenes type;
	list<Object*> objList;
	UiMgr* uiMgr;

	View worldView;
	View uiView;

public:
	Scene(Scenes type);
	virtual ~Scene();

	virtual void Init() = 0;	// ????
	virtual void Release();		// ????

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	Texture* GetTexture(const string id);

	View& GetWorldView() { return worldView; }
	View& GetUiView() { return uiView; }

	Vector2f ScreenToWorldPos(Vector2i screenPos);
	//Vector2f ScreenToUiPos(Vector2i screenPos);

	Object* FindGameObj(String name);

	void AddGameObj(Object* obj);

	UiMgr* GetUiMgr() { return uiMgr; }
};

