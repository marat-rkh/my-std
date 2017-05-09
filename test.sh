source 'settings.sh'

TEST_SUITE=$1

pushd $BUILD_DIR/bin/test > /dev/null

if [ "$TEST_SUITE" = "" ]; then
    find . -name "*.test" -exec echo {} \; -exec {} \;
else
    if [ -f "$TEST_SUITE.test" ]; then
        ./"$TEST_SUITE.test"
    else
        echo "$TEST_SUITE.test file not found for $TEST_SUITE test suite"
    fi
fi

popd > /dev/null