#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

print_header "RUN cppcheck"
check_log "cppcheck untitled --enable=all --inconclusive --error-exitcode=1 -I untitled/start --suppress=missingIncludeSystem" "\(information\)"

print_header "RUN clang-tidy"
#check_log "clang-tidy untitled/start/*.c untitled/start/*.h -warnings-as-errors=* -extra-arg=-std=c99 -- -Istart" "Error (?:reading|while processing)"
check_log "clang-tidy untitled/start/*.c untitled/start/*.h -extra-arg=-std=c99 -- -Istart" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --extensions=c --filter=-runtime/arrays untitled/start/*.h untitled/start/*.c untitled/*.c" "Can't open for reading"

print_header "SUCCESS"
