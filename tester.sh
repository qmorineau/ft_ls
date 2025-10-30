#!/usr/bin/env bash
# Simple robust tester for ft_ls vs /bin/ls over all flag combinations.
# No PTY, outputs captured to files for diffing.
# LC_ALL=C ensures deterministic locale.

set -u

DIFF_LOG="diff_results.log"
SUMMARY_LOG="summary.log"
FLAGS=(l R a r t u f g d)
N=${#FLAGS[@]}

: > "$DIFF_LOG"
: > "$SUMMARY_LOG"

# Build combined short flags like -lRtu
flags_from_mask() {
    local mask=$1
    local out=""
    for (( j=0; j<N; j++ )); do
        if (( (mask >> j) & 1 )); then
            out+="${FLAGS[j]}"
        fi
    done
    [[ -n "$out" ]] && printf -- "-%s" "$out" || printf ""
}

# Compare two files and log diff
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

# Run one test combination
run_test() {
    local combo="$1"
    local label
    [[ -z "$combo" ]] && label="<no-flags>" || label="$combo"

    local my_out real_out
    my_out="$(mktemp)"
    real_out="$(mktemp)"

    # Run commands under LC_ALL=C
    LC_ALL=C ./ft_ls $combo > "$my_out" 2>&1
    LC_ALL=C /bin/ls $combo > "$real_out" 2>&1

    # Normalize newline
    : >> "$my_out"
    : >> "$real_out"

    if diff "$real_out" "$my_out" >/dev/null 2>&1; then
        echo "✅ $label" | tee -a "$SUMMARY_LOG"
        rm -f "$my_out" "$real_out"
        return 0
    else
        echo "❌ $label" | tee -a "$SUMMARY_LOG"
        log_diff "$real_out" "$my_out" "$label"
        rm -f "$my_out" "$real_out"
        return 1
    fi
}

# Iterate all flag combinations
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

# exit non-zero if any diffs were found
(( fails > 0 )) && exit 1 || exit 0
