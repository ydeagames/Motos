//======================================================
// File Name : DebugFont.cpp
// Summary : �f�o�b�O�t�H���g
// Date : 2019/4/18
// Author : Takafumi Ban
//======================================================
#include "pch.h"
#include "DebugFont.h"
#if DEBUG_FONT_SINGLETON_TYPE == 0
DebugFont* DebugFont::m_pInstance = nullptr;
#elif DEBUG_FONT_SINGLETON_TYPE == 1
std::unique_ptr<DebugFont> DebugFont::m_pInstance = nullptr;
#else
DebugFont DebugFont::m_instance;
#endif
DebugFont::DebugFont()
	: m_pos(0, 0)
	, m_string(nullptr)
{
	m_string = new wchar_t[STRING_SIZE_MAX];
}
DebugFont::~DebugFont()
{
	delete[] m_string;
}
// ����
void DebugFont::create(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"SegoeUI_18.spritefont");
}
// �`��
void DebugFont::draw()
{
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(m_spriteBatch.get(), m_string, m_pos);
	m_spriteBatch->End();
}
// ���Z�b�g
void DebugFont::reset()
{
	m_spriteBatch.reset();
	m_spriteFont.reset();
}
// �\�����o�^
void DebugFont::print(float posX, float posY, wchar_t const * const format, ...)
{
	m_pos.x = posX;
	m_pos.y = posY;
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vswprintf(m_string, STRING_SIZE_MAX, format, arg_ptr);
	va_end(arg_ptr);
}
// �\�����o�^�iVector2�Łj
void DebugFont::print(DirectX::SimpleMath::Vector2 pos, wchar_t const * const format, ...)
{
	m_pos = pos;
	va_list arg_ptr;
	va_start(arg_ptr, format);
	vswprintf(m_string, STRING_SIZE_MAX, format, arg_ptr);
	va_end(arg_ptr);
}