source 'settings.sh'

TEST_SUITE=$1

pushd $BUILD_DIR/bin/test > /dev/null

if [ "$TEST_SUITE" = "" ]; then
    find . -name "*.tests" -exec echo {} \; -exec {} \;
else
    if [ -f "$TEST_SUITE.tests" ]; then
        ./"$TEST_SUITE.tests"
    else
        echo "$TEST_SUITE.tests file not found for $TEST_SUITE test suite"
    fi
fi

popd > /dev/null