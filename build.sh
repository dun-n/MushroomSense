#build webpage.h
#this involves gzip-ing the files and building byte arrays of them that
# can be stored in PROGMEM

echo '' > ./src/MushroomSense/Webpage.h
cd ./src/web-interface/
rm -rf dist
mkdir dist

echo "======================="
echo "gzip-ing html files"
echo "======================="

for i in ./*.html; do
    n=$(echo "$i" | sed -e 's/^..//g')
    u=$(echo ${n^^} | sed -e 's/\.HTML//g')
    echo "processing $n"
    html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --remove-tag-whitespace --use-short-doctype --minify-css true --minify-js true ${i} > temp.html
    echo "static const char HTML_$u[] PROGMEM = {" >> ../MushroomSense/Webpage.h
    gzip temp.html -c | xxd -i >> ../MushroomSense/Webpage.h
    gzip temp.html -c > ./dist/${n}.gz
    echo "};" >> ../MushroomSense/Webpage.h
done

echo "======================="
echo "gzip-ing js files"
echo "======================="

for i in ./*.js; do
    n=$(echo "$i" | sed -e 's/^..//g')
    u=$(echo ${n^^} | sed -e 's/\.JS//g')
    echo "processing $n"
    echo "static const char JS_$u[] PROGMEM = {" >> ../MushroomSense/Webpage.h
    gzip ${i} -c | xxd -i >> ../MushroomSense/Webpage.h
    gzip ${i} -c > ./dist/${n}.gz
    echo "};" >> ../MushroomSense/Webpage.h
done

echo "======================="
echo "gzip-ing css files"
echo "======================="

for i in ./*.css; do
    n=$(echo "$i" | sed -e 's/^..//g')
    u=$(echo ${n^^} | sed -e 's/\.CSS//g')
    echo "processing $n"
    html-minifier --collapse-whitespace --remove-comments --minify-css true ${i} > temp.css
    echo "static const char CSS_$u[] PROGMEM = {" >> ../MushroomSense/Webpage.h
    gzip temp.css -c | xxd -i >> ../MushroomSense/Webpage.h
    gzip temp.css -c > ./dist/${n}.gz
    echo "};" >> ../MushroomSense/Webpage.h
done

rm -rf temp.*

echo '' > extern.h


echo "done!"
