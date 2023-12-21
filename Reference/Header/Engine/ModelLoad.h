#pragma once

namespace ModelBinary {}
using namespace ModelBinary;
#include "Base.h"

BEGIN(Engine)

struct TModelHeader
{
	_uint_16 iID;			// �ĺ��� ���
	_uint_16 iNumMeshes;
	_uint_16 iNumMaterials;
	_uint_16 iNumBones;
	_uint_16 iNumAnims;
};

//---------------�޽�

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
	_int		iBoneID;	// �ش� �� ID
	_float4x4	matOffset;	// ���� ���� ������
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

//-----------------�ִ�

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


//-----------------��

struct TBone
{
	_uint			iID;
	wstring			strName;

	_int			iParentID;
	_float4x4		matTransform;
};


//-----------------���͸���

struct TMaterial
{
	wstring strName;
	wstring strTextures[AI_TEXTURE_TYPE_MAX];
};


/// <summary>
/// �����ÿ� �� ������ ���� Ŭ����
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