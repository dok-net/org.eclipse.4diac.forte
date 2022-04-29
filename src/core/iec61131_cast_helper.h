/*******************************************************************************
 * Copyright (c) 2018 TU Vienna/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation of template metaprograms
 *      - adds macro functions for easier defintion of allowed casts, metaprograms
 *      for is_base_of including is_base_helper, conditional, and is_same
 *      - adds get_equivalent_CIEC_class template
 *      - added cast helper for MUL and DIV which can be mixed with ANY_DURATION
 *******************************************************************************/

#include "datatype.h"

#ifndef SRC_CORE_IEC61131_CAST_HELPER_CPP_
#define SRC_CORE_IEC61131_CAST_HELPER_CPP_

class CIEC_ANY_DURATION;

class CIEC_BOOL;
class CIEC_BYTE;
class CIEC_WORD;
class CIEC_DWORD;
#ifdef FORTE_USE_64BIT_DATATYPES
class CIEC_LWORD;
#endif
class CIEC_USINT;
class CIEC_UINT;
class CIEC_UDINT;
#ifdef FORTE_USE_64BIT_DATATYPES
class CIEC_ULINT;
#endif
class CIEC_SINT;
class CIEC_INT;
class CIEC_DINT;
#ifdef FORTE_USE_64BIT_DATATYPES
class CIEC_LINT;
#endif
#ifdef FORTE_USE_REAL_DATATYPE
class CIEC_REAL;
#ifdef FORTE_USE_64BIT_DATATYPES
class CIEC_LREAL;
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

#define ALLOW_IMPLICIT_CAST(from, to) \
template<> struct implicit_cast<from, to>{ \
    typedef to type; \
};

#define ALLOW_EXPLICIT_CAST(from, to) \
template<> struct explicit_cast<from, to>{ \
    typedef to type; \
};

namespace forte {
  namespace core {
    namespace mpl {

      struct NullType{
        private:
          NullType();
      };

      struct false_ {
          static const bool value = false;
      };

      struct true_ {
          static const bool value = true;
      };

      template<class c> struct is_integral : false_ {};

      #define IS_INTEGRAL(type) \
      template<> struct is_integral<type> : true_ {};

      template<class c> struct is_float : false_ {};

      #define IS_FLOAT(type) \
      template<> struct is_float<type> : true_ {};

      IS_FLOAT(TForteFloat)
      IS_FLOAT(TForteDFloat)

      template<typename A, typename B> struct or_ {
          static const bool value = A::value || B::value;
      };

      template<typename A, typename B> struct and_ {
          static const bool value = A::value && B::value;
      };

      template<class c> struct negate {
          static const bool value = !(c::value);
      };

      template<class c> struct is_scalar {
          static const bool value = or_<is_integral<c>, is_float<c> >::value;
      };

      template<typename T, typename U> struct is_same {
          static const bool value = false;
      };

      template<typename T> struct is_same<T, T> {
          static const bool value = true;
      };

      template<bool Condition, typename TrueResult, typename FalseResult> struct conditional;

      template<typename TrueResult, typename FalseResult> struct conditional<true, TrueResult, FalseResult> {
          typedef TrueResult type;
      };

      template<typename TrueResult, typename FalseResult> struct conditional<false, TrueResult, FalseResult> {
          typedef FalseResult type;
      };

      template<typename Base, typename Derived> struct is_base_of_helper{
          operator Base*() const;
          operator Derived*();
      };

      struct no_type{
          char dummy[1];
      };

      struct yes_type{
          char dummy[2];
      };

      template<typename Base, typename Derived> struct is_base_of{
          template<typename T>
          static yes_type check(Derived*, T);
          static no_type check(Base*, int);

          static const bool value = (sizeof(check(is_base_of_helper<Base, Derived>(), int())) == sizeof(yes_type));
      };

      template<typename Base> struct is_base_of<Base, Base>{
          static const bool value = true;
      };

      template<typename CommonSubtype, typename T, typename U> struct are_of_subtype{
          enum {
            value = (is_base_of<CommonSubtype, T>::value && is_base_of<CommonSubtype, U>::value)
          };
      };

      /* For PARTIAL classes, which derive from their base CIEC class, which can be used to identify the CIEC class*/
      template<typename T> struct get_equivalent_CIEC_class {
        typedef typename conditional<is_base_of<CIEC_BOOL, T>::value, CIEC_BOOL, typename conditional<is_base_of<CIEC_BYTE, T>::value, CIEC_BYTE, typename conditional<is_base_of<CIEC_WORD, T>::value, CIEC_WORD, typename conditional<is_base_of<CIEC_DWORD, T>::value, CIEC_DWORD, typename conditional<is_base_of<CIEC_LWORD, T>::value, CIEC_LWORD, NullType>::type>::type>::type>::type>::type type;
      };

