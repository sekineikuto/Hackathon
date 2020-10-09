//*************************************************************************************************************
//
// ���e���� [bomb.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _BOMB_H_
#define _BOMB_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CScene2D;
class CBomb : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = -1,
		STATE_STANDBY,
		STATE_MOVEING,
		STATE_LANDING,
		STATE_MAX
	}STATE;

	CBomb() : CScene(PRIORITY_PLAYER) {};		// �R���X�g���N�^
	~CBomb() {};								// �f�X�g���N�^

	static CBomb *Create(D3DXVECTOR3 &pos, D3DXVECTOR2 &size, int nPlayer_ID);		// ����
	void Init(D3DXVECTOR3 &pos, D3DXVECTOR2 &size, int &nPlayer_ID);				// ������
	void Init(void) {};																// ������
	void Uninit(void);																// �I��
	void Update(void);																// �X�V
	void Draw(void);																// �`��

	void UpdateStandby(void);														// �X�V
	void UpdateMoveing(void);														// �X�V
	void UpdateLanding(void);														// �X�V

	void Expansion(void);															// �c��

	void Fire(float& fForce, float& fForceY, D3DXVECTOR3 &pos);

	void SetState(STATE state) { m_State = state; }
	STATE & GetState(void) { return m_State; }

	float GetDistance(void) { return m_fDistance; }

	D3DXVECTOR3 & GetPos(void);

private:
	CScene2D *m_pScene2D;					// �V�[���QD�|�C���^
	STATE     m_State;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_StartPos;
	float       m_fDistance;
	int       m_nPlayer_ID;

	bool m_bBGM;
};

#endif