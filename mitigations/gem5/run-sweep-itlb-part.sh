#!/bin/sh 
BASE=$(pwd)


mem_bin="$BASE/tests/test-progs/memcached_selfContained/memcached-selfContained/memcached"
l2fwd_bin="$BASE/tests/test-progs/l2fwd/l2fwd-fix"

outdir=$BASE/results-sweep-itlb-part-test
ckptdir=$BASE/checkpoints-sweep-itlb-part-test-test
mkdir -p $outdir
mkdir -p $ckptdir

# part1="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9"
# part2="0.9 0.8 0.7 0.6 0.5 0.4 0.3 0.2 0.1"
part1="0.1"
part2="0.9"
part1_array=$(echo $part1 | tr " " "\n")
part2_array=$(echo $part2 | tr " " "\n")
i=0
part1_nl=$(echo $part1 | tr " " "\n")
part2_nl=$(echo $part2 | tr " " "\n")
set -- $part1


# outdir=$BASE/results-sweep-itlb-part
# ckptdir=$BASE/checkpoints-fix
# mkdir -p $outdir
# mkdir -p $ckptdir

# Restore
for itb_entries_part1 in $@; do
  itb_entries_part2=$(echo $part2 | cut -d' ' -f$((i + 1)))
  echo "Part1: $itb_entries_part1, Part2: $itb_entries_part2"

  outdir_name="memcached-l2fwd-corun-part1-$itb_entries_part1-part2-$itb_entries_part2"
  ckptdir_name="memcached-l2fwd-corun-ckpt-part1-$itb_entries_part1-part2-$itb_entries_part2"

  # ckptdir_name="memcached-l2fwd-corun-ckpt"
  mkdir -p $outdir/$outdir_name
  $BASE/build/X86/gem5.opt  --debug-flags=SMTPolicies,TLB --outdir $outdir/$outdir_name --stats-file=stat-file.txt \
  $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" --warmup-dpdk=200000000000 --rel-max-tick=1000000000000 --caches --cpu-type=Skylake_3 --restore-with-cpu=Skylake_3 \
  --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 --itb-entries=32 \
  --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --mem-type=DDR4_2400_8x8 -r 1 \
  --smt --smtTLBPolicy=PartitionedSMTPolicy --smtTLBPolicy-part1=$itb_entries_part1 --smtTLBPolicy-part2=$itb_entries_part2 \
  # > $outdir/$outdir_name/log.txt 2>&1 &

  i=$((i + 1))
done


# # Checkpoint
# for itb_entries_part1 in $@; do
#   itb_entries_part2=$(echo $part2 | cut -d' ' -f$((i + 1)))
#   echo "Part1: $itb_entries_part1, Part2: $itb_entries_part2"

#   outdir_name="memcached-l2fwd-corun"
#   ckptdir_name="memcached-l2fwd-corun-ckpt-part1-$itb_entries_part1-part2-$itb_entries_part2"
#   mkdir -p $ckptdir/$ckptdir_name
  
#   $BASE/build/X86/gem5.opt --debug-flags=SMTPolicies,TLB --outdir $ckptdir/$ckptdir_name --stats-file=stat-file.txt \
#   $BASE/configs/example/se.py -c "$mem_bin;$l2fwd_bin" -I 100000000 --caches --cpu-type=AtomicSimpleCPU \
#   --l2cache --l2_size=2MB --l2_assoc=16 --l1d_size=32kB --l1d_assoc=8 --l1i_size=32kB --l1i_assoc=8 \
#   --maxinsts_threadID=0 --checkpoint-dir=$ckptdir/$ckptdir_name --checkpoint-at-end --mem-type=DDR4_2400_8x8 \
#   --smt --smtTLBPolicy=PartitionedSMTPolicy --smtTLBPolicy-part1=$itb_entries_part1 --smtTLBPolicy-part2=$itb_entries_part2 \
#   # > $ckptdir/$ckptdir_name/log.txt 2>&1 &

#   i=$((i + 1))
# done

# 2000000000