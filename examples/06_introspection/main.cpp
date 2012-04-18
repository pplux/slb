#include <SLB/SLB.hpp>

int main(int, char**) {

	const char *lua_code = 
		"print('print SLB functions:')\n"
		"for k,_ in pairs(SLB) do print('SLB.'..k) end\n" 
		"print('---------------')\n";

	// Custom SLB::SCript, a simplification to use SLB
	SLB::Manager m;
	SLB::Script s(&m);
	s.doString(lua_code);

  const SLB::Manager::ClassMap &cls_map = m.getClassMap();

  // for each class
  for(SLB::Manager::ClassMap::const_iterator i = cls_map.begin();
      i != cls_map.end(); ++i) {
    // get the class
    const SLB::ClassInfo *cls = i->second.get();
    
    // Basic info -------------------------------------------------------------
    std::cout << "<class>" << std::endl;
    std::cout << "<name>" << cls->getName().c_str() << "</name>" << std::endl;
    std::cout << "<info>" << cls->getInfo().c_str() << "</info>" << std::endl;
    
    // Base Classes -----------------------------------------------------------
    const SLB::ClassInfo::BaseClassMap &base = cls->getBaseClasses();
    std::cout << "<baseClasses>" << std::endl;
    for(SLB::ClassInfo::BaseClassMap::const_iterator i = base.begin();
        i != base.end(); ++i) {
          std::cout << "<classRef>" << i->second->getName().c_str()
                    << "</classRef>" << std::endl;
    }
    std::cout << "</baseClasses>" << std::endl;
    
    // Content of the Class ---------------------------------------------------
    const SLB::Table::Elements &elements = cls->getElementsMap();
    for(SLB::Table::Elements::const_iterator i = elements.begin(); 
        i != elements.end(); ++i) {
      const SLB::Object *obj = i->second.get();
      const SLB::FuncCall *fc = SLB::slb_dynamic_cast<SLB::FuncCall>(obj);
      std::cout << "<element>" << std::endl;
      std::cout << "<name>" << i->first.c_str() <<"</name>" << std::endl;
      std::cout << "<typeInfo>"<< obj->typeInfo().name()
                << "</typeInfo>" <<std::endl;
      std::cout << "<info>" << obj->getInfo().c_str() << "</info>" << std::endl;
      if (fc) {
        // A function, let's check the arguments
        std::cout << "<function>" <<std::endl;
        std::cout << "<returns>" << fc->getReturnedType().name()
                  << "</returns>" << std::endl;
        for(size_t arg = 0; arg < fc->getNumArguments(); arg++) {
          std::cout << "<arg>" << std::endl;
          std::cout << "<type>" << fc->getArgType(arg).name()
                    << "</type>" << std::endl;
          std::cout << "<comment>" << fc->getArgComment(arg)
                    << "</comment>" << std::endl;
          std::cout << "</arg>" << std::endl;
        }
        std::cout << "</function>" <<std::endl;
      }
      std::cout << "</element>" << std::endl;
    }

    // End --------------------------------------------------------------------
    std::cout << "</class>" << std::endl;
  }
	

	return 0;
}
