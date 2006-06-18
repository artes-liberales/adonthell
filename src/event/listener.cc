/*
   $Id: listener.cc,v 1.8 2006/06/18 19:25:53 ksterker Exp $

   Copyright (C) 2004/2005 Kai Sterker <kaisterker@linuxgames.com>
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

/**
 * @file   event/listener.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Declares the %listener class.
 * 
 */

#include "event/manager.h"
#include "event/listener.h"

using events::factory;
using events::listener;

// ctor
listener::listener (factory *f, event *e)
{
    Registered = false;
    Factory = f;
    Event = e;
    Paused = 0;
    Id = "";
}

// destructor
listener::~listener ()
{
    // automatically remove myself from the event manager
    if (Registered) manager::remove (this);
    // ... and from the factory
    if (Factory) Factory->remove (this);
    
    delete Event;
}

// disable the event temporarily
void listener::pause (const u_int16 & level)
{
    if (Paused == 0) manager::remove (this);
    Paused += level;
}

// resume a disabled event
void listener::resume ()
{
    if (Paused == 0)
    {
        fprintf (stderr, "*** warning: listener::resume: listener was not paused!\n");
        if (!Registered) manager::add (this);
    }
    else 
    {
        Paused--;
        if (Paused == 0) manager::add (this);
    }
}

// save the state of the script associated with the event
void listener::put_state (base::flat & out) const
{
    // save id and paused state
    out.put_string ("lid", Id);
    out.put_uint16 ("lps", Paused);

    // save attached event
    out.put_bool ("lev", Event != NULL);
    if (Event != NULL) Event->put_state (out);
}

// load the state of the script associated with the event 
bool listener::get_state (base::flat & in) 
{
    Id = in.get_string ("lid");
    Paused = in.get_uint16 ("lps");
    
    // load event structure
    if (in.get_bool ("lev") == true)
    {
        std::string type = in.get_string ("etp");
        
        // Instanciate an event of the given type
        Event = event_type::instanciate_event (type);
 
        // try to load it
        if (Event == NULL || Event->get_state (in) == false)
        {
            fprintf (stderr, "*** listener::get_state: could not load event of type '%s'!\n", type.c_str ());
            return false;
        }    
    }

    return in.success ();
}


