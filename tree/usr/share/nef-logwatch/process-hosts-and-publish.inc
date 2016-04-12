#   -*- shell-script -*-
# To be source from /etc/cron.*/nef-logwatch-* scripts
#


#Check if removed-but-not-purged
test -x /usr/share/logwatch/scripts/logwatch.pl || exit 0

hosts=$(cat /var/log/messages | cut -d' ' -f4 | uniq | sort | uniq)

for host in $hosts; do
    /usr/sbin/logwatch --range $range --detail $detail --hostlimit $host --format html \
        >/var/lib/logwatch/daily/$date.$host.html
done

# Compile all files

all_file=/var/lib/logwatch/daily/$date.html

header_lines=$(wc -l /etc/logwatch/conf/html/header.html | awk '{ print $1 }')
footer_lines=$(wc -l /etc/logwatch/conf/html/footer.html | awk '{ print $1 }')

#echo header_lines=$header_lines gooter=$footer_lines
#echo cat /var/lib/logwatch/daily/$date.$host.html  head -n +$(( header_lines + 1 ))  head -n -$footer_lines

summary_detail=low

metahost=$(cat /etc/mailname)

(
    cat /etc/logwatch/conf/html/header.html
    cat <<EOF
<h1>$metahost <a href="./">$period</a> <a href="../">logwatch</a> on $date</h1>
<p class="info">
   Running on <b>$(hostname)</b> at <b>$(date)</b>
   &middot; range = <b>$range</b>
   &middot; detail = <b>$summary_detail</b>
</p>
<ul class="hosts">
EOF
    for host in $hosts; do
cat <<EOF
  <li>Host: <a class="hostlink" href="#${host}p">$host</a> &middot; <a href="$date.$host.html">detailed report</a>
EOF
    done
    cat <<EOF
</ul>
EOF
    for host in $hosts; do
        echo "<h1><a href="$date.$host.html">$host</a></h1>"
        #cat /var/lib/logwatch/daily/$date.$host.html \
        /usr/sbin/logwatch --range $range --detail $summary_detail --hostlimit $host --format html \
            | tail -n +$(( header_lines + 1 )) \
            | head -n -$footer_lines \
            | sed -r 's/(name|href)="?(#?)([^"> ])+"?/\1="\2'${host}'\3"/g'
    done
cat <<EOF
<p>This page was assembled by <code>${BASH_SOURCE[0]}</code>, running as <code>$0</code>
EOF
    cat /etc/logwatch/conf/html/footer.html
) | tee $all_file | (
  echo "To: sysadmin"
  echo "Content-Type: text/html; "
  echo Subject: $metahost $period logwatch
  echo
  cat
) | sendmail -t

#mail -a "Content-type: text/html" -s "$metahost $period logwatch" sysadmin

