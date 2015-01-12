ePath=..
file=`find $ePath/*.cpp $ePath/include/*.h $ePath/include/fountain/*.h| grep -v '~'| grep -v main`
formatCom='astyle --indent=tab'
$formatCom $file
