#!/bin/zsh

echo                                                                        >> GLOBAL-REPORT.md
echo -n "Tests for $1 finished at " $(date)                                 >> GLOBAL-REPORT.md
echo                                                                        >> GLOBAL-REPORT.md

git add GLOBAL-REPORT.md || true
git commit -am "Updated global report for $1."

rm -rf checked-out checked-out-ref 
#rm -rf logs

git push
