#include <lib/ccircularbuffer.h>
#include <lib/ccircularbufferext.h>
#include <gtest/gtest.h>



TEST(StlAlgoTestSuite, SortTest) {
    CCircularBuffer<int> buffer(10);
    buffer.push_back(3);
    buffer.push_back(5);
    buffer.push_back(4);
    buffer.push_back(1);
    buffer.push_back(2);
    std::sort(buffer.begin(), buffer.end());

    for (size_t i = 1; i < 5; ++i) {
        ASSERT_EQ(buffer[i - 1], i);
    }
}

TEST(StaticTestSuite, Constructors1) {
    CCircularBuffer<int> buffer(15);

    ASSERT_EQ(buffer.capacity(), 15);
    ASSERT_EQ(buffer.size(), 0);
}

TEST(StaticTestSuite, Constructors2) {
    CCircularBuffer<char> buffer1(4, 'q');
    CCircularBuffer<char> buffer2(buffer1);

    ASSERT_EQ(buffer1.size(), 4);
    ASSERT_EQ(buffer2.size(), 4);

    for (auto it1 = buffer1.begin(), it2 = buffer2.begin();
         it1 != buffer1.end(); ++it1, ++it2) {
        ASSERT_EQ(*it1, 'q');
        ASSERT_EQ(*it2, 'q');
    }
}

TEST(StaticTestSuite, Constructors3) {
    CCircularBuffer<std::string> buffer({"first", "second", "third"});
    ASSERT_EQ(buffer.size(), 3);
    ASSERT_EQ(buffer.capacity(), 3);
    ASSERT_EQ(buffer[0], "first");
    ASSERT_EQ(buffer[1], "second");
    ASSERT_EQ(buffer[2], "third");
}

TEST(StaticTestSuite, Constructors4) {
    CCircularBuffer<int> buffer1 = {1, 2, 3, 4, 5};
    CCircularBuffer<int> buffer2(buffer1.cbegin() + 1, buffer1.cend());

    ASSERT_EQ(buffer2.size(), 4);
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer2[i], i + 2);
    }
}


TEST(StaticTestSuite, Assignment) {
    CCircularBuffer<int> buffer1 = {1, 2, 3, 4, 5, 6};
    CCircularBuffer<int> buffer2 = buffer1;

    for (int i = 0; i < buffer1.size(); ++i) {
        ASSERT_EQ(buffer1[i], i + 1);
        ASSERT_EQ(buffer2[i], i + 1);
    }
}

TEST(StaticTestSuite, Empty) {
    CCircularBuffer<int> buffer(10);

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.empty());
}

TEST(StaticTestSuite, Compare) {
    CCircularBuffer<int> buffer1 = {1, 2, 3, 4, 5};
    CCircularBuffer<int> buffer2 = {10, 10, 10};
    CCircularBuffer<int> buffer3 = {1, 2, 3, 4, 5};

    ASSERT_TRUE(buffer1 != buffer2);
    ASSERT_TRUE(buffer1 == buffer3);
}

TEST(StaticTestSuite, FrontBack) {
    CCircularBuffer<int> buffer = {1, 2, 3};

    ASSERT_EQ(buffer.front(), 1);
    ASSERT_EQ(buffer.back(), 3);
}

TEST(StaticTestSuite, Swap) {
    CCircularBuffer<int> buffer1 = {1, 2, 3};
    CCircularBuffer<int> buffer2 = {4, 5, 6, 7};

    CCircularBuffer<int> check1 = {4, 5, 6, 7};
    CCircularBuffer<int> check2 = {1, 2, 3};

    buffer1.swap(buffer2);
    ASSERT_TRUE(buffer1 == check1);
    ASSERT_TRUE(buffer2 == check2);
}

TEST(StaticTestSuite, Clear) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.clear();
    ASSERT_EQ(buffer.size(), 0);
    ASSERT_EQ(buffer.capacity(), 3);
}


TEST(StaticTestSuite, PushBack1) {
    CCircularBuffer<int> buffer(10);
    for (int i = 0; i < 5; ++i) {
        buffer.push_back(i);
    }

    int i = 0;
    for (CCircularBuffer<int>::iterator it = buffer.begin();
         it != buffer.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 10);
}

