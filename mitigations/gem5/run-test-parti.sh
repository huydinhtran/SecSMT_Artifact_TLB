#!/bin/sh 
BASE=$(pwd)

# mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
# l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"

bin1="$BASE/tests/test-progs/loop/loop"
bin2="$BASE/tests/test-progs/loop/fibo"

outdir=$BASE/results-parti
# ckptdir=$BASE/checkpoints-itlb
mkdir -p $outdir
# mkdir -p $ckptdir


outdir_name="test-parti-test"
# ckptdir_name="memcached-l2fwd-corun-ckpt"
mkdir -p $outdir/$outdir_name
$BASE/build/X86/gem5.opt --debug-flags=SMTPolicies --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" -I 1000000000000  \
  --caches --cpu-type=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 \
  --maxinsts_threadID=0  --mem-type=DDR4_2400_8x8 --smt --smtTLBPolicy=PartitionedSMTPolicy --smtTLBPolicy-part1=0.9 --smtTLBPolicy-part2=0.1

# --debug-flags=SMTPolicies