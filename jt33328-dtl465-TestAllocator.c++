// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <vector>    // containers
#include <numeric>   // iota

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocatorE
// --------------
/* Tests exceptions */
template <typename A> struct TestAllocatorE : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>> my_types_e;

TYPED_TEST_CASE(TestAllocatorE, my_types_e);

TYPED_TEST(TestAllocatorE, test_1) {
  /* Check bad_alloc exception thrown in constructor */
  try {
    Allocator<double, 6> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocatorE, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  /* Check bad_alloc exception thrown in allocate() */
  allocator_type x;
  const size_type s = 300;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocatorE, test_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  /* Check invalid_argument exception thrown in free() */
  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  try {
    x.deallocate(nullptr, 10);
    ASSERT_EQ(true, false);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

// --------------
// TestAllocator1
// --------------

template <typename A> struct TestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       Allocator<int, 100>, Allocator<double, 100>> my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator1, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator1, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
  const Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

// --------------
// TestAllocator3
// --------------

template <typename A> struct TestAllocator3 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>> my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator3, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// TestAllocator4
// --------------

template <typename A> struct TestAllocator4 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>> my_types_4;

TYPED_TEST_CASE(TestAllocator4, my_types_4);

TYPED_TEST(TestAllocator4, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  /* Right merge */
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(q, s);
    x.deallocate(p, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator4, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  /* Left Merge */
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(p, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator4, test_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  /* Merge both */
  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    pointer r = x.allocate(s);
    x.deallocate(r, s);
    x.deallocate(p, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator4, test_4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  /* Check two allocates */
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}
