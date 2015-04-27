<?php
/**
 * Collectd Control Panel configuration specific to this sSysconf profile
 */

$CONFIG['term'] = array(
        '1hour'  => 3600 * 1,
        '2hour'  => 3600 * 2,
        '8hour'  => 3600 * 8,
        'day'    => 86400,
        'week'   => 86400 * 7,
        'month'  => 86400 * 31,
        'quarter'=> 86400 * 31 * 3,
        'year'   => 86400 * 365,
);

# "png", "svg", "canvas" or "hybrid" (canvas on detail page, png on the others) graphs
$CONFIG['graph_type'] = 'hybrid';

# add XXth percentile line + legend to network graphs
# false = disabled; 95 = 95th percentile
$CONFIG['percentile'] = true;

# create smooth graphs (rrdtool -E)
$CONFIG['graph_smooth'] = true;

# draw min/max spikes in a lighter color in graphs with type default
$CONFIG['graph_minmax'] = true;
