#ifndef Engine_Macro_h__
#define Engine_Macro_h__


// ġȯ ��ũ��
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)			namespace NAMESPACE {
#define			END							}

#define			USING(NAMESPACE)			using namespace NAMESPACE;

#define			SUPER __super



// Ư���� �Լ� �տ� ������ ���̱� ���� �뵵
#define PRIVATE		private:	// private:
#define PROTECTED	protected:	// protected:
#define PUBLIC		public:		// public:


// ���� �ɼ� ��ũ��
#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)

// ����׿� ���� �ڵ� ���̳ʸ�ȭ ��ũ��, ȣ�⽺�ÿ��� ������ �ʴ� ������ ����� ��忡���� �ذ��Ѵ�.
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


// �˻� ��ũ��
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



// ���� ��Ģ ��ũ��
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

// getset ��ũ��
// �Ϲ� Set �Լ�
#define SET(TYPE, MVALUE, NAME)										\
			void Set_##NAME(TYPE value) { MVALUE = value; }
// �Է� ���� const Type
#define SET_C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(const TYPE value) { MVALUE = value; }
// �Է� ���� Type const (������ ��)
#define SET__C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE const value) { MVALUE = value; }
// �Է� ���� const Type const (������ ��)
#define SET_C_C(TYPE, MVALUE, NAME)									\
			void Set_##NAME(const TYPE const value) { MVALUE = value; }

// �Է� ���� Type*
#define SET_PTR(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE* value) { MVALUE = *value; }
// �Է� ���� cosnt Type*
#define SET_C_PTR(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE* value) { MVALUE = *value; }
// �Է� ���� Type const*
#define SET_PTR_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE const* value) { MVALUE = *value; }
// �Է� ���� const Type const*
#define SET_C_PTR_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE const* value) { MVALUE = *value; }

// �Է� ���� Type&
#define SET_REF(TYPE, MVALUE, NAME)									\
			void Set_##NAME(TYPE& value) { MVALUE = value; }
// �Է� ���� const Type&
#define SET_C_REF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE& value) { MVALUE = value; }
// �Է� ���� Type const&
#define SET_REF_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE& const value) { MVALUE = value; }
// �Է� ���� const Type const&
#define SET_C_REF_C(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE const& value) { MVALUE = value; }

// �Է� ���� Type&&
#define SET_RREF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(TYPE&& value) { MVALUE = value; }
// �Է� ���� const Type&&
#define SET_C_RREF(TYPE, MVALUE, NAME)								\
			void Set_##NAME(const TYPE&& value) { MVALUE = value; }


// ��ȯ���� Type
#define GET(TYPE, MVALUE, NAME)										\
			TYPE Get_##NAME() { return MVALUE; }	
// ��ȯ���� const Type
#define GET_C(TYPE, MVALUE, NAME)									\
			const TYPE Get_##NAME() const { return MVALUE; }	
// ��ȯ���� Type const
#define GET__C(TYPE, MVALUE, NAME)									\
			TYPE const Get_##NAME() const { return MVALUE; }	
// ��ȯ���� const Type const
#define GET_C_C(TYPE, MVALUE, NAME)									\
			const TYPE const Get_##NAME() const { return MVALUE; }	

// ��ȯ���� Type&
#define GET_REF(TYPE, MVALUE, NAME)									\
			TYPE& Get_##NAME() { return MVALUE; }	
// ��ȯ���� const Type&
#define GET_C_REF(TYPE, MVALUE, NAME)								\
			const TYPE& Get_##NAME() const { return MVALUE; }	
// ��ȯ���� Type const&
#define GET_REF_C(TYPE, MVALUE, NAME)								\
			TYPE const& Get_##NAME() const { return MVALUE; }	
// ��ȯ���� const Type const&
#define GET_C_REF_C(TYPE, MVALUE, NAME)								\
			const TYPE const& Get_##NAME() const { return MVALUE; }

// ��ȯ���� Type* 
#define GET_PTR(TYPE, MVALUE, NAME)									\
			TYPE* Get_##NAME() { return &MVALUE; }
// ��ȯ���� const Type*
#define GET_C_PTR(TYPE, MVALUE, NAME)								\
			const TYPE* Get_##NAME() const { return &MVALUE; }
// ��ȯ���� Type const*
#define GET_PTR_C(TYPE, MVALUE, NAME)								\
			TYPE const* Get_##NAME() const { return &MVALUE; }
// ��ȯ���� const Type const*
#define GET_C_PTR_C(TYPE, MVALUE, NAME)								\
			const TYPE const* Get_##NAME() const { return &MVALUE; }



// ���� GET ��ũ��
#pragma region GET SET �Լ� �����ε� ��ũ��
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


#define DXCOLOR_WHITE         0xFFFFFFFF			// ���
#define DXCOLOR_BLACK         0xFF000000			// ������
#define DXCOLOR_RED           0xFFFF0000 // ������
#define DXCOLOR_PURPLE        0xFF880088 // ���ֻ�
#define DXCOLOR_MAGENTA       0xFFFF00FF // ��ȫ��
#define DXCOLOR_ORANGE        0xFFFFA500 // ��Ȳ��
#define DXCOLOR_DARK_ORANGE   0xFF733900 // £�� ��Ȳ��
#define DXCOLOR_YELLOW        0xFFFFFF00 // �����
#define DXCOLOR_GREEN         0xFF00FF00 // �ʷϻ�
#define DXCOLOR_CYAN          0xFF00FFFF // û�ϻ�
#define DXCOLOR_DARK_GREEN    0xFF008800 // ��ο� �ʷϻ�
#define DXCOLOR_OLIVE         0xFF888800 // �ø����
#define DXCOLOR_BLUE          0xFF0000FF // �Ķ���
#define DXCOLOR_NAVY          0xFF000088 // £�� �Ķ���
#define DXCOLOR_TEAL          0xFF008888 // ����
#define DXCOLOR_MAROON        0xFF880000 // ����
#define DXCOLOR_PINK          0xFFFFC0CB // ��ȫ��
//#define DXCOLOR_LAVENDER      0xFF, 230, 230, 250 // �󺥴���
//#define DXCOLOR_GOLD          0xFF, 255, 215,   0 // �ݻ�
//#define DXCOLOR_SILVER        0xFF, 192, 192, 192 // ����
//#define DXCOLOR_BROWN         0xFF, 165,  42,  42 // ����
//#define DXCOLOR_GRAY          0xFF, 128, 128, 128 // ȸ��
//#define DXCOLOR_LIGHT_GRAY    0xFF, 194, 194, 194 // ȸ��
//#define DXCOLOR_INDIGO        0xFF,  75,   0, 130 // ����
//#define DXCOLOR_TURQUOISE     0xFF,  64, 224, 208 // ��Ű����
//#define DXCOLOR_LIME          0xFF,   0, 255,   0 // ���ӻ�


			

#endif // Engine_Macro_h__
