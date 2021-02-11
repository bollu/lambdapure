#!/usr/bin/env zsh
set -e
set -o xtrace
LEANPATH=/home/bollu/work/mlir/lean4/build/release/stage1/bin/lean
DIRPATH=$(dirname $1)
BASENAME=$(basename -s .lean $1)
LAMPUREPATH=$DIRPATH/$BASENAME-bollu.lambdapure

$LEANPATH $1 &> $LAMPUREPATH
