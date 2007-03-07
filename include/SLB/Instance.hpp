#ifndef __SLB_INSTANCE__
#define __SLB_INSTANCE__

#include "ref_ptr.hpp"
#include "Manager.hpp"
#include "Debug.hpp"
#include "Export.hpp"

namespace SLB {

	class SLB_EXPORT InstanceBase
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

		bool isCopy()  const     { return _flags & I_Copy; }
		bool isConst() const     { return (_flags & I_Const_Pointer) != 0; }
		bool isPointer() const   { return (_flags & I_Pointer) || (_flags & I_Const_Pointer); }
		bool isReference() const { return (_flags & I_Reference) != 0; }

	protected:
		int _flags;
		ref_ptr<ClassInfo> _class;
	};

	namespace Instance {

		struct Default {
			template<class T>
			class Implementation : public InstanceBase
			{
			public:
				// constructor from a pointer 
				// @fromConstructor is true if this instance was created inside the scripting language calling a 
				//    class constructor method. If this instance was returned by a function, thus was not 
				//    created inside the script then @fromConstructor = false.
				Implementation( T* ptr, bool fromConstructor = false ) : InstanceBase( I_Pointer, typeid(T) ), _ptr(ptr)
				{
					if (fromConstructor) _flags |= I_Copy;
				}
				// constructor from const pointer
				Implementation( const T *ptr ) : InstanceBase( I_Const_Pointer, typeid(T)), _const_ptr(ptr)
				{
				}

				// constructor from reference
				Implementation( T &ref ) : InstanceBase( I_Reference, typeid(T) ), _ptr( &ref )
				{
				}

				// copy constructor,  
				Implementation( const T &ref) : InstanceBase( I_Copy, typeid(T) ), _ptr( 0L )
				{
					_ptr = new T( ref );
				}

				virtual ~Implementation() { if (isCopy()) delete _ptr; }

				void* get_ptr() { return (isConst())? 0L : _ptr; }
				const void* get_const_ptr() { return _const_ptr; }
			protected:
				union {
					T *_ptr;
					const T *_const_ptr;
				};
			};
		};


		struct NoCopyNoDestroy 
		{
			template<class T>
			class Implementation : public InstanceBase
			{
			public:
				// constructor form a pointer 
				Implementation( T* ptr, bool fromConstructor) : InstanceBase( I_Pointer, typeid(T) ), _ptr(ptr)
				{
					if (fromConstructor)
					{
						_flags = I_Invalid;
						_ptr = 0;
					}
				}
				// constructor from const pointer
				Implementation( const T *ptr ) : InstanceBase( I_Const_Pointer, typeid(T)), _const_ptr(ptr)
				{
				}

				// constructor from reference
				Implementation( T &ref ) : InstanceBase( I_Reference, typeid(T) ), _ptr( &ref )
				{
				}

				// copy constructor,  
				Implementation( const T &) : InstanceBase( I_Invalid, typeid(T) ), _ptr( 0L )
				{
				}

				virtual ~Implementation() {}

				void* get_ptr() { return (isConst())? 0L : _ptr; }
				const void* get_const_ptr() { return _const_ptr; }
			protected:
				union {
					T *_ptr;
					const T *_const_ptr;
				};
			};
		};

		template<template <class> class T_SmartPtr>
		struct SmartPtr 
		{
			template<class T>
			class Implementation : public InstanceBase
			{
			public:
				Implementation( T* ptr, bool) : InstanceBase( I_Pointer, typeid(T) ), _sm_ptr(ptr)
				{
					_const_ptr = &(*_sm_ptr);
				}
				Implementation( const T *ptr ) : InstanceBase( I_Const_Pointer, typeid(T)), _const_ptr(ptr)
				{
				}
				// What should we do with references and smart pointers?
				Implementation( T &ref ) : InstanceBase( I_Reference, typeid(T) ), _sm_ptr( &ref )
				{
					_const_ptr = &(*_sm_ptr);
				}

				// copy constructor,  
				Implementation( const T &ref) : InstanceBase( I_Copy, typeid(T) ), _sm_ptr( 0L ), _const_ptr(&ref)
				{
					_sm_ptr = new T( ref );
				}

				virtual ~Implementation() {}

				void* get_ptr() { return &(*_sm_ptr); }
				const void* get_const_ptr() { return _const_ptr; }
			protected:
				T_SmartPtr<T> _sm_ptr;
				const T *_const_ptr;
			};
		};
	
	} // end of Instance namespace


	struct SLB_EXPORT InstanceFactory
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

	template<class T, class TInstance >
	struct InstanceFactoryAdapter : public InstanceFactory
	{
		virtual InstanceBase *create_ref(void *v_ref)
		{
			T &ref = *reinterpret_cast<T*>(v_ref);
			return new TInstance( ref );
		}

		virtual InstanceBase *create_ptr(void *v_ptr, bool fromConstructor )
		{
			T *ptr = reinterpret_cast<T*>(v_ptr);
			return new TInstance( ptr, fromConstructor );
		}

		virtual InstanceBase *create_const_ptr(const void *v_ptr)
		{
			const T *const_ptr = reinterpret_cast<const T*>(v_ptr);
			return new TInstance( const_ptr );
		}

		virtual InstanceBase *create_copy(const void *v_ptr)
		{
			const T &const_ref = *reinterpret_cast<const T*>(v_ptr);
			return new TInstance( const_ref );
		}

		virtual ~InstanceFactoryAdapter() {}
	};
}


#endif
