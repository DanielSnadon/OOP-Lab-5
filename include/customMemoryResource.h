#pragma once

#include <iostream>
#include <memory_resource>
#include <vector>
#include <concepts>
#include <memory>
#include <exception>
#include <algorithm>

// Полиморфный аллокатор
class customMemoryResource : public std::pmr::memory_resource {
    private:
        struct memoryBlock {
            size_t offset{0};
            size_t size{0};

            memoryBlock(size_t o = 0, size_t s = 0) : offset(o), size(s) {}
        };
        
        static constexpr size_t BUFFER_SIZE{1024};
        char memoryBuffer[BUFFER_SIZE];
        std::vector<memoryBlock> usedBlocks;

    public:

        void* do_allocate(size_t bytes, size_t alignment) override
        {
            size_t allocOffset = 0;

            // Сортировка блоков по их расположению
            std::sort(usedBlocks.begin(), usedBlocks.end(), 
            [](const memoryBlock& left, const memoryBlock& right) {
                return left.offset < right.offset;
            });

            // Поиск места перед занятыми блоками
            for (const memoryBlock& block : usedBlocks) {
                if ((allocOffset + bytes <= block.offset) || 
                    (allocOffset >= block.offset + block.size)) {
                    break;
                
                } else {
                    allocOffset = block.offset + block.size;
                }
            }

            if (allocOffset + bytes > BUFFER_SIZE) {
                throw std::bad_alloc();
            }

            usedBlocks.emplace_back(allocOffset, bytes);

            std::cout << "Выделение: смещение: " << allocOffset << ", размер: " << bytes << " байт." << std::endl;
            
            return memoryBuffer + allocOffset;
        }

        void do_deallocate(void* ptr, size_t bytes, size_t alignment) override
        {
            std::cout << "Освобождение: адрес: " << ptr << ", размер: " << bytes << " байт" << std::endl;

            for (size_t i = 0; i < usedBlocks.size(); ++i) {
                
                // Поиск блока для удаления
                if (ptr == memoryBuffer + usedBlocks[i].offset) {
                    usedBlocks.erase(usedBlocks.begin() + i);
                    return;
                }
            }

            throw std::logic_error("Попытка освобождения не выделенного блока.");
        }

        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
        {
            return this == &other;
        }
};