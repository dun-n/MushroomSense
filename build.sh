#clear webpage.h
echo '' > webpage.h

#write main.html
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --remove-tag-whitespace --use-short-doctype --minify-css true --minify-js true main.html > dist.html
echo 'const char* MAIN_WEB_PAGE = ' >> webpage.h
jsesc < dist.html -j >> webpage.h
echo ';' >> webpage.h

#write connect.html
html-minifier --collapse-whitespace --remove-comments --remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes --remove-tag-whitespace --use-short-doctype --minify-css true --minify-js true connect.html > dist.html
echo 'const char* CONNECT_WEB_PAGE = ' >> webpage.h
jsesc < dist.html -j >> webpage.h
echo ';' >> webpage.h

rm -rf dist.*
