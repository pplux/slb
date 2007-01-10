#ifndef __SLB_MANAGER__
#define __SLB_MANAGER__

#include "Object.hpp"
#include "ref_ptr.hpp"
#include "Export.hpp"
#include <typeinfo>
#include <map>

namespace SLB {

	class Class;

	class SLB_EXPORT Manager : public virtual Object
	{
	public:
		typedef std::map< const std::type_info*, ref_ptr<Class> > ClassMap;
		typedef std::map< const std::string, const std::type_info *> NameMap;

		static Manager &getInstance();
		static Manager *getInstancePtr();

		const Class *getClass(const std::type_info&) const;
		const Class *getClass(const std::string&) const;
		Class *getClass(const std::type_info&);
		Class *getClass(const std::string&);
		Class *getOrCreateClass(const std::type_info &);

	protected:

		Manager();
		virtual ~Manager();

		void pushImplementation(lua_State *L);
		void setName( const std::string&, const std::type_info*);
		void addClass( Class *c );

	private:
		Manager(const Manager&);
		Manager& operator=(const Manager&);

		static ref_ptr<Manager> _instance;
		ClassMap _classes;
		NameMap  _names;

		friend class Class;
	};
	
	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	inline Manager &Manager::getInstance()
	{
		return *getInstancePtr();
	}
}

#endif
