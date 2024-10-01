#ifndef CCObject_H
#define CCObject_H
#pragma once
#pragma GCC system_header

#include <initializer_list>
#include <typeinfo>
#include <new>
#include <type_traits>
#include <bits/utility.h>

using namespace std;

class bad_CCObject_cast : public bad_cast
{
public:
    virtual const char* what() const noexcept { return "bad CCObject_cast"; }
};

[[gnu::noreturn]] inline void __throw_bad_CCObject_cast()
{
#if __cpp_exceptions
    throw bad_CCObject_cast{};
#else
    __builtin_abort();
#endif
}

#define __cpp_lib_CCObject 201606L

class CCObject
{
    // Holds either pointer to a heap object or the contained object itself.
    union _Storage
    {
        constexpr _Storage() : _M_ptr{nullptr} {}

        // Prevent trivial copies of this type, buffer might hold a non-POD.
        _Storage(const _Storage&) = delete;
        _Storage& operator=(const _Storage&) = delete;

        void* _M_ptr;
        aligned_storage<sizeof(_M_ptr), alignof(void*)>::type _M_buffer;
    };

    template<typename _Tp, typename _Safe = is_nothrow_move_constructible<_Tp>,
            bool _Fits = (sizeof(_Tp) <= sizeof(_Storage))
                         && (alignof(_Tp) <= alignof(_Storage))>
    using _Internal = std::integral_constant<bool, _Safe::value && _Fits>;

    template<typename _Tp>
    struct _Manager_internal; // uses small-object optimization

    template<typename _Tp>
    struct _Manager_external; // creates contained object on the heap

    template<typename _Tp>
    using _Manager = __conditional_t<_Internal<_Tp>::value,
            _Manager_internal<_Tp>,
            _Manager_external<_Tp>>;

    template<typename _Tp, typename _VTp = decay_t<_Tp>>
    using _Decay_if_not_CCObject = enable_if_t<!is_same_v<_VTp, CCObject>, _VTp>;

    /// Emplace with an object created from @p __args as the contained object.
    template <typename _Tp, typename... _Args,
            typename _Mgr = _Manager<_Tp>>
    void __do_emplace(_Args&&... __args)
    {
        reset();
        _Mgr::_S_create(_M_storage, std::forward<_Args>(__args)...);
        _M_manager = &_Mgr::_S_manage;
    }
    template <typename _Tp, typename _Up, typename... _Args,
            typename _Mgr = _Manager<_Tp>>
    void __do_emplace(initializer_list<_Up> __il, _Args&&... __args)
    {
        reset();
        _Mgr::_S_create(_M_storage, __il, std::forward<_Args>(__args)...);
        _M_manager = &_Mgr::_S_manage;
    }

    template <typename _Res, typename _Tp, typename... _Args>
    using __CCObject_constructible
            = enable_if<__and_<is_copy_constructible<_Tp>,
    is_constructible<_Tp, _Args...>>::value,
    _Res>;

    template <typename _Tp, typename... _Args>
    using __CCObject_constructible_t
            = typename __CCObject_constructible<bool, _Tp, _Args...>::type;

