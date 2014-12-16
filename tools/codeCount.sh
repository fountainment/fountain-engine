cd ..
wc `find ./*.cpp ./include/*.h ./include/fountain/*.h| grep -v "~"| grep -v main`
