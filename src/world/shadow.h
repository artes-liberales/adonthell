/*
 $Id: shadow.h,v 1.5 2009/02/11 17:17:00 rians Exp $
 
 Copyright (C) 2009 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   world/shadow.h
 * @author Kai Sterker <kai.sterker@gmail.com>
 * 
 * @brief  Declares the shadow class.
 * 
 */

#ifndef WORLD_SHADOW_H
#define WORLD_SHADOW_H

#include <list>
#include <vector>
#include "gfx/surface.h"
#include "world/coordinates.h"

namespace world
{

class chunk_info;
    
/**
 * Represents a character shadow that will be drawn on tiles
 * below the character. It requires an (optionally masked) image
 * with per-surface alpha (so no alpha channel) that will 
 * represent the shadow. Depending on the distance between
 * character and ground, the opacity of the shadow will be
 * changed.
 */
class shadow
{
public:
    /**
     * Create a new shadow object.
     * @param shadow filename of the shadow image.
     * @param pos position of the object that casts the shadow.
     * @param offset offset of the object that casts the shadpw.
     */
    shadow (const std::string & shadow, const coordinates * pos, const vector3<s_int32> & offset);

    /**
     * Destroy shadow object.
     */
    ~shadow ();
    
    /**
     * Clear shadow object for rendering next frame.
     * @param ox projected x position offset at end of move.
     * @param ox projected y position offset at end of move.
     */
    void reset (const s_int16 & ox = 0, const s_int16 & oy = 0);
    
    /**
     * Update offset of shadow.
     * @param offset the new offset.
     */
    void set_offset (const vector3<s_int32> & offset) { Offset = offset; }
    
    /**
     * Cast (part) of shadow onto the given target.
     * @param ci placeable to draw shadow on.
     */
    void cast_on (chunk_info* ci);
    
protected:
    /**
     * Remove area a from area b and update #Remaining on the way.
     * @param a current area of shadow
     * @param b area to remove from shadow
     */
    void subtract_area (const gfx::drawing_area & a, const gfx::drawing_area & b);

    /// a list of shadow pieces
    typedef std::list<gfx::drawing_area> parts;
    
private:
    /// current position of shadow casting object
    const coordinates *Pos;
    /// offset of shadow casting object
    vector3<s_int32> Offset;
    /// image representing the shadow
    const gfx::surface *Shadow;
    
    /**
     * @name Runtime data
     */
    ///@{
    /// parts of the shadow not rendered
    shadow::parts Remaining;
    /// list of objects with a shadow on them
    std::vector<chunk_info*> TilesWithShadow;
    ///@}
};

}

#endif
