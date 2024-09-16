#pragma once
#ifndef Engine_Macro_h__
#define Engine_Macro_h__

// [ RGB ]
#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))


// [ 메시지 박스 ]
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif


// [ 네임스페이스 ]
#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}
#define			USING(NAMESPACE)	using namespace NAMESPACE;


// [ dll, Export ]
#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


// [ 경고 무시 ]
#pragma warning(disable : 4251)


// [ 싱글톤 ]
#define DELETE_COPY(classType)								\
		public:												\
		classType() = default;								\
		~classType() = default;								\
															\
		classType(const classType&) = delete;				\
		classType(classType&&) = delete;					\
															\
		classType& operator=(const classType&) = delete;	\
		classType& operator=(classType&&) = delete;			\


#define DECLARE_SINGLETON(classType)						\
		DELETE_COPY(classType)								\
		private:											\
		static std::unique_ptr<classType> m_upInstance;		\
															\
		public:												\
		static std::unique_ptr<classType>& GetInstance();	\


#define IMPLEMENT_SINGLETON(classType)									\
		std::unique_ptr<classType> classType::m_upInstance = nullptr;	\
																		\
		std::unique_ptr<classType>& classType::GetInstance()			\
		{																\
			if (m_upInstance == nullptr)								\
			{															\
				m_upInstance = std::make_unique<classType>();			\
			}															\
			return m_upInstance;										\
		}																\

#endif // Engine_Macro_h__