TEST(StaticTestSuite, PushBack2) {
    CCircularBuffer<int> buffer(4);
    for (int i = 0; i < 12; ++i) {
        buffer.push_back(i);
    }

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer[i], i + 8);
    }

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 4);
    ASSERT_EQ(buffer.empty(), false);
}

TEST(StaticTestSuite, PushBack3) {
    CCircularBuffer<int> buffer(5);
    for (int i = 0; i < 12; ++i) {
        buffer.push_back(i);
    }

    ASSERT_EQ(buffer[0], 7);
    ASSERT_EQ(buffer[1], 8);
    ASSERT_EQ(buffer[2], 9);
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 11);


    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
    ASSERT_EQ(buffer.empty(), false);
}


TEST(StaticTestSuite, PushFront1) {
    CCircularBuffer<int> buffer(10);
    for (int i = 0; i < 5; ++i) {
        buffer.push_front(i);
    }

    int i = 4;
    for (CCircularBuffer<int>::iterator it = buffer.begin();
         it != buffer.end(); ++it, --i) {
        ASSERT_EQ(*it, i);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 10);
}

TEST(StaticTestSuite, PushFront2) {
    CCircularBuffer<int> buffer(4);
    for (int i = 0; i < 12; ++i) {
        buffer.push_front(i);
    }

    ASSERT_EQ(buffer[0], 11);
    ASSERT_EQ(buffer[1], 10);
    ASSERT_EQ(buffer[2], 9);
    ASSERT_EQ(buffer[3], 8);

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 4);
}

TEST(StaticTestSuite, PushFront3) {
    CCircularBuffer<int> buffer(5);
    for (int i = 0; i < 12; ++i) {
        buffer.push_front(i);
    }

    ASSERT_EQ(buffer[0], 11);
    ASSERT_EQ(buffer[1], 10);
    ASSERT_EQ(buffer[2], 9);
    ASSERT_EQ(buffer[3], 8);
    ASSERT_EQ(buffer[4], 7);


    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
    ASSERT_EQ(buffer.empty(), false);

}


TEST(StaticTestSuite, PopFront1) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4, 5, 6};
    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    for (int i = 4; i < 7; ++i) {
        ASSERT_EQ(buffer[i - 4], i);
    }

    ASSERT_TRUE(buffer.size() == 3);
    ASSERT_TRUE(buffer.capacity() == 6);
}

TEST(StaticTestSuite, PopFront2) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.capacity() == 3);
    ASSERT_TRUE(buffer.empty() == true);
}

TEST(StaticTestSuite, PopBack1) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4, 5, 6};
    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }

    ASSERT_TRUE(buffer.size() == 3);
    ASSERT_TRUE(buffer.capacity() == 6);
}

TEST(StaticTestSuite, PopBack2) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.capacity() == 3);
    ASSERT_TRUE(buffer.empty() == true);
}


TEST(StaticTestSuite, PushAndPop1) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4};
    buffer.pop_front();
    buffer.pop_front();
    buffer.push_back(5);
    buffer.push_back(6);

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer[i], i + 3);
    }
}

TEST(StaticTestSuite, PushAndPop2) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4, 5};
    buffer.pop_front(); // 2, 3, 4, 5
    buffer.push_front(7); // 7, 2, 3, 4, 5
    buffer.pop_back(); // 7, 2, 3, 4
    buffer.push_front(1); // 1, 7, 2, 3, 4

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 7);
    ASSERT_EQ(buffer[2], 2);
    ASSERT_EQ(buffer[3], 3);
    ASSERT_EQ(buffer[4], 4);
}

TEST(StaticTestSuite, PushAndPop3) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4};

    for (int i = 0; i < 4; ++i) {
        buffer.pop_back();
    }

    for (int i = 0; i < 4; ++i) {
        buffer.push_front(i);
    }

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer[i], 3 - i);
    }
}


