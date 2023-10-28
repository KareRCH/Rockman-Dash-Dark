#pragma once


#include "System/GraphicDev.h"

BEGIN_NAME(Engine);

inline HRESULT Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
	_float fScreenDepth, _float fScreenNear);
inline HRESULT Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha);
inline HRESULT Render_End();




inline void Release_Engine();

#include "Export_Engine.inl"

END_NAME

