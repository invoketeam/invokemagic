
undo off

-- mdx3 exporter (work in progress)


fn getVertLookUp mdl t_ =
(
	local i
	local m
	local numface
	local numvert
	local ppos
	local puv
	local puv2
	local mf
	local mf2
	local found
	local f
	local trans
	local v
	local veclook
	local vecuv
	local vecuv2
	local vecvert
	local vecnorm
	local vecindex
	local vectemp
	local it
	
	veclook = #()
	vecuv = #()
	vecuv2 = #()
	vecvert = #()
	vecnorm = #()
	vecindex = #()
	vectemp = #()
	
	it = 0
	
at time t_
(
	--m = copy mdl
	m = snapshot mdl
)
	m.name = uniquename "export"
	converttomesh m

	trans = m.transform
	
	numface = meshop.getnumfaces m
	numvert = meshop.getnumverts m

	if  ((meshop.getMapSupport m 1) == false)  then
		(	meshop.defaultMapFaces m 1	)
		
	if  ((meshop.getMapSupport m 2) == false)  then
		(	meshop.defaultMapFaces m 2	)


	for i = 1 to  numface do
	(
		f = getface m i
		mf = meshop.getmapface m 1 i 
		mf2 = meshop.getmapface m 2 i 
		
		for v = 1 to 3 do
		(
			pos = in coordsys trans  getvert m f[v]
			uv = meshop.getmapvert m 1 mf[v]
			uv2 = meshop.getmapvert m 2 mf2[v]
			norm =  getNormal m f[v]
			
			found = 0		
		 
		--	num = vecVert.count
		
			ppos = findItem vecVert pos
			puv = findItem vecUv uv
			puv2 = finditem vecUv2 uv2
			
			--format "pos % \n "  (findItem vecVert pos)
			--format "uv % \n "  (findItem vecUv uv)
			
			--normals are not tested for uniqueness yet
			-- (but it can be used as option easily, e.g. expert based on smoothing groups)
			
			if (ppos != 0 and ppos == puv and ppos == puv2) then
			(
				found = 1
			--	format "index % \n " ppos
			)
			
			
			if (found == 0) then
			(
				append vecVert pos
				append vecUv uv
				append vecUv2 uv2
				append vecNorm norm
				
				append vecIndex it
				it += 1
			)
			else
			(
				append vecIndex ppos
			)
			
		)--nextv
	)--nexti
	
	
	for i = 1 to numvert do
	(
		pos = in coordsys trans getvert m i
		append vecTemp pos
	)--nexti

	num = vecVert.count
	for i = 1 to num do
	(
		append vecLook (finditem vecTemp vecVert[i])
	)--nexti

	
	delete m
	
vecLook	
)--getlookup




fn writeMesh out mdl inworld = 
(
	local m
	local vecVert
	local vecUv
	local vecUv2
	local vecNorm
	local vecIndex
	local vecTemp
	local vecLook
	local it
	local found
	local i
	local v
	local pos
	local norm
	local uv
	local uv2
	local ppos
	local puv
	local puv2
	local num
	local numface
	local numvert
	local trans
	local vecTarg
	
	
--todo  snapshot instead?
at time 0
(	
--	vecTarg = getVecTarg()


	--m = copy mdl
	m = snapshot mdl
)
	m.name = uniquename "export"
	converttomesh m

	numface = meshop.getnumfaces m
	numvert = meshop.getnumverts m

	if  ((meshop.getMapSupport m 1) == false)  then
		(	meshop.defaultMapFaces m 1	)
		
	if  ((meshop.getMapSupport m 2) == false)  then
		(	meshop.defaultMapFaces m 2	)

	

	vecVert = #()
	vecUv = #()
	vecUv2 = #()
	vecNorm = #()
	--todo: add normal, second uv
	vecIndex = #() --actually faces index
	vecLook = #()
	vecTemp =#()
	--also need to build index for just vertices 
	--or just use vecindex and export more than once (?)
	it = 1
	found = 0
	

	
	trans = m.transform
	
	if (inworld != unsupplied) then
	(
		if (inworld == true) then
		( trans = matrix3 1) --world coords
	)
	
	for i = 1 to  numface do
	(
		f = getface m i
		mf = meshop.getmapface m 1 i 
		mf2 = meshop.getmapface m 2 i 
		
		for v = 1 to 3 do
		(
			pos = in coordsys trans  getvert m f[v]
			uv = meshop.getmapvert m 1 mf[v]
			uv2 = meshop.getmapvert m 2 mf2[v]
			norm =  getNormal m f[v]
			
			found = 0		
		 
		--	num = vecVert.count
		
			ppos = findItem vecVert pos
			puv = findItem vecUv uv
			puv2 = finditem vecUv2 uv2
			
			--format "pos % \n "  (findItem vecVert pos)
			--format "uv % \n "  (findItem vecUv uv)
			
			--normals are not tested for uniqueness yet
			-- (but it can be used as option easily, e.g. expert based on smoothing groups)
			
			if (ppos != 0 and ppos == puv and ppos == puv2) then
			(
				found = 1
			--	format "index % \n " ppos
			)
			
			
			if (found == 0) then
			(
				append vecVert pos
				append vecUv uv
				append vecUv2 uv2
				append vecNorm norm
				
				append vecIndex it
				it += 1
			)
			else
			(
				append vecIndex ppos
			)
			
		)--nextv
	)--nexti


	--build lookuptable for vertex positions (used for saving vertex animation a bit faster)
	-- (todo: need to be at skin position)
	for i = 1 to numvert do
	(
		pos = in coordsys trans getvert m i
		append vecTemp pos
	)--nexti

	num = vecVert.count
	for i = 1 to num do
	(
		append vecLook (finditem vecTemp vecVert[i])
	)--nexti

	--format "vecLook % \n" vecLook


	--format "count %  vecIndex % \n" vecIndex.count vecIndex
	--format "count %  vecVert % \n" vecVert.count vecVert
	--format "count %  vecUv % \n" vecUv.count vecUv

	delete m


	format " numface %  numVert % \n" numface vecVert.count
	
	writelong out numface
	writelong out  (vecVert.count)
	
	
	local mmin
	local mmax
	local mpos
	local rad
	
	mmin = in coordsys mdl.transform mdl.min
	mmax = in coordsys mdl.transform mdl.max
	

	
	rad = length( mmax-mmin)
	mpos = mmin + (mmax - mmin) * 0.5
	
	--10 float (40 bytes)
	
	writefloat out rad 		--used for frustum culling (radius of sphere that this mesh fits in)
	
	writefloat out mmin.x
	writefloat out mmin.z
	writefloat out (-mmax.y)
	
	writefloat out mmax.x
	writefloat out mmax.z
	writefloat out (-mmin.y)
	
	--offset (need to add this to pivot to be on the center of the model)
	writefloat out mpos.x
	writefloat out mpos.z
	writefloat out (-mpos.y)
	

	--32 bytes reserved
	-- (i hope it will be enough, i dont have a use it for yet though)
	--writelong out 0
	
	writeshort out 0 --(vecTarg.count) --no targets
	writeshort out 0
	
	writelong out 0
	writelong out 0
	writelong out 0
	
	writelong out 0
	writelong out 0
	writelong out 0
	writelong out 0
	
	--total extra is 72 bytes
	

	for i in vecIndex do
	(
		writeshort out (i - 1 )
	)--nexti

	num = vecVert.count

	for v = 1 to num do
	(
		pos = vecVert[v]
		uv = vecUv[v]
		uv2 = vecUv2[v]
		norm = vecNorm[v]
		
		writefloat out pos.x
		writefloat out pos.z
		writefloat out (-pos.y)
		
		writefloat out uv.x
		writefloat out uv.y
		
		writefloat out uv2.x
		writefloat out uv2.y
		
		--writelong out 0	--normal (only used for software light calculation, 4 bytes)
		--compression:  1 byte 1 byte 1 byte ..  +  1 unused byte (always 0) as a normal is normalised: x*100 y*100 z*100 as integer
		writebyte out ((norm.x * 100) as integer)
		writebyte out ((norm.z * 100) as integer)
		writebyte out (((-norm.y) * 100) as integer)
		writebyte out 0  --unused byte
		writelong out 0 	--color (rgba) not exported yet, but reserved
	)--nextv
	
	
	at time 0
	(
	--write target data
	--	writeVecTarg out vecTarg trans
	)

	-- not part of binary
	-- target text guide
	--	makeVecTargGuide vecTarg

)--writemesh





