TEST(StaticTestSuite, AssignInitList) {
    CCircularBuffer<int> buffer(10);
    buffer.assign({1, 2, 3, 4, 5, 6});
    for (int i = 0; i < buffer.size(); ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }

    ASSERT_EQ(buffer.size(), 6);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(StaticTestSuite, AssignValueCopies) {
    CCircularBuffer<int> buffer(10);
    buffer.assign(5, 2);

    for (auto e: buffer) {
        ASSERT_EQ(e, 2);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
    ASSERT_EQ(buffer.empty(), false);
}


TEST(StaticTestSuite, EraseOne) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4, 5, 6};
    CCircularBuffer<int>::const_iterator it = (buffer.cbegin() + 4);
    buffer.erase(it);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 4);
    ASSERT_EQ(buffer[4], 6);

    it = (buffer.cbegin());
    buffer.erase(it);

    ASSERT_EQ(buffer[0], 2);
    ASSERT_EQ(buffer[1], 3);
    ASSERT_EQ(buffer[2], 4);
    ASSERT_EQ(buffer[3], 6);

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(StaticTestSuite, EraseMany) {
    CCircularBuffer<int> buffer = {1, 2, 3, 4, 5, 6, 7, 8};
    CCircularBuffer<int>::const_iterator lhs = (buffer.cbegin() + 3);
    CCircularBuffer<int>::const_iterator rhs = (buffer.cbegin() + 7);

    buffer.erase(lhs, rhs);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 8);

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 8);
}


TEST(BufferStaticTestSuite, Insert) {
    CCircularBuffer<int> buffer(10);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }
    buffer.insert(buffer.cbegin() + 2, 7);
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 7);
    ASSERT_EQ(buffer[3], 3);
    ASSERT_EQ(buffer[4], 4);
    ASSERT_EQ(buffer[5], 5);

    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.size(), 6);
}

TEST(BufferStaticTestSuite, InsertMany) {
    CCircularBuffer<int> buffer(10);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    buffer.insert(buffer.cbegin() + 3, 2, 10);
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.size(), 7);
}

TEST(BufferStaticTestSuite, InsertIterators) {
    CCircularBuffer<int> buffer(10);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    CCircularBuffer<int> buffer1 = {7, 8};
    buffer.insert(buffer.cbegin() + 2, buffer1.cbegin(), buffer1.cend());
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 7);
    ASSERT_EQ(buffer[3], 8);
    ASSERT_EQ(buffer[4], 3);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.size(), 7);
}

TEST(BufferStaticTestSuite, InsertList) {
    CCircularBuffer<int> buffer(10);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    buffer.insert(buffer.cbegin(), {7, 8});
    ASSERT_EQ(buffer[0], 7);
    ASSERT_EQ(buffer[1], 8);
    ASSERT_EQ(buffer[2], 1);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 3);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.size(), 7);
}


TEST(BufferStaticTestSuite, Reserve) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.reserve(6);
    for (size_t i = 0; i < 3; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer.size(), 3);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(BufferStaticTestSuite, Resize1) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.resize(2);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer.size(), 2);
    ASSERT_EQ(buffer.capacity(), 3);
}

TEST(BufferStaticTestSuite, Resize2) {
    CCircularBuffer<int> buffer = {1, 2, 3};
    buffer.resize(5, 10);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
}

TEST(BufferStaticTestSuite, Resize3) {
    CCircularBuffer<int> buffer(10);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    buffer.resize(6, 10);

    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);
    ASSERT_EQ(buffer[5], 10);

    ASSERT_EQ(buffer.size(), 6);
    ASSERT_EQ(buffer.capacity(), 10);
}








// ------------- Dynamic --------------


TEST(DynamicTestSuite, Constructors1) {
    CCircularBufferExt<int> buffer(15);

    ASSERT_EQ(buffer.capacity(), 15);
    ASSERT_EQ(buffer.size(), 0);
}

TEST(DynamicTestSuite, Constructors2) {
    CCircularBufferExt<char> buffer1(4, 'q');
    CCircularBufferExt<char> buffer2(buffer1);

    ASSERT_EQ(buffer1.size(), 4);
    ASSERT_EQ(buffer2.size(), 4);

    for (auto it1 = buffer1.begin(), it2 = buffer2.begin();
         it1 != buffer1.end(); ++it1, ++it2) {
        ASSERT_EQ(*it1, 'q');
        ASSERT_EQ(*it2, 'q');
    }
}

