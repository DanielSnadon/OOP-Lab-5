#include <gtest/gtest.h>
#include "../include/forwardList.h"
#include "../include/customMemoryResource.h"

struct Person {
    int id;
    std::string name;
    int age;
    
    Person(int i = 0, std::string n = "", int a = 0) 
        : id(i), name(n), age(a) {}
    
    bool operator==(const Person& other) const {
        return id == other.id && name == other.name && age == other.age;
    }
};

TEST(ForwardListTest, BasicOperations) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0);
    
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    
    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 3);
}

TEST(ForwardListTest, PushFrontAndBack) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    list.pushFront(3);
    list.pushFront(2);
    list.pushFront(1);
    list.pushBack(4);
    
    EXPECT_EQ(list.getSize(), 4);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 4);
}

TEST(ForwardListTest, FindAndInsert) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    list.pushBack(10);
    list.pushBack(30);
    
    EXPECT_EQ(list.find(20), -1);
    
    list.insert(20, 1);
    EXPECT_EQ(list.find(20), 1);
    EXPECT_EQ(list.getSize(), 3);
}

TEST(ForwardListTest, RemoveOperations) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    
    list.remove(1);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.find(20), -1);
    
    list.popFront();
    EXPECT_EQ(*list.begin(), 30);
    
    list.popBack();
    EXPECT_TRUE(list.isEmpty());
}

TEST(ForwardListTest, ComplexType) {
    customMemoryResource resource;
    auto list = forwardList<Person, std::pmr::polymorphic_allocator<Person>>(
        std::pmr::polymorphic_allocator<Person>{&resource});
    
    list.pushBack(Person(1, "Alice", 25));
    list.pushBack(Person(2, "Bob", 30));
    
    EXPECT_EQ(list.getSize(), 2);
    
    Person search(2, "Bob", 30);
    EXPECT_EQ(list.find(search), 1);
    
    for (const auto& person : list) {
        EXPECT_TRUE(person.id == 1 || person.id == 2);
    }
}

TEST(ForwardListTest, IteratorFunctionality) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    list.pushBack(100);
    list.pushBack(200);
    list.pushBack(300);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 100);
    ++it;
    EXPECT_EQ(*it, 200);
    it++;
    EXPECT_EQ(*it, 300);
    ++it;
    EXPECT_EQ(it, list.end());
}

TEST(ForwardListTest, ClearAndEmpty) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    
    EXPECT_FALSE(list.isEmpty());
    
    list.clear();
    
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), 0);
}

TEST(ForwardListTest, SimpleStressTest) {
    customMemoryResource resource;
    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});
    
    for (int i = 0; i < 10; ++i) {
        list.pushBack(i);
    }
    
    EXPECT_EQ(list.getSize(), 10);
    
    for (int i = 0; i < 5; ++i) {
        list.remove(0);
    }
    
    EXPECT_EQ(list.getSize(), 5);
    
    int expected = 5;
    for (auto it = list.begin(); it != list.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}