#ifndef __SLB_INSTANCE__
#define __SLB_INSTANCE__

#include "ref_ptr.hpp"
#include "Manager.hpp"
#include "Debug.hpp"

namespace SLB {

	class InstanceBase
	{
	public:
		enum Type
		{
			I_Invalid   = 0x00,
			I_Copy      = 0x01,
			I_Reference = 0x02,
			I_Pointer   = 0x04,
			I_Const_Pointer   = 0x08,
		};

		virtual void* get_ptr() = 0;
		virtual const void* get_const_ptr() = 0;

		InstanceBase( Type , const std::type_info &);
		virtual ~InstanceBase();

		ClassInfo *getClass() { return _class.get(); }

		bool isCopy()  const     { return (_flags & I_Copy); }
		bool isConst() const     { return (_flags & I_Const_Pointer); }
		bool isPointer() const   { return (_flags & I_Pointer) || (_flags & I_Const_Pointer); }
		bool isReference() const { return (_flags & I_Reference); }

	protected:
		int _flags;
		ref_ptr<ClassInfo> _class;
	};


	template<class T>
	class DefaultInstance : public InstanceBase
	{
	public:
		// constructor form a pointer 
		DefaultInstance( T* ptr, bool fromConstructor = false ) : InstanceBase( I_Pointer, typeid(T) ), _ptr(ptr)
	   	{
			if (fromConstructor) _flags |= I_Copy;
		}
		// constructor from const pointer
		DefaultInstance( const T *ptr ) : InstanceBase( I_Const_Pointer, typeid(T)), _const_ptr(ptr)
		{
		}

		// constructor from reference
		DefaultInstance( T &ref ) : InstanceBase( I_Reference, typeid(T) ), _ptr( &ref )
		{
		}

		// copy constructor,  
		DefaultInstance( const T &ref) : InstanceBase( I_Copy, typeid(T) ), _ptr( 0L )
		{
			_ptr = new T( ref );
		}

		virtual ~DefaultInstance() { if (isCopy()) delete _ptr; }

		void* get_ptr() { return (isConst())? 0L : _ptr; }
		const void* get_const_ptr() { return _const_ptr; }
	protected:
		union {
			T *_ptr;
			const T *_const_ptr;
		};
	};


	struct InstanceFactory
	{
		// create an Instance from a reference
		virtual InstanceBase *create_ref(void *ref) = 0;
		// create an Instance from a pointer
		virtual InstanceBase *create_ptr(void *ptr, bool fromConstructor = false) = 0;
		// create an Instance from a const pointer
		virtual InstanceBase *create_const_ptr(const void *const_ptr) = 0;
		// create an Instance with copy
		virtual InstanceBase *create_copy(const void *ptr) = 0;

		virtual ~InstanceFactory();
	};

	template<class T, template<class> class TInstance >
	struct InstanceFactoryAdapter : public InstanceFactory
	{
		virtual InstanceBase *create_ref(void *v_ref)
		{
			T &ref = *reinterpret_cast<T*>(v_ref);
			return new TInstance<T>( ref );
		}

		virtual InstanceBase *create_ptr(void *v_ptr, bool fromConstructor )
		{
			T *ptr = reinterpret_cast<T*>(v_ptr);
			return new TInstance<T>( ptr, fromConstructor );
		}

		virtual InstanceBase *create_const_ptr(const void *v_ptr)
		{
			const T *const_ptr = reinterpret_cast<const T*>(v_ptr);
			return new TInstance<T>( const_ptr );
		}

		virtual InstanceBase *create_copy(const void *v_ptr)
		{
			const T &const_ref = *reinterpret_cast<const T*>(v_ptr);
			return new TInstance<T>( const_ref );
		}

		virtual ~InstanceFactoryAdapter() {}
	};
}


#endif
