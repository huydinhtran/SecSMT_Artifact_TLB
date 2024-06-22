/*
 * Copyright (c) 2020 Kazem Taram
 * All rights reserved
*/
#include "cpu/smt_policies/partitioned.hh"

#include "debug/SMTPolicies.hh"

PartitionedSMTPolicy::PartitionedSMTPolicy(const Params* p)
    : BaseSMTPolicy(p), part1(p->part1), part2(p->part2)
{
    assert(numThreads);
   

    occupiedEntries = new int[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        occupiedEntries[i] = 0;
    }
}

void
PartitionedSMTPolicy::consume(ThreadID tid)
{
    // DPRINTF(SMTPolicies, "consume tid=%d\n", tid);
    // HUY
    // assert(occupiedEntries[tid] <= size / numThreads);
    if (tid == 0){
        assert(occupiedEntries[tid] < ceil(size * part1));
        occupiedEntries[tid]++;
    } else if (tid == 1){
        assert(occupiedEntries[tid] < floor(size * part2));
        occupiedEntries[tid]++;

    }
    // if (occupiedEntries[tid] < size / numThreads)
    // occupiedEntries[tid]++;
}

void
PartitionedSMTPolicy::free(ThreadID tid)
{


    // DPRINTF(SMTPolicies, "free tid=%d\n", tid);
    assert(occupiedEntries[tid] > 0);
    occupiedEntries[tid]--;
    if (occupiedEntries[tid]<0)
        occupiedEntries = 0;
}

void
PartitionedSMTPolicy::reset()
{
    for (int i = 0; i < numThreads; i++)
    {
        occupiedEntries[i] = 0;
    }
}

bool
PartitionedSMTPolicy::isFull(ThreadID tid)
{

    // DPRINTF(SMTPolicies, "isFull tid=%d occupied=%d size=%d\n", 
    //                         tid, occupiedEntries[tid], size/numThreads );
    // HUY
    if (tid == 0){
        if (occupiedEntries[tid] >= ceil(size * part1)){
            DPRINTF(SMTPolicies, "isFull tid=%d occupied=%d size=%d\n", 
                tid, occupiedEntries[tid], ceil(size * part1));
            return true;
        }
    } else if (tid == 1){
        if (occupiedEntries[tid] >= floor(size * part2)){
            DPRINTF(SMTPolicies, "isFull tid=%d occupied=%d size=%d\n", 
                tid, occupiedEntries[tid], floor(size * part2));
            return true;
        }
    }
    // if (occupiedEntries[tid] >= size / numThreads)
    //     return true;
    return false;
}

unsigned
PartitionedSMTPolicy::numFree(ThreadID tid)
{

    // DPRINTF(SMTPolicies, "numFree tid=%d occupied=%d size=%d\n", 
    //                         tid, occupiedEntries[tid], size/numThreads );
    assert(numThreads>0);
    // assert((size / numThreads - occupiedEntries[tid]) >= 0);
    // return size / numThreads - occupiedEntries[tid];
    // HUY
    if (tid == 0){
        assert((ceil(size * part1) - occupiedEntries[tid]) >= 0);
        DPRINTF(SMTPolicies, "numFree tid=%d occupied=%d size=%d\n", 
                            tid, occupiedEntries[tid], ceil(size * part1));
        return ceil(size * part1) - occupiedEntries[tid];
    } else if (tid == 1){
        DPRINTF(SMTPolicies, "numFree tid=%d occupied=%d size=%d\n", 
                           tid, occupiedEntries[tid], floor(size * part2));
        assert((floor(size * part2) - occupiedEntries[tid]) >= 0);
        return floor(size * part2) - occupiedEntries[tid];
    }
    return 0;
}

unsigned
PartitionedSMTPolicy::evictMask(ThreadID tid)
{
    assert(tid<numThreads);
    return (0x1<<(tid));
}

void
PartitionedSMTPolicy::init (int _size, int _numThreads, ClockedObject* _cpu)
{
    size = _size;
    numThreads = _numThreads;
    cpu = _cpu;
    occupiedEntries = new int[numThreads];

    

    for (int i = 0; i < numThreads; i++)
    {
        occupiedEntries[i] = 0;
    }

    if (numThreads == 1){
        part1 = 0.5;
        part2 = 0.5;
    }


    DPRINTF(SMTPolicies, "init size=%d numThreads=%d part1=%f part2=%f\n", _size, _numThreads, part1, part2);
}


PartitionedSMTPolicy*
PartitionedSMTPolicyParams::create()
{
    return new PartitionedSMTPolicy(this);
}
