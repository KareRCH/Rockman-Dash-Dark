#pragma once

#include "BaseClass/Scene.h"
#include "Client_Define.h"

BEGIN(Client)

class CTestScene : public CScene
{
	DERIVED_CLASS(CScene, CTestScene)
protected:
	explicit CTestScene(ID3D11Device* pGraphicDev);
	explicit CTestScene(const CTestScene& rhs) = delete;
	virtual ~CTestScene() = default;

public:
	virtual HRESULT		Initialize();
	virtual _int		Tick(const _float& fTimeDelta);
	virtual void		LateTick();
	virtual void		Render(ID3D11DeviceContext* const pDeviceContext);

	virtual HRESULT		InitializeLate_Scene();

public:
	static CTestScene* Create(ID3D11Device* const pGraphicDev);

private:
	virtual void	Free();

private:
	virtual HRESULT Initialize_Layer_Completed() override;
};

END

