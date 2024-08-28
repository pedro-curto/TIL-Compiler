#!/bin/zsh

for group in checked-out/???; do
  GROUP=`print -f "%.3d" $group:t`

  RES=$(grep "Bad project" logs/$GROUP.res.html)
  if [ "$RES" != "" ]; then
    echo -n "* $GROUP [res](logs/$GROUP.res.html) [log](logs/$GROUP.log.html) " >> GLOBAL-REPORT.md
    echo "❌ **NO DELIVERY**"                           >> GLOBAL-REPORT.md
    continue
  fi

  RES=$(grep "successfully" logs/$GROUP.res.html)
  if [ "$RES" != "" ]; then
    PC=$(grep Total logs/$GROUP.res.html | awk '{print $2}' | sed -e 's/\.\([0-9]\)\([0-9]\)[0-9]*/.\1\2/g')
    echo -n "* $GROUP [res](logs/$GROUP.res.html) [log](logs/$GROUP.log.html) " >> GLOBAL-REPORT.md
    echo "✅ **SUCCESS: $PC%**"                      >> GLOBAL-REPORT.md
    continue
  fi

  # default
  echo -n "* $GROUP [res](logs/$GROUP.res.html) [log](logs/$GROUP.log.html) "   >> GLOBAL-REPORT.md
  echo "⚠ **BUILD FAILED**"                         >> GLOBAL-REPORT.md

done

