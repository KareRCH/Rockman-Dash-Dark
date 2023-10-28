#pragma once


inline HRESULT Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
	_float fScreenDepth, _float fScreenNear)
{
	return CGraphicDev::GetInstance()->Initialize(iScreenWidth, iScreenHeight, bVsync, hWnd, bFullScreen, fScreenDepth, fScreenNear);
}
inline HRESULT Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha)
{
	return CGraphicDev::GetInstance()->Render_Begin(fRed, fGreen, fBlue, fAlpha);
}
inline HRESULT Render_End()
{
	return CGraphicDev::GetInstance()->Render_End();
}




inline void Release_Engine()
{
	CGraphicDev::GetInstance()->Release();
}