    template<typename _VTp, typename... _Args>
    using __emplace_t
            = typename __CCObject_constructible<_VTp&, _VTp, _Args...>::type;

public:
    constexpr CCObject() noexcept : _M_manager(nullptr) { }
    CCObject(const CCObject& __other)
    {
        if (!__other.has_value())
            _M_manager = nullptr;
        else
        {
            _Arg __arg;
            __arg._M_CCObject = this;
            __other._M_manager(_Op_clone, &__other, &__arg);
        }
    }
    CCObject(CCObject&& __other) noexcept
    {
        if (!__other.has_value())
            _M_manager = nullptr;
        else
        {
            _Arg __arg;
            __arg._M_CCObject = this;
            __other._M_manager(_Op_xfer, &__other, &__arg);
        }
    }
    template <typename _Tp, typename _VTp = _Decay_if_not_CCObject<_Tp>,
            typename _Mgr = _Manager<_VTp>,
            enable_if_t<is_copy_constructible_v<_VTp>
            && !__is_in_place_type_v<_VTp>, bool> = true>
    CCObject(_Tp&& __value)
            : _M_manager(&_Mgr::_S_manage)
    {
        _Mgr::_S_create(_M_storage, std::forward<_Tp>(__value));
    }
    template <typename _Tp, typename... _Args, typename _VTp = decay_t<_Tp>,
            typename _Mgr = _Manager<_VTp>,
            __CCObject_constructible_t<_VTp, _Args&&...> = false>
    explicit
    CCObject(in_place_type_t<_Tp>, _Args&&... __args)
            : _M_manager(&_Mgr::_S_manage)
    {
        _Mgr::_S_create(_M_storage, std::forward<_Args>(__args)...);
    }
    template <typename _Tp, typename _Up, typename... _Args,
            typename _VTp = decay_t<_Tp>, typename _Mgr = _Manager<_VTp>,
            __CCObject_constructible_t<_VTp, initializer_list<_Up>&,
            _Args&&...> = false>
    explicit
    CCObject(in_place_type_t<_Tp>, initializer_list<_Up> __il, _Args&&... __args)
            : _M_manager(&_Mgr::_S_manage)
    {
        _Mgr::_S_create(_M_storage, __il, std::forward<_Args>(__args)...);
    }
    ~CCObject() { reset(); }
    CCObject&
    operator=(const CCObject& __rhs)
    {
        *this = CCObject(__rhs);
        return *this;
    }
    CCObject&
    operator=(CCObject&& __rhs) noexcept
    {
        if (!__rhs.has_value())
            reset();
        else if (this != &__rhs)
        {
            reset();
            _Arg __arg;
            __arg._M_CCObject = this;
            __rhs._M_manager(_Op_xfer, &__rhs, &__arg);
        }
        return *this;
    }
    template<typename _Tp>
    enable_if_t<is_copy_constructible<_Decay_if_not_CCObject<_Tp>>::value, CCObject&>
    operator=(_Tp&& __rhs)
    {
        *this = CCObject(std::forward<_Tp>(__rhs));
        return *this;
    }
    template <typename _Tp, typename... _Args>
    __emplace_t<decay_t<_Tp>, _Args...>
    emplace(_Args&&... __args)
    {
        using _VTp = decay_t<_Tp>;
        __do_emplace<_VTp>(std::forward<_Args>(__args)...);
        return *CCObject::_Manager<_VTp>::_S_access(_M_storage);
    }
    template <typename _Tp, typename _Up, typename... _Args>
    __emplace_t<decay_t<_Tp>, initializer_list<_Up>&, _Args&&...>
    emplace(initializer_list<_Up> __il, _Args&&... __args)
    {
        using _VTp = decay_t<_Tp>;
        __do_emplace<_VTp, _Up>(__il, std::forward<_Args>(__args)...);
        return *CCObject::_Manager<_VTp>::_S_access(_M_storage);
    }
    void reset() noexcept
    {
        if (has_value())
        {
            _M_manager(_Op_destroy, this, nullptr);
            _M_manager = nullptr;
        }
    }
    void swap(CCObject& __rhs) noexcept
    {
        if (!has_value() && !__rhs.has_value())
            return;

        if (has_value() && __rhs.has_value())
        {
            if (this == &__rhs)
                return;

            CCObject __tmp;
            _Arg __arg;
            __arg._M_CCObject = &__tmp;
            __rhs._M_manager(_Op_xfer, &__rhs, &__arg);
            __arg._M_CCObject = &__rhs;
            _M_manager(_Op_xfer, this, &__arg);
            __arg._M_CCObject = this;
            __tmp._M_manager(_Op_xfer, &__tmp, &__arg);
        }
        else
        {
            CCObject* __empty = !has_value() ? this : &__rhs;
            CCObject* __full = !has_value() ? &__rhs : this;
            _Arg __arg;
            __arg._M_CCObject = __empty;
            __full->_M_manager(_Op_xfer, __full, &__arg);
        }
    }
    bool has_value() const noexcept { return _M_manager != nullptr; }
    CCObject Get(){return *this;}
    void * GetPointer() const;
    const char* ToString() const;
    operator int () const;
    operator double () const;
    operator float () const;
    operator const char* () const;

#if __cpp_rtti
    const type_info& type() const noexcept
    {
        if (!has_value())
            return typeid(void);
        _Arg __arg;
        _M_manager(_Op_get_type_info, this, &__arg);
        return *__arg._M_typeinfo;
    }
#endif
    template<typename _Tp>
    static constexpr bool __is_valid_cast()
    { return __or_<is_reference<_Tp>, is_copy_constructible<_Tp>>::value; }

private:
    enum _Op {
        _Op_access, _Op_get_type_info, _Op_clone, _Op_destroy, _Op_xfer
    };
    union _Arg
    {
        void* _M_obj;
        const std::type_info* _M_typeinfo;
        CCObject* _M_CCObject;
    };
    void (*_M_manager)(_Op, const CCObject*, _Arg*);
    _Storage _M_storage;
    template<typename _Tp>
    friend void* __CCObject_caster(const CCObject* __CCObject);
    template<typename _Tp>
    struct _Manager_internal
    {
        static void
        _S_manage(_Op __which, const CCObject* __CCObjectp, _Arg* __arg);

