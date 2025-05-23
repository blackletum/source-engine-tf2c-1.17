//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================
#include "cbase.h"
#include "vgui_int.h"
#include "ienginevgui.h"
#include "vgui_rootpanel_tdc.h"
#include "vgui/IVGui.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

C_TDCRootPanel *g_pRootPanel = NULL;


//-----------------------------------------------------------------------------
// Global functions.
//-----------------------------------------------------------------------------
void VGUI_CreateClientDLLRootPanel( void )
{
	g_pRootPanel = new C_TDCRootPanel( enginevgui->GetPanel( PANEL_CLIENTDLL ) );
}

void VGUI_DestroyClientDLLRootPanel( void )
{
	delete g_pRootPanel;
	g_pRootPanel = NULL;
}

vgui::VPANEL VGui_GetClientDLLRootPanel( void )
{
	return g_pRootPanel->GetVPanel();
}


//-----------------------------------------------------------------------------
// C_TDCRootPanel implementation.
//-----------------------------------------------------------------------------
C_TDCRootPanel::C_TDCRootPanel( vgui::VPANEL parent )
	: BaseClass( NULL, "TF Root Panel" )
{
	SetParent( parent );
	SetPaintEnabled( false );
	SetPaintBorderEnabled( false );
	SetPaintBackgroundEnabled( false );

	// This panel does post child painting
	SetPostChildPaintEnabled( true );

	// Make it screen sized
	SetBounds( 0, 0, ScreenWidth(), ScreenHeight() );

	// Ask for OnTick messages
	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TDCRootPanel::~C_TDCRootPanel( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TDCRootPanel::PostChildPaint()
{
	BaseClass::PostChildPaint();

	// Draw all panel effects
	RenderPanelEffects();
}

//-----------------------------------------------------------------------------
// Purpose: For each panel effect, check if it wants to draw and draw it on
//  this panel/surface if so
//-----------------------------------------------------------------------------
void C_TDCRootPanel::RenderPanelEffects( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TDCRootPanel::OnTick( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Reset effects on level load/shutdown
//-----------------------------------------------------------------------------
void C_TDCRootPanel::LevelInit( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TDCRootPanel::LevelShutdown( void )
{
}

