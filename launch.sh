#!/bin/bash

export gtmdir="/home/eggled/rss/gtm/db"
. ${gtmdir}/../gtmV60002/gtmprofile
export LD_LIBRARY_PATH=${gtm_dist}
./rssgen
