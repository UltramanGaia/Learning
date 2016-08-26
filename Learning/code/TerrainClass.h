//=============================================================================
// Name: TerrainClass.h
//	Des: ��ά����ϵͳ���ͷ�ļ�
//=============================================================================

#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>
#include  "D3DUtil.h"

class TerrainClass
{
private:
    LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D�豸
    LPDIRECT3DTEXTURE9				m_pTexture;						//����
    LPDIRECT3DINDEXBUFFER9			m_pIndexBuffer;					//���㻺��
    LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;				//��������

    int								m_nCellsPerRow;					// ÿ�еĵ�Ԫ����
    int								m_nCellsPerCol;					// ÿ�еĵ�Ԫ����
    int								m_nVertsPerRow;					// ÿ�еĶ�����
    int								m_nVertsPerCol;					// ÿ�еĶ�����
    int								m_nNumVertices;					// ��������
    float							m_fTerrainWidth;				// ���εĿ��
    float							m_fTerrainDepth;				// ���ε����
    float							m_fCellSpacing;					// ��Ԫ��ļ��
    float							m_fHeightScale;					// �߶�����ϵ��
    std::vector<float>				m_vHeightInfo;					// ���ڴ�Ÿ߶���Ϣ

	//����һ�����ε�FVF�����ʽ
    struct TERRAINVERTEX
    {
        float _x, _y, _z;
        float _u, _v;
        TERRAINVERTEX(float x, float y, float z, float u, float v) 
            :_x(x), _y(y), _z(z), _u(u), _v(v) {}
        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
    };

public:
    TerrainClass(IDirect3DDevice9 *pd3dDevice);						//���캯��
    virtual ~TerrainClass(void);									//��������

public:
    BOOL LoadTerrainFromFile(wchar_t *pRawFileName, wchar_t *pTextureFile);		//���ļ����ظ߶�ͼ������ĺ���
    BOOL InitTerrain(INT nRows, INT nCols, float fSpace, float fScale,float fUTextureNumbers , float fVTextureNumbers);			//���γ�ʼ������
    BOOL RenderTerrain(D3DXMATRIX *pMatWorld, BOOL bDrawFrame=FALSE);			//������Ⱦ����

	float getPositionHeight(D3DXVECTOR3 pos);
};
