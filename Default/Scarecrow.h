#pragma once
#include "Obj.h"
class CScarecrow :
	public CObj
{
public:
	CScarecrow();
	virtual ~CScarecrow();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void	Late_Update(void);
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void Update_ShadowRect_();
	int m_iCount;
};

