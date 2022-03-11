#pragma once

#include <typeinfo>
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <assert.h>
//#include "../SWKBase/DebugStream.h"

namespace swktool {
	// supports two object creation type
	//   Instance = every time resolve is requested, new object is allocated
	//   Singleton = returns single ton object when reserve is requested
	enum class object_type {
		Instance,
		Singleton,
	};

	/// <summary>
	/// IOC Object factory
	/// Keep track of registration parameter(s)
	/// use registration parameter to create proper objects on heap
	/// </summary>
	class IOCFactory {
		object_type type_;

	public:
		IOCFactory() :
			type_(object_type::Instance)
		{
			;
		}
		IOCFactory(object_type CreateType) :
			type_(CreateType) {

		}

		IOCFactory(const IOCFactory& O) {
			type_ = O.type_;
		}

		object_type getType() const {
			return type_;
		}

		template <typename T, typename... Args>
		T* Create(Args&&... args) const {
			// requesting new instance
			if (type_ == object_type::Instance) {
				return new T(std::forward<Args>(args)...);
			}
			// requesting singleton
			else if (type_ == object_type::Singleton) {
				static T data(std::forward<Args>(args)...);
				return &data;
			}
			return nullptr;
		}
	};


	/// <summary>
	/// ties interface and object factory together
	/// object factory is then used to create the actual object depending on the registration data
	/// </summary>
	struct IOCData {
	public:
		IOCData(const std::string& oType_name, IOCFactory oFactory) :
			type_name_(oType_name),
			factory_(oFactory)
		{
		}

		IOCData(const IOCData& Other) {
			type_name_ = Other.type_name_;
			factory_ = Other.factory_;
		}

		std::string GetTypeName() const {
			return type_name_;
		}

		IOCFactory& GetFactory() {
			return factory_;
		}

		std::string type_name_;
		IOCFactory  factory_;
	};



	class IOCContainer {
		using IOCList = std::vector<IOCData>;
		using IOCListItr = std::vector<IOCData>::iterator;

	public:
		/// <summary>
		/// registers the interface with the Class
		/// </summary>
		/// <typeparam name="I">Interface</typeparam>
		/// <typeparam name="T">Class T</typeparam>
		/// <param name="Type"></param>
		template<typename I, class T>
		void Register(object_type Type = object_type::Instance) {
			IOCData* pDataExists = FindData<I, T>();
			if (pDataExists == nullptr) {
				IOCData Data((char*)typeid(I).name(), IOCFactory(Type));

				//			DebugOut << "TypeName = " << Data.GetTypeName().c_str() << std::endl;
				mList.push_back(Data);
			}
		}

		/// <summary>
		/// Create the proper object and returns the pointer
		/// </summary>
		/// <typeparam name="I">Interface</typeparam>
		/// <typeparam name="T">Class to create</typeparam>
		/// <typeparam name="...Args">arguments to pass to the constructor of the T</typeparam>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename I, class T, typename... Args>
		T* Resolve(Args&&... args) {
			T* pCreatedData = nullptr;
			IOCData* pIOCData = FindData<I, T>();
			if (pIOCData) {
				// use its factory to create the object
				pCreatedData = pIOCData->GetFactory().Create<T>(std::forward<Args>(args)...);
			}

			// cannot resolve
			assert(pCreatedData != nullptr);
			return pCreatedData;
		}
		template<typename I, class T, typename... Args>
		T GetInstance(Args&&... args) {
			return Resolve<I, T>(std::forward<Args>(args)...);
		}
		/// <summary>
		/// Resolve, but the pointer is assigned to std::unique_ptr
		/// </summary>
		/// <typeparam name="I"></typeparam>
		/// <typeparam name="T"></typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename I, class T, typename... Args>
		std::unique_ptr<T> Resolve_Unique(Args&&... args) {
			IOCData* pIOCData = FindData<I, T>();
			assert(pIOCData != nullptr);
			// can't assign singleton to unique_ptr
			assert(pIOCData->GetFactory().getType() != object_type::Singleton);
			return std::unique_ptr<T>(pIOCData->GetFactory().Create<T>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// Resolve, but the pointer is assigned to shared_ptr
		/// </summary>
		/// <typeparam name="I"></typeparam>
		/// <typeparam name="T"></typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename I, class T, typename... Args>
		std::shared_ptr<T> Resolve_Shared(Args&&... args) {
			IOCData* pIOCData = FindData<I, T>();
			assert(pIOCData != nullptr);
			// can't assign singleton to unique_ptr
			assert(pIOCData->GetFactory().getType() != object_type::Singleton);
			return std::shared_ptr<T>(pIOCData->GetFactory().Create<T>(std::forward<Args>(args)...));
		}

	private:
		// finds IOC data item from the IOCList
		template<typename I, class T>
		IOCData* FindData() {
			IOCData* pIOCData = nullptr;

			// Find the data by the name
			IOCListItr itrFind = std::find_if(mList.begin(), mList.end(), [](const IOCData& oData) {
				return (oData.GetTypeName() == typeid(I).name()); });
			if (itrFind != mList.end()) {
				IOCData& Data = *itrFind;
				pIOCData = &Data;
			}
			return pIOCData;
		}

	private:
		IOCList mList;
	};
}