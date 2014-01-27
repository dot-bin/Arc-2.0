/* This file is part of ArcData.
 * 
 * ArcData is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * ArcData is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArcData; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 * Author: Stephen Lane-Walsh
 * 
 */

#ifndef ARC_DATA_FBX_PROPERTY_H
#define ARC_DATA_FBX_PROPERTY_H

#include <Arc/ManagedObject.h>

namespace Arc
{

class FBXProperty
	: public ManagedObject
{
public:

	virtual inline string getClassName( void ) const { return "Arc FBX Property"; }

private:

	string m_Type;

	string m_ID;

}; // class FBXProperty

} // namespace Arc

#endif // ARC_DATA_FBX_PROPERTY_H