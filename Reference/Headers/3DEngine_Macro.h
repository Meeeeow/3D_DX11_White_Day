#pragma once


// DLL EXPORTS //////////////////////////////////////////
#ifdef MY3D_ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif
/////////////////////////////////////////////////////////

#define DEFAULT = default
#define BEGIN(NAME) namespace NAME {
#define END			}

// SINGLETON ////////////////////////////////////////////
#define DECLARE_SINGLETON(ClassName)					\
public:													\
static ClassName* Get_Instance()						\
{														\
	if (!m_pInstance)									\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
static unsigned long Destroy_Instance()					\
{														\
	unsigned long		dwRefCnt = 0;					\
	if (nullptr != m_pInstance)							\
	{													\
		dwRefCnt = m_pInstance->Release();				\
		if(0 == dwRefCnt)								\
			m_pInstance = nullptr;						\
	}													\
	return dwRefCnt;									\
}														\
private:												\
	static ClassName* m_pInstance;						
														
#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;
/////////////////////////////////////////////////////////

// INSTANCE /////////////////////////////////////////////
#define GET_INSTANCE(CLASSNAME) [](){					\
	CLASSNAME* pInstance = CLASSNAME::Get_Instance();	\
	if(pInstance != nullptr){							\
	pInstance->AddRef();}								\
	return pInstance;									\
}();													\

#define RELEASE_INSTANCE(CLASSNAME) [](){				\
	CLASSNAME* pInstance = CLASSNAME::Get_Instance();	\
	if(pInstance != nullptr){							\
	pInstance->Release();}								\
}();													\
/////////////////////////////////////////////////////////


// DEBUG MESSAGE ////////////////////////////////////////
#ifndef _AFX
#define MSGBOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)
#else
#define MSGBOX(MESSAGE) AfxMessageBox(TEXT(MESSAGE))
#endif // !_AFX

#define FAILMSG(MESSAGE) { MSGBOX(MESSAGE); return E_FAIL; }
#define NULLMSG(MESSAGE) { MSGBOX(MESSAGE); return nullptr; }
#define NEGAMSG(MESSAGE) { MSGBOX(MESSAGE); return -1; }
#define VOIDMSG(MESSAGE) { MSGBOX(MESSAGE); return; }
/////////////////////////////////////////////////////////