      /* invalid implicit casts */
      template<typename T, typename U> struct implicit_cast{
          typedef NullType type;

      };

      /* Self-casts */
      template<typename T> struct implicit_cast<T, T> {
          typedef T type;
      };

      /* BOOL casts */
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_BYTE)
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_WORD)
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_LWORD)
#endif //FORTE_USE_64BIT_DATATYPES

      /* BYTE casts */
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_WORD)
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_LWORD)
#endif //FORTE_USE_64BIT_DATATYPES

      /*WORD casts */
      ALLOW_IMPLICIT_CAST(CIEC_WORD, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_WORD, CIEC_LWORD)
#endif //FORTE_USE_64BIT_DATATYPES

      /* DWORD casts */
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_DWORD, CIEC_LWORD)
#endif //FORTE_USE_64BIT_DATATYPES

      /* USINT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_UINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_ULINT)
#endif //FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_REAL)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

      /* UINT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_ULINT)
#endif //FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_REAL)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

      /* UDINT casts*/
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_ULINT)
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

      /* SINT casts */
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_REAL)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

      /* INT casts */
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_REAL)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

      /* DINT casts */
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_DINT, CIEC_LINT)
#endif //FORTE_USE_64BIT_DATATYPES

#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_DINT, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

// REAL implicit casts
#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_IMPLICIT_CAST(CIEC_REAL, CIEC_LREAL)
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

// Explicit casts
      /* invalid explicit casts */
      template<typename T, typename U> struct explicit_cast{
          typedef NullType type;
      };

// BOOL explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_LINT)
#endif

// BYTE explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_LINT)
#endif

// WORD explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_LINT)
#endif

// DWORD explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_LINT)
#endif
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_REAL)
#endif // FORTE_USE_REAL_DATATYPE

// LWORD explicit casts
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_LINT)
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_LREAL)
#endif // FORTE_USE_REAL_DATATYPE
#endif

// USINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_LWORD)
#endif

// UINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_LWORD)
#endif

// UDINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_LWORD)
#endif
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_REAL)
#endif //FORTE_USE_REAL_DATATYPE

// ULINT explicit casts
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LWORD)
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LREAL)
#endif //FORTE_USE_REAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES

// SINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_LWORD)
#endif

// INT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_LWORD)
#endif

// DINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_DWORD)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_LWORD)
#endif
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_REAL)
#endif

// LINT explicit casts
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_LWORD)
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_LREAL)
#endif
#endif

// REAL explicit casts
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_DINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_LINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_UDINT)
#ifdef FORTE_USE_64BIT_DATATYPES
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_ULINT)
#endif
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_DWORD)
#endif

// LREAL explicit casts
#ifdef FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_USE_REAL_DATATYPE
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_LWORD)
#endif
#endif

      IS_INTEGRAL(TForteInt8)
      IS_INTEGRAL(TForteInt16)
      IS_INTEGRAL(TForteInt32)
      IS_INTEGRAL(TForteInt64)
      IS_INTEGRAL(TForteUInt8)
      IS_INTEGRAL(TForteUInt16)
      IS_INTEGRAL(TForteUInt32)
      IS_INTEGRAL(TForteUInt64)

      template<typename T, typename U> struct get_castable_type{
          typedef typename conditional<is_same<NullType, typename implicit_cast<T, U>::type>::value, typename implicit_cast<U, T>::type, typename implicit_cast<T, U>::type>::type type;
      };

      template<typename T, typename U> struct implicit_or_explicit_cast{
          typedef typename conditional<is_same<NullType, typename explicit_cast<T, U>::type>::value, typename implicit_cast<T, U>::type, typename explicit_cast<T, U>::type>::type type;
      };

      template <typename T, typename U>
      struct get_castable_type_duration_mixed_operations {
      private:
        typedef typename get_castable_type<T, U>::type castableType;
        constexpr static const bool isTAndUcastable = !is_same<NullType, castableType>::value;
        constexpr static const bool isTAnyDuration = is_base_of<CIEC_ANY_DURATION, T>::value;
        constexpr static const bool isUAnyDuration = is_base_of<CIEC_ANY_DURATION, U>::value;
        constexpr static const bool isTAndUAnyDuration = isTAnyDuration && isUAnyDuration;

      public:
        typedef typename conditional<isTAndUAnyDuration, NullType, typename conditional<isTAndUcastable, castableType, typename conditional<isTAnyDuration, T, typename conditional<isUAnyDuration, U, NullType>::type>::type>::type>::type type;
      };
    }
  }
}

#endif /* SRC_CORE_IEC61131_CAST_HELPER_CPP_ */

