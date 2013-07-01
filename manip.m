q
; 
; s out="/tmp/mumps" o out:(append) u out
; markread
; g (input): guid of feed item to mark read
markread(g)
 s pubDate=^ART(g,"pubDate")
 k ^IND("unread",pubDate,g)
 l +^IND("unread",pubDate)
 k:0=$d(^IND("unread",pubDate)) ^IND("unread",pubDate)
 l -^IND("unread",pubDate)
 s ^ART(g,"read")=$h
 q
; isread
; g (input): guid of feed item
; returns: positive integer if that item has been read, otherwise zero.
isread(g)
 q $d(^ART(g,"read"))
; newfname
; g (input): guid of feed item
; file (output): receives an integer value, unique since database creation
; stores this file number under the guid as the file name
newfname(g,file)
 s:0=$d(^FILES) ^FILES=0
 s file=$i(^FILES)
 l +^FILES(file):0
 f  q:($TEST=1)&(0=$d(^FILES(file)))  s file=$i(^FILES) l +^FILES(file):0
 s ^ART(g,"file")=file
 q
; getfname
; g (input): guid of the feed item
; file (output): will be an empty string if no file is defined, otherwise returns the defined content file.
getfname(g,file)
 s file=""
 q:0=$d(^ART(g,"file"))
 s file=^ART(g,"file")
 q
; setfields
; Takes all the info for a feed and stores it in the database. All params are input-only.
setfields(g,title,link,pubDate,description,creator,publisher,publink)
 s markunread=$s(0=$d(^ART(g)):1,1:0)
 s updatepubDate=$s(1=$d(^ART(g,"pubDate")):$s(pubDate'=^ART(g,"pubDate"):1,1:0),1:1)
 s ^ART(g,"title")=title,^ART(g,"link")=link,^ART(g,"pubDate")=pubDate,^ART(g,"description")=description,^ART(g,"creator")=creator,^ART(g,"publisher")=publisher,^ART(g,"publink")=publink
 l +^IND("unread",pubDate) 
 s:markunread=1 ^IND("unread",pubDate,g)=1 
 l -^IND("unread",pubDate)
 s:updatepubDate'=0 ^IND("all",pubDate,g)=1
 q
; reset
; Clears all feed info (start over)
reset
 k ^ART,^IND
 q
; getmetadata
; g (input/output), all other parameters are output-only.
; gets the entry after g (so, pass "" to get the first one). Stores the info in the output parameters.
; If there is no entry after g, g will be set to "", and the other field values are undefined.
getmetadata(g,title,link,publink,publisher,creator)
 s pubDate=$s(g="":$o(^IND("unread","")),1:^ART(g,"pubDate"))
 q:pubDate=""
 s g=$o(^IND("unread",pubDate,g))
 s:g="" pubDate=$o(^IND("unread",pubDate))
 q:pubDate=""
 s:g="" g=$o(^IND("unread",pubDate,g))
 s title=^ART(g,"title"),link=^ART(g,"link"),publink=^ART(g,"publink"),publisher=^ART(g,"publisher"),creator=^ART(g,"creator")
 q
; get
; gets a single piece of metadata.
; g (input) : guid
; which (input): string name of the field
; value (output): gets the value (or null if no such value exists)
get(g,which,value)
 s value=""
 q:'$d(^ART(g,which))
 s value=^ART(g,which)
 q
; getfeed
; gets the feed url for the feed following what's passed
; feed (I/O)
getfeed(feed)
 s feed=$o(^FEEDS("active",feed))
 q
getanyfeed(feed)
 s feed=$o(^FEEDS("all",feed))
 q:feed="" 0
 q ^FEEDS("all",feed)
setfeedinfo(feedurl,title)
 s ^FEEDS(feedurl,"title")=title
 q
