source 'settings.sh'

mkdir -p $BUILD_DIR
pushd $BUILD_DIR > /dev/null
cmake ..
make
popd > /dev/null