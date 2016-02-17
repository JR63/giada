/* ---------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * recorder
 *
 * ---------------------------------------------------------------------
 *
 * Copyright (C) 2010-2016 Giovanni A. Zuliani | Monocasual
 *
 * This file is part of Giada - Your Hardcore Loopmachine.
 *
 * Giada - Your Hardcore Loopmachine is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Giada - Your Hardcore Loopmachine is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Giada - Your Hardcore Loopmachine. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * ------------------------------------------------------------------ */

#ifndef RECORDER_H
#define RECORDER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../utils/utils.h"
#include "const.h"
#include "mixer.h"

#ifdef WITH_VST

/* before including aeffetx(x).h we must define __cdecl, otherwise VST
 * headers can't be compiled correctly. In windows __cdecl is already
 * defined. */

	#ifdef __GNUC__
		#ifndef _WIN32
			#define __cdecl
		#endif
	#endif
	#include "../deps/vst/aeffectx.h"
#endif


using std::vector;


/*
 * [global0]-->[vector<_action*>0]-->[a0][a1][a2]				0[frames1]
 * [global1]-->[vector<_action*>1]-->[a0][a1][a2]				1[frames2]
 * [global2]-->[vector<_action*>2]-->[a0][a1][a2]				2[frames3]
 * [global3]-->[vector<_action*>3]-->[a0][a1][a2]				3[frames4]
 * */

namespace recorder {

/* action
 * struct containing fields to describe an atomic action. Note from
 * VST sdk: parameter values, like all VST parameters, are declared as
 * floats with an inclusive range of 0.0 to 1.0 (fValue). */

struct action {
	int      chan;    // channel index, i.e. Channel->index
 	int      type;
	int      frame;   // redundant info, used by helper functions
	float    fValue;  // used only for envelopes (volumes, vst params).
	uint32_t iValue;  // used only for MIDI events
};

/* composite
 * a group of two actions (keypress+keyrel, muteon+muteoff) used during
 * the overdub process */

struct composite {
	action a1;
	action a2;
};

extern vector<int>  frames;					      // frame counter (sentinel) frames.size == global.size
extern vector< vector<action*> > global;	// container of containers of actions
extern vector<action*>  actions;				    // container of actions

extern bool active;
extern bool sortedActions;                  // are actions sorted via sortActions()?

/* init
 * everything starts from here. */

void init();

/* chanHasActions
 * Check if the channel has at least one action recorded. If false, sets
 * ch->hasActions = false. Used after an action deletion. */

void chanHasActions(int chan);

/* canRec
 * can a channel rec an action? Call this one BEFORE rec(). */

bool canRec(Channel *ch);

/* rec
 * record an action. */

void rec(int chan, int action, int frame, uint32_t iValue=0, float fValue=0.0f);

/* clearChan
 * clear all actions from a channel. */

void clearChan(int chan);

/* clearAction
 * clear the 'action' action type from a channel. */

void clearAction(int chan, char action);

/* deleteAction
 * delete ONE action. Useful in the action editor. 'type' can be a mask. */

void deleteAction(int chan, int frame, char type, bool checkValues, uint32_t iValue=0, float fValue=0.0);

/* deleteActions
 * delete A RANGE of actions from frame_a to frame_b in channel 'chan'.
 * 'type' can be a bitmask. Exclusive range (frame_a, frame_b). */

void deleteActions(int chan, int frame_a, int frame_b, char type);

/* clearAll
 * delete everything. */

void clearAll();

/* optimize
 * clear frames without actions. */

void optimize();

/* sortActions
 * sorts actions by frame, asc mode. */

void sortActions();

/* updateBpm
 * reassign frames by calculating the new bpm value. */

void updateBpm(float oldval, float newval, int oldquanto);

/* updateSamplerate
 * reassign frames taking in account the samplerate. If f_system ==
 * f_patch nothing changes, otherwise the conversion is mandatory. */

void updateSamplerate(int systemRate, int patchRate);

void expand(int old_fpb, int new_fpb);
void shrink(int new_fpb);

/* getNextAction
 * return the nearest action in chan 'chan' of type 'action' starting
 * from 'frame'. Action can be a bitmask. If iValue != -1 search for
 * next action with iValue == iValue: useful for MIDI key_release. */

int getNextAction(int chan, char action, int frame, struct action **out, uint32_t iValue=0);

/* getAction
 * return a pointer to action in chan 'chan' of type 'action' at frame
 * 'frame'. */

int getAction(int chan, char action, int frame, struct action **out);

/* start/endOverdub */

void startOverdub(int chan, char action, int frame);
void stopOverdub(int frame);

/* print
 * debug of the frame stack. */

void print();

}  // namespace

#endif
