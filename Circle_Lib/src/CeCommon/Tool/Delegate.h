#pragma once

#include <typeinfo.h>
#include <list>
#include <vector>
#include <string>
using std::string;
namespace CircleEngine
{

	// IDelegate   提供接口的基类

	template<typename ReturnType, typename ...ParamType>
	class IDelegate
	{
	public:
		IDelegate(){}
		virtual ~IDelegate(){}
		/**RTTI动态类型判断*/
		virtual bool isType(const std::type_info& _type) = 0;
		/**调用注册函数*/
		virtual ReturnType invoke(ParamType ... params) = 0;
		/** 动态比较类型*/
		virtual bool compare(IDelegate<ReturnType, ParamType...> *_delegate) const = 0;
	};
/** */
	
	//StaticDelegate 普通函数的委托

	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		/**给函数指针别名 */
		typedef  ReturnType(*Func)(ParamType...);
		/**构造函数 */
		StaticDelegate(Func _func) : mFunc(_func) { }
		/** */
		virtual bool isType(const std::type_info& _type) { return typeid(StaticDelegate<ReturnType, ParamType...>) == _type; }

		virtual ReturnType invoke(ParamType ... params) { return mFunc(params...); }

		//传过来的是委托类型的基类，需要动态判定该委托是否是该类的委托的类型
		virtual bool compare(IDelegate<ReturnType, ParamType ...> *_delegate)const
		{
			//如果委托为空或者委托类型不是
			if (0 == _delegate || !_delegate->isType(typeid(StaticDelegate<ReturnType, ParamType ...>))) return false;
			//动态类型转换
			StaticDelegate<ReturnType, ParamType ...> * cast = static_cast<StaticDelegate<ReturnType, ParamType ...>*>(_delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~StaticDelegate(){}
	private:
		Func mFunc;
	};


	//普通函数的委托特化版本
	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate<ReturnType(*)(ParamType ...)> :
		public IDelegate<ReturnType, ParamType ...>
	{
	public:

		//定义 Func 为 void (void) 函数类型指针。
		typedef  ReturnType(*Func)(ParamType...);

		StaticDelegate(Func _func) : mFunc(_func) { }

		virtual bool isType(const std::type_info& _type) { return typeid(StaticDelegate<ReturnType(*)(ParamType ...)>) == _type; }

		virtual ReturnType invoke(ParamType ... params) { return mFunc(params...); }

		virtual bool compare(IDelegate<ReturnType, ParamType ...> *_delegate)const
		{
			if (0 == _delegate || !_delegate->isType(typeid(StaticDelegate<ReturnType(*)(ParamType ...)>))) return false;
			StaticDelegate<ReturnType(*)(ParamType ...)> * cast = static_cast<StaticDelegate<ReturnType(*)(ParamType ...)>*>(_delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~StaticDelegate(){}
	private:
		Func mFunc;
	};

	//成员函数委托
	template<typename T, typename ReturnType, typename ...ParamType>
	class MethodDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::*Method)(ParamType...);

		MethodDelegate(T * _object, Method _method) : mObject(_object), mMethod(_method) { }

		virtual bool isType(const std::type_info& _type) { return typeid(MethodDelegate<T, ReturnType, ParamType...>) == _type; }

		virtual ReturnType invoke(ParamType...params)
		{
			(mObject->*mMethod)(params...);
		}

		virtual bool compare(IDelegate<ReturnType, ParamType...> *_delegate) const
		{
			if (0 == _delegate || !_delegate->isType(typeid(MethodDelegate<ReturnType, ParamType...>))) return false;
			MethodDelegate<ReturnType, ParamType...>* cast = static_cast<MethodDelegate<ReturnType, ParamType...>*>(_delegate);
			return cast->mObject == mObject && cast->mMethod == mMethod;
		}

		MethodDelegate(){}
		virtual ~MethodDelegate(){}
	private:
		T * mObject;
		Method mMethod;
	};

	//成员函数委托特化
	template<typename T, typename ReturnType, typename ...ParamType>
	class MethodDelegate<T,ReturnType (T:: *)(ParamType...)> :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		typedef ReturnType(T::*Method)(ParamType...);

		MethodDelegate(T * _object, Method _method) : mObject(_object), mMethod(_method) { }

		virtual bool isType(const std::type_info& _type) { return typeid(MethodDelegate<T,ReturnType(T:: *)(ParamType...)>) == _type; }

		virtual ReturnType invoke(ParamType...params)
		{
			return (mObject->*mMethod)(params...);
		}

		virtual bool compare(IDelegate<ReturnType, ParamType...> *_delegate) const
		{
			if (0 == _delegate || !_delegate->isType(typeid(MethodDelegate<T, ReturnType(T:: *)(ParamType...)>))) return false;
			MethodDelegate<T, ReturnType(T:: *)(ParamType...)>* cast = static_cast<MethodDelegate<T, ReturnType(T:: *)(ParamType...)>*>(_delegate);
			return cast->mObject == mObject && cast->mMethod == mMethod;
		}

		MethodDelegate(){}
		virtual ~MethodDelegate(){}
	private:
		T * mObject;
		Method mMethod;
	};

	


