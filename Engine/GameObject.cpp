#include "GameObject.h"
#include "SphereCollider.h"

GameObject::GameObject()
	:pParent_(nullptr), isDead_(false)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent), objectName_(name), isDead_(false), pCollider_(nullptr)
{
	if (parent != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	Draw();
		for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
		{
			(*itr)->DrawSub();
		}
}

void GameObject::UpdateSub()
{
	Update();
	RoundRobin(GetRootJob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}
	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->isDead_ == true)
		{
			(*itr)->ReleaseSub();
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void GameObject::ReleaseSub()
{
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->ReleaseSub();
		SAFE_DELETE(*itr);
	}	
	Release();
}

bool GameObject::KillMe()
{
	return isDead_ = true;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

void GameObject::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

GameObject* GameObject::FindChildObject(std::string _objName)
{
	if (_objName == this->objectName_)
		return(this);
	else
	{
		for (auto itr : childList_)
		{
			GameObject* obj = itr->FindChildObject(_objName);
				if (obj != nullptr)
					return obj;
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
		return this;

	return pParent_->GetRootJob();
}

GameObject* GameObject::FindObject(std::string _objName)
{
	GameObject* rootJob = GetRootJob();
	GameObject* result = rootJob->FindChildObject(_objName);
	return(result);
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (pTarget == this || pTarget->pCollider_ == nullptr)
		return;//�������g�A�܂��̓^�[�Q�b�g�ɃR���C�_�[���A�^�b�`����Ă��Ȃ�
	//XMVECTOR v{transform_.position_.x - pTarget->transform_.position_.x,
	//		   transform_.position_.y - pTarget->transform_.position_.y, 
	//		   transform_.position_.z - pTarget->transform_.position_.z, 
	//			0};
	//XMVECTOR dist = XMVector3Dot(v, v);
	float dist = (transform_.position_.x - pTarget->transform_.position_.x) * (transform_.position_.x - pTarget->transform_.position_.x)
		+ (transform_.position_.y - pTarget->transform_.position_.y) * (transform_.position_.y - pTarget->transform_.position_.y)
		+ (transform_.position_.z - pTarget->transform_.position_.z) * (transform_.position_.z - pTarget->transform_.position_.z);
	float rDist = (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius()) * (this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius());

	//�����ƃ^�[�Q�b�g�̋����@<= R1+R2�Ȃ�
	//�����A�����̃R���C�_�[�ƃ^�[�Q�b�g���Ԃ����Ă�����
	//onCollision(pTarget)���Ăяo���I
	if (dist <= rDist)
	{
		//onCollinsion();�Ăڂ��I
		double p = 0;
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)
		return;
	if (pTarget->pCollider_ != nullptr) //�����ƃ^�[�Q�b�g
		Collision(pTarget);
	//�����̎q���S���ƃ^�[�Q�b�g
	for (auto itr : pTarget->childList_)
		RoundRobin(itr);
}
