#!/usr/bin/env bash
# Robust tester for ft_ls vs /bin/ls over all flag combinations.
# Features: PTY capture (script), timeout, exit-code/signal logging, continue-on-error.

set -u
# don't use set -e so we can catch failures and continue

DIFF_LOG="diff_results.log"
SUMMARY_LOG="summary.log"
TMPDIR="${TMPDIR:-/tmp}"
TIMEOUT_CMD=${TIMEOUT_CMD:-timeout}   # rely on `timeout` command (coreutils). Set TIMEOUT_CMD="" to disable.
CMD_TIMEOUT="${CMD_TIMEOUT:-5s}"      # 5s default per invocation

: > "$DIFF_LOG"
: > "$SUMMARY_LOG"

FLAGS=(l R a r t u f g d)
N=${#FLAGS[@]}

# Run a command inside a PTY (script), save output and return status info.
# Usage: run_tty "cmd line" /path/to/outfile
run_tty() {
    local cmd="$1"
    local outfile="$2"

    # Use script to get PTY behavior. 'script' returns exit status of the child (on most systems),
    # but may differ; we'll capture child's exit via shell hack.
    # Prepend with timeout if available.
    if [[ -n "$TIMEOUT_CMD" ]]; then
        fullcmd="$TIMEOUT_CMD $CMD_TIMEOUT sh -c \"$cmd\""
    else
        fullcmd="sh -c \"$cmd\""
    fi

    # run inside script; we filter out the "Script started/done" lines
    # Use a temporary file for script's raw output then sanitize into outfile
    local raw="$(mktemp)"
    # -q quiet, -e return exit code normally; using sh -c preserves exit code in most script impls
    # If your system's script doesn't forward exit code, we still capture output and treat non-zero exit via $?
    script -q -e -c "$fullcmd" "$raw" >/dev/null 2>&1 || true
    sed '/^Script started/d;/^Script done/d' < "$raw" > "$outfile"
    rm -f "$raw"

    # get exit status of last background command ($?) isn't reliable because script swallowed it.
    # As a pragmatic approach we run the command *again* in a subshell to capture proper exit code but redirect output to /dev/null.
    # That avoids changing test artifacts but gives us status. If that is unacceptable, you can write a wrapper that
    # echoes exit code to a file when run inside the PTY.
    if [[ -n "$TIMEOUT_CMD" ]]; then
        ( $TIMEOUT_CMD $CMD_TIMEOUT sh -c "$cmd" ) >/dev/null 2>&1
        echo $?  # caller will capture
    else
        ( sh -c "$cmd" ) >/dev/null 2>&1
        echo $?
    fi
}

# Build combined short flags like -lRtu (not "-l -R" etc)
flags_from_mask() {
    local mask=$1
    local out=""
    for (( j=0; j<N; j++ )); do
        if (( (mask >> j) & 1 )); then
            out+="${FLAGS[j]}"
        fi
    done
    # return as "-<letters>" or empty
    if [[ -z "$out" ]]; then
        printf ""
    else
        printf -- "-%s" "$out"
    fi
}

# Compare two files, log diff (using diff program) with nice format
log_diff() {
    local real="$1"
    local mine="$2"
    local desc="$3"
    {
        echo "--- $desc ---"
        diff --old-line-format='-%L' --new-line-format='+%L' --unchanged-line-format=' %L' "$real" "$mine" || true
        echo
    } >> "$DIFF_LOG"
}

# Main test executor for one combination
run_test() {
    local combo="$1"   # e.g. -lR or "" for no flags
    local label
    [[ -z "$combo" ]] && label="<no-flags>" || label="$combo"

    local my_out real_out
    my_out="$(mktemp)"
    real_out="$(mktemp)"

    # Build commands: ensure flags are passed as a single argument so ls receives them together (e.g. -lR)
    local ft_cmd="./ft_ls $combo"
    local real_cmd="/bin/ls --color=never $combo"

    # Capture outputs and get exit statuses
    local my_status real_status
    my_status=$(run_tty "$ft_cmd" "$my_out") || true
    real_status=$(run_tty "$real_cmd" "$real_out") || true

    # Normalize: sort of ensure files end with newline to make diff behave predictably
    : >> "$my_out"
    : >> "$real_out"

    # compare exit codes
    if [[ "$my_status" -ne 0 || "$real_status" -ne 0 ]]; then
        # If both non-zero *and* equal, we may still want to compare outputs
        echo "⚠ $label: exit codes -> ft_ls=$my_status ls=$real_status" >> "$SUMMARY_LOG"
    fi

    # Run diff
    if diff "$real_out" "$my_out" >/dev/null 2>&1; then
        echo "✅ $label" | tee -a "$SUMMARY_LOG"
        rm -f "$my_out" "$real_out"
        return 0
    else
        echo "❌ $label (ft_ls=$my_status ls=$real_status)" | tee -a "$SUMMARY_LOG"
        log_diff "$real_out" "$my_out" "$label (ft_ls=$my_status ls=$real_status)"
        rm -f "$my_out" "$real_out"
        return 1
    fi
}

# iterate all combinations
total=0
fails=0
echo "🧪 Running all combinations..."
for ((mask=0; mask < (1<<N); mask++)); do
    combo="$(flags_from_mask "$mask")"
    ((total++))
    if ! run_test "$combo"; then
        ((fails++))
    fi
done

echo
echo "📄 Summary saved to: $SUMMARY_LOG"
echo "📄 Diffs saved to:   $DIFF_LOG"
echo "✅ Tests run: $total"
echo "❌ Failures: $fails"

# exit non-zero if any diffs were found (useful for CI / make)
if (( fails > 0 )); then
    exit 1
else
    exit 0
fi
