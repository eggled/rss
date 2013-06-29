markread(g)
 s ^ART(g,"read")=$h
 s ^IND("read",$h)=g
 q
isread(g)
 q $d(^ART(g,"read"))
newfname(g,file)
 s:0=$d(^FILES) ^FILES=0
 s file=$i(^FILES)
 l +^FILES(file):0
 f  q:($TEST=1)&(0=$d(^FILES(file)))  s file=$i(^FILES) l +^FILES(file):0
 s ^ART(g,"file")=file
 q
getfname(g,file)
 s file=""
 q:0=$d(^ART(g,"file"))
 s file=^ART(g,"file")
 q
setfields(g,title,link,pubDate,description,creator,publisher,publink)
 s ^ART(g,"title")=title,^ART(g,"link")=link,^ART(g,"pubDate")=pubDate,^ART(g,"description")=description,^ART(g,"creator")=creator,^ART(g,"publisher")=publisher,^ART(g,"publink")=publink
 q
reset
 k ^ART,^IND
 q
getmetadata(g,title,link,publink,publisher,creator)
 f  s g=$o(^ART(g)) q:g=""  q:'$d(^ART(g,"read"))
 q:g=""
 s title=^ART(g,"title"),link=^ART(g,"link"),publink=^ART(g,"publink"),publisher=^ART(g,"publisher"),creator=^ART(g,"creator")
 q
get(g,which,value)
 s value=""
 o "/tmp/mumps":(append)
 u "/tmp/mumps"
 w "In get function",!
 q:'$d(^ART(g,which))
 w "even have data to return: ",value,!
 s value=^ART(g,which)
 q
