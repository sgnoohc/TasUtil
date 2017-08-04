usage() {
    echo "usage: $0 CORENAME"
    echo "error: check your arguments."
}

if [ -z $1 ]; then usage; fi

source root.sh ""

cd $1;
make -j 40;
cp *CORE.so ../
cd ../