/*
get all exportable models
*/
fn getMdVec =
(
  local g
	local vec
	
	vec = #()
	
	for g in geometry do
	(
		if ((isvalidnode g) and  g.ishidden == false ) then
		(
			append vec g		
		)--endif	
	)--nextg

  --return
	vec
)--getmdvec


/* 
xml guide of models
*/
fn fileMdXm vec =
(
  local out
  local i
  local num
  local g
  local fname
  
  out = openfile  "c://max/modguid.wxm" mode:"wt"
	
 format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" to:out
  format "<modelguide >\n" to:out
  
	
  num = vec.count
  for i = 1 to num do
  (
    g = vec[i]
	
     
     fname = "model_" + (g.handle as string) + ".mdx3"

    format "  <model id=\"%\" name=\"%\"  filename=\"%\" />\n" g.handle g.name fname to:out
  
  )--nexti
  
  
   format "</modelguide >\n" to:out

  flush out
	close out
)--filemdxm



fn exportAllMdx =
(
  local vec
  local g
  local out
  local outfile
  local num
  local i
  local fname
  
  vec = getMdVec()
  num = vec.count
  
  fileMdXm vec
  
  for i = 1 to num do
  ( 
    g = vec[i]
  
    fname = "model_" + (g.handle as string) + ".mdx3"
    
     outfile = fopen ("c:\\max\\"+fname) "wb" 
     out = outfile

		-- 12-1  (-1 byte for /0)
		head = "MDX003_2013"
		writestring out head

		writemesh out g

    fflush outfile
    fclose outfile
  )--nexti


)--exportall


fn exportMdx =
(
  local g
  local out
  local outfile
  
  g = selection[1]

	outfile = fopen "c:\\max\\export.mdx3" "wb" 
	out = outfile

		-- 12-1  (-1 byte for /0)
		head = "MDX003_2013"
		writestring out head

		writemesh out g false

	fflush outfile
	fclose outfile
  
)--exmdx



Rollout mdxRoll "MDX3 tool"
(
  button saveMdx "Save selected MDX"
  button saveAllMdx "Save all MDX"
  
  on saveMdx pressed do
  (
	  exportMdx()
    --try ( exportMdx() ) 
	 -- catch (format "*** % ***\n" (getCurrentException()))
  )--savemdx
    
  on saveAllMdx pressed do
  (
    try ( exportAllMdx() ) 
	  catch (format "*** % ***\n" (getCurrentException()))
  )--savemdx
  
)--mdxroll

if (dontOpen == undefined or  dontOpen != 1  ) then
(
try  (   closeRolloutFloater mdx3Tool )  catch ( )
global mdx3Tool = newRolloutFloater "MDX3 Tool" 350 400 50 50
addRollout mdxRoll mdx3Tool
)
dontOpen = 0