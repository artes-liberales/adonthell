/*
   $Id: surface_sdlgl.h,v 1.7 2009/03/06 22:53:30 ksterker Exp $

   Copyright (C) 2003   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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

#ifndef SURFACE_SDL_H_
#define SURFACE_SDL_H_


#include "gfx/surface_ext.h"
#include "SDL.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


namespace gfx
{
    class surface_sdlgl : public surface_ext
    {
    public:
        surface_sdlgl ();

        virtual ~surface_sdlgl (); 

        virtual void set_mask (bool m); 

        virtual void set_alpha (const u_int8 & surface_alpha, const bool & alpha_channel = false);

        virtual void draw (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy, u_int16 sl,
                           u_int16 sh, const drawing_area * da_opt = NULL,
                           surface * target = NULL) const;

        virtual void fillrect (s_int16 x, s_int16 y, u_int16 l, u_int16 h,
                               u_int32 col, drawing_area * da_opt = NULL);

        virtual void draw_line (const s_int16 sx, const s_int16 sy, const s_int16 ex, const s_int16 ey, 
                                const u_int32 col, const drawing_area * da_opt = NULL); 

        virtual u_int32 map_color(const u_int8 & r, const u_int8 & g, const u_int8 & b, const u_int8 & a = 255) const;
        virtual void unmap_color(u_int32 col, u_int8 & r, u_int8 & g, u_int8 & b, u_int8 & a) const;
        virtual void lock () const; 
        virtual void unlock () const;
        virtual void put_pix (u_int16 x, u_int16 y, u_int32 col); 
        virtual u_int32 get_pix (u_int16 x, u_int16 y) const; 

        virtual surface& operator = (const surface& src); 

        virtual void resize (u_int16 l, u_int16 h);

        virtual void clear (); 

    protected:
        void set_data (void * data, u_int16 l, u_int16 h,
                       u_int8 bytes_per_pixel = BYTES_PER_PIXEL,
                       u_int32 red_mask = R_MASK, u_int32 green_mask = G_MASK,
                       u_int32 blue_mask = B_MASK, u_int32 alpha_mask = 0);

        void * get_data (u_int8 bytes_per_pixel,
                         u_int32 red_mask, u_int32 green_mask,
                         u_int32 blue_mask, u_int32 alpha_mask) const;

        /// Image texture
        GLuint texture;
        
        /// Raw data stored within 'texture', or NULL
        void *texture_data;

    private: 

        /// Has the mask setting changed?
        mutable bool mask_changed; 

        /// Used internally for blitting operations with drawing_areas.
        void setup_rects (u_int16 x, u_int16 y, const drawing_area * draw_to) const
        {
            setup_rects (x, y, 0, 0, length (), height (), draw_to); 
        }

        /// Used internally for blitting operations with drawing_areas.
        void setup_rects (s_int16 x, s_int16 y, s_int16 sx, s_int16 sy,
                          u_int16 sl, u_int16 sh, const drawing_area * draw_to) const; 
    };
}

#endif
