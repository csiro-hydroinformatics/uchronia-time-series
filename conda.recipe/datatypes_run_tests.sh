#!/bin/bash
#

OUTPUTDIR=$1
ROOTDIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -z "${SWIFT_TEST_DIR}" ]
then
  export SWIFT_TEST_DIR=${EHP_SHARED}/STF/SWIFTUnitTestData/UnitTests/
fi

if [ -n "${OUTPUTDIR}" ]
then
    mkdir -p ${OUTPUTDIR}
fi

for testexe in datatypes_tests ;
do
    ARGS=
    if [ -n "${OUTPUTDIR}" ]
    then
        ARGS="--reporter junit --out ${OUTPUTDIR}/test_results_${testexe}.xml"
    fi
    ${ROOTDIR}/${testexe} ${ARGS} || echo "Hidden failure"
done
