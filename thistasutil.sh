# This directory path
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Binary path
export PATH=$DIR:$PATH
# C/C++ path
# export CPATH=$CPATH:$DIR
# export C_INCLUDE_PATH=$C_INCLUDE_PATH:$DIR
# export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$DIR
# ROOT path
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$DIR
