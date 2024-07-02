#include "cpu/smt_policies/partitioned.hh"
#include "debug/SMTPolicies.hh"

PartitionedSMTPolicy::PartitionedSMTPolicy(const Params* p)
    : BaseSMTPolicy(p), part1(p->part1), part2(p->part2)
{
    assert(numThreads);
    DPRINTF(SMTPolicies, "PartitionedSMTPolicy constructor: part1=%f part2=%f\n", part1, part2);

    occupiedEntries = new int[numThreads];
    for (int i = 0; i < numThreads; i++) {
        occupiedEntries[i] = 0;
    }
}

// HUY
void PartitionedSMTPolicy::setPartition(float p1, float p2) {
    part1 = p1;
    part2 = p2;
    // Ensure the partitions sum to 1
    assert(part1 + part2 == 1.0);
}


void
PartitionedSMTPolicy::consume(ThreadID tid)
{
    assert(tid < numThreads); // Ensure tid is within bounds
    if (tid == 0) {
        DPRINTF(SMTPolicies, "[consume] [tid: %d] occupiedEntries[tid] = %d, size = %d\n", tid, occupiedEntries[tid], (int)ceil(size * part1));
        assert(occupiedEntries[tid] <= (int)ceil(size * part1)); 
        occupiedEntries[tid]++;
    } else if (tid == 1) {
        DPRINTF(SMTPolicies, "[consume] [tid: %d] occupiedEntries[tid] = %d, size = %d\n", tid, occupiedEntries[tid], (int)floor(size * part2));
        assert(occupiedEntries[tid] <= (int)floor(size * part2)); 
        occupiedEntries[tid]++;
    } else {
        DPRINTF(SMTPolicies, "Thread ID %d is out of expected range.\n", tid);
        assert(false);
    }
}

void
PartitionedSMTPolicy::free(ThreadID tid)
{
    // DPRINTF(SMTPolicies, "free tid=%d\n", tid);
    // assert(occupiedEntries[tid] > 0);
    // occupiedEntries[tid]--;
    // if (occupiedEntries[tid]<0)
    //     occupiedEntries = 0;
    if (tid == 0) {
        DPRINTF(SMTPolicies, "[free] [tid: %d] occupiedEntries[tid] = %d, size = %d\n", tid, occupiedEntries[tid], (int)ceil(size * part1));
        assert(occupiedEntries[tid] > 0);
        occupiedEntries[tid]--;
        if (occupiedEntries[tid]<0)
            occupiedEntries = 0;
    } else if (tid == 1) {
        DPRINTF(SMTPolicies, "[free] [tid: %d] occupiedEntries[tid] = %d, size = %d\n", tid, occupiedEntries[tid], (int)floor(size * part2));
        assert(occupiedEntries[tid] > 0);
        occupiedEntries[tid]--;
        if (occupiedEntries[tid]<0)
            occupiedEntries = 0;
    } else {
        DPRINTF(SMTPolicies, "Thread ID %d is out of expected range.\n", tid);
        assert(false);
    }
}

void
PartitionedSMTPolicy::reset()
{
    for (int i = 0; i < numThreads; i++) {
        occupiedEntries[i] = 0;
    }
}

bool
PartitionedSMTPolicy::isFull(ThreadID tid)
{
    if (tid == 0) {
        if (occupiedEntries[tid] >= (int)ceil(size * part1)) {
            DPRINTF(SMTPolicies, "isFull tid=%d occupied=%d size=%d\n", tid, occupiedEntries[tid], (int)ceil(size * part1));
            return true;
        }
    } else if (tid == 1) {
        if (occupiedEntries[tid] >= (int)floor(size * part2)) {
            DPRINTF(SMTPolicies, "isFull tid=%d occupied=%d size=%d\n", tid, occupiedEntries[tid], (int)floor(size * part2));
            return true;
        }
    }
    return false;
}

unsigned
PartitionedSMTPolicy::numFree(ThreadID tid)
{
    assert(numThreads > 0);
    if (tid == 0) {
        DPRINTF(SMTPolicies, "numFree tid=%d occupied=%d size=%d\n", tid, occupiedEntries[tid], (int)ceil(size * part1));
        assert(((int)ceil(size * part1) - occupiedEntries[tid]) >= 0);
        return (int)ceil(size * part1) - occupiedEntries[tid];
    } else if (tid == 1) {
        DPRINTF(SMTPolicies, "numFree tid=%d occupied=%d size=%d\n", tid, occupiedEntries[tid], (int)floor(size * part2));
        assert(((int)floor(size * part2) - occupiedEntries[tid]) >= 0);
        return (int)floor(size * part2) - occupiedEntries[tid];
    }
    return 0;
}

unsigned
PartitionedSMTPolicy::evictMask(ThreadID tid)
{
    assert(tid < numThreads);
    return (0x1 << tid);
}

void
PartitionedSMTPolicy::init(int _size, int _numThreads, ClockedObject* _cpu)
{
    size = _size;
    numThreads = _numThreads;
    cpu = _cpu;
    occupiedEntries = new int[numThreads];

    setPartition(part1, part2);

    for (int i = 0; i < numThreads; i++) {
        occupiedEntries[i] = 0;
    }

    DPRINTF(SMTPolicies, "init size=%d numThreads=%d part1=%f part2=%f\n", _size, _numThreads, part1, part2);
}

PartitionedSMTPolicy*
PartitionedSMTPolicyParams::create()
{
    return new PartitionedSMTPolicy(this);
}
