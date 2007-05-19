/*
 $Id: area.h,v 1.1 2007/05/19 07:42:08 ksterker Exp $
 
 Copyright (C) 2002 Alexandre Courbot <alexandrecourbot@linuxgames.com>
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
 * @file   world/area.h
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Declares the area class.
 * 
 * 
 */



#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <vector>

#include "world/character_with_gfx.h"
#include "world/object_with_gfx.h"

namespace world
{
    /**
     * A cell in the world grid.
     */
    class square_info : public coordinates
    {
    public:
        s_int32 zground;
        
        placeable * obj;
        
        square_info (coordinates & pos);
        
        bool operator < (const square_info & mi) const;
        
        bool operator == (const square_info & mi) const
        {
            // We call coordinates::operator == here
            return ((coordinates)*this == (coordinates)mi && 
                    obj == mi.obj);
        }
    }; 
    
    class square
    {
    private:
        std::vector <square_info> objects; 
        
    public:
        typedef std::vector <square_info>::iterator iterator;
        
        iterator begin ()
        {
            return objects.begin (); 
        }
        
        iterator end ()
        {
            return objects.end (); 
        }
        
        bool add (placeable * obj, coordinates & pos); 
        bool add (moving * obj);
        bool remove (placeable * obj, coordinates & pos); 
        bool exist (placeable * obj, coordinates & pos); 
        
        square_info * project_moving(moving & obj)
        {
            //        placeable_area * movstat = obj.current_state();
            
            square::iterator it = begin();
            while (it != end())
            {
                placeable_area * objstat = it->obj->current_state();
                if (obj.z() + obj.climb_capability() > it->z() + objstat->zsize) break;
                ++it;
            }
            
            if (it == end()) return NULL;
            return NULL;
        }
    };
    
    /**
     * The plane of existance.
     */
    class area
    {
    private:
        template<class T, class T_gfx>
        class thing_manager
        {
        protected:
            std::vector<T *> objects;
            
            bool handle_gfx;
            
        public:
            thing_manager()
            {
                handle_gfx = true;
            }
            
            ~thing_manager()
            {
                clear();
            }
            
            void clear()
            {
                // FIXME: doesn't a virtual destructor take care of deleting the proper object already???
                if(handle_gfx)
                    for (typename std::vector<T *>::iterator i = objects.begin();
                         i != objects.end(); i++)
                        // FIXME: check whether it works or not.
                        delete ((T_gfx*)*i);
                else
                    for (typename std::vector<T *>::iterator i = objects.begin();
                         i != objects.end(); i++)
                        delete *i;
                
                objects.clear();
            }
            
            bool update()
            {
                if (handle_gfx) return update_gfx();
                else return update_nogfx();
            }
            
            bool update_nogfx()
            {
                for (typename std::vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                    (*i)->update();
                
                return true;
            }
            
            bool update_gfx()
            {
                for (typename std::vector<T *>::iterator i = objects.begin();
                     i != objects.end(); i++)
                {
                    (*i)->update();
                    ((T_gfx *)(*i))->placeable_gfx::update();
                }
                return true;
            }
            
            T * add(area & lm)
            {
                T * obj;
                if (handle_gfx) obj = new T_gfx(lm);
                else obj = new T(lm);
                
                objects.push_back(obj);
                
                return (obj);
            }
            
            
            T * operator [] (const int & index)
            {
                return objects[index];
            }
        };
        
        /**
         * Objects that are placed on this map.
         * 
         */
        thing_manager<object, object_with_gfx> objects;
        
        /**
         * Pointers to the characters that are on this
         * map.
         * 
         */
        thing_manager<character, character_with_gfx> characters;
        
        std::vector <std::vector <square> > Grid;
        
        bool put (placeable * obj, coordinates & pos); 
        bool put (moving * obj); 
        bool remove (placeable * obj, coordinates & pos); 
        bool remove (moving * obj); 
        
    public:
        area ()
        {
        }
        
        ~area ();
        
        void clear();
        
        u_int16 length () const
        {
            return Grid.size ();
        }
        
        u_int16 height () const
        {
            if (Grid.size ()) return Grid[0].size ();
            else return 0; 
        }
        
        void resize (const u_int16 nx, const u_int16 ny);
        
        square * get (const u_int16 x, const u_int16 y); 
        
        
        void update();
        
        /** 
         * Adds a object to this map.
         * @warning Ownership of the object will be given
         * to the map, which will delete it. DO NOT use the object
         * separately after having called this method!
         * 
         * @todo this function should load objects from files, being given
         * a object file name. That way, there won't be any ownership problem
         * anymore.
         *
         * @param mobj object to add to this map.
         * 
         * @return index of the object in case of success, \e -1 otherwise.
         */
        object * add_object();
        
        /** 
         * Tell a map to handle a character. The map will take
         * ownership of the character.
         * 
         * @warning Ownership of the character will be given
         * to the map, which will delete it. DO NOT use the character
         * separately after having called this method!
         * 
         * @todo this function should load objects from files, being given
         * a character file name. That way, there won't be any ownership problem
         * anymore.
         * @param mchar character to handle.
         * 
         * @return \e true in case of success, \e false otherwise.
         */
        character * add_character();
        
        bool put_object(u_int32 index, coordinates & pos);
        
        void output_occupation();
        
        friend class moving;
    }; 
}

#endif // WORLD_MAP_H
