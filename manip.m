markread(g)
 s ^ART(g,"read")=$h
 s ^IND("read",$h)=g
 q
isread(g)
 q $d(^ART(g,"read"))
setfname(g,file)
 s ^ART(g,"file")=file
 q
getfname(g,file)
 s file=^ART(g,"file")
 q
