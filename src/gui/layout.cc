/*
   $Id: layout.cc,v 1.4 2004/12/21 22:03:17 jol Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "gui/container.h"
#include "gui/layout.h"

using namespace gui;


layout::layout ()
{
  m_container = NULL;
  m_length = 0;
  m_height = 0;
}

bool layout::update ()
{
  return true;
}

void layout::set_container (container * ct)
{
  m_container = ct;
  if (ct) update ();
}
