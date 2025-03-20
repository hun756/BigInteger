#include <biginteger/biginteger.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <vector>

using namespace Numerics::detail;

class MemoryManagerTest : public ::testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

class TestObject
{
public:
    TestObject() : value_(0) {}
    explicit TestObject(int value) : value_(value) {}
    ~TestObject() {}

    int getValue() const { return value_; }
    void setValue(int value) { value_ = value; }

private:
    int value_;
};

TEST_F(MemoryManagerTest, AllocateAlignedTest)
{

    int* intPtr = MemoryManager<int>::allocate_aligned(10);
    ASSERT_NE(intPtr, nullptr);

    uintptr_t address = reinterpret_cast<uintptr_t>(intPtr);
    EXPECT_EQ(address % MemoryManager<int>::ALIGNMENT, 0);

    for (size_t i = 0; i < 10; ++i)
    {
        intPtr[i] = static_cast<int>(i);
        EXPECT_EQ(intPtr[i], static_cast<int>(i));
    }

    MemoryManager<int>::deallocate_aligned(intPtr);
}

TEST_F(MemoryManagerTest, AllocateAlignedLargeBlockTest)
{

    constexpr size_t largeSize = 1024 * 1024;
    double* doublePtr = MemoryManager<double>::allocate_aligned(largeSize);
    ASSERT_NE(doublePtr, nullptr);

    uintptr_t address = reinterpret_cast<uintptr_t>(doublePtr);
    EXPECT_EQ(address % MemoryManager<double>::ALIGNMENT, 0);

    MemoryManager<double>::deallocate_aligned(doublePtr);
}

TEST_F(MemoryManagerTest, AllocateAlignedCustomTypeTest)
{

    TestObject* objPtr = MemoryManager<TestObject>::allocate_aligned(5);
    ASSERT_NE(objPtr, nullptr);

    for (size_t i = 0; i < 5; ++i)
    {
        new (&objPtr[i]) TestObject(static_cast<int>(i));
    }

    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(objPtr[i].getValue(), static_cast<int>(i));
    }

    for (size_t i = 0; i < 5; ++i)
    {
        objPtr[i].~TestObject();
    }

    MemoryManager<TestObject>::deallocate_aligned(objPtr);
}

TEST_F(MemoryManagerTest, AllocateAlignedExceptionTest)
{
    constexpr size_t veryLargeSize = std::numeric_limits<size_t>::max() / 2;

    try {
        int* ptr = MemoryManager<int>::allocate_aligned(veryLargeSize);

        if (ptr != nullptr) {
            MemoryManager<int>::deallocate_aligned(ptr);
            SUCCEED() << "System was able to allocate a very large memory block, unexpected but valid";
        }
    } catch(const std::bad_alloc& e) {

        SUCCEED() << "Expected std::bad_alloc exception occurred";
    } catch(const std::exception& e) {

        SUCCEED() << "Another exception occurred: " << e.what();
    }
}

TEST_F(MemoryManagerTest, PoolBasicTest)
{
    MemoryManager<int>::Pool pool;

    auto block = pool.acquire(100);
    ASSERT_NE(block.get(), nullptr);

    for (size_t i = 0; i < 100; ++i)
    {
        block[i] = static_cast<int>(i);
        EXPECT_EQ(block[i], static_cast<int>(i));
    }
}

TEST_F(MemoryManagerTest, PoolMultipleBlocksTest)
{
    MemoryManager<int>::Pool pool(2);

    auto block1 = pool.acquire(MemoryManager<int>::BLOCK_SIZE / 2);
    ASSERT_NE(block1.get(), nullptr);

    auto block2 = pool.acquire(MemoryManager<int>::BLOCK_SIZE / 2);
    ASSERT_NE(block2.get(), nullptr);

    auto block3 = pool.acquire(MemoryManager<int>::BLOCK_SIZE / 2);
    ASSERT_NE(block3.get(), nullptr);

    block1[0] = 100;
    block2[0] = 200;
    block3[0] = 300;

    EXPECT_EQ(block1[0], 100);
    EXPECT_EQ(block2[0], 200);
    EXPECT_EQ(block3[0], 300);
}

TEST_F(MemoryManagerTest, PoolReuseBlockTest)
{
    MemoryManager<int>::Pool pool(1);

    {
        auto block = pool.acquire(100);
        ASSERT_NE(block.get(), nullptr);
        block[0] = 42;
    }

    auto reusedBlock = pool.acquire(100);
    ASSERT_NE(reusedBlock.get(), nullptr);

    reusedBlock[0] = 55;
    EXPECT_EQ(reusedBlock[0], 55);
}

TEST_F(MemoryManagerTest, PoolCustomSizeBlockTest)
{
    MemoryManager<int>::Pool pool;

    constexpr size_t largeBlockSize = MemoryManager<int>::BLOCK_SIZE * 2;
    auto largeBlock = pool.acquire(largeBlockSize);
    ASSERT_NE(largeBlock.get(), nullptr);

    for (size_t i = 0; i < largeBlockSize; ++i)
    {
        largeBlock[i] = static_cast<int>(i);
    }

    for (size_t i = 0; i < largeBlockSize; ++i)
    {
        EXPECT_EQ(largeBlock[i], static_cast<int>(i));
    }
}