	//多播委托
	template<typename ReturnType, typename ...ParamType>
	class Delegate
	{
		
	public:
		
		typedef std::list<IDelegate<ReturnType, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		Delegate() { }
		~Delegate() { clear(); }

		bool empty() const
		{			 
			return mListDelegates.empty();
		}

		void clear()
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
			this->mListDelegates.clear();
		}


		Delegate<ReturnType, ParamType...>& operator+=(IDelegate<ReturnType, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				//检查是否重复
				if ((*iter) && (*iter)->compare(_delegate))
				{
					delete _delegate;
					return *this;
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		Delegate<ReturnType, ParamType...>& operator-=(IDelegate<ReturnType, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					if ((*iter) != _delegate) delete (*iter);       //避免同一个地址被delete两次
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		//有返回值的函数调用
		std::vector<ReturnType> operator()(ParamType... params)
		{
			ListDelegateIterator iter = mListDelegates.begin();
			std::vector<ReturnType> _Results;
			while (iter != mListDelegates.end())
			{
				if (0 == (*iter))
				{
					iter = mListDelegates.erase(iter);
				}
				else
				{
					_Results.push_back((*iter)->invoke(params...));
					++iter;
				}
			}
			return _Results;
		}
	private:
		Delegate<ReturnType, ParamType...>(const Delegate& _event);
		Delegate<ReturnType, ParamType...>& operator=(const Delegate& _event);

	private:
		ListDelegate mListDelegates;
	};

	template< typename ...ParamType>
	class Delegate<void, ParamType...>
	{

	public:

		typedef std::list<IDelegate<void, ParamType...>*> ListDelegate;
		typedef typename ListDelegate::iterator ListDelegateIterator;
		typedef typename ListDelegate::const_iterator ConstListDelegateIterator;

		Delegate() { }
		~Delegate() { clear(); }

		bool empty() const
		{
			return mListDelegates.empty();
		}

		void clear()
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if (*iter)
				{
					delete (*iter);
					(*iter) = nullptr;
				}
			}
		}

		Delegate<void, ParamType...>& operator+=(IDelegate<void, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					delete _delegate;
					return *this;
				}
			}
			mListDelegates.push_back(_delegate);
			return *this;
		}

		Delegate<void, ParamType...>& operator-=(IDelegate<void, ParamType...>* _delegate)
		{
			for (ListDelegateIterator iter = mListDelegates.begin(); iter != mListDelegates.end(); ++iter)
			{
				if ((*iter) && (*iter)->compare(_delegate))
				{
					if ((*iter) != _delegate) delete (*iter);       //避免同一个地址被delete两次
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		void operator()(ParamType... params)
		{
			ListDelegateIterator iter = mListDelegates.begin();
			while (iter != mListDelegates.end())
			{
				if (0 == (*iter))
				{
					iter = mListDelegates.erase(iter);
				}
				else
				{
					(*iter)->invoke(params...);
					++iter;
				}
			}
		}

		ListDelegate GetDelegates()
		{
			return mListDelegates;
		}

	private:
		Delegate<void, ParamType...>(const Delegate& _event) = delete;
		Delegate<void, ParamType...>& operator=(const Delegate& _event) = delete;

	private:
		ListDelegate mListDelegates;
	};




	/**
	* @brief 生成非成员函数委托
	*/
	template< typename T>
	StaticDelegate<T>* NewDelegate(T _func)
	{
		return new StaticDelegate<T>(_func);
	}
	/**
	* @brief 生成成员函数委托
	*/
	template< typename T,typename F>
	MethodDelegate<T,F>* NewDelegate(T * _object, F _func)
	{
		return new MethodDelegate<T, F>(_object, _func);
	}

	 
}


