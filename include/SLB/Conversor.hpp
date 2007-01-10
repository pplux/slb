#ifndef __SLB_CONVERSOR__
#define __SLB_CONVERSOR__

namespace SLB {
	
	struct Conversor
	{
		virtual void* operator()(void* obj) = 0;
		virtual ~Conversor() {}
	};

	template<class B, class D>
	struct BaseToDerived : public Conversor
	{
		void *operator()(void* obj_raw)
		{
			B *obj = reinterpret_cast<B*>(obj_raw);
			return dynamic_cast<D*>(obj);
		}
	};

	template<class D, class B>
	struct DerivedToBase : public Conversor
	{
		void *operator()(void* obj_raw)
		{
			D *obj = reinterpret_cast<D*>(obj_raw);
			B *base_obj = obj; // D -> B
			return base_obj;
		}
	};

}

#endif
