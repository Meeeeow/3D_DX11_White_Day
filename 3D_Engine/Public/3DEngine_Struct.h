#pragma once
#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

namespace Engine {

	typedef struct tagKeyFrame {
		tagKeyFrame() {
			ZeroMemory(this, sizeof(tagKeyFrame));
		}
		XMFLOAT4			vScale;
		XMFLOAT4			vRotation;
		XMFLOAT4			vPosition;
		_double				dTime;
	}KEYFRAME;

	typedef struct tagMeshgMetarials {
		class CTexture*		pMaterialTextures[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	typedef struct tagVertexMeshes {
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;
		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;
	}VTXMESH;

	typedef struct tagVertexNormalRect {
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORMALRECT;

	typedef struct tagVertexRectTexture {
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct tagVertexPoint {
		XMFLOAT3			vPosition;
		float				fSize;
	}VTXPOINT;

	typedef struct tagVertexCube {
		XMFLOAT3			vPosition;
		XMFLOAT3			vTexUV;
	}VTXCUBE;

	typedef struct tagInstanceBufferMatrix {
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
	}VTXMATRIX;

	typedef struct tagVertexRectColor {
		XMFLOAT3			vPosition;
		XMFLOAT4			vColor;
	}VTXCOL;

	typedef struct tagIndicesLine16 {
		unsigned	short	_1, _2;
	}LINE16;

	typedef struct tagIndicesLine32 {
		unsigned	long	_1, _2;
	}LINE32;

	typedef struct tagIndicesTriangle16 {
		unsigned	short	_1, _2, _3;
	}TRI16;

	typedef struct tagIndicesTriangle32 {
		unsigned	long	_1, _2, _3;
	}TRI32;

	typedef struct tagLineStripTriangle {
		unsigned	short	_1, _2, _3, _4;
	}LINESTRIPTRI16;

	typedef struct tagLightDesc {
		tagLightDesc() {
			ZeroMemory(this, sizeof(tagLightDesc));
			eLightType = LIGHT_END;
		}
		enum LIGHT { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT, LIGHT_END };

		LIGHT		eLightType;
		XMFLOAT4	vDiffuse;
		XMFLOAT4	vSpecular;
		XMFLOAT4	vAmbient;

		/* Use Type Directional */
		XMFLOAT4	vDir;
		/* Use Type Point */
		XMFLOAT4	vPos;
		float		fRange = 0.f;
	}LIGHTDESC;

	typedef struct tagPointDistanceDesc{
		XMFLOAT3	vPoint;
		float		fDistance;
	}PDDESC;

	
}

#endif // !__ENGINE_STRUCT_H__
