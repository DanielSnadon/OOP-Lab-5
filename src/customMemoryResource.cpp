#include "customMemoryResource.h"

void* customMemoryResource::do_allocate(size_t bytes, size_t alignment)
{
    size_t allocOffset = 0;

    std::sort(usedBlocks.begin(), usedBlocks.end(), 
    [](const memoryBlock& left, const memoryBlock& right) {
        return left.offset < right.offset;
    });

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

void customMemoryResource::do_deallocate(void* ptr, size_t bytes, size_t alignment)
{
    std::cout << "Освобождение: адрес: " << ptr << ", размер: " << bytes << " байт" << std::endl;

    for (size_t i = 0; i < usedBlocks.size(); ++i) {
        
        if (ptr == memoryBuffer + usedBlocks[i].offset) {
            usedBlocks.erase(usedBlocks.begin() + i);
            return;
        }
    }

    throw std::logic_error("Попытка освобождения не выделенного блока.");
}

bool customMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept
{
    return this == &other;
}