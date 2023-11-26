if test $# -ne 1
then
    echo "nu s-a primit exact un argument"
    exit 1
fi

count=0
while read linie
do
    v=`echo "$linie" | grep -E "^$1[a-zA-Z0-9, ]*[\.?!]$" | grep -v " , si" | grep -v ",si" | wc -l`
    count=`expr $count + $v` 
done 

echo $count
