#pragma once
#include "UI.h"


class CSkill_UI :
	public CUI
{
public:
	CSkill_UI();
	virtual ~CSkill_UI();

#define	SkillEnd	5
#define	InvenEnd	3
#define InvenStart	0
#define ReversRot	3
	typedef struct tag_SkillInfo {
		float fSpendMp;
		float fMaxCoolTIme;	
		float fCoolTIme;
		DWORD			m_dwCoolTime;
		bool  bCanUse;
		const TCHAR* SkillName;
		const TCHAR* SkillIcon;
		
	}SKILLINFO;

	list<SKILLINFO> m_listSkill;
	//1. �� 2.�� 3.�� 4. �� 5. ��?
public:
	virtual		void	Initialize(void) override;
	virtual		int		Update(void)	override;
	virtual		void	Late_Update(void) override;
	virtual		void	Render(HDC hDC) override;
	virtual		void	Release(void) override;
public:
	void		Rot_Icon(bool _Rot); //1�� �� ������ 0�� �� ����

	int			Get_CurSkill(void) { return m_iCurSkill; }
	void		Set_CheckBool(void);
	bool		Get_CheckBool(void);

	float		Get_Cooltime(void);
	float		Get_MaxCooltime(void);
	const TCHAR*		Get_Icon(void);
	float		Get_SpendMp(void);
private:
	void		Insert_Bmp(void);
	void		Register_Map(void);

	void		Find_FameKey(void);

	void Update_InfoPos();
	void Update_IconRect(void);
	void Init_SkillInfo(void);


	void CoolTimeDown(void);
	
private:
	map<int, int> m_mapInven;
	//1.���� 2. ����̵� 3. ����
	//map<int, const TCHAR*> m_mapIcon;
	map<int, SKILLINFO> m_mapSkillInfo;
	CObj* m_pPlayer;

	//bool	m_bShow;

	int		m_iNextSkill;
	int		m_iCurSkill;
	int		m_iBackSkill;
	//������ ����ü�� �̿��ؾ� �ϴ°�?  
	ICON	m_SkillIcon[3];
	INFO	m_tIconPos[4];
	RECT	m_tIconRect[4];
	
	const TCHAR*	m_pIconKey[5];
	HDC				m_hDC[4];
	//1�� 2�� 3�� �ڸ��� ���缭 
	
};


