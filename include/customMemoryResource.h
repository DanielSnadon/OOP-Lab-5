#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class customMemoryResource : public std::pmr::memory_resource {
    private:
        struct memoryBlock {
            size_t offset{0};
            size_t size{0};
        };

        static constexpr size_t BUFFER_SIZE{1024};
        char memoryBuffer[BUFFER_SIZE];
        std::vector<memoryBlock> usedBlocks;
    
    public:
        void* do_allocate(size_t bytes, size_t alignment) override;
        void do_deallocate(void* ptr, size_t bytes, size_t alignment) override;
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};