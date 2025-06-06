
//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "tdc_steamstats.h"
#include "tdc_hud_statpanel.h"
#include "achievementmgr.h"
#include "engine/imatchmaking.h"
#include "ipresence.h"
#include "tdc_shareddefs.h"
#include "tdc_gamestats_shared.h"

struct StatMap_t
{
	const char *pszName;
	int	iStat;
	int iLiveStat;
};

// subset of stats which we store in Steam
StatMap_t g_SteamStats[] = { 
		{ "iNumberOfKills", TFSTAT_KILLS, PROPERTY_KILLS },
		{ "iDamageDealt", TFSTAT_DAMAGE, PROPERTY_DAMAGE_DEALT },
		{ "iPlayTime", TFSTAT_PLAYTIME, PROPERTY_PLAY_TIME },
		{ "iPointCaptures", TFSTAT_CAPTURES, PROPERTY_POINT_CAPTURES },
		{ "iPointDefenses", TFSTAT_DEFENSES, PROPERTY_POINT_DEFENSES },
		{ "iDominations", TFSTAT_DOMINATIONS, PROPERTY_DOMINATIONS },
		{ "iRevenge", TFSTAT_REVENGE, PROPERTY_REVENGE },
		{ "iPointsScored", TFSTAT_POINTSSCORED, PROPERTY_POINTS_SCORED },
		{ "iBuildingsDestroyed", TFSTAT_BUILDINGSDESTROYED, PROPERTY_BUILDINGS_DESTROYED },
		{ "iHeadshots", TFSTAT_HEADSHOTS, PROPERTY_HEADSHOTS },
		{ "iHealthPointsHealed", TFSTAT_HEALING, PROPERTY_HEALTH_POINTS_HEALED },
		{ "iNumInvulnerable", TFSTAT_INVULNS, PROPERTY_INVULNS },
		{ "iKillAssists", TFSTAT_KILLASSISTS, PROPERTY_KILL_ASSISTS },
		{ "iBackstabs", TFSTAT_BACKSTABS, PROPERTY_BACKSTABS },
		{ "iHealthPointsLeached", TFSTAT_HEALTHLEACHED, PROPERTY_HEALTH_POINTS_LEACHED },
		{ "iBuildingsBuilt", TFSTAT_BUILDINGSBUILT, PROPERTY_BUILDINGS_BUILT },
		{ "iSentryKills", TFSTAT_MAXSENTRYKILLS, PROPERTY_SENTRY_KILLS },
		{ "iNumTeleports", TFSTAT_TELEPORTS, PROPERTY_TELEPORTS } };


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CTDCSteamStats::CTDCSteamStats() 
{
	m_flTimeNextForceUpload = 0;
}

//-----------------------------------------------------------------------------
// Purpose: called at init time after all systems are init'd.  We have to
//			do this in PostInit because the Steam app ID is not available earlier
//-----------------------------------------------------------------------------
void CTDCSteamStats::PostInit()
{
	SetNextForceUploadTime();
	ListenForGameEvent( "player_stats_updated" );
	ListenForGameEvent( "user_data_downloaded" );
}

//-----------------------------------------------------------------------------
// Purpose: called at level shutdown
//-----------------------------------------------------------------------------
void CTDCSteamStats::LevelShutdownPreEntity()
{
	// upload user stats to Steam on every map change
	UploadStats();
}