TEST_F(MemoryManagerTest, PoolThreadSafetyTest)
{
    constexpr int numThreads = 8;
    constexpr int operationsPerThread = 100;

    MemoryManager<int>::Pool pool(numThreads);
    std::atomic<int> successCount(0);

    auto threadFunc = [&pool, &successCount]()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> blockSizeDist(10, MemoryManager<int>::BLOCK_SIZE);

        for (int i = 0; i < operationsPerThread; ++i)
        {
            size_t blockSize = blockSizeDist(gen);
            try
            {
                auto block = pool.acquire(blockSize);
                EXPECT_NE(block.get(), nullptr);

                for (size_t j = 0; j < std::min(blockSize, size_t(10)); ++j)
                {
                    block[j] = static_cast<int>(j);
                    EXPECT_EQ(block[j], static_cast<int>(j));
                }

                std::this_thread::sleep_for(std::chrono::microseconds(50));

                successCount++;
            }
            catch (const std::exception& e)
            {
                ADD_FAILURE() << "Exception in thread: " << e.what();
            }
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(threadFunc);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    EXPECT_EQ(successCount.load(), numThreads * operationsPerThread);
}

TEST_F(MemoryManagerTest, PoolStressTest)
{
    constexpr int numOperations = 1000;

    MemoryManager<int>::Pool pool(4);
    std::vector<std::unique_ptr<int[], typename MemoryManager<int>::Pool::BlockDeleter>> blocks;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> blockSizeDist(10, MemoryManager<int>::BLOCK_SIZE);
    std::uniform_int_distribution<> operationDist(0, 1);

    for (int i = 0; i < numOperations; ++i)
    {
        int operation = operationDist(gen);

        if (operation == 0 || blocks.empty())
        {

            size_t blockSize = blockSizeDist(gen);
            try
            {
                auto block = pool.acquire(blockSize);
                ASSERT_NE(block.get(), nullptr);

                for (size_t j = 0; j < blockSize; ++j)
                {
                    block[j] = static_cast<int>(j * i);
                }

                blocks.push_back(std::move(block));
            }
            catch (const std::exception& e)
            {
                ADD_FAILURE() << "Exception during acquire: " << e.what();
            }
        }
        else
        {

            std::uniform_int_distribution<> blockIdxDist(0, blocks.size() - 1);
            size_t blockIdx = blockIdxDist(gen);
            blocks.erase(blocks.begin() + blockIdx);
        }
    }

    blocks.clear();
}

TEST_F(MemoryManagerTest, BlockDeleterTest)
{
    MemoryManager<TestObject>::Pool pool(1);

    static std::atomic<int> objectCount(0);

    class CountedTestObject
    {
    public:
        CountedTestObject() { objectCount++; }
        ~CountedTestObject() { objectCount--; }

        int getValue() const { return value_; }
        void setValue(int value) { value_ = value; }

    private:
        int value_ = 0;
    };

    {
        auto block = pool.acquire(10);
        ASSERT_NE(block.get(), nullptr);

        for (size_t i = 0; i < 10; ++i)
        {
            new (&block[i]) CountedTestObject();
            block[i].setValue(static_cast<int>(i));
        }

        EXPECT_EQ(objectCount.load(), 10);

        EXPECT_EQ(block[5].getValue(), 5);
    }

    auto reusedBlock = pool.acquire(10);
    ASSERT_NE(reusedBlock.get(), nullptr);
}

TEST_F(MemoryManagerTest, MemoryLeakTest)
{

    for (int i = 0; i < 100; ++i)
    {
        MemoryManager<double>::Pool pool(10);

        std::vector<std::unique_ptr<double[], typename MemoryManager<double>::Pool::BlockDeleter>>
            blocks;

        for (int j = 0; j < 20; ++j)
        {
            blocks.push_back(pool.acquire(MemoryManager<double>::BLOCK_SIZE / 2));
        }

        for (size_t j = 0; j < blocks.size(); j += 2)
        {
            blocks[j].reset();
        }
    }
}

TEST_F(MemoryManagerTest, DISABLED_PerformanceTest)
{

    constexpr int numIterations = 100000;
    constexpr size_t blockSize = 128;

    auto startTime = std::chrono::high_resolution_clock::now();

    MemoryManager<int>::Pool pool;
    for (int i = 0; i < numIterations; ++i)
    {
        auto block = pool.acquire(blockSize);
        block[0] = i;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Pool performance: " << numIterations << " allocations took " << duration << "ms"
              << std::endl;

    startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numIterations; ++i)
    {
        std::unique_ptr<int[]> block(new int[blockSize]);
        block[0] = i;
    }

    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Standard allocator: " << numIterations << " allocations took " << duration << "ms"
              << std::endl;
}