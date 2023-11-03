#pragma once

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

template<typename T>
void Safe_Delete_Array(T& Temp)
{
	if (Temp)
	{
		delete[] Temp;
		Temp = nullptr;
	}
}

class CDeleteObj
{
public:
	template<typename T>
	void operator()(T& Temp) 
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

class CDeleteMap
{
public:
	template<typename T>
	void operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

class CTag_Finder
{
private:
	const TCHAR*		m_pTag;

public:
	CTag_Finder(const TCHAR* ptag) : m_pTag(ptag) {}

public:
	template<typename t>
	bool	operator()(t& Pair)
	{
		if (!lstrcmp(m_pTag, Pair.first))
			return true;

		return false;
	}

};

class CInt_Finder
{
private:
	const int		iNum;

public:
	CInt_Finder(const int _iNum) : iNum(_iNum) {}

public:
	template<typename T>
	bool	operator()(T& Pair)
	{

		if (iNum == Pair.first)
			return true;

		return false;
	}

};

//class CEnum_Finder
//{
//private:
//	CSkillInventory::SKILL m_eSkill;
//
//public:
//	CEnum_Finder(CSkillInventory::SKILL _eSkill) : m_eSkill(_eSkill) {}
//
//public:
//	template<typename t>
//	bool	operator()(t& Pair)
//	{
//		if (m_eSkill == Pair.first)
//			return true;
//			
//		return false;
//	}
//
//};
//class CTag_Finder
//{
//private:
//	const TCHAR*		m_pTag;
//	CSkillInventory::SKILL m_eSkill;
//public:
//	CTag_Finder(const TCHAR* pTag) : m_pTag(pTag), m_eSkill(CSkillInventory::SKILL_END) {}
//	CTag_Finder(CSkillInventory::SKILL _eSkill) : m_eSkill(_eSkill), m_pTag(nullptr) {}
//public:
//	template<typename T>
//	bool	operator()(T& Pair)
//	{
//		if (auto test = dynamic_cast<CSkillInventory::SKILL>(Pair.first))
//			return true;
//
//		if (!lstrcmp(m_pTag, Pair.first))
//			return true;
//
//		//if (m_eSkill == Pair.first))
//		//	return true;
//
//		return false;
//	}
//};

template<typename T>
bool		CompareY(T Dest, T Sour)
{
	return Dest->Get_ShadowInfo().fY < Sour->Get_ShadowInfo().fY;
}