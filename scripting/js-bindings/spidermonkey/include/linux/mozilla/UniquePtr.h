/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Smart pointer managing sole ownership of a resource. */

#ifndef mozilla_UniquePtr_h
#define mozilla_UniquePtr_h

#include "mozilla/Assertions.h"
#include "mozilla/Attributes.h"
#include "mozilla/Compiler.h"
#include "mozilla/Move.h"
#include "mozilla/NullPtr.h"
#include "mozilla/Pair.h"
#include "mozilla/TypeTraits.h"

namespace mozilla {

template<typename T> class DefaultDelete;
template<typename T, class D = DefaultDelete<T>> class UniquePtr;

} // namespace mozilla

namespace mozilla {

/**
 * UniquePtr is a smart pointer that wholly owns a resource.  Ownership may be
 * transferred out of a UniquePtr through explicit action, but otherwise the
 * resource is destroyed when the UniquePtr is destroyed.
 *
 * UniquePtr is similar to C++98's std::auto_ptr, but it improves upon auto_ptr
 * in one crucial way: it's impossible to copy a UniquePtr.  Copying an auto_ptr
 * obviously *can't* copy ownership of its singly-owned resource.  So what
 * happens if you try to copy one?  Bizarrely, ownership is implicitly
 * *transferred*, preserving single ownership but breaking code that assumes a
 * copy of an object is identical to the original.  (This is why auto_ptr is
 * prohibited in STL containers.)
 *
 * UniquePtr solves this problem by being *movable* rather than copyable.
 * Instead of passing a |UniquePtr u| directly to the constructor or assignment
 * operator, you pass |Move(u)|.  In doing so you indicate that you're *moving*
 * ownership out of |u|, into the target of the construction/assignment.  After
 * the transfer completes, |u| contains |nullptr| and may be safely destroyed.
 * This preserves single ownership but also allows UniquePtr to be moved by
 * algorithms that have been made move-safe.  (Note: if |u| is instead a
 * temporary expression, don't use |Move()|: just pass the expression, because
 * it's already move-ready.  For more information see Move.h.)
 *
 * UniquePtr is also better than std::auto_ptr in that the deletion operation is
 * customizable.  An optional second template parameter specifies a class that
 * (through its operator()(T*)) implements the desired deletion policy.  If no
 * policy is specified, mozilla::DefaultDelete<T> is used -- which will either
 * |delete| or |delete[]| the resource, depending whether the resource is an
 * array.  Custom deletion policies ideally should be empty classes (no member
 * fields, no member fields in base classes, no virtual methods/inheritance),
 * because then UniquePtr can be just as efficient as a raw pointer.
 *
 * Use of UniquePtr proceeds like so:
 *
 *   UniquePtr<int> g1; // initializes to nullptr
 *   g1.reset(new int); // switch resources using reset()
 *   g1 = nullptr; // clears g1, deletes the int
 *
 *   UniquePtr<int> g2(new int); // owns that int
 *   int* p = g2.release(); // g2 leaks its int -- still requires deletion
 *   delete p; // now freed
 *
 *   struct S { int x; S(int x) : x(x) {} };
 *   UniquePtr<S> g3, g4(new S(5));
 *   g3 = Move(g4); // g3 owns the S, g4 cleared
 *   S* p = g3.get(); // g3 still owns |p|
 *   assert(g3->x == 5); // operator-> works (if .get() != nullptr)
 *   assert((*g3).x == 5); // also operator* (again, if not cleared)
 *   Swap(g3, g4); // g4 now owns the S, g3 cleared
 *   g3.swap(g4);  // g3 now owns the S, g4 cleared
 *   UniquePtr<S> g5(Move(g3)); // g5 owns the S, g3 cleared
 *   g5.reset(); // deletes the S, g5 cleared
 *
 *   struct FreePolicy { void operator()(void* p) { free(p); } };
 *   UniquePtr<int, FreePolicy> g6(static_cast<int*>(malloc(sizeof(int))));
 *   int* ptr = g6.get();
 *   g6 = nullptr; // calls free(ptr)
 *
 * Now, carefully note a few things you *can't* do:
 *
 *   UniquePtr<int> b1;
 *   b1 = new int; // BAD: can only assign another UniquePtr
 *   int* ptr = b1; // BAD: no auto-conversion to pointer, use get()
 *
 *   UniquePtr<int> b2(b1); // BAD: can't copy a UniquePtr
 *   UniquePtr<int> b3 = b1; // BAD: can't copy-assign a UniquePtr
 *
 * A few miscellaneous notes:
 *
 * UniquePtr, when not instantiated for an array type, can be move-constructed
 * and move-assigned, not only from itself but from "derived" UniquePtr<U, E>
 * instantiations where U converts to T and E converts to D.  If you want to use
 * this, you're going to have to specify a deletion policy for both UniquePtr
 * instantations, and T pretty much has to have a virtual destructor.  In other
 * words, this doesn't work:
 *
 *   struct Base { virtual ~Base() {} };
 *   struct Derived : Base {};
 *
 *   UniquePtr<Base> b1;
 *   // BAD: DefaultDelete<Base> and DefaultDelete<Derived> don't interconvert
 *   UniquePtr<Derived> d1(Move(b));
 *
 *   UniquePtr<Base> b2;
 *   UniquePtr<Derived, DefaultDelete<Base>> d2(Move(b2)); // okay
 *
 * UniquePtr is specialized for array types.  Specializing with an array type
 * creates a smart-pointer version of that array -- not a pointer to such an
 * array.
 *
 *   UniquePtr<int[]> arr(new int[5]);
 *   arr[0] = 4;
 *
 * What else is different?  Deletion of course uses |delete[]|.  An operator[]
 * is provided.  Functionality that doesn't make sense for arrays is removed.
 * The constructors and mutating methods only accept array pointers (not T*, U*
 * that converts to T*, or UniquePtr<U[]> or UniquePtr<U>) or |nullptr|.
 *
 * It's perfectly okay to return a UniquePtr from a method to assure the related
 * resource is properly deleted.  You'll need to use |Move()| when returning a
 * local UniquePtr.  Otherwise you can return |nullptr|, or you can return
 * |UniquePtr(ptr)|.
 *
 * UniquePtr will commonly be a member of a class, with lifetime equivalent to
 * that of that class.  If you want to expose the related resource, you could
 * expose a raw pointer via |get()|, but ownership of a raw pointer is
 * inherently unclear.  So it's better to expose a |const UniquePtr&| instead.
 * This prohibits mutation but still allows use of |get()| when needed (but
 * operator-> is preferred).  Of course, you can only use this smart pointer as
 * long as the enclosing class instance remains live -- no different than if you
 * exposed the |get()| raw pointer.
 *
 * To pass a UniquePtr-managed resource as a pointer, use a |const UniquePtr&|
 * argument.  To specify an inout parameter (where the method may or may not
 * take ownership of the resource, or reset it), or to specify an out parameter
 * (where simply returning a |UniquePtr| isn't possible), use a |UniquePtr&|
 * argument.  To unconditionally transfer ownership of a UniquePtr
 * into a method, use a |UniquePtr| argument.  To conditionally transfer
 * ownership of a resource into a method, should the method want it, use a
 * |UniquePtr&&| argument.
 */
template<typename T, class D>
class UniquePtr
{
  public:
    typedef T* Pointer;
    typedef T ElementType;
    typedef D DeleterType;

