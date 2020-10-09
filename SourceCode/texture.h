//*************************************************************************************************************
//
// �e�N�X�`������[texture.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CTexture
{
public:
	enum
	{
		NAME_NONE = -1,					// ����
		/* �G�t�F�N�g */
		NAME_EFFECT000,
		NAME_EFFECT001,
		NAME_EFFECT002,
		NAME_EFFECT003,
		NAME_EFFECT004,
		NAME_ORBIT000,
		NAME_ORBIT001,
		NAME_ORBIT002,
		NAME_ORBIT003,
		NAME_WALLFSKIN000,
		NAME_WALLFSKIN001,
		/* �w�i */
		NAME_GAMEBG,
		/* �v���C���[ */
		NAME_PLAYER,
		/* UI */
		NAME_FRAME,
		NAME_NUMBER,
		NAME_PLEASEPRESSBUTTON000,
		NAME_ANGLEGAUGE_1P,
		NAME_POWERGAUGE_1P,
		NAME_ANGLEGAUGE_2P,
		NAME_POWERGAUGE_2P,
		NAME_TITLENAME000,

		NAME_MAX						// �ő吔
	};
	enum
	{
		TEXTURE_TOPID_NONE     = -1,						// ����
		TEXTURE_TOPID_MAX      = NAME_MAX,					// �ő吔
	};
	CTexture();																												// �R���X�g���N�^
	~CTexture();																											// �f�X�g���N�^
	static void               Load(void);																					// �ǂݍ���
	static void               Unload(void);																					// �J��
	static void               CreateTexture(void);																			// �e�N�X�`���̐���
	static LPDIRECT3DTEXTURE9 GetTextureInfo(int nIndex);																	// �e�N�X�`���̎擾
	static int                GetNumTextureMax(void);																		// �e�N�X�`�����̍ő�
	static void               GetFileName(const int nIndex, STRING output);													// �t�@�C�����̎擾
	static CONST_STRING       GetFileName(const int nIndex);																// �t�@�C�����̎擾
private:
	/* �����o�֐� */
	static void               GetResource(void);																			// �����̒��B
	static void               ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);		// 1�s�������ǂݎ��
	static void               NumLoad(CONST_STRING cnpLine);																// ���̓ǂݍ���
	static void               FileNameLoad(CONST_STRING cnpLine);															// �t�@�C�����̓ǂݍ���
	/* �����o�ϐ� */
	static MyVector<LPDIRECT3DTEXTURE9> m_pTexture;																			// �e�N�X�`�����̃|�C���^
	static MyVector<CString>            m_pFileName;																		// �t�@�C�����̃|�C���^
	static int                          m_nNumTextureMax;																	// �ő�e�N�X�`����
};
#endif