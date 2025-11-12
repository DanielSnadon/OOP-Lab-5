#include "customMemoryResource.h"
#include "forwardList.h"
#include <iostream>

struct Person {
    int id;
    std::string name;
    int age;

    Person(int i = 0, std::string n = "", int a = 0) : id(i), name(n), age(a) {}

    bool operator==(const Person& other) const {
        return id == other.id;
    }
};

int main() {
    std::cout << "INT-Тест" << std::endl;

    customMemoryResource resource;

    auto list = forwardList<int, std::pmr::polymorphic_allocator<int>>(
        std::pmr::polymorphic_allocator<int>{&resource});

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.pushFront(5);
    
    std::cout << "Список: ";
    list.print();

    std::cout << "Размер: " << list.getSize() << std::endl;

    std::cout << "Поиск 20: позиция " << list.find(20) << std::endl;

    list.insert(15, 2);
    std::cout << "Список после вставки: ";
    list.print();

    list.remove(1);
    std::cout << "Список после удаления: ";
    list.print();

    std::cout << "PERSON-Тест" << std::endl;

    auto peopleList = forwardList<Person, std::pmr::polymorphic_allocator<Person>>(
        std::pmr::polymorphic_allocator<Person>{&resource});

    peopleList.pushBack(Person(1, "Alice", 25));
    peopleList.pushBack(Person(2, "Bob", 30));
    peopleList.pushBack(Person(3, "Charlie", 35));

    std::cout << "Список людей:" << std::endl;
    for (const auto& person : peopleList) {
        std::cout << "  ID: " << person.id << ", Name: " << person.name << ", Age: " << person.age << std::endl;
    }
    return 0;
}
