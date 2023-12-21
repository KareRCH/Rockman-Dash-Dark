#pragma once

namespace ModelBinary {}
using namespace ModelBinary;
#include "Base.h"

BEGIN(Engine)

struct TModelHeader
{
	_uint_16 iID;			// 식별자 헤더
	_uint_16 iNumMeshes;
	_uint_16 iNumMaterials;
	_uint_16 iNumBones;
	_uint_16 iNumAnims;
};

//---------------메쉬

struct TMeshVertex
{
	_float3			vPosition;
	_float2			vTexCoord;
	_float3			vNormal;
	_float3			vTangent;
	_float3			vBiTangent;
	vector<_int>	vecBoneID;
	vector<_float>	vecWeights;
};

struct TMeshBone
{
	_int		iBoneID;	// 해당 뼈 ID
	_float4x4	matOffset;	// 뼈에 대한 오프셋
};

struct TMesh
{
	_uint				iID;
	wstring				strName;
	vector<TMeshVertex>	vecVertices;
	vector<_uint>		vecIndices;
	vector<TMeshBone>	vecBones;
	_uint				iMaterialIndex;
};

//-----------------애님

struct TKeyFrame
{
	_float fTrackPosition;
	_float3 vPos;
	_float4 qtRot;
	_float3 vScale;
};

struct TChannel
{
	_uint				iBoneID;
	wstring				strName;
	vector<TKeyFrame>	vecKeyFrames;
};

struct TAnim
{
	_uint				iID;
	wstring				strName;
	_float				fDuration;
	_float				fTickPerSeconds;
	vector<TChannel>	vecChannels;
};


//-----------------본

struct TBone
{
	_uint			iID;
	wstring			strName;

	_int			iParentID;
	_float4x4		matTransform;
};


//-----------------머터리얼

struct TMaterial
{
	wstring strName;
	wstring strTextures[AI_TEXTURE_TYPE_MAX];
};


/// <summary>
/// 컨버팅용 모델 데이터 저장 클래스
/// </summary>
class ENGINE_DLL CModelLoad : public CBase
{
	DERIVED_CLASS(CBase, CModelLoad)

protected:
	explicit CModelLoad();
	explicit CModelLoad(const CModelLoad& rhs) = default;
	virtual ~CModelLoad() = default;

public:
	virtual HRESULT	Initialize();

public:
	static CModelLoad* Create();

private:
	virtual void	Free() override;


public:
	vector<TMesh>		vecMeshes;
	vector<TMaterial>	vecMaterials;
	vector<TBone>		vecBones;
	vector<TAnim>		vecAnims;

};

END