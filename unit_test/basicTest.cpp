#include <gtest/gtest.h>
#include <mld-core.h>

class TestObj {
public:
  int n;
};

TEST(basicTest, alloc1)
{
  int allocations = mld_get_number_of_leaks();
  TestObj* pObj = new TestObj();
  ASSERT_EQ(mld_get_number_of_leaks(), allocations + 1);
  delete pObj;
  ASSERT_EQ(mld_get_number_of_leaks(), allocations);
}

TEST(basicTest, stackDump)
{
  TestObj* pObj = new TestObj();
  mld_dump_leaks();
}

TEST(basicTest, deleteError)
{
  delete ((void*)0x56575656);
}