# This directory path
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Binary path
export PATH=$DIR:$PATH
# C/C++ path
export CPATH=$DIR:$CPATH
export C_INCLUDE_PATH=$DIR:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$DIR:$CPLUS_INCLUDE_PATH
# ROOT path
export ROOT_INCLUDE_PATH=$DIR:$ROOT_INCLUDE_PATH
