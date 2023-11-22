#pragma once

#include "BaseClass/Level.h"
#include "Client_Define.h"

BEGIN(Client)

class CTestScene : public CLevel
{
	DERIVED_CLASS(CLevel, CTestScene)
protected:
	explicit CTestScene() = default;
	explicit CTestScene(const CTestScene& rhs) = delete;
	virtual ~CTestScene() = default;

public:
	virtual HRESULT		Initialize() override;
	virtual void		Priority_Tick(const _float& fTimeDelta) override;
	virtual _int		Tick(const _float& fTimeDelta) override;
	virtual void		Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT		InitializeLate_Scene();

public:
	static CTestScene*	Create();

private:
	virtual void		Free();

private:
	virtual HRESULT		Initialize_Layer_Completed() override;
};

END

