/*
   $Id: factory.cc,v 1.12 2009/04/08 21:52:09 ksterker Exp $

   Copyright (C) 2000/2001/2002/2003/2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file   event/factory.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Implements the %event %factory class.
 * 
 */

#include "event/factory.h"
#include "event/manager.h"
#include "event/listener_cxx.h"
#include "event/listener_python.h"
#include <algorithm>

using std::vector;
using events::factory;
using events::listener;
using events::manager;

// constructor
factory::factory ()
{
    Paused = 0;
}

// destructor
factory::~factory ()
{
    clear (); 
}

// Unregisters and deletes all listeners.
void factory::clear () 
{
    Paused = 0;
    
    listener *li;
    while (!Listeners.empty ())
    {
        // deleting the listener will remove it from our list too
        li = Listeners.front ();
        delete li;
    }
}

// Add an event to the factory and register it with the event manager.
listener *factory::add (event* ev, int type)
{
    listener *li = NULL;
    
    // create listener according to desired type
    if (type == LISTENER_CXX) li = new events::listener_cxx (this, ev);
    else li = new events::listener_python (this, ev);
        
    Listeners.push_back (li);

    // if the factory is paused, also pause new events
    if (Paused) li->pause (Paused);

    // only register event if not paused
    else if (!li->is_paused ()) manager::add (li);
    
    // finally return the listener, so a callback can be attached
    return li;
}

// Remove a listener from the list
void factory::remove (listener *li)
{
    vector<listener*>::iterator i;

    // Search for the listener we want to remove
    i = find (Listeners.begin (), Listeners.end (), li);

    // found? -> get rid of it :)
    if (i != Listeners.end ()) Listeners.erase (i);
}

// retrieve listener by its id
listener *factory::get_listener (const string & id)
{
    vector<listener*>::iterator i;

    for (i = Listeners.begin (); i != Listeners.end (); i++)
         if ((*i)->id () == id) return *i;
    
    return NULL;   
}

// disable all events in the list
void factory::pause ()
{
    Paused++;
    for (vector<listener*>::iterator i = Listeners.begin (); i != Listeners.end (); i++)
        (*i)->pause ();
}

// enable all events in the list
void factory::resume ()
{
    Paused--;
    for (vector<listener*>::iterator i = Listeners.begin (); i != Listeners.end (); i++)
        (*i)->resume ();
}

// Save a factory to file
void factory::put_state (base::flat& file) const
{
    base::flat listeners;
    std::vector <listener *>::iterator i;

    file.put_uint16 ("fps", Paused);
    
    for (i = Listeners.begin (); i != Listeners.end (); i++)
        if (!(*i)->is_destroyed ())
            (*i)->put_state (listeners);

    file.put_flat ("fls", listeners);
}

// Loads a factory from file
bool factory::get_state (base::flat& file)
{
    void *value;
    listener *li;
    u_int32 size, type;
    base::flat::data_type data_type;
    
    Paused = file.get_uint16 ("fps");
    base::flat listeners = file.get_flat ("fls");

    // get registered listeners
    while ((data_type = listeners.next (&value, &size)) != -1) 
    {
        // get listener container
        if (data_type != base::flat::T_FLAT)
        {
            fprintf (stderr, "*** error: factory::get_state: expected list but got %s!\n", 
                     base::flat::name_for_type (data_type));
            return false;
        }
        base::flat state ((const char*) value, size);
        
        // get type of listener
        type = state.get_uint8 ("ltp");

        // create listener according to desired type
        if (type == LISTENER_CXX) li = new events::listener_cxx (this, NULL);
        else li = new events::listener_python (this, NULL);
        
        // get listener data
        if (li->get_state (state))
        {
            Listeners.push_back (li);
            if (!li->is_paused ()) manager::add (li);
        }
        else 
        {
            delete li;
            return false;
        }
    }
    
    return true;
}
