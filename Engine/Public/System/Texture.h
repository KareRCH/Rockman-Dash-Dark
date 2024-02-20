#pragma once

#include "TextureMgr.h"

BEGIN(Engine)

/// <summary>
/// �ؽ�ó ������ ����� Ŭ����
/// �߻�Ŭ����
/// </summary>
class CTexture : public CBase
{
	DERIVED_CLASS(CBase, CTexture)

protected:
	explicit CTexture(const DX11DEVICE_T tDevice);
	explicit CTexture(const CTexture& rhs) = delete;
	virtual ~CTexture() = default;

public:
	virtual HRESULT		Initialize();

public:
	static	CTexture* Create(const DX11DEVICE_T tDevice);
	
protected:
	virtual void		Free();

protected:
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };					// ��ġ
	ComPtr<ID3D11DeviceContext>			m_pDeviceContext = { nullptr };				// ��ġ ���ؽ�Ʈ

public:
	// �޸𸮸� ������ �� ���.
	void	Unload();
	HRESULT	Load(ID3D11ShaderResourceView* _pSRV);
	
public:
	virtual HRESULT		Insert_Texture(const wstring& strFilePath, const _uint iNumTextures, const _bool bPermanent);
	void				Load_Complete() { m_bIsLoaded = true; }

public:
	void								Set_Permanent(const _bool value) { m_bIsPermanent = value; }
	const _bool&						Is_Loaded() const { return m_bIsLoaded; }
	const _uint&						Get_TextureCount() const { return m_iNumTextures;}
	ID3D11ShaderResourceView* const		Get_SRV(const _uint iIndex);
	HRESULT								Reference_SRVs(vector<ComPtr<ID3D11ShaderResourceView>>& RefSRVs);

private:
	_bool										m_bIsLoaded = false;			// �ε�� �ؽ�ó üũ��
	_bool										m_bIsPermanent = false;		// ���� ���� �ؽ�ó ����, �������� ���Ǵ� �뵵
	_uint										m_iNumTextures = 0;
	vector<ComPtr<ID3D11ShaderResourceView>>	m_SRVs;					// ���̴� ���÷�

};

END