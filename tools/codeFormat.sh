ePath=..
file=`find $ePath/*.cpp $ePath/*.h $ePath/include/*.h $ePath/include/fountain/*.h| grep -v '~'`
formatCom='astyle --indent=tab'
$formatCom $file
