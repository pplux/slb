/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __SLB_STATEFUL_HYBRID__
#define __SLB_STATEFUL_HYBRID__

#include "Hybrid.hpp" 
#include "Script.hpp" 

namespace SLB {
	
	template<class T, class S = SLB::Script>
	class StatefulHybrid :
		public S, /* Requires to have a method "getState" and "close" */
		public Hybrid< T >
	{	
	public:
		StatefulHybrid(){ reAttach(); }
		virtual ~StatefulHybrid() { HybridBase::unAttach(); S::close(); }
	protected:
		void reAttach() { if (!HybridBase::isAttached()) HybridBase::attach( S::getState() ); }
	};

}

#endif
