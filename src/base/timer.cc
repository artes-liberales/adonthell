/*
   $Id: timer.cc,v 1.2 2003/11/22 09:35:21 ksterker Exp $

   Copyright (C) 2003  Alexandre Courbot <alexandrecourbot@linuxgames.com>
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


#include "base/timer.h"

namespace base
{
    timer::timer() : Slice(100), Lasttime(0), Frames_missed(0)
    {
        gettimeofday(&initial_time, NULL);
    }

    void timer::set_slice(unsigned long int sl)
    {
        Slice = sl;
    }

    unsigned long int timer::current_time() const
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return convert_timeval(tv);
    }

    void timer::sleep(unsigned long int msecs) const
    {
        unsigned char err;
        struct timespec req, rem;
        rem.tv_sec = msecs / 1000;
        rem.tv_nsec = (msecs % 1000) * 1000000;

        do
        {
            req = rem;
            err = nanosleep(&req, &rem);
        } while (err && (errno == EINTR));

        
    }

    void timer::update()
    {
        unsigned long int curtime = current_time();

        while (curtime - Lasttime < Slice)
        {
            //this->sleep(1);
            curtime = current_time();
        }
        
        Frames_missed = (curtime - Lasttime) / Slice - 1;
        Lasttime = curtime - (curtime - Lasttime) % Slice;
    }
}