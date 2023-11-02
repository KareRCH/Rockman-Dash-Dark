#ifndef Engine_Macro_h__
#define Engine_Macro_h__


// 치환 매크로
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)			namespace NAMESPACE {
#define			END							}

#define			USING(NAMESPACE)			using namespace NAMESPACE;

#define			SUPER __super



// 특수한 함수 앞에 별도로 붙이기 위한 용도
#define PRIVATE		private:	// private:
#define PROTECTED	protected:	// protected:
#define PUBLIC		public:		// public:


// 빌드 옵션 매크로
#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)

// 디버그용 구현 코드 바이너리화 매크로, 호출스택에서 보이지 않는 문제를 디버그 모드에서만 해결한다.
#ifdef _DEBUG
#ifdef _DEBUG_DLL
#define ENGINE_DLL_DBG	_declspec(dllexport)
#else 
#define ENGINE_DLL_DBG
#endif
#endif

#else
#define ENGINE_DLL		_declspec(dllimport)

#ifdef _DEBUG
#ifdef _DEBUG_DLL
#define ENGINE_DLL_DBG	_declspec(dllimport)
#else 
#define ENGINE_DLL_DBG
#endif
#endif

#endif


// 검사 매크로
#define NULL_CHECK( _ptr)	\
	{if( (_ptr) == 0){ DebugBreak();return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( (_ptr) == 0){ DebugBreak();return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( (_ptr) == 0){MessageBox(NULL, _message, L"System Message",MB_OK); DebugBreak();}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( (_ptr) == 0){MessageBox(NULL, _message, L"System Message",MB_OK); DebugBreak();return _return;}}



#define FALSE_CHECK( _false)	\
	NULL_CHECK( _false)

#define FALSE_CHECK_RETURN( _false, _return)	\
	NULL_CHECK_RETURN( _false, _return)

#define FALSE_CHECK_MSG( _false, _message )	\
	NULL_CHECK_MSG( _false, _message )

#define FALSE_CHECK_RETURN_MSG( _false, _return, _message )	\
	NULL_CHECK_RETURN_MSG( _false, _return, _message )



#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); DebugBreak(); return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); DebugBreak(); return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); DebugBreak();}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); DebugBreak();return _return;}



// 생성 규칙 매크로
#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&);					\
		CLASSNAME& operator = (const CLASSNAME&);

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static _uint DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = nullptr;	\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(nullptr == m_pInstance) {				\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		_uint CLASSNAME::DestroyInstance( void ) {		\
			return Safe_Release(m_pInstance);			\
		}

#define			THIS_CLASS(CLASSNAME)		using ThisClass = CLASSNAME;
#define			BASE_CLASS(CLASSNAME)		using Base = CLASSNAME;
#define			DERIVED_CLASS(BASENAME, THISCLASS)	\
					THIS_CLASS(THISCLASS)			\
					BASE_CLASS(BASENAME)
#define			DERIVED_CLASS_SINGLETON(BASENAME, THISCLASS) \
					DERIVED_CLASS(BASENAME, THISCLASS)	\
					DECLARE_SINGLETON(THISCLASS)

// getset 매크로
// 일반 Set 함수
#define SET(TYPE, MVALUE, NAME)										\
			void Set_##NAME(TYPE value) { MVALUE = value; }
// 입력 값이 const Type
#define SET_C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(const TYPE value) { MVALUE = value; }
// 입력 값이 Type const (포인터 용)
#define SET__C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE const value) { MVALUE = value; }
// 입력 값이 const Type const (포인터 용)
#define SET_C_C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(const TYPE const value) { MVALUE = value; }

// 입력 값이 Type*
#define SET_PTR(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE* value) { MVALUE = *value; }
// 입력 값이 cosnt Type*
#define SET_C_PTR(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE* value) { MVALUE = *value; }
// 입력 값이 Type const*
#define SET_PTR_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE const* value) { MVALUE = *value; }
// 입력 값이 const Type const*
#define SET_C_PTR_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE const* value) { MVALUE = *value; }

// 입력 값이 Type&
#define SET_REF(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE& value) { MVALUE = value; }
// 입력 값이 const Type&
#define SET_C_REF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE& value) { MVALUE = value; }
// 입력 값이 Type const&
#define SET_REF_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE& const value) { MVALUE = value; }
// 입력 값이 const Type const&
#define SET_C_REF_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE const& value) { MVALUE = value; }

// 입력 값이 Type&&
#define SET_RREF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE&& value) { MVALUE = value; }
// 입력 값이 const Type&&
#define SET_C_RREF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE&& value) { MVALUE = value; }


// 반환값이 Type
#define GET(TYPE, MVALUE, NAME)										\
			TYPE Get_##NAME() { return MVALUE; }	
// 반환값이 const Type
#define GET_C(TYPE, MVALUE, NAME)									\
			const TYPE Get_##NAME() const { return MVALUE; }	
// 반환값이 Type const
#define GET__C(TYPE, MVALUE, NAME)									\
			TYPE const Get_##NAME() const { return MVALUE; }	
// 반환값이 const Type const
#define GET_C_C(TYPE, MVALUE, NAME)									\
			const TYPE const Get_##NAME() const { return MVALUE; }	

// 반환값이 Type&
#define GET_REF(TYPE, MVALUE, NAME)									\
			TYPE& Get_##NAME() { return MVALUE; }	
// 반환값이 const Type&
#define GET_C_REF(TYPE, MVALUE, NAME)								\
			const TYPE& Get_##NAME() const { return MVALUE; }	
// 반환값이 Type const&
#define GET_REF_C(TYPE, MVALUE, NAME)								\
			TYPE const& Get_##NAME() const { return MVALUE; }	
// 반환값이 const Type const&
#define GET_C_REF_C(TYPE, MVALUE, NAME)								\
			const TYPE const& Get_##NAME() const { return MVALUE; }

// 반환값이 Type* 
#define GET_PTR(TYPE, MVALUE, NAME)									\
			TYPE* Get_##NAME() { return &MVALUE; }
// 반환값이 const Type*
#define GET_C_PTR(TYPE, MVALUE, NAME)								\
			const TYPE* Get_##NAME() const { return &MVALUE; }
// 반환값이 Type const*
#define GET_PTR_C(TYPE, MVALUE, NAME)								\
			TYPE const* Get_##NAME() const { return &MVALUE; }
// 반환값이 const Type const*
#define GET_C_PTR_C(TYPE, MVALUE, NAME)								\
			const TYPE const* Get_##NAME() const { return &MVALUE; }



// 복수 GET 매크로
#pragma region GET SET 함수 복수로드 매크로
#define GETSET_1(TYPE, MVALUE, NAME, ARG1)		\
			ARG1##(TYPE, MVALUE, NAME)

#define GETSET_2(TYPE, MVALUE, NAME, ARG1, ARG2)			  \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)

#define GETSET_3(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3)			  \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)

#define GETSET_4(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4)				  \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)

#define GETSET_5(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5)				  \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)

#define GETSET_6(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)					  \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)	\
			ARG6##(TYPE, MVALUE, NAME)

#define GETSET_7(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)					 \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)	\
			ARG6##(TYPE, MVALUE, NAME)	\
			ARG7##(TYPE, MVALUE, NAME)

#define GETSET_8(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)					 \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)	\
			ARG6##(TYPE, MVALUE, NAME)	\
			ARG7##(TYPE, MVALUE, NAME)	\
			ARG8##(TYPE, MVALUE, NAME)

#define GETSET_9(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)			   \
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)	\
			ARG6##(TYPE, MVALUE, NAME)	\
			ARG7##(TYPE, MVALUE, NAME)	\
			ARG8##(TYPE, MVALUE, NAME)	\
			ARG9##(TYPE, MVALUE, NAME)

#define GETSET_10(TYPE, MVALUE, NAME, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)		\
			ARG1##(TYPE, MVALUE, NAME)	\
			ARG2##(TYPE, MVALUE, NAME)	\
			ARG3##(TYPE, MVALUE, NAME)	\
			ARG4##(TYPE, MVALUE, NAME)	\
			ARG5##(TYPE, MVALUE, NAME)	\
			ARG6##(TYPE, MVALUE, NAME)	\
			ARG7##(TYPE, MVALUE, NAME)	\
			ARG8##(TYPE, MVALUE, NAME)	\
			ARG9##(TYPE, MVALUE, NAME)	\
			ARG10##(TYPE, MVALUE, NAME)
#pragma endregion


#define DXCOLOR_WHITE         0xFFFFFFFF			// 흰색
#define DXCOLOR_BLACK         0xFF000000			// 검은색
#define DXCOLOR_RED           0xFFFF0000 // 빨간색
#define DXCOLOR_PURPLE        0xFF880088 // 자주색
#define DXCOLOR_MAGENTA       0xFFFF00FF // 심홍색
#define DXCOLOR_ORANGE        0xFFFFA500 // 주황색
#define DXCOLOR_DARK_ORANGE   0xFF733900 // 짙은 주황색
#define DXCOLOR_YELLOW        0xFFFFFF00 // 노란색
#define DXCOLOR_GREEN         0xFF00FF00 // 초록색
#define DXCOLOR_CYAN          0xFF00FFFF // 청록색
#define DXCOLOR_DARK_GREEN    0xFF008800 // 어두운 초록색
#define DXCOLOR_OLIVE         0xFF888800 // 올리브색
#define DXCOLOR_BLUE          0xFF0000FF // 파란색
#define DXCOLOR_NAVY          0xFF000088 // 짙은 파란색
#define DXCOLOR_TEAL          0xFF008888 // 옥색
#define DXCOLOR_MAROON        0xFF880000 // 고동색
#define DXCOLOR_PINK          0xFFFFC0CB // 분홍색
//#define DXCOLOR_LAVENDER      0xFF, 230, 230, 250 // 라벤더색
//#define DXCOLOR_GOLD          0xFF, 255, 215,   0 // 금색
//#define DXCOLOR_SILVER        0xFF, 192, 192, 192 // 은색
//#define DXCOLOR_BROWN         0xFF, 165,  42,  42 // 갈색
//#define DXCOLOR_GRAY          0xFF, 128, 128, 128 // 회색
//#define DXCOLOR_LIGHT_GRAY    0xFF, 194, 194, 194 // 회색
//#define DXCOLOR_INDIGO        0xFF,  75,   0, 130 // 남색
//#define DXCOLOR_TURQUOISE     0xFF,  64, 224, 208 // 터키스색
//#define DXCOLOR_LIME          0xFF,   0, 255,   0 // 라임색


			

#endif // Engine_Macro_h__
