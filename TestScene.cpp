#include "Engine/Input.h"
#include "Engine/SceneManager.h"

#include "TestScene.h"
#include "Stage.h"
//#include "ImageScene.h"


TestScene::TestScene(GameObject* parent)
	:GameObject(parent, "TestScene")
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	//Instantiate<ImageScene>(this);
	//sImage = new Sprite("Assets\\Sea.png");
	//sImage = new Sprite("Assets\\Sea.png");
	//sImage->Initialize();
	//sImage->Load("Sea.png");

}

void TestScene::Update()
{
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}