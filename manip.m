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
 s file=^ART(g,"file")
 q
