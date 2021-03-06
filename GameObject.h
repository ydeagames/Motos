#pragma once


#include <SimpleMath.h>
#include <string>


class GameObject
{
	private:
		bool m_isValid;

		// 描画順序管理用(小さいほど手前に描画される）
		int m_drawPrio;
		std::string m_tag;


	protected:
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_rotation;
		DirectX::SimpleMath::Vector3 m_scale;


	public:
		GameObject(const std::string& tag = "GameObject");

	public:
		virtual ~GameObject();


	public:
		virtual void Update(float elapsedTime) = 0;
		virtual void Render() = 0;

		virtual void OnCollision(GameObject* object);

	public:
		void Invalidate();
		bool IsValid() const;
		bool IsInvalid() const;

		const std::string& GetTag() const;
		const DirectX::SimpleMath::Vector3& GetPosition() const;
		const DirectX::SimpleMath::Vector3& GetRotation() const;
		const DirectX::SimpleMath::Vector3& GetScale() const;
		const int GetDrawPrio() const;

		void SetTag(const std::string& tag);
		void SetPosition(DirectX::SimpleMath::Vector3& position);
		void SetRotation(DirectX::SimpleMath::Vector3& rotation);
		void SetScale(DirectX::SimpleMath::Vector3& scale);
		void SetDrawPrio(int prio);


	public:
		static void Destroy(GameObject* object);
};



inline bool GameObject::IsValid() const
{
	return m_isValid;
}



inline bool GameObject::IsInvalid() const
{
	return !m_isValid;
}



inline const std::string& GameObject::GetTag() const
{
	return m_tag;
}


inline const DirectX::SimpleMath::Vector3& GameObject::GetPosition() const
{
	return m_position;
}



inline const DirectX::SimpleMath::Vector3 & GameObject::GetRotation() const
{
	return m_rotation;
}



inline const DirectX::SimpleMath::Vector3 & GameObject::GetScale() const
{
	return m_scale;
}

inline const int GameObject::GetDrawPrio() const
{
	return m_drawPrio;
}



inline void GameObject::SetTag(const std::string& tag)
{
	m_tag = tag;
}

inline void GameObject::SetPosition(DirectX::SimpleMath::Vector3 & position)
{
	m_position = position;
}



inline void GameObject::SetRotation(DirectX::SimpleMath::Vector3 & rotation)
{
	m_rotation = rotation;
}



inline void GameObject::SetScale(DirectX::SimpleMath::Vector3 & scale)
{
	m_scale = scale;
}


inline void GameObject::SetDrawPrio(int prio)
{
	m_drawPrio = prio;
}



inline void GameObject::Destroy(GameObject * object)
{
	object->Invalidate();
}
