EXECUTABLE=../cat/s21_cat
DATA=./cat_data

cat_test() {
    for file in "$DATA"/*; do
        echo "Testing cat $1 $file"
        cat $1 $file > res.txt
        $EXECUTABLE $1 $file > res1.txt
    
        error=$?

        if [[ "$error" == 0 ]]; then
            $(diff res.txt res1.txt) > diff.txt
            error=$?
            if [[ "$error" != 0 ]]; then
                echo FAILED
            else
                echo PASSED
            fi
        else
            echo FAILED
        fi
    done
}

cat_test "-n"
cat_test "-n"
cat_test "-b"
cat_test "-s"
cat_test "-t"
cat_test "-v"
cat_test "-e"
cat_test "-n -b -s -e -v"
cat_test "-bn"
cat_test "-t -v -e"

rm diff.txt res.txt res1.txt
