#include "pch.h"

#include "GameObject.h"



GameObject::GameObject(const std::string& tag)
	: m_isValid(true)
	, m_drawPrio(0)
	, m_tag(tag)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_rotation(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
{
}



GameObject::~GameObject()
{
}


void GameObject::OnCollision(GameObject * object)
{
}



void GameObject::Invalidate()
{
	m_isValid = false;
}
