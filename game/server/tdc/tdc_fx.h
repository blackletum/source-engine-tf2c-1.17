//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======
//
//  
//
//=============================================================================

#ifndef TDC_FX_H
#define TDC_FX_H
#ifdef _WIN32
#pragma once
#endif

#include "particle_parse.h"
#include "tdc_shareddefs.h"

void TE_TFExplosion( IRecipientFilter &filter, float flDelay, const Vector &vecOrigin, const Vector &vecNormal, ETDCWeaponID iWeaponID, int nEntIndex, CBasePlayer *pPlayer = NULL, int iTeam = TEAM_UNASSIGNED, bool bCrit = false, ETDCDmgCustom iCustomDmg = TDC_DMG_CUSTOM_NONE );

void TE_TFParticleEffect( IRecipientFilter &filter, float flDelay, const char *pszParticleName, ParticleAttachment_t iAttachType, CBaseEntity *pEntity, const char *pszAttachmentName, bool bResetAllParticlesOnEntity = false );
void TE_TFParticleEffect( IRecipientFilter &filter, float flDelay, const char *pszParticleName, ParticleAttachment_t iAttachType, CBaseEntity *pEntity = NULL, int iAttachmentPoint = -1, bool bResetAllParticlesOnEntity = false );
void TE_TFParticleEffect( IRecipientFilter &filter, float flDelay, const char *pszParticleName, Vector vecOrigin, QAngle vecAngles, CBaseEntity *pEntity = NULL, int iAttachType = PATTACH_CUSTOMORIGIN );
void TE_TFParticleEffect( IRecipientFilter &filter, float flDelay, const char *pszParticleName, Vector vecOrigin, Vector vecStart, QAngle vecAngles, CBaseEntity *pEntity = NULL );
void TE_TFParticleEffect( IRecipientFilter &filter, float flDelay, int iEffectIndex, Vector vecOrigin, Vector vecStart, QAngle vecAngles, CBaseEntity *pEntity = NULL );

#endif	// TDC_FX_H
