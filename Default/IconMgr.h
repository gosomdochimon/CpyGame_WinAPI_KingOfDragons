#pragma once
class CIconMgr
{
public:
	CIconMgr();
	~CIconMgr();

public:
	static		CIconMgr*		Get_Instance(void)
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CIconMgr;

		return m_pInstance;
	}

	static		void			Destroy_Instance(void)
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CIconMgr* m_pInstance;
};

