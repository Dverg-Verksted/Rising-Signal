git lfs uninstall
git lfs ls-files | sed -r 's/^.{13}//' > files.txt
while read line; do git rm --cached "$line"; done < files.txt
while read line; do git add "$line"; done < files.txt
git add .gitattributes
git commit -m "unlfs"
git push origin
git lfs ls-files
rm -rf .git/lfs