        template<typename _Up>
        static void
        _S_create(_Storage& __storage, _Up&& __value)
        {
            void* __addr = &__storage._M_buffer;
            ::new (__addr) _Tp(std::forward<_Up>(__value));
        }

        template<typename... _Args>
        static void
        _S_create(_Storage& __storage, _Args&&... __args)
        {
            void* __addr = &__storage._M_buffer;
            ::new (__addr) _Tp(std::forward<_Args>(__args)...);
        }

        static _Tp*
        _S_access(const _Storage& __storage)
        {
            // The contained object is in __storage._M_buffer
            const void* __addr = &__storage._M_buffer;
            return static_cast<_Tp*>(const_cast<void*>(__addr));
        }
    };
    template<typename _Tp>
    struct _Manager_external
    {
        static void
        _S_manage(_Op __which, const CCObject* __CCObjectp, _Arg* __arg);

        template<typename _Up>
        static void
        _S_create(_Storage& __storage, _Up&& __value)
        {
            __storage._M_ptr = new _Tp(std::forward<_Up>(__value));
        }
        template<typename... _Args>
        static void
        _S_create(_Storage& __storage, _Args&&... __args)
        {
            __storage._M_ptr = new _Tp(std::forward<_Args>(__args)...);
        }
        static _Tp*
        _S_access(const _Storage& __storage)
        {
            // The contained object is in *__storage._M_ptr
            return static_cast<_Tp*>(__storage._M_ptr);
        }
    };
};




