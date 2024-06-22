#!/bin/sh 
BASE=$(pwd)

mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"

outdir=$BASE/results-fix-ddr4
ckptdir=$BASE/checkpoints-fix
mkdir -p $outdir
mkdir -p $ckptdir

# l1d_size="16kB 32kB 64kB"
# l1i_size="16kB 32kB 64kB"
# l2_size="1MB 2MB 4MB"
l1d_size="32kB"
l1i_size="1kB 2kB 4kB 8kB"
l2_size="2MB"

for l1d in $l1d_size; do
  for l1i in $l1i_size; do
    for l2 in $l2_size; do
      if [ $(pgrep -c gem5.opt) -le 36 ]; then
          echo "Launching simulations..."

            # restore Memcached SOLO
            outdir_name="memcached-solo"
            ckptdir_name="memcached-solo-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin --warmup-dpdk=200000000000 --rel-max-tick=800000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &

            # restore L2FWD SOLO
            outdir_name="l2fwd-solo"
            ckptdir_name="l2fwd-solo-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin --warmup-dpdk=200000000000 --rel-max-tick=800000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &

            # restore Memcached L2FWD CORUN
            outdir_name="memcached-l2fwd-corun"
            ckptdir_name="memcached-l2fwd-corun-ckpt"
            mkdir -p $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2
            $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2 --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" --warmup-dpdk=200000000000 --rel-max-tick=800000000000  \
              --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=$l2 --l2_assoc=16 --l1d_size=$l1d --l1d_assoc=8 --l1i_size=$l1i --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 > $outdir/$outdir_name/$outdir_name-l1d$l1d-l1i$l1i-l2$l2/log.txt 2>&1 &
      
      fi

      while [ $(pgrep -c gem5.opt) -ge 36 ]
      do
          echo "Waiting for the number of gem5.opt processes to drop below 35..."
          sleep 120
      done
    done
  done
done

echo "All simulations launched!"