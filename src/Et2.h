//==============================================================================
//
// Title:		Et2.h
// Purpose:		A short description of the interface.
//
// Created on:	12/6/2024 at 1:01:13 AM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __Et2_H__
#define __Et2_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions
#define  WAVE_PANEL                       2       /* callback function: OnPanelCallback2 */
#define  WAVE_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  WAVE_PANEL_GRAPH                 3       /* control type: graph, callback function: GraphFreq */
#define  WAVE_PANEL_NUMERIC               4       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_TIMER                 5       /* control type: timer, callback function: FreqTmr */
int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Et2_H__ */
