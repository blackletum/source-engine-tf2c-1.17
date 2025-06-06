#include "shaderlib/cshader.h"
class tdc_watercheap_vs30_Static_Index
{
private:
	int m_nBLEND;
#ifdef _DEBUG
	bool m_bBLEND;
#endif
public:
	void SetBLEND( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBLEND = i;
#ifdef _DEBUG
		m_bBLEND = true;
#endif
	}
	void SetBLEND( bool i )
	{
		m_nBLEND = i ? 1 : 0;
#ifdef _DEBUG
		m_bBLEND = true;
#endif
	}
public:
	tdc_watercheap_vs30_Static_Index( )
	{
#ifdef _DEBUG
		m_bBLEND = false;
#endif // _DEBUG
		m_nBLEND = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bBLEND;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nBLEND ) + 0;
	}
};
#define shaderStaticTest_tdc_watercheap_vs30 vsh_forgot_to_set_static_BLEND + 0
class tdc_watercheap_vs30_Dynamic_Index
{
public:
	tdc_watercheap_vs30_Dynamic_Index()
	{
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
#endif // _DEBUG
		return 0;
	}
};
#define shaderDynamicTest_tdc_watercheap_vs30 0
