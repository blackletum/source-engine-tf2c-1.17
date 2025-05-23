//========= Copyright � 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include <KeyValues.h>
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>
#include <filesystem.h>
#include <vgui_controls/AnimationController.h>
#include "iclientmode.h"
#include "clientmode_shared.h"
#include "shareddefs.h"
#include "tdc_shareddefs.h"
#include "tdc_controls.h"
#include "tdc_gamerules.h"
#ifdef _WIN32
#include "winerror.h"
#endif
#include "ixboxsystem.h"
#include "intromenu.h"
#include "tdc_intromenu.h"
#include "tdc_viewport.h"

// used to determine the action the intro menu should take when OnTick handles a think for us
enum
{
	INTRO_NONE,
	INTRO_STARTVIDEO,
	INTRO_BACK,
	INTRO_CONTINUE,
};

using namespace vgui;

// sort function for the list of captions that we're going to show
int CaptionsSort( CVideoCaption* const *p1, CVideoCaption* const *p2 )
{
	// check the start time
	if ( (*p2)->m_flStartTime < (*p1)->m_flStartTime )
	{
		return 1;
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTDCIntroMenu::CTDCIntroMenu( IViewPort *pViewPort ) : CIntroMenu( pViewPort )
{
	m_pVideo = new CTDCVideoPanel( this, "VideoPanel" );
	m_pModel = new CModelPanel( this, "MenuBG" );
	m_pCaptionLabel = new CExLabel(this, "VideoCaption", "");

#ifdef _X360
	m_pFooter = new CTDCFooter( this, "Footer" );
#else
	m_pBack = new CExButton( this, "Back", "" );
	m_pOK = new CExButton( this, "Skip", "" );
#endif

	m_iCurrentCaption = 0;
	m_flVideoStartTime = 0;

	m_flActionThink = -1;
	m_iAction = INTRO_NONE;

	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CTDCIntroMenu::~CTDCIntroMenu()
{
	m_Captions.PurgeAndDeleteElements();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	LoadControlSettings( "Resource/UI/IntroMenu.res" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::SetNextThink( float flActionThink, int iAction )
{
	m_flActionThink = flActionThink;
	m_iAction = iAction;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::OnTick()
{
	if ( !TDCGameRules() )
		return;

	// do we have anything special to do?
	if ( m_flActionThink > 0 && m_flActionThink < gpGlobals->curtime )
	{
		if ( m_iAction == INTRO_STARTVIDEO )
		{
			if ( m_pVideo )
			{
				// turn on the captions if we have them
				if ( LoadCaptions() )
				{
					if ( m_pCaptionLabel && !m_pCaptionLabel->IsVisible() )
					{
						m_pCaptionLabel->SetText( " " );
						m_pCaptionLabel->SetVisible( true );
					}
				}
				else
				{
					if ( m_pCaptionLabel && m_pCaptionLabel->IsVisible() )
					{
						m_pCaptionLabel->SetVisible( false );
					}
				}

				m_pVideo->Activate();
				m_pVideo->BeginPlayback( TDCGameRules()->GetVideoFileForMap() );
				m_pVideo->MoveToFront();

				m_flVideoStartTime = gpGlobals->curtime;
			}
		}
		else if ( m_iAction == INTRO_BACK )
		{
			m_pViewPort->ShowPanel( this, false );
			m_pViewPort->ShowPanel( PANEL_MAPINFO, true );
		}
		else if ( m_iAction == INTRO_CONTINUE )
		{
			m_pViewPort->ShowPanel( this, false );

			if ( GetLocalPlayerTeam() == TEAM_UNASSIGNED )
			{
				GetTDCViewPort()->ShowTeamMenu( true );
			}
		}

		// reset our think
		SetNextThink( -1, INTRO_NONE );
	}

	// check if we need to update our captions
	if ( m_pCaptionLabel && m_pCaptionLabel->IsVisible() )
	{
		UpdateCaptions();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTDCIntroMenu::LoadCaptions( void )
{
	bool bSuccess = false;

	// clear any current captions
	m_Captions.PurgeAndDeleteElements();
	m_iCurrentCaption = 0;

	if ( m_pCaptionLabel )
	{
		KeyValues *kvCaptions = NULL;
		char strFullpath[MAX_PATH];

		V_strcpy_safe( strFullpath, TDCGameRules()->GetVideoFileForMap( false ) );	// Assume we must play out of the media directory
		V_strcat_safe( strFullpath, ".res" );					// Assume we're a .res extension type

		if ( g_pFullFileSystem->FileExists( strFullpath ) )
		{
			kvCaptions = new KeyValues( strFullpath );

			if ( kvCaptions )
			{
				if ( kvCaptions->LoadFromFile( g_pFullFileSystem, strFullpath ) )
				{
					for ( KeyValues *pData = kvCaptions->GetFirstSubKey(); pData != NULL; pData = pData->GetNextKey() )
					{
						CVideoCaption *pCaption = new CVideoCaption;
						if ( pCaption )
						{
							pCaption->m_pszString = ReadAndAllocStringValue( pData, "string" );
							pCaption->m_flStartTime = pData->GetFloat( "start", 0.0 );
							pCaption->m_flDisplayTime = pData->GetFloat( "length", 3.0 );

							m_Captions.AddToTail( pCaption );

							// we have at least one caption to show
							bSuccess = true;
						}
					}
				}

				kvCaptions->deleteThis();
			}
		}
	}

	if ( bSuccess )
	{
		// sort the captions so we show them in the correct order (they're not necessarily in order in the .res file)
		m_Captions.Sort( CaptionsSort );
	}

	return bSuccess;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::UpdateCaptions( void )
{
	if ( m_pCaptionLabel && m_pCaptionLabel->IsVisible() && ( m_Captions.Count() > 0 ) )
	{
		CVideoCaption *pCaption = m_Captions[m_iCurrentCaption];

		if ( pCaption )
		{
			if ( ( pCaption->m_flCaptionStart >= 0 ) && ( pCaption->m_flCaptionStart + pCaption->m_flDisplayTime < gpGlobals->curtime ) )
			{
				// fade out the caption
				g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "VideoCaptionFadeOut" );

				// move to the next caption
				m_iCurrentCaption++;

				if ( !m_Captions.IsValidIndex( m_iCurrentCaption ) )
				{
					// we're done showing captions
					m_pCaptionLabel->SetVisible( false );
				}
			}
			// is it time to show the caption?
			else if ( m_flVideoStartTime + pCaption->m_flStartTime < gpGlobals->curtime )
			{
				// have we already started this video?
				if ( pCaption->m_flCaptionStart < 0 )
				{
					m_pCaptionLabel->SetText( pCaption->m_pszString );
					pCaption->m_flCaptionStart = gpGlobals->curtime;

					// fade in the next caption
					g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "VideoCaptionFadeIn" );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	// reset our think
	SetNextThink( -1, INTRO_NONE );

	if ( bShow )
	{
		Activate();
		SetMouseInputEnabled( true );

		if ( m_pVideo )
		{
			m_pVideo->Shutdown(); // make sure we're not currently running
			SetNextThink( gpGlobals->curtime + m_pVideo->GetStartDelay(), INTRO_STARTVIDEO );
		}

		if ( m_pModel )
		{
			m_pModel->SetPanelDirty();
		}
	}
	else
	{
		Shutdown();

		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::OnIntroFinished( void )
{
	float flTime = gpGlobals->curtime;

	if ( m_pModel && m_pModel->SetSequence( "UpSlow" ) )
	{	
		// wait for the model sequence to finish before going to the next menu
		flTime = gpGlobals->curtime + m_pVideo->GetEndDelay();
	}

	Shutdown();

	SetNextThink( flTime, INTRO_CONTINUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::OnCommand( const char *command )
{
	if ( !Q_strcmp( command, "back" ) )
	{
		float flTime = gpGlobals->curtime;

		Shutdown();

		// try to play the screenup sequence
		if ( m_pModel && m_pModel->SetSequence( "Up" ) )
		{
			flTime = gpGlobals->curtime + 0.35f;
		}

		// wait for the model sequence to finish before going back to the mapinfo menu
		SetNextThink( flTime, INTRO_BACK );
	}
	else if ( !Q_strcmp( command, "skip" ) )
	{
		Shutdown();

		// continue right now
		SetNextThink( gpGlobals->curtime, INTRO_CONTINUE );
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::OnKeyCodePressed( KeyCode code )
{
	if ( code == KEY_XBUTTON_A )
	{
		OnCommand( "skip" );
	}
	else if ( code == KEY_XBUTTON_B )
	{
		OnCommand( "back" );
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTDCIntroMenu::Shutdown( void )
{
	if ( m_pVideo )
	{
		m_pVideo->Shutdown();
	}

	if ( m_pCaptionLabel && m_pCaptionLabel->IsVisible() )
	{
		m_pCaptionLabel->SetVisible( false );
	}

	m_iCurrentCaption = 0;
	m_flVideoStartTime = 0;
}

const char *COM_GetModDirectory();