  private:
    Pair<Pointer, DeleterType> tuple;

    Pointer& ptr() { return tuple.first(); }
    const Pointer& ptr() const { return tuple.first(); }

    DeleterType& del() { return tuple.second(); }
    const DeleterType& del() const { return tuple.second(); }

  public:
    /**
     * Construct a UniquePtr containing |nullptr|.
     */
    MOZ_CONSTEXPR UniquePtr()
      : tuple(static_cast<Pointer>(nullptr), DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

    /**
     * Construct a UniquePtr containing |p|.
     */
    explicit UniquePtr(Pointer p)
      : tuple(p, DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

    UniquePtr(Pointer p,
              typename Conditional<IsReference<D>::value,
                                   D,
                                   const D&>::Type d1)
      : tuple(p, d1)
    {}

    // If you encounter an error with MSVC10 about RemoveReference below, along
    // the lines that "more than one partial specialization matches the template
    // argument list": don't use UniquePtr<T, reference to function>!  Ideally
    // you should make deletion use the same function every time, using a
    // deleter policy:
    //
    //   // BAD, won't compile with MSVC10, deleter doesn't need to be a
    //   // variable at all
    //   typedef void (&FreeSignature)(void*);
    //   UniquePtr<int, FreeSignature> ptr((int*) malloc(sizeof(int)), free);
    //
    //   // GOOD, compiles with MSVC10, deletion behavior statically known and
    //   // optimizable
    //   struct DeleteByFreeing
    //   {
    //     void operator()(void* ptr) { free(ptr); }
    //   };
    //
    // If deletion really, truly, must be a variable: you might be able to work
    // around this with a deleter class that contains the function reference.
    // But this workaround is untried and untested, because variable deletion
    // behavior really isn't something you should use.
    UniquePtr(Pointer p,
              typename RemoveReference<D>::Type&& d2)
      : tuple(p, Move(d2))
    {
      static_assert(!IsReference<D>::value,
                    "rvalue deleter can't be stored by reference");
    }

    UniquePtr(UniquePtr&& other)
      : tuple(other.release(), Forward<DeleterType>(other.getDeleter()))
    {}

    template<typename N>
    UniquePtr(N,
              typename EnableIf<IsNullPointer<N>::value, int>::Type dummy = 0)
      : tuple(static_cast<Pointer>(nullptr), DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

    template<typename U, class E>
    UniquePtr(UniquePtr<U, E>&& other,
              typename EnableIf<IsConvertible<typename UniquePtr<U, E>::Pointer,
                                              Pointer>::value &&
                                !IsArray<U>::value &&
                                (IsReference<D>::value
                                 ? IsSame<D, E>::value
                                 : IsConvertible<E, D>::value),
                                int>::Type dummy = 0)
      : tuple(other.release(), Forward<E>(other.getDeleter()))
    {
    }

    ~UniquePtr() {
      reset(nullptr);
    }

    UniquePtr& operator=(UniquePtr&& other) {
      reset(other.release());
      getDeleter() = Forward<DeleterType>(other.getDeleter());
      return *this;
    }

    template<typename U, typename E>
    UniquePtr& operator=(UniquePtr<U, E>&& other)
    {
      static_assert(IsConvertible<typename UniquePtr<U, E>::Pointer, Pointer>::value,
                    "incompatible UniquePtr pointees");
      static_assert(!IsArray<U>::value,
                    "can't assign from UniquePtr holding an array");

      reset(other.release());
      getDeleter() = Forward<E>(other.getDeleter());
      return *this;
    }

    UniquePtr& operator=(NullptrT n) {
      MOZ_ASSERT(n == nullptr);
      reset(nullptr);
      return *this;
    }

    T& operator*() const { return *get(); }
    Pointer operator->() const {
      MOZ_ASSERT(get(), "dereferencing a UniquePtr containing nullptr");
      return get();
    }

    Pointer get() const { return ptr(); }

    DeleterType& getDeleter() { return del(); }
    const DeleterType& getDeleter() const { return del(); }

  private:
    typedef void (UniquePtr::* ConvertibleToBool)(double, char);
    void nonNull(double, char) {}

  public:
    operator ConvertibleToBool() const {
      return get() != nullptr ? &UniquePtr::nonNull : nullptr;
    }

    Pointer release() {
      Pointer p = ptr();
      ptr() = nullptr;
      return p;
    }

    void reset(Pointer p = Pointer()) {
      Pointer old = ptr();
      ptr() = p;
      if (old != nullptr)
        getDeleter()(old);
    }

    void swap(UniquePtr& other) {
      tuple.swap(other.tuple);
    }

  private:
    UniquePtr(const UniquePtr& other) MOZ_DELETE; // construct using Move()!
    void operator=(const UniquePtr& other) MOZ_DELETE; // assign using Move()!
};

// In case you didn't read the comment by the main definition (you should!): the
// UniquePtr<T[]> specialization exists to manage array pointers.  It deletes
// such pointers using delete[], it will reject construction and modification
// attempts using U* or U[].  Otherwise it works like the normal UniquePtr.
template<typename T, class D>
class UniquePtr<T[], D>
{
  public:
    typedef T* Pointer;
    typedef T ElementType;
    typedef D DeleterType;

  private:
    Pair<Pointer, DeleterType> tuple;

  public:
    /**
     * Construct a UniquePtr containing nullptr.
     */
    MOZ_CONSTEXPR UniquePtr()
      : tuple(static_cast<Pointer>(nullptr), DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

    /**
     * Construct a UniquePtr containing |p|.
     */
    explicit UniquePtr(Pointer p)
      : tuple(p, DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

  private:
    // delete[] knows how to handle *only* an array of a single class type.  For
    // delete[] to work correctly, it must know the size of each element, the
    // fields and base classes of each element requiring destruction, and so on.
    // So forbid all overloads which would end up invoking delete[] on a pointer
    // of the wrong type.
    template<typename U>
    UniquePtr(U&& u,
              typename EnableIf<IsPointer<U>::value &&
                                IsConvertible<U, Pointer>::value,
                                int>::Type dummy = 0)
    MOZ_DELETE;

  public:
    UniquePtr(Pointer p,
              typename Conditional<IsReference<D>::value,
                                   D,
                                   const D&>::Type d1)
      : tuple(p, d1)
    {}

    // If you encounter an error with MSVC10 about RemoveReference below, along
    // the lines that "more than one partial specialization matches the template
    // argument list": don't use UniquePtr<T[], reference to function>!  See the
    // comment by this constructor in the non-T[] specialization above.
    UniquePtr(Pointer p,
              typename RemoveReference<D>::Type&& d2)
      : tuple(p, Move(d2))
    {
      static_assert(!IsReference<D>::value,
                    "rvalue deleter can't be stored by reference");
    }

  private:
    // Forbidden for the same reasons as stated above.
    template<typename U, typename V>
    UniquePtr(U&& u, V&& v,
              typename EnableIf<IsPointer<U>::value &&
                                IsConvertible<U, Pointer>::value,
                                int>::Type dummy = 0)
    MOZ_DELETE;

  public:
    UniquePtr(UniquePtr&& other)
      : tuple(other.release(), Forward<DeleterType>(other.getDeleter()))
    {}

    template<typename N>
    UniquePtr(N,
              typename EnableIf<IsNullPointer<N>::value, int>::Type dummy = 0)
      : tuple(static_cast<Pointer>(nullptr), DeleterType())
    {
      static_assert(!IsPointer<D>::value, "must provide a deleter instance");
      static_assert(!IsReference<D>::value, "must provide a deleter instance");
    }

    ~UniquePtr() {
      reset(nullptr);
    }

    UniquePtr& operator=(UniquePtr&& other) {
      reset(other.release());
      getDeleter() = Forward<DeleterType>(other.getDeleter());
      return *this;
    }

    UniquePtr& operator=(NullptrT) {
      reset();
      return *this;
    }

    T& operator[](decltype(sizeof(int)) i) const { return get()[i]; }
    Pointer get() const { return tuple.first(); }

    DeleterType& getDeleter() { return tuple.second(); }
    const DeleterType& getDeleter() const { return tuple.second(); }

  private:
    typedef void (UniquePtr::* ConvertibleToBool)(double, char);
    void nonNull(double, char) {}

  public:
    operator ConvertibleToBool() const {
      return get() != nullptr ? &UniquePtr::nonNull : nullptr;
    }

    Pointer release() {
      Pointer p = tuple.first();
      tuple.first() = nullptr;
      return p;
    }

    void reset(Pointer p = Pointer()) {
      Pointer old = tuple.first();
      tuple.first() = p;
      if (old != nullptr)
        tuple.second()(old);
    }

  private:
    // Kill off all remaining overloads that aren't true nullptr (the overload
    // above should handle that) or emulated nullptr (which acts like int/long
    // on gcc 4.4/4.5).
    template<typename U>
    void reset(U,
               typename EnableIf<!IsNullPointer<U>::value &&
                                 !IsSame<U,
                                         Conditional<(sizeof(int) == sizeof(void*)),
                                                     int,
                                                     long>::Type>::value,
                                 int>::Type dummy = 0)
    MOZ_DELETE;

  public:
    void swap(UniquePtr& other) {
      tuple.swap(other.tuple);
    }

  private:
    UniquePtr(const UniquePtr& other) MOZ_DELETE; // construct using Move()!
    void operator=(const UniquePtr& other) MOZ_DELETE; // assign using Move()!
};

/** A default deletion policy using plain old operator delete. */
template<typename T>
class DefaultDelete
{
  public:
    MOZ_CONSTEXPR DefaultDelete() {}

    template<typename U>
    DefaultDelete(const DefaultDelete<U>& other,
                  typename EnableIf<mozilla::IsConvertible<U*, T*>::value,
                                    int>::Type dummy = 0)
    {}

    void operator()(T* ptr) const {
      static_assert(sizeof(T) > 0, "T must be complete");
      delete ptr;
    }
};

/** A default deletion policy using operator delete[]. */
template<typename T>
class DefaultDelete<T[]>
{
  public:
    MOZ_CONSTEXPR DefaultDelete() {}

    void operator()(T* ptr) const {
      static_assert(sizeof(T) > 0, "T must be complete");
      delete[] ptr;
    }

  private:
    template<typename U>
    void operator()(U* ptr) const MOZ_DELETE;
};

template<typename T, class D>
void
Swap(UniquePtr<T, D>& x, UniquePtr<T, D>& y)
{
  x.swap(y);
}

template<typename T, class D, typename U, class E>
bool
operator==(const UniquePtr<T, D>& x, const UniquePtr<U, E>& y)
{
  return x.get() == y.get();
}

template<typename T, class D, typename U, class E>
bool
operator!=(const UniquePtr<T, D>& x, const UniquePtr<U, E>& y)
{
  return x.get() != y.get();
}

template<typename T, class D>
bool
operator==(const UniquePtr<T, D>& x, NullptrT n)
{
  MOZ_ASSERT(n == nullptr);
  return !x;
}

template<typename T, class D>
bool
operator==(NullptrT n, const UniquePtr<T, D>& x)
{
  MOZ_ASSERT(n == nullptr);
  return !x;
}

template<typename T, class D>
bool
operator!=(const UniquePtr<T, D>& x, NullptrT n)
{
  MOZ_ASSERT(n == nullptr);
  return bool(x);
}

template<typename T, class D>
bool
operator!=(NullptrT n, const UniquePtr<T, D>& x)
{
  MOZ_ASSERT(n == nullptr);
  return bool(x);
}

// No operator<, operator>, operator<=, operator>= for now because simplicity.

namespace detail {

template<typename T>
struct UniqueSelector
{
    typedef UniquePtr<T> SingleObject;
};

template<typename T>
struct UniqueSelector<T[]>
{
    typedef UniquePtr<T[]> UnknownBound;
};

template<typename T, decltype(sizeof(int)) N>
struct UniqueSelector<T[N]>
{
    typedef UniquePtr<T[N]> KnownBound;
};

} // namespace detail

// We don't have variadic template support everywhere, so just hard-code arities
// 0-4 for now.  If you need more arguments, feel free to add the extra
// overloads.
//
// Beware!  Due to lack of true nullptr support in gcc 4.4 and 4.5, passing
// literal nullptr to MakeUnique will not work on some platforms.  See Move.h
// for more details.

template<typename T>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique()
{
  return UniquePtr<T>(new T());
}

template<typename T, typename A1>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique(A1&& a1)
{
  return UniquePtr<T>(new T(Forward<A1>(a1)));
}

template<typename T, typename A1, typename A2>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique(A1&& a1, A2&& a2)
{
  return UniquePtr<T>(new T(Forward<A1>(a1), Forward<A2>(a2)));
}

template<typename T, typename A1, typename A2, typename A3>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique(A1&& a1, A2&& a2, A3&& a3)
{
  return UniquePtr<T>(new T(Forward<A1>(a1), Forward<A2>(a2), Forward<A3>(a3)));
}

template<typename T, typename A1, typename A2, typename A3, typename A4>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique(A1&& a1, A2&& a2, A3&& a3, A4&& a4)
{
  return UniquePtr<T>(new T(Forward<A1>(a1), Forward<A2>(a2), Forward<A3>(a3), Forward<A4>(a4)));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
typename detail::UniqueSelector<T>::SingleObject
MakeUnique(A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5)
{
  return UniquePtr<T>(new T(Forward<A1>(a1), Forward<A2>(a2), Forward<A3>(a3), Forward<A4>(a4), Forward<A5>(a5)));
}

template<typename T>
typename detail::UniqueSelector<T>::UnknownBound
MakeUnique(decltype(sizeof(int)) n)
{
  typedef typename RemoveExtent<T>::Type ArrayType;
  return UniquePtr<T>(new ArrayType[n]());
}

template<typename T>
typename detail::UniqueSelector<T>::KnownBound
MakeUnique() MOZ_DELETE;

template<typename T, typename A1>
typename detail::UniqueSelector<T>::KnownBound
MakeUnique(A1&& a1) MOZ_DELETE;

template<typename T, typename A1, typename A2>
typename detail::UniqueSelector<T>::KnownBound
MakeUnique(A1&& a1, A2&& a2) MOZ_DELETE;

template<typename T, typename A1, typename A2, typename A3>
typename detail::UniqueSelector<T>::KnownBound
MakeUnique(A1&& a1, A2&& a2, A3&& a3) MOZ_DELETE;

template<typename T, typename A1, typename A2, typename A3, typename A4>
typename detail::UniqueSelector<T>::KnownBound
MakeUnique(A1&& a1, A2&& a2, A3&& a3, A4&& a4) MOZ_DELETE;

} // namespace mozilla

#endif /* mozilla_UniquePtr_h */
