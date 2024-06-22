#!/bin/sh 
BASE=$(pwd)


mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"
fibo_bin="$BASE/tests/test-progs/loop/fibo"


outdir=$BASE/results-trace
ckptdir=$BASE/checkpoints-fix
mkdir -p $outdir
mkdir -p $ckptdir


# test trace
# outdir_name="test"
# mkdir -p $outdir/$outdir_name/$outdir_name-trace/
# $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-trace/ --stats-file=stat-file.txt $BASE/configs/example/se.py -c $fibo_bin \
#   --itb-entries=32 -I 1000000000 --caches --cpu-type=Skylake_3_Etrace --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB \
#   --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0  --mem-type=DDR4_2400_8x8 \
#   --inst-trace-file=test-fetchtrace.proto.gz --data-trace-file=test-deptrace.proto.gz \


# restore Memcached SOLO
outdir_name="memcached-solo"
ckptdir_name="memcached-solo-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $mem_bin  --rel-max-tick=80000000000  \
  --caches --cpu-type=Skylake_3_Etrace --restore-with-cpu=Skylake_3_Etrace --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 \
  --inst-trace-file=fetchtrace.proto.gz --data-trace-file=deptrace.proto.gz --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/log.txt 2>&1 &

# restore L2FWD SOLO
outdir_name="l2fwd-solo"
ckptdir_name="l2fwd-solo-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin --rel-max-tick=80000000000  \
  --caches --cpu-type=Skylake_3_Etrace --restore-with-cpu=Skylake_3_Etrace --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 \
  --inst-trace-file=fetchtrace.proto.gz --data-trace-file=deptrace.proto.gz --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/log.txt 2>&1 &

# restore Memcached L2FWD CORUN
outdir_name="memcached-l2fwd-corun"
ckptdir_name="memcached-l2fwd-corun-ckpt"
mkdir -p $outdir/$outdir_name
mkdir -p $ckptdir/$ckptdir_name
$BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" --rel-max-tick=80000000000  \
  --caches --cpu-type=Skylake_3_Etrace --restore-with-cpu=Skylake_3_Etrace --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --smt $simopts --maxinsts_threadID=0 \
  --inst-trace-file=fetchtrace.proto.gz --data-trace-file=deptrace.proto.gz --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 > $outdir/$outdir_name/log.txt 2>&1 &

