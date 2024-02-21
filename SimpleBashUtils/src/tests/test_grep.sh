EXECUTABLE=../grep/s21_grep
DATA=./grep_data

grep_test() {

    for file in "$DATA"/*; do
        echo "Testing grep $1 $2 $file"
        grep $1 $2 $file > res.txt
        $EXECUTABLE $1 $2 $file > res1.txt

        $(diff res.txt res1.txt) > diff.txt
        error=$?
        if [[ "$error" != 0 ]]; then
            echo FAILED
        else
            echo PASSED
        fi
    done
}

grep_test "-v" "Lorem" "t.txt"
grep_test "-vi" "Lorem" "t.txt"
grep_test "-e" "Lorem" "t.txt"

cat > pat.txt <<-'EOF'
Lorem
EOF

grep_test "-f" "pat.txt"
grep_test "-l" "Lorem"
grep_test "-o" "Lorem"
grep_test "-c" "Lorem"
grep_test "-n" "Lorem"

rm diff.txt pat.txt res.txt res1.txt
