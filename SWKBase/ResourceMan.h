#pragma once

#include <memory>
namespace swktool {

    //template <class _Ty, class... _Types>
    //unique_ptr<_Ty> make_unique(_types&&... _Args)

    //}


    //template <class _Ty, class... _Types, enable_if_t<!is_array_v<_Ty>, int> = 0>
    //_NODISCARD unique_ptr<_Ty> make_unique(_Types&&... _Args) { // make a unique_ptr
    //    return unique_ptr<_Ty>(new _Ty(_STD forward<_Types>(_Args)...));
    //}

	//template <class _Res, class... _Types>
 //   class Resource_Guard
 //   {
 //   public:
 //       Resource_Guard(_Res& oRes, _Types&&... _Args) : _MyRes(_Res) {
 //           _MyRes.Create( std::forward<_Types>(_Args)...);
 //       }

 //       ~Resource_Guard()
 //       {
 //           _MyRes.Destroy();
 //       }

 //   private:
 //       _Res& _MyRes;
 //   };    
}