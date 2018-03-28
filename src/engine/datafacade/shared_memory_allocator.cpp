#include "engine/datafacade/shared_memory_allocator.hpp"

#include "storage/serialization.hpp"

#include "util/log.hpp"

#include "boost/assert.hpp"

namespace osrm
{
namespace engine
{
namespace datafacade
{

SharedMemoryAllocator::SharedMemoryAllocator(storage::SharedDataType data_region)
{
    util::Log(logDEBUG) << "Loading new data for region " << regionToString(data_region);

    BOOST_ASSERT(storage::SharedMemory::RegionExists(data_region));
    m_large_memory = storage::makeSharedMemory(data_region);

    storage::io::BufferReader reader(reinterpret_cast<char *>(m_large_memory->Ptr()),
                                     m_large_memory->Size());
    storage::serialization::read(reader, data_layout);
    layout_size = reader.GetPosition();
    util::Log(logDEBUG) << "Data layout has size " << layout_size;
}

SharedMemoryAllocator::~SharedMemoryAllocator() {}

const storage::DataLayout &SharedMemoryAllocator::GetLayout() { return data_layout; }
char *SharedMemoryAllocator::GetMemory()
{
    return reinterpret_cast<char *>(m_large_memory->Ptr()) + layout_size;
}

} // namespace datafacade
} // namespace engine
} // namespace osrm
