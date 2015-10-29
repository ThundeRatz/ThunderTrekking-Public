MAX_W=640
MAX_H=480
cat /tmp/tld_output | while read line ; do
	num=$(printf %04d $(echo $line | awk -F ' ' '{print $1}'))
	x=$(echo $line | awk -F ' ' '{print $2 - 10}')
	y=$(echo $line | awk -F ' ' '{print $3 - 10}')
	w=$(echo $line | awk -F ' ' '{print $4 + 10}')
	h=$(echo $line | awk -F ' ' '{print $5 + 10}')
	if "$x" -lt 0; then
		x=0
	fi
	if "$y" -lt 0; then
		y=0
	fi
	if "$w" -gt "$MAX_W"; then
		w=$MAX_W
	fi
	if "$h" -gt "$MAX_H"; then
		h=$MAX_H
	fi
	echo "$num $w $h $x $y"
	convert /tmp/frames/output_$num.png -crop ${w}x${h}+${x}+${y} /tmp/crop/output_$num.png
done
