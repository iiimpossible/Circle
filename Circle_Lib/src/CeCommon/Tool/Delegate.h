#pragma once

#include <typeinfo.h>
#include <list>
#include <vector>
#include <string>
using std::string;
namespace CircleEngine
{

	// IDelegate   �ṩ�ӿڵĻ���

	template<typename ReturnType, typename ...ParamType>
	class IDelegate
	{
	public:
		IDelegate(){}
		virtual ~IDelegate(){}
		/**RTTI��̬�����ж�*/
		virtual bool isType(const std::type_info& _type) = 0;
		/**����ע�ắ��*/
		virtual ReturnType invoke(ParamType ... params) = 0;
		/** ��̬�Ƚ�����*/
		virtual bool compare(IDelegate<ReturnType, ParamType...> *_delegate) const = 0;
	};
/** */
	
	//StaticDelegate ��ͨ������ί��

	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate :
		public IDelegate<ReturnType, ParamType...>
	{
	public:
		/**������ָ����� */
		typedef  ReturnType(*Func)(ParamType...);
		/**���캯�� */
		StaticDelegate(Func _func) : mFunc(_func) { }
		/** */
		virtual bool isType(const std::type_info& _type) { return typeid(StaticDelegate<ReturnType, ParamType...>) == _type; }

		virtual ReturnType invoke(ParamType ... params) { return mFunc(params...); }

		//����������ί�����͵Ļ��࣬��Ҫ��̬�ж���ί���Ƿ��Ǹ����ί�е�����
		virtual bool compare(IDelegate<ReturnType, ParamType ...> *_delegate)const
		{
			//���ί��Ϊ�ջ���ί�����Ͳ���
			if (0 == _delegate || !_delegate->isType(typeid(StaticDelegate<ReturnType, ParamType ...>))) return false;
			//��̬����ת��
			StaticDelegate<ReturnType, ParamType ...> * cast = static_cast<StaticDelegate<ReturnType, ParamType ...>*>(_delegate);
			return cast->mFunc == mFunc;
		}

		virtual ~StaticDelegate(){}
	private:
		Func mFunc;
	};


	//��ͨ������ί���ػ��汾
	template<typename ReturnType, typename ...ParamType>
	class StaticDelegate<ReturnType(*)(ParamType ...)> :
		public IDelegate<ReturnType, ParamType ...>
	{
	public:

		//���� Func Ϊ void (void) ��������ָ�롣
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

	//��Ա����ί��
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

	//��Ա����ί���ػ�
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

	


	//�ಥί��
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
				//����Ƿ��ظ�
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
					if ((*iter) != _delegate) delete (*iter);       //����ͬһ����ַ��delete����
					(*iter) = 0;
					break;
				}
			}
			delete _delegate;
			return *this;
		}

		//�з���ֵ�ĺ�������
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
					if ((*iter) != _delegate) delete (*iter);       //����ͬһ����ַ��delete����
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
	* @brief ���ɷǳ�Ա����ί��
	*/
	template< typename T>
	StaticDelegate<T>* NewDelegate(T _func)
	{
		return new StaticDelegate<T>(_func);
	}
	/**
	* @brief ���ɳ�Ա����ί��
	*/
	template< typename T,typename F>
	MethodDelegate<T,F>* NewDelegate(T * _object, F _func)
	{
		return new MethodDelegate<T, F>(_object, _func);
	}

	 
}


