#include <iostream>
#include <vector>

class customMemoryResource : public std::pmr::memory_resource {
    protected:
        void* do_allocate(size_t bytes, size_t alignment) override;

        void do_deallocate(void* ptr, size_t bytes, size_t alignment) override;

        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

    private:
        void* mainBlock;
        size_t mainSize;

        struct block {
            void* address;
            size_t size;
            bool isFree;
        };

        std::vector<block> blocks;


};

int main() {

    return 0;
}