TEST(DynamicTestSuite, Constructors3) {
    CCircularBufferExt<std::string> buffer({"first", "second", "third"});
    ASSERT_EQ(buffer.size(), 3);
    ASSERT_EQ(buffer.capacity(), 3);
    ASSERT_EQ(buffer[0], "first");
    ASSERT_EQ(buffer[1], "second");
    ASSERT_EQ(buffer[2], "third");
}

TEST(DynamicTestSuite, Constructors4) {
    CCircularBufferExt<int> buffer1 = {1, 2, 3, 4, 5};
    CCircularBufferExt<int> buffer2(buffer1.cbegin() + 1, buffer1.cend());

    ASSERT_EQ(buffer2.size(), 4);
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer2[i], i + 2);
    }
}


TEST(DynamicTestSuite, Assignment) {
    CCircularBufferExt<int> buffer1 = {1, 2, 3, 4, 5, 6};
    CCircularBufferExt<int> buffer2 = buffer1;

    for (int i = 0; i < buffer1.size(); ++i) {
        ASSERT_EQ(buffer1[i], i + 1);
        ASSERT_EQ(buffer2[i], i + 1);
    }
}

TEST(DynamicTestSuite, Empty) {
    CCircularBufferExt<int> buffer(10);

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.empty());
}

TEST(DynamicTestSuite, Compare) {
    CCircularBufferExt<int> buffer1 = {1, 2, 3, 4, 5};
    CCircularBufferExt<int> buffer2 = {10, 10, 10};
    CCircularBufferExt<int> buffer3 = {1, 2, 3, 4, 5};

    ASSERT_TRUE(buffer1 != buffer2);
    ASSERT_TRUE(buffer1 == buffer3);
}

TEST(DynamicTestSuite, FrontBack) {
    CCircularBufferExt<int> buffer = {1, 2, 3};

    ASSERT_EQ(buffer.front(), 1);
    ASSERT_EQ(buffer.back(), 3);
}

TEST(DynamicTestSuite, Swap) {
    CCircularBufferExt<int> buffer1 = {1, 2, 3};
    CCircularBufferExt<int> buffer2 = {4, 5, 6, 7};

    CCircularBufferExt<int> check1 = {4, 5, 6, 7};
    CCircularBufferExt<int> check2 = {1, 2, 3};

    buffer1.swap(buffer2);
    ASSERT_TRUE(buffer1 == check1);
    ASSERT_TRUE(buffer2 == check2);
}

TEST(DynamicTestSuite, Clear) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.clear();
    ASSERT_EQ(buffer.size(), 0);
    ASSERT_EQ(buffer.capacity(), 3);
}


TEST(DynamicTestSuite, PushBack1) {
    CCircularBufferExt<int> buffer(10);
    for (int i = 0; i < 5; ++i) {
        buffer.push_back(i);
    }

    int i = 0;
    for (CCircularBufferExt<int>::iterator it = buffer.begin();
         it != buffer.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 10);
}

TEST(DynamicTestSuite, PushBack2) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.push_back(6);
    buffer.push_back(7);

    for (int i = 0; i < 7; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }

    ASSERT_EQ(buffer.size(), 7);
    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.empty(), false);
}

TEST(DynamicTestSuite, PushBack3) {
    CCircularBufferExt<int> buffer(5);
    for (int i = 0; i < 12; ++i) {
        buffer.push_back(i);
    }

    for (int i = 0; i < 12; ++i) {
        ASSERT_EQ(buffer[i], i);
    }

    ASSERT_EQ(buffer.size(), 12);
    ASSERT_EQ(buffer.capacity(), 20);
    ASSERT_EQ(buffer.empty(), false);
}


TEST(DynamicTestSuite, PushFront1) {
    CCircularBufferExt<int> buffer(10);
    for (int i = 0; i < 5; ++i) {
        buffer.push_front(i);
    }

    int i = 4;
    for (CCircularBufferExt<int>::iterator it = buffer.begin();
         it != buffer.end(); ++it, --i) {
        ASSERT_EQ(*it, i);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 10);
}

