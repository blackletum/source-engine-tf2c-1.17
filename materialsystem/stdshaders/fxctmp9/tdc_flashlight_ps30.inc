#include "shaderlib/cshader.h"
class tdc_flashlight_ps30_Static_Index
{
private:
	int m_nNORMALMAP;
#ifdef _DEBUG
	bool m_bNORMALMAP;
#endif
public:
	void SetNORMALMAP( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nNORMALMAP = i;
#ifdef _DEBUG
		m_bNORMALMAP = true;
#endif
	}
	void SetNORMALMAP( bool i )
	{
		m_nNORMALMAP = i ? 1 : 0;
#ifdef _DEBUG
		m_bNORMALMAP = true;
#endif
	}
private:
	int m_nNORMALMAP2;
#ifdef _DEBUG
	bool m_bNORMALMAP2;
#endif
public:
	void SetNORMALMAP2( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nNORMALMAP2 = i;
#ifdef _DEBUG
		m_bNORMALMAP2 = true;
#endif
	}
	void SetNORMALMAP2( bool i )
	{
		m_nNORMALMAP2 = i ? 1 : 0;
#ifdef _DEBUG
		m_bNORMALMAP2 = true;
#endif
	}
private:
	int m_nWORLDVERTEXTRANSITION;
#ifdef _DEBUG
	bool m_bWORLDVERTEXTRANSITION;
#endif
public:
	void SetWORLDVERTEXTRANSITION( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nWORLDVERTEXTRANSITION = i;
#ifdef _DEBUG
		m_bWORLDVERTEXTRANSITION = true;
#endif
	}
	void SetWORLDVERTEXTRANSITION( bool i )
	{
		m_nWORLDVERTEXTRANSITION = i ? 1 : 0;
#ifdef _DEBUG
		m_bWORLDVERTEXTRANSITION = true;
#endif
	}
private:
	int m_nSEAMLESS;
#ifdef _DEBUG
	bool m_bSEAMLESS;
#endif
public:
	void SetSEAMLESS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSEAMLESS = i;
#ifdef _DEBUG
		m_bSEAMLESS = true;
#endif
	}
	void SetSEAMLESS( bool i )
	{
		m_nSEAMLESS = i ? 1 : 0;
#ifdef _DEBUG
		m_bSEAMLESS = true;
#endif
	}
private:
	int m_nDETAILTEXTURE;
#ifdef _DEBUG
	bool m_bDETAILTEXTURE;
#endif
public:
	void SetDETAILTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDETAILTEXTURE = i;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
#endif
	}
	void SetDETAILTEXTURE( bool i )
	{
		m_nDETAILTEXTURE = i ? 1 : 0;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
#endif
	}
private:
	int m_nDETAIL_BLEND_MODE;
#ifdef _DEBUG
	bool m_bDETAIL_BLEND_MODE;
#endif
public:
	void SetDETAIL_BLEND_MODE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDETAIL_BLEND_MODE = i;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = true;
#endif
	}
	void SetDETAIL_BLEND_MODE( bool i )
	{
		m_nDETAIL_BLEND_MODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = true;
#endif
	}
private:
	int m_nFLASHLIGHTDEPTHFILTERMODE;
#ifdef _DEBUG
	bool m_bFLASHLIGHTDEPTHFILTERMODE;
#endif
public:
	void SetFLASHLIGHTDEPTHFILTERMODE( int i )
	{
		Assert( i >= 0 && i <= 2 );
		m_nFLASHLIGHTDEPTHFILTERMODE = i;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = true;
#endif
	}
	void SetFLASHLIGHTDEPTHFILTERMODE( bool i )
	{
		m_nFLASHLIGHTDEPTHFILTERMODE = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = true;
#endif
	}
