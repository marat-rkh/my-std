#include "gtest/gtest.h"
#include "shared_ptr.h"

#include <string>
using std::string;

TEST(shared_ptr, should_be_created_null) {
    SharedPtr<int> sp;
    ASSERT_FALSE(sp);
    ASSERT_EQ(sp.use_count(), 0L);

    SharedPtr<int> sp2(nullptr);
    ASSERT_FALSE(sp2);
    ASSERT_EQ(sp2.use_count(), 0L);
}

TEST(shared_ptr, should_be_created_from_ptr) {
    SharedPtr<int> sp(new int(10));
    ASSERT_TRUE(sp);
    ASSERT_EQ(sp.use_count(), 1L);
    ASSERT_EQ(*sp, 10);
    sp.~SharedPtr();
    ASSERT_FALSE(sp);
    ASSERT_EQ(sp.use_count(), 0L);
}

int delete_count = 0;

void FuncDeleter(int *p) {
    delete p;
    ++delete_count;
}

struct FuncObjDeleter {
    void operator()(int *p) {
        delete p;
        ++delete_count;
    }
};

TEST(shared_ptr, should_be_created_with_deleter) {
    SharedPtr<int> sp1(new int(10), &FuncDeleter);
    sp1.~SharedPtr();
    ASSERT_EQ(delete_count, 1);
    SharedPtr<int> sp2(new int(11), FuncObjDeleter());
    sp2.~SharedPtr();
    ASSERT_EQ(delete_count, 2);
    SharedPtr<int> sp3(new int(12), [](int *p) -> void {
        delete p;
        ++delete_count;
    });
    sp3.~SharedPtr();
    ASSERT_EQ(delete_count, 3);
}

TEST(shared_ptr, should_be_copied) {
    SharedPtr<int> sp1(new int(10));
    ASSERT_EQ(sp1.use_count(), 1L);
    SharedPtr<int> sp2(sp1);
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 2L);
    ASSERT_EQ(sp1.use_count(), 2L);
    sp1.~SharedPtr();
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp2.~SharedPtr();
    ASSERT_FALSE(sp2);
    ASSERT_EQ(sp2.use_count(), 0L);
}

TEST(shared_ptr, should_be_moved) {
    SharedPtr<int> sp1(new int(10));
    ASSERT_EQ(sp1.use_count(), 1L);
    SharedPtr<int> sp2(std::move(sp1));
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp1.~SharedPtr();
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp2.~SharedPtr();
    ASSERT_FALSE(sp2);
    ASSERT_EQ(sp2.use_count(), 0L);
}

TEST(shared_ptr, should_be_copy_assigned) {
    SharedPtr<int> sp1(new int(10));
    SharedPtr<int> sp2(new int(20));
    sp2 = sp1;
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 2L);
    ASSERT_EQ(sp1.use_count(), 2L);
    sp1.~SharedPtr();
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp2.~SharedPtr();
    ASSERT_FALSE(sp2);
    ASSERT_EQ(sp2.use_count(), 0L);
    ASSERT_EQ(sp2.use_count(), 0L);
}

TEST(shared_ptr, should_be_copy_moved) {
    SharedPtr<int> sp1(new int(10));
    SharedPtr<int> sp2(new int(20));
    sp2 = std::move(sp1);
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp1.~SharedPtr();
    ASSERT_TRUE(sp2);
    ASSERT_EQ(*sp2, 10);
    ASSERT_EQ(sp2.use_count(), 1L);
    sp2.~SharedPtr();
    ASSERT_FALSE(sp2);
    ASSERT_EQ(sp2.use_count(), 0L);
}

TEST(shared_ptr, should_be_reset) {
    SharedPtr<int> sp(new int(10));
    ASSERT_EQ(*sp, 10);
    sp.reset(new int(20));
    ASSERT_EQ(*sp, 20);
    sp.reset(nullptr);
    ASSERT_FALSE(sp);

    delete_count = 0;
    sp.reset(new int(30), &FuncDeleter);
    sp.reset(new int(40), [](int *p) {
        delete p;
        delete_count += 2;
    });
    ASSERT_EQ(delete_count, 1);
    sp.reset(nullptr);
    ASSERT_EQ(delete_count, 3);
}

TEST(shared_ptr, should_be_created_by_MakeShared) {
    SharedPtr<string> sp = MakeShared<string>(3, 'c');
    ASSERT_TRUE(sp);
    ASSERT_EQ(*sp, string("ccc"));
    ASSERT_EQ(sp.use_count(), 1L);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}