TEST(DynamicTestSuite, PushFront2) {
    CCircularBufferExt<int> buffer(4);
    for (int i = 0; i < 12; ++i) {
        buffer.push_front(i);
    }

    for (int i = 0; i < 12; ++i) {
        ASSERT_EQ(buffer[i], 11 - i);
    }

    ASSERT_EQ(buffer.size(), 12);
    ASSERT_EQ(buffer.capacity(), 16);
}

TEST(DynamicTestSuite, PushFront3) {
    CCircularBufferExt<int> buffer(5);
    for (int i = 0; i < 12; ++i) {
        buffer.push_front(i);
    }

    for (int i = 0; i < 12; ++i) {
        ASSERT_EQ(buffer[i], 11 - i);
    }

    ASSERT_EQ(buffer.size(), 12);
    ASSERT_EQ(buffer.capacity(), 20);
}


TEST(DynamicTestSuite, PopFront1) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5, 6};
    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    for (int i = 4; i < 7; ++i) {
        ASSERT_EQ(buffer[i - 4], i);
    }

    ASSERT_TRUE(buffer.size() == 3);
    ASSERT_TRUE(buffer.capacity() == 6);
}

TEST(DynamicTestSuite, PopFront2) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.pop_front();
    buffer.pop_front();
    buffer.pop_front();

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.capacity() == 3);
    ASSERT_TRUE(buffer.empty() == true);
}

TEST(DynamicTestSuite, PopBack1) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5, 6};
    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }

    ASSERT_TRUE(buffer.size() == 3);
    ASSERT_TRUE(buffer.capacity() == 6);
}

TEST(DynamicTestSuite, PopBack2) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.pop_back();
    buffer.pop_back();
    buffer.pop_back();

    ASSERT_TRUE(buffer.size() == 0);
    ASSERT_TRUE(buffer.capacity() == 3);
    ASSERT_TRUE(buffer.empty() == true);
}


TEST(DynamicTestSuite, PushAndPop1) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4};
    buffer.pop_front(); // 2, 3, 4
    buffer.pop_back(); // 2, 3
    buffer.push_back(5); // 2, 3, 5
    buffer.push_back(6); // 2, 3, 5, 6
    buffer.push_back(10); // 2, 3, 5, 6, 10

    ASSERT_EQ(buffer[0], 2);
    ASSERT_EQ(buffer[1], 3);
    ASSERT_EQ(buffer[2], 5);
    ASSERT_EQ(buffer[3], 6);
    ASSERT_EQ(buffer[4], 10);

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 8);
}

TEST(DynamicTestSuite, PushAndPop2) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5};
    buffer.pop_front(); // 2, 3, 4, 5
    buffer.push_front(7); // 7, 2, 3, 4, 5
    buffer.pop_back(); // 7, 2, 3, 4
    buffer.push_front(1); // 1, 7, 2, 3, 4

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 7);
    ASSERT_EQ(buffer[2], 2);
    ASSERT_EQ(buffer[3], 3);
    ASSERT_EQ(buffer[4], 4);
}

TEST(DynamicTestSuite, PushAndPop3) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4};
    buffer.push_back(5);
    buffer.pop_front(); // 2, 3, 4, 5

    for (int i = 0; i < 4; ++i) {
        buffer.pop_back();
    }

    for (int i = 0; i < 4; ++i) {
        buffer.push_front(i);
    }

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(buffer[i], 3 - i);
    }

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 8);
}