public:
	tdc_flashlight_ps30_Static_Index( )
	{
#ifdef _DEBUG
		m_bNORMALMAP = false;
#endif // _DEBUG
		m_nNORMALMAP = 0;
#ifdef _DEBUG
		m_bNORMALMAP2 = false;
#endif // _DEBUG
		m_nNORMALMAP2 = 0;
#ifdef _DEBUG
		m_bWORLDVERTEXTRANSITION = false;
#endif // _DEBUG
		m_nWORLDVERTEXTRANSITION = 0;
#ifdef _DEBUG
		m_bSEAMLESS = false;
#endif // _DEBUG
		m_nSEAMLESS = 0;
#ifdef _DEBUG
		m_bDETAILTEXTURE = false;
#endif // _DEBUG
		m_nDETAILTEXTURE = 0;
#ifdef _DEBUG
		m_bDETAIL_BLEND_MODE = false;
#endif // _DEBUG
		m_nDETAIL_BLEND_MODE = 0;
#ifdef _DEBUG
		m_bFLASHLIGHTDEPTHFILTERMODE = false;
#endif // _DEBUG
		m_nFLASHLIGHTDEPTHFILTERMODE = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllStaticVarsDefined = m_bNORMALMAP && m_bNORMALMAP2 && m_bWORLDVERTEXTRANSITION && m_bSEAMLESS && m_bDETAILTEXTURE && m_bDETAIL_BLEND_MODE && m_bFLASHLIGHTDEPTHFILTERMODE;
		Assert( bAllStaticVarsDefined );
#endif // _DEBUG
		return ( 4 * m_nNORMALMAP ) + ( 12 * m_nNORMALMAP2 ) + ( 24 * m_nWORLDVERTEXTRANSITION ) + ( 48 * m_nSEAMLESS ) + ( 96 * m_nDETAILTEXTURE ) + ( 192 * m_nDETAIL_BLEND_MODE ) + ( 384 * m_nFLASHLIGHTDEPTHFILTERMODE ) + 0;
	}
};
#define shaderStaticTest_tdc_flashlight_ps30 psh_forgot_to_set_static_NORMALMAP + psh_forgot_to_set_static_NORMALMAP2 + psh_forgot_to_set_static_WORLDVERTEXTRANSITION + psh_forgot_to_set_static_SEAMLESS + psh_forgot_to_set_static_DETAILTEXTURE + psh_forgot_to_set_static_DETAIL_BLEND_MODE + psh_forgot_to_set_static_FLASHLIGHTDEPTHFILTERMODE + 0
class tdc_flashlight_ps30_Dynamic_Index
{
private:
	int m_nPIXELFOGTYPE;
#ifdef _DEBUG
	bool m_bPIXELFOGTYPE;
#endif
public:
	void SetPIXELFOGTYPE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nPIXELFOGTYPE = i;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
	void SetPIXELFOGTYPE( bool i )
	{
		m_nPIXELFOGTYPE = i ? 1 : 0;
#ifdef _DEBUG
		m_bPIXELFOGTYPE = true;
#endif
	}
private:
	int m_nFLASHLIGHTSHADOWS;
#ifdef _DEBUG
	bool m_bFLASHLIGHTSHADOWS;
#endif
public:
	void SetFLASHLIGHTSHADOWS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nFLASHLIGHTSHADOWS = i;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = true;
#endif
	}
	void SetFLASHLIGHTSHADOWS( bool i )
	{
		m_nFLASHLIGHTSHADOWS = i ? 1 : 0;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = true;
#endif
	}
public:
	tdc_flashlight_ps30_Dynamic_Index()
	{
#ifdef _DEBUG
		m_bPIXELFOGTYPE = false;
#endif // _DEBUG
		m_nPIXELFOGTYPE = 0;
#ifdef _DEBUG
		m_bFLASHLIGHTSHADOWS = false;
#endif // _DEBUG
		m_nFLASHLIGHTSHADOWS = 0;
	}
	int GetIndex()
	{
		// Asserts to make sure that we aren't using any skipped combinations.
		// Asserts to make sure that we are setting all of the combination vars.
#ifdef _DEBUG
		bool bAllDynamicVarsDefined = m_bPIXELFOGTYPE && m_bFLASHLIGHTSHADOWS;
		Assert( bAllDynamicVarsDefined );
#endif // _DEBUG
		return ( 1 * m_nPIXELFOGTYPE ) + ( 2 * m_nFLASHLIGHTSHADOWS ) + 0;
	}
};
#define shaderDynamicTest_tdc_flashlight_ps30 psh_forgot_to_set_dynamic_PIXELFOGTYPE + psh_forgot_to_set_dynamic_FLASHLIGHTSHADOWS + 0
