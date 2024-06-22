#!/bin/sh 
BASE=$(pwd)
multf=1.5
multc=1
multi=1.2
case "$1" in
  shared)
    simpots=''
    ;;
  partitioned)
    simopts="--smtFetchPolicy=StrictRoundRobin --smtBTBPolicy=PartitionedSMTPolicy --smtCachePolicy=PartitionedSMTPolicy --smtROBPolicy=PartitionedSMTPolicy --smtIQPolicy=PartitionedSMTPolicy --smtTLBPolicy=PartitionedSMTPolicy --smtPhysRegPolicy=PartitionedSMTPolicy --smtSQPolicy=PartitionedSMTPolicy --smtLQPolicy=PartitionedSMTPolicy --smtCommitPolicy=StrictRoundRobin --smtIssuePolicy=MultiplexingFUs"
    ;;
  adaptive)
    simopts="--smtFetchPolicy=Adaptive --smtBTBPolicy=AdaptiveSMTPolicy --smtCachePolicy=AdaptiveSMTPolicy --smtROBPolicy=AdaptiveSMTPolicy --smtIQPolicy=AdaptiveSMTPolicy --smtTLBPolicy=AdaptiveSMTPolicy --smtPhysRegPolicy=AdaptiveSMTPolicy  --smtSQPolicy=AdaptiveSMTPolicy --smtLQPolicy=AdaptiveSMTPolicy --smtIssuePolicy=Adaptive --smtCommitPolicy=Adaptive --smtAdaptiveInterval=100000 --smtAdaptiveLimit=0.8 --smtFetchPolicy-limit=10 --smtIssuePolicy-limit=10 --smtCommitPolicy-limit=10 --smtFetchPolicy-mult=${multf} --smtIssuePolicy-mult=${multi} --smtCommitPolicy-mult=${multc} --smtSQPolicy-limit=0.7 --smtCachePolicy-limit=0.7 --smtBTBPolicy-limit=0.7"
    ;;  
  asymmetric)
    simopts="--smtFetchPolicy=Asymmetric --smtBTBPolicy=AsymmetricSMTPolicy --smtCachePolicy=AsymmetricSMTPolicy  --smtROBPolicy=AdaptiveSMTPolicy --smtIQPolicy=AdaptiveSMTPolicy --smtTLBPolicy=AdaptiveSMTPolicy --smtPhysRegPolicy=AdaptiveSMTPolicy --smtSQPolicy=AdaptiveSMTPolicy  --smtLQPolicy=AdaptiveSMTPolicy  --smtIssuePolicy=Asymmetric --smtCommitPolicy=Asymmetric  --smtAdaptiveInterval=100000 --smtAdaptiveLimit=0.9 --smtFetchPolicy-limit=10 --smtIssuePolicy-limit=10 --smtCommitPolicy-limit=10 --smtFetchPolicy-mult=1.20 --smtIssuePolicy-mult=1.20 --smtCommitPolicy-mult=1.2 --smtSQPolicy-limit=0.7 --smtCachePolicy-limit=0.7 --smtBTBPolicy-limit=0.7"
    ;;
  *)
esac

bin1="$BASE/tests/test-progs/loop/loop"

bin2="$BASE/tests/test-progs/loop/fibo"

# mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
# l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-self"

outdir=$BASE/results
ckptdir=$BASE/checkpoints
mkdir -p $outdir
mkdir -p $ckptdir



#################################restore############################################
# # restore Memcached SOLO
# outdir_name="memcached-solo"
# ckptdir_name="memcached-solo-ckpt"
# mkdir -p $outdir/$outdir_name/restore
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/restore --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
#   --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/restore/log.txt 2>&1 &

# # restore L2FWD SOLO
# outdir_name="l2fwd-solo"
# ckptdir_name="l2fwd-solo-ckpt"
# mkdir -p $outdir/$outdir_name/restore
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/restore --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
#   --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/restore/log.txt 2>&1 &

# # restore Memcached L2FWD CORUN
# outdir_name="memcached-l2fwd-corun"
# ckptdir_name="memcached-l2fwd-corun-ckpt"
# mkdir -p $outdir/$outdir_name/restore
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/restore --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
#   --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/restore/log.txt 2>&1 &
#############################################################################



#################################checkpoint############################################
# # checkpoint Memcached SOLO
# outdir_name="memcached-solo"
# ckptdir_name="memcached-solo-ckpt"
# mkdir -p $outdir/$outdir_name
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin -I 1000000000000  \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end

# # checkpoint L2FWD SOLO
# outdir_name="l2fwd-solo"
# ckptdir_name="l2fwd-solo-ckpt"
# mkdir -p $outdir/$outdir_name
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin -I 1000000000000  \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end 

# checkpoint Memcached L2FWD CORUN
# outdir_name="memcached-l2fwd-corun"
# ckptdir_name="memcached-l2fwd-corun-ckpt"
# mkdir -p $outdir/$outdir_name
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" -I 1000000000000  \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end
#############################################################################















# 1 thread
# $BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c $bin1 -I 100000000  \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 


# checkpoint
# $BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" --options="$args1;$args2" -I 100000000  \
#   --caches --cpu-type=Skylake_3 --l2cache --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8  --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$outdir/checkpoints --checkpoint-at-end

# $BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8  --smt $simopts --maxinsts_threadID=0 > $outdir/log-atomic.txt 2>&1 &

$BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" \
  --caches --cpu-type=Skylake_3 --l2cache --l2_size=4MB --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8  --smt $simopts --maxinsts_threadID=0 --mem-type=DDR4_2400_8x8 --mem-size=2GB  --cpu-clock=2GHz


# 2 threads
# $BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" --options="$args1;$args2" -I 100000000  \
#   --caches --cpu-type=Skylake_3 --l2cache --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8  --smt $simopts --maxinsts_threadID=0


# restore
# $BASE/build/X86/gem5.fast  --outdir $outdir --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$bin1;$bin2" \
#   --caches --cpu-type=AtomicSimpleCPU --l2cache --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8  --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$outdir/checkpoints-real-test --checkpoint-restore=1