TEST(DynamicTestSuite, AssignInitList) {
    CCircularBufferExt<int> buffer(10);
    buffer.assign({1, 2, 3, 4, 5, 6});
    for (int i = 0; i < buffer.size(); ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }

    ASSERT_EQ(buffer.size(), 6);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(DynamicTestSuite, AssignValueCopies) {
    CCircularBufferExt<int> buffer(10);
    buffer.assign(5, 2);

    for (auto e: buffer) {
        ASSERT_EQ(e, 2);
    }

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
    ASSERT_EQ(buffer.empty(), false);
}


TEST(DynamicTestSuite, EraseOne) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5, 6};
    CCircularBufferExt<int>::const_iterator it = (buffer.cbegin() + 4);
    buffer.erase(it);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 4);
    ASSERT_EQ(buffer[4], 6);

    it = (buffer.cbegin());
    buffer.erase(it);

    ASSERT_EQ(buffer[0], 2);
    ASSERT_EQ(buffer[1], 3);
    ASSERT_EQ(buffer[2], 4);
    ASSERT_EQ(buffer[3], 6);

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(DynamicTestSuite, EraseMany) {
    CCircularBufferExt<int> buffer = {1, 2, 3, 4, 5, 6, 7, 8};
    CCircularBufferExt<int>::const_iterator lhs = (buffer.cbegin() + 3);
    CCircularBufferExt<int>::const_iterator rhs = (buffer.cbegin() + 7);

    buffer.erase(lhs, rhs);

    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 8);

    ASSERT_EQ(buffer.size(), 4);
    ASSERT_EQ(buffer.capacity(), 8);
}


TEST(DynamicTestSuite, Insert) {
    CCircularBufferExt<int> buffer(5);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }
    buffer.insert(buffer.cbegin() + 2, 7);
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 7);
    ASSERT_EQ(buffer[3], 3);
    ASSERT_EQ(buffer[4], 4);
    ASSERT_EQ(buffer[5], 5);

    ASSERT_EQ(buffer.capacity(), 10);
    ASSERT_EQ(buffer.size(), 6);
}

TEST(DynamicTestSuite, InsertMany) {
    CCircularBufferExt<int> buffer(5);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    buffer.insert(buffer.cbegin() + 3, 2, 10);
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 7);
    ASSERT_EQ(buffer.size(), 7);
}

TEST(DynamicTestSuite, InsertIterators) {
    CCircularBufferExt<int> buffer(5);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    CCircularBuffer<int> buffer1 = {7, 8};
    buffer.insert(buffer.cbegin() + 2, buffer1.cbegin(), buffer1.cend());
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 7);
    ASSERT_EQ(buffer[3], 8);
    ASSERT_EQ(buffer[4], 3);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 7);
    ASSERT_EQ(buffer.size(), 7);
}

TEST(DynamicTestSuite, InsertList) {
    CCircularBufferExt<int> buffer(6);
    for (int i = 1; i < 6; ++i) {
        buffer.push_back(i);
    }

    buffer.insert(buffer.cbegin(), {7, 8});
    ASSERT_EQ(buffer[0], 7);
    ASSERT_EQ(buffer[1], 8);
    ASSERT_EQ(buffer[2], 1);
    ASSERT_EQ(buffer[3], 2);
    ASSERT_EQ(buffer[4], 3);
    ASSERT_EQ(buffer[5], 4);
    ASSERT_EQ(buffer[6], 5);

    ASSERT_EQ(buffer.capacity(), 7);
    ASSERT_EQ(buffer.size(), 7);
}



TEST(DynamicTestSuite, Reserve1) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.reserve(6);
    for (size_t i = 0; i < 3; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer.size(), 3);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(DynamicTestSuite, Reserve2) {
    CCircularBufferExt<int> buffer;
    buffer.reserve(6);
    for (size_t i = 0; i < 5; ++i) {
        buffer.push_back(i);
    }
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(buffer[i], i);
    }
    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 6);
}

TEST(DynamicTestSuite, Resize1) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.resize(2);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer.size(), 2);
    ASSERT_EQ(buffer.capacity(), 3);
}

TEST(DynamicTestSuite, Resize2) {
    CCircularBufferExt<int> buffer = {1, 2, 3};
    buffer.resize(5, 10);
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);

    ASSERT_EQ(buffer.size(), 5);
    ASSERT_EQ(buffer.capacity(), 5);
}

TEST(DynamicTestSuite, Resize3) {
    CCircularBufferExt<int> buffer(10);
    buffer.push_back(1);
    buffer.push_back(2);
    buffer.push_back(3);

    buffer.resize(6, 10);

    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(buffer[i], i + 1);
    }
    ASSERT_EQ(buffer[3], 10);
    ASSERT_EQ(buffer[4], 10);
    ASSERT_EQ(buffer[5], 10);

    ASSERT_EQ(buffer.size(), 6);
    ASSERT_EQ(buffer.capacity(), 10);
}