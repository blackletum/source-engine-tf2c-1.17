//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: A blood spray effect to expose successful hits.
//
//=============================================================================//

#include "cbase.h"
#include "clienteffectprecachesystem.h"
#include "fx_sparks.h"
#include "iefx.h"
#include "c_te_effect_dispatch.h"
#include "particles_ez.h"
#include "decals.h"
#include "engine/IEngineSound.h"
#include "fx_quad.h"
#include "engine/ivdebugoverlay.h"
#include "shareddefs.h"
#include "fx_blood.h"
#include "view.h"
#include "c_tdc_player.h"
#include "debugoverlay_shared.h"
#include "tdc_gamerules.h"
#include "c_basetempentity.h"
#include "tier0/vprof.h"

//-----------------------------------------------------------------------------
// Purpose: Intercepts the blood spray message.
//-----------------------------------------------------------------------------
void TFBloodSprayCallback( Vector vecOrigin, Vector vecNormal, ETDCDmgCustom iDamageCustom, ClientEntityHandle_t hEntity )
{
	QAngle	vecAngles;
	VectorAngles( -vecNormal, vecAngles );

	C_TDCPlayer *pPlayer = ToTDCPlayer( ClientEntityList().GetBaseEntityFromHandle( hEntity ) );
	if ( !pPlayer || pPlayer->IsDormant() )
		return;

	// determine if the bleeding player is underwater
	bool bUnderwater = ( pPlayer->GetWaterLevel() == WL_Eyes );
	bool bHeadshot = ( !pPlayer->IsAlive() && iDamageCustom == TDC_DMG_CUSTOM_HEADSHOT );
	const char *pszEffect = NULL;
	 
	if ( bUnderwater )
	{
		pszEffect = "water_blood_impact_red_01";
	}
	else if ( bHeadshot )
	{
		pszEffect = "blood_headshot";
	}
	else
	{
		pszEffect = "blood_impact_red_01";
	}

	DispatchParticleEffect( pszEffect, vecOrigin, vecAngles, pPlayer );

	// if underwater, don't add additional spray
	if ( bUnderwater || bHeadshot )
		return;

	// Now throw out a spray away from the view
	// Get the distance to the view
	float flDistance = (vecOrigin - MainViewOrigin()).Length();
	float flLODDistance = 0.25 * (flDistance / 512);

	Vector right, up;
	if (vecNormal != Vector(0, 0, 1) )
	{
		right = vecNormal.Cross( Vector(0, 0, 1) );
		up = right.Cross( vecNormal );
	}
	else
	{
		right = Vector(0, 0, 1);
		up = right.Cross( vecNormal );
	}

	// If the normal's too close to being along the view, push it out
	Vector vecForward, vecRight;
	AngleVectors( MainViewAngles(), &vecForward, &vecRight, NULL );
	float flDot = DotProduct( vecNormal, vecForward );
	if ( fabs(flDot) > 0.5 )
	{
		float flPush = random->RandomFloat(0.5, 1.5) + flLODDistance;
		float flRightDot = DotProduct( vecNormal, vecRight );

		// If we're up close, randomly move it around. If we're at a distance, always push it to the side
		// Up close, this can move it back towards the view, but the random chance still looks better
		if ( ( flDistance >= 512 && flRightDot > 0 ) || ( flDistance < 512 && RandomFloat(0,1) > 0.5 ) )
		{
			// Turn it to the right
			vecNormal += (vecRight * flPush);
		}
		else
		{
			// Turn it to the left
			vecNormal -= (vecRight * flPush);
		}
	}

	VectorAngles( vecNormal, vecAngles );

	if ( flDistance < 400 )
	{
		DispatchParticleEffect( "blood_spray_red_01", vecOrigin, vecAngles, pPlayer );
	}
	else
	{
		DispatchParticleEffect( "blood_spray_red_01_far", vecOrigin, vecAngles, pPlayer );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_TETDCBlood : public C_BaseTempEntity
{
public:
	DECLARE_CLASS( C_TETDCBlood, C_BaseTempEntity );

	DECLARE_CLIENTCLASS();

	C_TETDCBlood( void );

	virtual void	PostDataUpdate( DataUpdateType_t updateType );

public:
	Vector		m_vecOrigin;
	Vector		m_vecNormal;
	ClientEntityHandle_t m_hEntity;
	ETDCDmgCustom	m_iDamageCustom;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_TETDCBlood::C_TETDCBlood( void )
{
	m_vecOrigin.Init();
	m_vecNormal.Init();
	m_hEntity = INVALID_EHANDLE_INDEX;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_TETDCBlood::PostDataUpdate( DataUpdateType_t updateType )
{
	VPROF( "C_TETDCBlood::PostDataUpdate" );

	TFBloodSprayCallback( m_vecOrigin, m_vecNormal, m_iDamageCustom, m_hEntity );
}

static void RecvProxy_BloodEntIndex( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	int nEntIndex = pData->m_Value.m_Int;
	((C_TETDCBlood*)pStruct)->m_hEntity = (nEntIndex < 0) ? INVALID_EHANDLE_INDEX : ClientEntityList().EntIndexToHandle( nEntIndex );
}

IMPLEMENT_CLIENTCLASS_EVENT_DT( C_TETDCBlood, DT_TETDCBlood, CTETDCBlood )
	RecvPropFloat( RECVINFO( m_vecOrigin[0] ) ),
	RecvPropFloat( RECVINFO( m_vecOrigin[1] ) ),
	RecvPropFloat( RECVINFO( m_vecOrigin[2] ) ),
	RecvPropVector( RECVINFO( m_vecNormal ) ),
	RecvPropInt( "entindex", 0, SIZEOF_IGNORE, 0, RecvProxy_BloodEntIndex ),
	RecvPropInt( RECVINFO( m_iDamageCustom ) ),
END_RECV_TABLE()



