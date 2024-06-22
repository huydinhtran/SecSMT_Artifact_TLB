#!/bin/sh 
BASE=$(pwd)

mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"

outdir=$BASE/results-fix
ckptdir=$BASE/checkpoints-fix
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
# checkpoint Memcached SOLO
outdir_name="memcached-solo"
ckptdir_name="memcached-solo-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin -I 1000000000000  \
  --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end

# checkpoint L2FWD SOLO
outdir_name="l2fwd-solo"
ckptdir_name="l2fwd-solo-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin -I 1000000000000  \
  --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end 

# checkpoint Memcached L2FWD CORUN
outdir_name="memcached-l2fwd-corun"
ckptdir_name="memcached-l2fwd-corun-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.fast  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" -I 1000000000000  \
  --caches --cpu-type=AtomicSimpleCPU --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end
#############################################################################
