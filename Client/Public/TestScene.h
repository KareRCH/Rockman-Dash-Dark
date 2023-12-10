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
	virtual void		Tick(const _float& fTimeDelta) override;
	virtual HRESULT		Render() override;

public:
	static CTestScene*	Create();

private:
	virtual void		Free();
};

END

