#pragma once

#include "Define.h"
#include "Player.h"
#include "Bullet.h"

class CMainGame
{
public:					// 생성자, 소멸자
	CMainGame();
	~CMainGame();

 public:							 //  public -> protected-> private
	void		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(void);
	void		Release(void);

private:
	HDC			m_hDC;
	
	/*CObj*		m_pPlayer;
	list<CObj*>	m_BulletList;*/



	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[32];
	
};

// 디자인 패턴 : 코드의 구조들을 일정한 형태로 만들어 재사용하기 편리하게 만든 일정한 패턴
// 클래스 간의 관게, 클래스 간의 행동양식을 공통된 요소로 분류
// 분류한 각각에 대해 객체 지향적인 설계를 따른 노하우를 학문화 시킴

/*
생성(Creational) 패턴
객체 생성에 관련된 패턴
객체의 생성과 조합을 캡슐화해 특정 객체가 생성되거나 변경되어도 프로그램 구조에 영향을 크게 받지 않도록 유연성을 제공한다.

구조(Structural) 패턴
클래스나 객체를 조합해 더 큰 구조를 만드는 패턴
예를 들어 서로 다른 인터페이스를 지닌 2개의 객체를 묶어 단일 인터페이스를 제공하거나 객체들을 서로 묶어 새로운 기능을 제공하는 패턴이다.

행위(Behavioral)
객체나 클래스 사이의 알고리즘이나 책임 분배에 관련된 패턴
한 객체가 혼자 수행할 수 없는 작업을 여러 개의 객체로 어떻게 분배하는지, 또 그렇게 하면서도 객체 사이의 결합도를 최소화하는 것에 중점을 둔다.*/


// 메디에이터(중재자) 패턴 : 상호작용하는 객체들이 서로 복잡한 관계를 맺고 있을 경우 별도의 형식으로 정의하여 중재하기 위한 객체를 두는 패턴
// 기존에 사용하던 다 대 다 관계가 아닌 1 : 다 의 관계로 복잡성을 줄일 수 있다.

// 싱글톤 패턴 : 단 하나의 인스턴스만 생성하여 사용하는 패턴, 인스턴스(객체)가 필요할 때 똑같은 인스턴스를 재생성하는 것이 아니라 이미 만들어진 녀석을 불러와서 사용하는 패턴
// 기존에 만들어진 인스턴스를 사용하기 위해선 단 하나만 생성하고 나머지는 생성할 수 없게 하기 위해 생성자, 소멸자를 함부로 호출하지 못하도록 설계해야 한다.
