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

mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-self"

outdir=$BASE/results
ckptdir=$BASE/checkpoints
mkdir -p $outdir
mkdir -p $ckptdir

l1d_size="16kB 32kB 64kB"
l1i_size="16kB 32kB 64kB"
l2_size="1MB 2MB 4MB"

for l1d in $l1d_size; do
  for l1i in $l1i_size; do
    for l2 in $l2_size; do
      if [ $(pgrep -c gem5.opt) -le 35 ]; then
          echo "Launching simulations..."

            # restore Memcached SOLO
            outdir_name="memcached-solo"
            ckptdir_name="memcached-solo-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &

            # restore L2FWD SOLO
            outdir_name="l2fwd-solo"
            ckptdir_name="l2fwd-solo-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &

            # restore Memcached L2FWD CORUN
            outdir_name="memcached-l2fwd-corun"
            ckptdir_name="memcached-l2fwd-corun-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &
      
      fi

      while [ $(pgrep -c gem5.opt) -ge 35 ]
      do
          echo "Waiting for the number of gem5.opt processes to drop below 35..."
          sleep 120
      done
    done
  done
done