inline void swap(CCObject& __x, CCObject& __y) noexcept { __x.swap(__y); }
template <typename _Tp, typename... _Args>
inline
enable_if_t<is_constructible_v<CCObject, in_place_type_t<_Tp>, _Args...>, CCObject>
make_CCObject(_Args&&... __args)
{
return CCObject(in_place_type<_Tp>, std::forward<_Args>(__args)...);
}
template <typename _Tp, typename _Up, typename... _Args>
inline
enable_if_t<is_constructible_v<CCObject, in_place_type_t<_Tp>,
initializer_list<_Up>&, _Args...>, CCObject>
make_CCObject(initializer_list<_Up> __il, _Args&&... __args)
{
return CCObject(in_place_type<_Tp>, __il, std::forward<_Args>(__args)...);
}
template<typename _ValueType>
inline _ValueType CCObject_cast(const CCObject& __CCObject)
{
    using _Up = __remove_cvref_t<_ValueType>;
    static_assert(CCObject::__is_valid_cast<_ValueType>(),
                  "模板参数必须是引用或 CopyConstructible 类型");
    static_assert(is_constructible_v<_ValueType, const _Up&>,
                  "模板参数必须可以从 const 值构造。");
    auto __p = CCObject_cast<_Up>(&__CCObject);
    if (__p)
        return static_cast<_ValueType>(*__p);
    __throw_bad_CCObject_cast();
}
template<typename _ValueType>
inline _ValueType CCObject_cast(CCObject& __CCObject)
{
    using _Up = __remove_cvref_t<_ValueType>;
    static_assert(CCObject::__is_valid_cast<_ValueType>(),
                  "模板参数必须是引用或 CopyConstructible 类型");
    static_assert(is_constructible_v<_ValueType, _Up&>,
    "模板参数必须可以从左值构造。");
    auto __p = CCObject_cast<_Up>(&__CCObject);
    if (__p)
        return static_cast<_ValueType>(*__p);
    __throw_bad_CCObject_cast();
}
template<typename _ValueType>
inline _ValueType CCObject_cast(CCObject&& __CCObject)
{
    using _Up = __remove_cvref_t<_ValueType>;
    static_assert(CCObject::__is_valid_cast<_ValueType>(),
                  "模板参数必须是引用或 CopyConstructible 类型");
    static_assert(is_constructible_v<_ValueType, _Up>,
                  "模板参数必须可以从右值构造。");
    auto __p = CCObject_cast<_Up>(&__CCObject);
    if (__p)
        return static_cast<_ValueType>(std::move(*__p));
    __throw_bad_CCObject_cast();
}
template<typename _Tp>
void* __CCObject_caster(const CCObject* __CCObject)
{
    using _Up = remove_cv_t<_Tp>;
    if constexpr (!is_same_v<decay_t<_Up>, _Up>)
        return nullptr;
    else if constexpr (!is_copy_constructible_v<_Up>)
        return nullptr;
    else if (__CCObject->_M_manager == &CCObject::_Manager<_Up>::_S_manage
             #if __cpp_rtti
             || __CCObject->type() == typeid(_Tp)
#endif
            )
    {
        return CCObject::_Manager<_Up>::_S_access(__CCObject->_M_storage);
    }
    return nullptr;
}
template<typename _ValueType>
inline const _ValueType* CCObject_cast(const CCObject* __CCObject) noexcept
{
    if constexpr (is_object_v<_ValueType>)
        if (__CCObject)
            return static_cast<_ValueType*>(__CCObject_caster<_ValueType>(__CCObject));
    return nullptr;
}
template<typename _ValueType>
inline _ValueType* CCObject_cast(CCObject* __CCObject) noexcept
{
    if constexpr (is_object_v<_ValueType>)
        if (__CCObject)
            return static_cast<_ValueType*>(__CCObject_caster<_ValueType>(__CCObject));
    return nullptr;
}
template<typename _Tp>
void
CCObject::_Manager_internal<_Tp>::
_S_manage(_Op __which, const CCObject* __CCObject, _Arg* __arg)
{

    auto __ptr = reinterpret_cast<const _Tp*>(&__CCObject->_M_storage._M_buffer);
    switch (__which)
    {
        case _Op_access:
            __arg->_M_obj = const_cast<_Tp*>(__ptr);
            break;
        case _Op_get_type_info:
#if __cpp_rtti
            __arg->_M_typeinfo = &typeid(_Tp);
#endif
            break;
        case _Op_clone:
            ::new(&__arg->_M_CCObject->_M_storage._M_buffer) _Tp(*__ptr);
            __arg->_M_CCObject->_M_manager = __CCObject->_M_manager;
            break;
        case _Op_destroy:
            __ptr->~_Tp();
            break;
        case _Op_xfer:
            ::new(&__arg->_M_CCObject->_M_storage._M_buffer) _Tp
                    (std::move(*const_cast<_Tp*>(__ptr)));
            __ptr->~_Tp();
            __arg->_M_CCObject->_M_manager = __CCObject->_M_manager;
            const_cast<CCObject*>(__CCObject)->_M_manager = nullptr;
            break;
    }
}
template<typename _Tp>
void
CCObject::_Manager_external<_Tp>::
_S_manage(_Op __which, const CCObject* __CCObject, _Arg* __arg)
{
    auto __ptr = static_cast<const _Tp*>(__CCObject->_M_storage._M_ptr);
    switch (__which)
    {
        case _Op_access:
            __arg->_M_obj = const_cast<_Tp*>(__ptr);
            break;
        case _Op_get_type_info:
#if __cpp_rtti
            __arg->_M_typeinfo = &typeid(_Tp);
#endif
            break;
        case _Op_clone:
            __arg->_M_CCObject->_M_storage._M_ptr = new _Tp(*__ptr);
            __arg->_M_CCObject->_M_manager = __CCObject->_M_manager;
            break;
        case _Op_destroy:
            delete __ptr;
            break;
        case _Op_xfer:
            __arg->_M_CCObject->_M_storage._M_ptr = __CCObject->_M_storage._M_ptr;
            __arg->_M_CCObject->_M_manager = __CCObject->_M_manager;
            const_cast<CCObject*>(__CCObject)->_M_manager = nullptr;
            break;
    }
}
inline CCObject::operator int() const {
    int i = CCObject_cast<int>(*this);
    return i;
}
inline CCObject::operator double() const {
    auto d = CCObject_cast<double>(*this);
    return d;
}
inline CCObject::operator float() const {
    auto f = CCObject_cast<float>(*this);
    return f;
}
inline const char *CCObject::ToString() const {
    return (const char*)*this;
}

inline void *CCObject::GetPointer() const {
    if (auto p = CCObject_cast<void *>(*this))
        return p;
    return nullptr;
}

inline CCObject::operator const char *() const {
    if (auto s = CCObject_cast<const char *>(*this))
        return s;
    return nullptr;
}

#endif
