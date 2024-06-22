#!/bin/sh 
BASE=$(pwd)


mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"

outdir=$BASE/results-itlb
ckptdir=$BASE/checkpoints-itlb
mkdir -p $outdir
mkdir -p $ckptdir

# itb_entries="1 2 4 8 16 32"
# itb_entries="9 10 11 12 13 14 15"
itb_entries="8 16"
# for itb in $itb_entries; do
#   # restore L2FWD SOLO
#   outdir_name="l2fwd-solo"
#   ckptdir_name="l2fwd-solo-ckpt-itb-$itb"
#   mkdir -p $outdir/$outdir_name/$outdir_name-itlb-$itb/
#   $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-itlb-$itb/ --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin \
#     --itb-entries=$itb --warmup-dpdk=200000000000 --rel-max-tick=800000000000 --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB \
#     --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 \
#     > $outdir/$outdir_name/$outdir_name-itlb-$itb/log.txt 2>&1 &
# done

#full icache assoc
for itb in $itb_entries; do
  # restore L2FWD SOLO
  outdir_name="l2fwd-solo-fullassoc"
  ckptdir_name="l2fwd-solo-ckpt-itb-$itb"
  mkdir -p $outdir/$outdir_name/$outdir_name-itlb-$itb/
  $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name/$outdir_name-itlb-$itb/ --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin \
    --itb-entries=$itb --warmup-dpdk=200000000000 --rel-max-tick=800000000000 --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB \
    --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=512 --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 \
    > $outdir/$outdir_name/$outdir_name-itlb-$itb/log.txt 2>&1 &
done

#################################restore############################################
# # restore L2FWD SOLO
# outdir_name="l2fwd-solo"
# ckptdir_name="l2fwd-solo-ckpt"
# mkdir -p $outdir/$outdir_name
# mkdir -p $ckptdir/$ckptdir_name
# $BASE/build/X86/gem5.opt  --outdir $outdir/$outdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin --warmup-dpdk=100000000000 --rel-max-tick=1000000000000  \
#   --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 --itb-entries=4 --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --maxinsts_threadID=0 --mem-type=DDR4_2400_8x8 --checkpoint-dir=$ckptdir/$ckptdir_name -r 1 # > $outdir/$outdir_name/log.txt 2>&1 &
#############################################################################



#################################checkpoint############################################

# checkpoint L2FWD SOLO
# itb_entries="1 2 4 8 16 32"
# itb_entries="9 10 11 12 13 14 15"

# for itb in $itb_entries; do
#   outdir_name="l2fwd-solo"
#   ckptdir_name="l2fwd-solo-ckpt-itb-$itb"
#   mkdir -p $ckptdir/$ckptdir_name
#   $BASE/build/X86/gem5.fast  --outdir $ckptdir/$ckptdir_name --stats-file=stat-file.txt $BASE/configs/example/se.py -c $l2fwd_bin -I 1000000000  \
#     --caches --cpu-type=AtomicSimpleCPU --itb-entries=$itb --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 \
#     --maxinsts_threadID=0 --mem-type=DDR4_2400_8x8 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end > $ckptdir/$ckptdir_name/log.txt 2>&1 &
# done

#############################################################################
