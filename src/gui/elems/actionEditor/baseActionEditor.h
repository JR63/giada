/* -----------------------------------------------------------------------------
 *
 * Giada - Your Hardcore Loopmachine
 *
 * ge_actionWidget
 *
 * parent class of any tool inside the action editor.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2017 Giovanni A. Zuliani | Monocasual
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
 * -------------------------------------------------------------------------- */


#ifndef GE_BASE_ACTION_EDITOR_H
#define GE_BASE_ACTION_EDITOR_H


#include <FL/Fl_Group.H>


class gdActionEditor;


class geBaseActionEditor : public Fl_Group
{
protected:

	gdActionEditor *pParent;

  void baseDraw(bool clear=true);

public:

	virtual void updateActions() = 0;

	geBaseActionEditor(int x, int y, int w, int h, gdActionEditor *pParent);
	~geBaseActionEditor();
};

#endif
