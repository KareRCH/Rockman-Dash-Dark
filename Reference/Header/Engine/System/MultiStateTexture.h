#pragma once

#include "System/Texture.h"

BEGIN(Engine)

/// <summary>
/// ���¸� ������ �ؽ�ó ����
/// ������Ʈ Ű�� ���� 
/// </summary>
class CMultiStateTexture final : public CTexture
{
	DERIVED_CLASS(CTexture, CMultiStateTexture)

private:
	explicit CMultiStateTexture(ID3D11Device* pGraphicDev);
	virtual ~CMultiStateTexture() = default;

public:
	virtual HRESULT				Initialize();

public:
	static CMultiStateTexture*	Create(ID3D11Device* pGraphicDev);

private:
	virtual void				Free();

public:
	virtual HRESULT Insert_Texture(const _tchar* pFilePath, TEXTUREID eType, const _tchar* pStateKey = L"", const _range<_uint>& iCntRange = _range<_uint>(0U, 0U)) override;
	virtual HRESULT Insert_TextureAsync(const _tchar* pFilePath, TEXTUREID eType, vector<ID3D11Texture2D*>& vecTexture, _uint iIndex);
	virtual void	Transfer_Texture(vector<ID3D11Texture2D*>* pVecTexture, const _tchar* pStateKey);

protected:
	using multi_tex_map = _unmap<wstring, vector<ID3D11Texture2D*>>;
	multi_tex_map			m_mapMultiState;				// ������Ʈ Ű�� ���� �ؽ�ó ����
	TEXTUREID				m_eType = TEX_NORMAL;			// �ؽ�ó ����

	virtual void Transfer_Texture() override;

};

END