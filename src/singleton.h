#ifndef ZM_SINGLETON_H
#define ZM_SINGLETON_H

#include <assert.h>	

namespace zm{
	template <typename T>
	class Singleton
	{
	protected:
		static T* singleton_;
	public:
		Singleton(void)
		{
			assert(!singleton_);
			singleton_ = static_cast<T*>(this);
		}

		~Singleton(void){ assert(singleton_);  singleton_ = 0; }
		static T& getSingleton(void) { assert(singleton_);  return (*singleton_); }
		static T* getSingletonPtr(void){ return singleton_; }
	};

}
#define ZAE_SINGLETON_INIT( TYPE )		template <>	 TYPE * Singleton< TYPE >::singleton_ = 0;	\

#endif // ZM_SINGLETON_H
