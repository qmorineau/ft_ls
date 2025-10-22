#!/usr/bin/env bash
set -euo pipefail

# Temporary files for outputs
MY_OUT=$(mktemp)
REAL_OUT=$(mktemp)
DIFF_LOG="diff_results.log"
: > "$DIFF_LOG"  # empty the log

# Capture command output via a pseudo-tty (to mimic terminal output)
capture_tty() {
    local cmd="$1"
    local outfile="$2"
    # -q quiet, -e return exit code, /dev/null to discard script log
    script -q -e -c "$cmd" /dev/null | sed '/^Script started/d;/^Script done/d' > "$outfile"
}

# Compare ft_ls and real ls
assert_ls() {
    local testname="$1"
    local args="$2"

    echo "🧪 Test: $testname"

    # Capture outputs
    capture_tty "./ft_ls $args" "$MY_OUT"
    capture_tty "/bin/ls --color=never $args" "$REAL_OUT"

    # Compare silently
    if diff "$REAL_OUT" "$MY_OUT" > /dev/null 2>&1; then
        echo "✅ $testname: OK"
    else
        echo "❌ $testname: DIFFER"
        {
            echo "--- $testname ---"
            diff --old-line-format='-%L' \
                 --new-line-format='+%L' \
                 --unchanged-line-format=' %L' \
                 "$REAL_OUT" "$MY_OUT" || true
            echo
        } >> "$DIFF_LOG"
    fi

    echo
}


### Example tests ###
assert_ls "With -l" "-l"
assert_ls "With -Rl" "-Rl"
assert_ls "With -al" "-al"
assert_ls "With -rl" "-rl"
assert_ls "With -tl" "-tl"
assert_ls "With -ul" "-ul"
assert_ls "With -fl" "-fl"
assert_ls "With -gl" "-gl"
assert_ls "With -dl" "-dl"

# Clean up
rm -f "$MY_OUT" "$REAL_OUT"

echo "📄 All diffs saved to: $DIFF_LOG"
