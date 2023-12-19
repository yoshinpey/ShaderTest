#pragma once
#include <list>
#include <string>
#include "Transform.h"
#include "Direct3D.h"
#include <string>

class SphereCollider;

class GameObject
{
protected:
	Transform transform_;				//�ʒu������Ȃǂ��Ǘ�����I�u�W�F�N�g
	std::string	objectName_;			//�I�u�W�F�N�g�̖��O
	GameObject* pParent_;				//�e�I�u�W�F�N�g
	std::list<GameObject*> childList_;	//�q�I�u�W�F�N�g���X�g
	bool isDead_;						//�I�u�W�F�N�g�폜�p
	SphereCollider* pCollider_;

public:
	//�R���X�g���N�^
	GameObject();
	GameObject(GameObject* parent, const std::string& name);

	//�f�X�g���N�^
	~GameObject();

	GameObject* FindChildObject(std::string _objName);
	GameObject* GetRootJob();
	GameObject* FindObject(std::string _objName);

	void AddCollider(SphereCollider* pCollider);
	void Collision(GameObject* pTarget);
	void RoundRobin(GameObject* pTarget);

	//�e�I�u�W�F�N�g�ŕK�����֐�
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;

	//�֐�
	void DrawSub();
	void UpdateSub();
	void ReleaseSub();
	bool KillMe();

	void SetPosition(XMFLOAT3 position);
	void SetPosition(float x, float y, float z);

	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		childList_.push_back(p);
		return p;
	}
};