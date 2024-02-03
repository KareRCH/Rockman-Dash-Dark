#pragma once

#include "Base.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "CommonStates.h"

BEGIN(Engine)

// 
class ENGINE_DLL_DBG CCustomFont final : public CBase
{
	DERIVED_CLASS(CBase, CCustomFont)
private:
	explicit CCustomFont(const DX11DEVICE_T tDevice);
	virtual ~CCustomFont() = default;

public:
	HRESULT				Initialize(const wstring& strFontFilePath);
	HRESULT				Render(const wstring& strText, const _float2& vPosition, _fvector vColor = Colors::White, 
								_float fScale = 1.f, _float2 vOrigin = _float2(0.f, 0.f), _float fRotation = 0.f);

public:
	static CCustomFont* Create(const DX11DEVICE_T tDevice, const wstring& strFontFilePath);

private:
	virtual void		Free();

private:
	ComPtr<ID3D11Device>		m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>	m_pContext = { nullptr };
	SpriteBatch*				m_pBatch = { nullptr };
	SpriteFont*					m_pFont = { nullptr };
	CommonStates*				m_pStates = { nullptr };

};

END

// MakeSpriteFont "NeoµÕ±Ù¸ð Pro" /FontSize:30 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 NeoDungGeunMoPro-30.spritefont
// MakeSpriteFont "µÕ±Ù¸ð²Ã" /FontSize:30 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 DungGeunMo-30.spritefont