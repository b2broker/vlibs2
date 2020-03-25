/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef VCOMPILER_TRAITS_H
#define VCOMPILER_TRAITS_H


//=======================================================================================
//  Пока все выводится от компилятора, надо найти список стандартных предопределенных
//  макросов.
#ifdef __GNUC__
    //===================================================================================
    #define V_GNUC_COMPILER_VERSION ( (__GNUC__            * 0x10000) + \
                                      (__GNUC_MINOR__      *   0x100) + \
                                      (__GNUC_PATCHLEVEL__          )   \
                                    )
    //===================================================================================
#else
    //===================================================================================
    #error "Unknown compiler"
    //===================================================================================
#endif // __GNUC__
//=======================================================================================


//=======================================================================================
// V_NOEXCEPT_SIGNATURE_WARNING_ENABLED -- выставляется когда версия компилятора
//  достаточно высока, чтобы выдать такое сообщение: "<надо еще получить :(>"
//  NB! С версией не уверен.
#define V_NOEXCEPT_SIGNATURE_WARNING_ENABLED (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
// V_NORETURN_ENABLED -- выставляется когда компилятор в состоянии опознать [[noreturn]].
//  NB! С версией не уверен.
//  Use V_NORETURN macro instead of real [[noreturn]] :((
#define V_NORETURN_ENABLED (V_GNUC_COMPILER_VERSION > 0x040703)
#if V_NORETURN_ENABLED
    #define V_NORETURN [[noreturn]]
#else
    #define V_NORETURN __attribute__((noreturn))
#endif
//=======================================================================================


//=======================================================================================
// V_CONTAINERS_HAS_EMPLACE -- выставляется когда контейнеры стандартной библиотеки,
//  такие как map, set, unordered_map etc обучены emplace().
//  NB! С версией не уверен.
#define V_CONTAINERS_HAS_EMPLACE (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
// V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS -- может работать с variadic templates с лямбдами.
//  См. vpoll/vinvoke_iface
//  NB! С версией не уверен.
#define V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS (V_GNUC_COMPILER_VERSION > 0x040804)
//=======================================================================================


//=======================================================================================
//  Старые компиляторы не умеют конструкторы-прокси.
//  NB! С версией не уверен.
#define V_CAN_PROXY_CONSTRUCTORS (V_GNUC_COMPILER_VERSION > 0x040804)
//=======================================================================================


//=======================================================================================
// V_COMPILER_KNOWS_THREAD_LOCAL -- знает keyword thread_local.
//  См. vapplication/vpoll
//  NB! С версией не уверен.
#define V_COMPILER_KNOWS_THREAD_LOCAL (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
//  Deprecated, в vchrono стал использовать только posix функции.
// V_HAS_GET_TIME -- знает std::get_time().
//  NB! С версией не уверен.
#define V_HAS_GET_TIME (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
//  Для подавления предупреждений от компилятора.
//  Новые версии выводят предупреждение, а старые компиляторы прагму не знают.
//  NB! С версией не уверен.
#if  (V_GNUC_COMPILER_VERSION <= 0x050400)
    #define V_PRAGMA_DIAGNOSTIC_IGNORED_IMPLICIT_FALLTHROUGH
#else
    #define V_PRAGMA_DIAGNOSTIC_IGNORED_IMPLICIT_FALLTHROUGH \
      #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif
//=======================================================================================


//=======================================================================================
//  Для подавления предупреждений от компилятора.
//  Новые версии выводят предупреждение, а старые компиляторы прагму не знают.
//  NB! С версией не уверен.
#if  (V_GNUC_COMPILER_VERSION <= 0x050400)
    #define V_PRAGMA_DIAGNOSTIC_IGNORED_NOEXCEPT_TYPE
#else
    #define V_PRAGMA_DIAGNOSTIC_IGNORED_NOEXCEPT_TYPE \
      #pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif
//=======================================================================================





#endif // VCOMPILER_TRAITS_H
