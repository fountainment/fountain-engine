ePath=..
file=`find $ePath/*.cpp $ePath/include/*.h $ePath/include/fountain/*.h| grep -v '~'| grep -v main| grep -v test`
wc $file
co=`sed '/^$/d' $file| wc -l`
echo -e "\nNon-empty lines: $co\n"