//-----------------------------------------------------------------------------
// Purpose: called when the stats have changed in-game
//-----------------------------------------------------------------------------
void CTDCSteamStats::FireGameEvent( IGameEvent *event )
{
	const char *pEventName = event->GetName();
	if ( 0 == Q_strcmp( pEventName, "player_stats_updated" ) )
	{
		bool bForceUpload = event->GetBool( "forceupload" );

		// if we haven't uploaded stats in a long time, upload them 
		if ( ( gpGlobals->curtime >= m_flTimeNextForceUpload ) || bForceUpload )
		{
			UploadStats();
		}
	}
	else if ( 0 == Q_strcmp( pEventName, "user_data_downloaded" ) )
	{
		/*
		Assert( SteamUserStats() );
		if ( !SteamUserStats() )
			return; 
		CTDCStatPanel *pStatPanel = GET_HUDELEMENT( CTDCStatPanel );
		Assert( pStatPanel );

		CGameID gameID( engine->GetAppID() );

		for ( int iClass = TDC_FIRST_NORMAL_CLASS; iClass <= TDC_LAST_NORMAL_CLASS; iClass++ )
		{
			ClassStats_t &classStats = CTDCStatPanel::GetClassStats( iClass );
			for ( int iStat = 0; iStat < ARRAYSIZE( g_SteamStats ); iStat++ )
			{
				char szStatName[256];
				int iData;

				V_sprintf_safe( szStatName, "%s.accum.%s", g_aPlayerClassNames_NonLocalized[iClass], g_SteamStats[iStat].pszName );
				if ( SteamUserStats()->GetStat( gameID, szStatName, &iData ) )
				{
					if ( pStatPanel->IsLocalFileTrusted() )
					{
						// if local stats file is trusted, then use the higher of the current value (from local file) or Steam.  Handles case where we previously failed to write latest data to Steam.
						classStats.accumulated.m_iStat[g_SteamStats[iStat].iStat] = Max( iData, classStats.accumulated.m_iStat[g_SteamStats[iStat].iStat] );
					}
					else
					{
						// local file is not trusted, just use Steam's value
						classStats.accumulated.m_iStat[g_SteamStats[iStat].iStat] = iData;
					}					
				}
				V_sprintf_safe( szStatName, "%s.max.%s", g_aPlayerClassNames_NonLocalized[iClass], g_SteamStats[iStat].pszName );
				if ( SteamUserStats()->GetStat( gameID, szStatName, &iData ) )
				{
					if ( pStatPanel->IsLocalFileTrusted() )
					{
						// if local stats file is trusted, then use the higher of the current value (from local file) or Steam.  Handles case where we previously failed to write latest data to Steam.
						classStats.max.m_iStat[g_SteamStats[iStat].iStat] = Max( iData, classStats.max.m_iStat[g_SteamStats[iStat].iStat] );
					}
					else
					{
						// local file is not trusted, just use Steam's value
						classStats.max.m_iStat[g_SteamStats[iStat].iStat] = iData;
					}
				}			
			}
		}
*/
		IGameEvent * event = gameeventmanager->CreateEvent( "player_stats_updated" );
		if ( event )
		{
			event->SetBool( "forceupload", false );
			gameeventmanager->FireEventClientSide( event );
		}

//		pStatPanel->SetStatsChanged( true );
//		pStatPanel->UpdateStatSummaryPanel();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Uploads stats for current Steam user to Steam
//-----------------------------------------------------------------------------
void CTDCSteamStats::UploadStats()
{
	/*
	if ( IsX360() )
	{
		ReportLiveStats();
		return;
	}

	// only upload if Steam is running
	if ( !SteamUserStats() )
		return; 

	CGameID gameID( engine->GetAppID() );
	
	for ( int iClass = TDC_FIRST_NORMAL_CLASS; iClass <= TDC_LAST_NORMAL_CLASS; iClass++ )
	{
		ClassStats_t &classStats = CTDCStatPanel::GetClassStats( iClass );
		for ( int iStat = 0; iStat < ARRAYSIZE( g_SteamStats ); iStat++ )
		{
			char szStatName[256];

			// set the stats locally in Steam client
			V_sprintf_safe( szStatName, "%s.accum.%s", g_aPlayerClassNames_NonLocalized[iClass], g_SteamStats[iStat].pszName );
			SteamUserStats()->SetStat( gameID, szStatName, classStats.accumulated.m_iStat[g_SteamStats[iStat].iStat] );

			V_sprintf_safe( szStatName, "%s.max.%s", g_aPlayerClassNames_NonLocalized[iClass], g_SteamStats[iStat].pszName );
			SteamUserStats()->SetStat( gameID, szStatName, classStats.max.m_iStat[g_SteamStats[iStat].iStat] );
		}
	}

	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( pAchievementMgr )
	{		
		pAchievementMgr->UploadUserData();
	}

	SetNextForceUploadTime();*/
}

//-----------------------------------------------------------------------------
// Purpose: Accumulate player stats and send them to matchmaking for reporting to Live
//-----------------------------------------------------------------------------
void CTDCSteamStats::ReportLiveStats()
{
	int statsTotals[ARRAYSIZE( g_SteamStats )];
	Q_memset( &statsTotals, 0, sizeof( statsTotals ) );

	for ( int iClass = TDC_FIRST_NORMAL_CLASS; iClass < TDC_CLASS_COUNT_ALL; iClass++ )
	{
		ClassStats_t &classStats = CTDCStatPanel::GetClassStats( iClass );
		for ( int iStat = 0; iStat < ARRAYSIZE( g_SteamStats ); iStat++ )
		{
			statsTotals[iStat] = Max( statsTotals[iStat], classStats.max.m_iStat[g_SteamStats[iStat].iStat] );
		}
	}

	// send the stats to matchmaking
	for ( int i = 0; i < ARRAYSIZE( g_SteamStats ); ++i )
	{
		// Points scored is looked up by the stats reporting function
		if ( g_SteamStats[i].iLiveStat == PROPERTY_POINTS_SCORED )
			continue;

		presence->SetStat( g_SteamStats[i].iLiveStat, statsTotals[i], XUSER_DATA_TYPE_INT32 );
	}
		
	presence->UploadStats();
}

//-----------------------------------------------------------------------------
// Purpose: sets the next time to force a stats upload at
//-----------------------------------------------------------------------------
void CTDCSteamStats::SetNextForceUploadTime()
{
	// pick a time a while from now (an hour +/- 15 mins) to upload stats if we haven't gotten a map change by then
	m_flTimeNextForceUpload = gpGlobals->curtime + ( 60 * RandomInt( 45, 75 ) );
}

CTDCSteamStats g_TFSteamStats;