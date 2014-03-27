

/*
 - usage select model with a skin
 - run script (ctrl-e  or evaluate all)
*/


--just an export script anyway
undo off


--bone data

global vecBone = #()

--struct sbvert ( numweight,  vecid,  vecweight ) 
struct xbone (bn, dp, ix)

fn compBone v1 v2 =
(
	if (v1.dp < v2.dp) then ( -1 )
	else if (v1.dp > v2.dp) then ( 1)
	else ( 0 )
)--comp


fn getBones mdl =
(
	local i, sk, numbone, bon, d, par
	
	vecBone = #()
	sk = undefined
	sk = mdl.modifiers[#skin]
	
	if (sk == undefined) then
	(
			format "[vms_export]  no skin modifier on model % \n " mdl
			return false
	)--endif
	
	select mdl
	SetCommandPanelTaskMode #modify
	ModPanel.SetCurrentObject sk
	
	numbone = skinops.getnumberbones sk
	
	for i = 1 to numbone do
	(
		nam = skinops.getbonename sk i 1
		bon = getnodebyname nam
		
		--note -- converting into C++ array pos when defining the new id
		setuserprop bon "skel_bid" (i-1)
		
			par = bon.parent
			d = 0
			while (par != undefined) do
			(
				d += 1
				par = par.parent
			)--wend
		
			--format "bone: %  d: % \n" bon d
		vecBone[i] = xbone bon (copy d) (copy i)
	)--nexti
	
	qsort vecBone compBone

	for i = 1 to numbone do
	(
		bon = vecBone[i].bn
		setuserprop bon "skel_bid" (i-1)
		
	)--nexti
		
	return true
)--getbones


--return is supposed to be slow but for now who cares
-- (dont fix until broken)
--parameter is vecBone array
fn getBoneHand bindex vec =
(
	local i, b, num, nid

	num = vec.count
	
	for i = 1 to num do
	(
		b = vec[i]
		if (bindex == b.ix) then
		( 
			i = num
			
			nid = getuserprop b.bn "skel_bid"
			--format "nid % \n" nid
			--new id is pretty much the same as the skin id
			--but just in case we use the new id anyway 
			
			return nid
			--return b.bn.handle
		)
	)--nexti
-1
)--getbonehand



fn writeBonesBin out =
(
	local f, i, bon, numbone, parid, bonegeo
  
	numbone = vecBone.count

  for i = 1 to numbone do
	(
    bon = vecBone[i]
		
    bonegeo = bon.bn
    
    if (bonegeo.parent == undefined) then ( parid = -1 )
    else (	parid = getuserprop (bonegeo.parent) "skel_bid"	)
    
    bid = getuserprop bonegeo "skel_bid"
    
    -- id, parent id, depth 
    writelong out bid
    writelong out parid
    writelong out bon.dp

    --pos rot scale
    at time 0
    (   writeFrame bon.bn out   )
    
	)--nexti
	
)--writebonebin


--todo -- anim save and bone saving should be in the same script (add menu or something)
--todo -- maybe at time should be outside of this
fn writeFrame bonegeo out  =
(
	local pos, rot, sca, par	
	local mtx, pmtx, bid
	
	--todo -- so far only tested with biped
	--not sure if all bones are exported correctly 
	
	--at time fr
	--(
		
		if (exskin == 1) then
		(
				if (classof bonegeo == Biped_Object )	then
				(
					pos = biped.gettransform  bonegeo #pos
					rot = biped.gettransform  bonegeo #rotation
          sca = biped.gettransform  bonegeo #scale
					rot.x = -rot.x;					rot.y = -rot.y;					rot.z = -rot.z
				) 
				else
				(
					--ok so now what
					pos = in coordsys world  bonegeo.pos			
          rot = in coordsys world  bonegeo.rotation					
          sca = in coordsys world  bonegeo.scale			
					--seems this one is needed for non-biped as well (who knew)
					rot.x = -rot.x;					rot.y = -rot.y;					rot.z = -rot.z
				)--endif
		)
		else
		(
			--format "EXPORTING TEST \n"
			
			--http://www.gamedev.net/topic/572682-maxscript-retrieving-bone-transformation-help-anyone/
			
			if (bonegeo.parent == undefined) then			(		mtx =  bonegeo.transform			)
			else (		pmtx = inverse ( bonegeo.parent.transform); 	mtx = ( bonegeo.transform) * pmtx		)
      
			--inverse rotation -- now its ok but upside down (??)
			--so what to do now (?)
			pos = copy mtx.translationpart;			rot = copy ( inverse mtx).rotationpart;			sca = copy mtx.scalepart 
		
		)--endifexskin	
		
		/* --update: the skin is now put together in the engine itself
       --inverted skin
		if (exskin == 2) then
		(
			mtx = inverse bonegeo.transform;			pos = copy mtx.translationpart
			rot =  copy ( inverse mtx).rotationpart;			sca = copy mtx.scalepart 
		)--exskin		*/
	--)--time
	
  --export is in opengl coordinates
    rot = normalize rot
  

    
    --write pos
      writefloat out pos.x;      writefloat out pos.z;      writefloat out -pos.y
     
    --write rotation
      writefloat out rot.x;      writefloat out rot.z;      writefloat out -rot.y;      writefloat out rot.w
      
    --write scale
      writefloat out sca.x;      writefloat out sca.z;      writefloat out sca.y

)--writeframe


-- i wrote this thing long ago and even forgot it exists
-- and attempted to write it again
fn getVertLookUp mdl t_ =
(
	local i, m, numface, numvert
	local ppos, puv, puv2, mf, mf2
	local found, f, trans , v
	local veclook, vecuv, vecuv2, vecvert, vecnorm, vecindex, vectemp
	local it
	
	veclook = #();	vecuv = #();	vecuv2 = #();	vecvert = #();	vecnorm = #();	vecindex = #();	vectemp = #()

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

	if  ((meshop.getMapSupport m 1) == false)  then(	meshop.defaultMapFaces m 1	)
	if  ((meshop.getMapSupport m 2) == false)  then	(	meshop.defaultMapFaces m 2	)


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

			ppos = findItem vecVert pos
			puv = findItem vecUv uv
			puv2 = finditem vecUv2 uv2
			
			--normals are not tested for uniqueness yet
			-- (but it can be used as option easily, e.g. export based on smoothing groups)
			
			if (ppos != 0 and ppos == puv and ppos == puv2) then	(	found = 1		)
			
			if (found == 0) then
			(
				append vecVert pos; append vecUv uv; 	append vecUv2 uv2;	append vecNorm norm
				append vecIndex it
				it += 1
			)
			else (	append vecIndex ppos	)
			
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


/*
--only a reference now
fn writeVertLookUp vec =
(
out = openfile  "c://max/vertlook.vlu" mode:"wt"
 format "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" to:out
  num = vec.count;  format "<vertlook  numvert=\"%\">\n" num to:out
	--remember max indexing starts at 1	--anything else sensible starts at 0
  for i = 1 to num do (  g = vec[i];  format "  <vert id=\"%\"  />\n" (g-1) to:out )--nexti
  format "</vertlook >\n" to:out
  flush out; close out
)--writevertlookup
*/



-- mesh - geometry 
-- veg - veclookup
-- out - binary outfile
-- skinframe - frame on timeline used as skin pose 
fn writeSkinBin mesh vec out skinframe =
(
  local numbone, numvert, xmode
	local sk, v, m, w, rw, bid, num, bhand
	local vecLookup
  vecLookup = vec
  
  sk = undefined
	sk = mesh.modifiers[#skin]
	
	if (sk == undefined) then
	(	format "no skin modifier on model % \n " mesh; 	return false )--endif

  select mesh
	SetCommandPanelTaskMode #modify
	ModPanel.SetCurrentObject sk

	numbone = skinops.getnumberbones sk
	numvert = vecLookup.count
  
  xmode = 0 --mesh
	if ( (getclassname mesh) == "Editable Poly") then
  ( xmode = 1 ) --poly
  
  
  at time skinframe
  (
 	num = vecLookup.count
 	for rw = 1 to num do
		(
			v = vecLookup[rw]
			numbone = skinops.getvertexweightcount sk v
      if (numbone > 4) then ( numbone = 4 ) --todo warning
			
			if (xmode == 0) then
			(	pos = in coordsys world getvert mesh v )
			else ( pos = in coordsys world polyop.getvert mesh v )
			
      
				writefloat out pos.x
				writefloat out pos.z
				writefloat out -pos.y
      
				writelong out numbone
      
		
				for m = 1 to numbone do
				(
					bid = skinops.GetVertexWeightBoneID sk v m
					bhand = getBoneHand bid vecBone
					writelong out bhand          
				)--nextm
				for m = numbone to 3 do (  writelong out 0  )
        
				for m = 1 to numbone do
				(
					w = skinops.getvertexweight sk v m
					writefloat out w
				)
				for m = numbone to 3 do (  writefloat out 0.0  )
        
		)--nextrw
	)--at

/*
struct xSkVert
{
  gamex::cVec3f pos;
  int numbone;
  int bone[4];
  float weight[4];
};//skvert
*/  

)--writeskinbin



--todo -- [filename] and [object to export] as parameter 
fn saveSkel =
(
  --try

    g = selection[1]
    if ( getBones(g) ) then
    (      
        fname = "export" 
        fname = ("c://max/"+fname+".xskb") 
        
        local out = fopen fname "wb"
           --  "MDX003_2013";
        skid = "SKELBINF002"; -- +/0
          writestring out skid
        numbone = vecBone.count
          writelong out numbone --number of bones
        vecLookup = getVertLookUp g 0;  	numvert = vecLookup.count
          writelong out numvert --number of skin vertices
          
        --reserved 16bytes
          writelong out 0;      writelong out 0;      writelong out 0;      writelong out 0
      
        writeBonesBin out
        writestring out "SKINPAR" -- +/0  == 8 bytes
        writeSkinBin g vecLookup out 0
        
      fflush out
      fclose out
    )--endif

)--saveskeleton





-- ANIM --






--binary format
fn saveAnim fname mesh startframe endframe =
(	
	local i,f,k

  if ( getBones(mesh) ) then
  (
      numbone = vecBone.count
      numframe = (endframe - startframe)+1 --so in max 0 is a frame too and 100 is a frame too
    
      outfile = fopen fname "wb"
      out = outfile
      
      fileid = "BAX003_2013"
      writestring out fileid
      
      writelong out numbone;      writelong out numframe
      
      --reserved 16bytes
      writelong out 0;      writelong out 0;      writelong out 0;      writelong out 0
      
        for f = startframe to endframe do
        (
          at time f
          (
            for i = 1 to numbone do
            (
              bon = vecBone[i]
              writeFrame bon.bn out
            )--nexti
          )--time
        )--nextf
        
    fflush outfile
    fclose outfile
	)--endif	
	
)--exportanim






-- MAIN --


Rollout xSkelRoll "xSkel exporter"
(

  group "Skel (.xskb)"
  (
    button btnSaveSkel "Save skeleton"
  )
  
  group "Anim (.xska)"
  (
    button btnSaveAnim "Save animation"
    edittext edtPrefix "Prefix: " text:"export_" fieldWidth:70 height:17  
    edittext edtAnim "Animname: " text:"run" fieldWidth:70 height:17
    spinner spinStart "Frame1 :" range:[0,512,0] type:#integer
    spinner spinEnd "Frame2 :" range:[0,512,30] type:#integer
  )

  -- big editbox for multiple anims
  -- save it in some global object
  -- its for expert use only
  -- button for export expert or something
  -- #("anim",0,20,  )
  -- string to array with maxscript black magic:
  -- myArray = execute string
  -- array to string:
  -- myString = myArray as string
  
 
  
  -- todo -- editbox or sliders or whatever to set up animations for export
  -- maybe some way to save global values on what to export etc
  
  on btnSaveSkel pressed do
  (
	--saveSkel(); 
	  
    try (   saveSkel();   print "Saved!"  )
    catch (format "*** % ***\n" (getCurrentException()))
	  
	  
  )--btnskel
  
  on btnSaveAnim pressed do
  (
    local g, sf, ef, fname
     try
	  (   
      g = selection[1]
      if (g != undefined) then
      (
		fname = "c://max/" +edtPrefix.text + edtAnim.text + ".xska"
        sf = spinStart.value
        ef = spinEnd.value
        saveAnim fname g sf ef  
      )
    print "Saved!"  
    )
    catch (format "*** % ***\n" (getCurrentException()))
  )--btnanim
  
)--skeltool



try  (   closeRolloutFloater xSkelTool )  catch ( )
global xSkelTool = newRolloutFloater "xSkel Tool" 350 400 50 50
addRollout xSkelRoll xSkelTool



/*
 edittext eName "Name: " fieldWidth:70 height:17 across:3
  button addKey "Add key"
	button removeKey "Rem key"
  listbox listKey "Key" items:#("empty") selection:1 readOnly:false align:#Righ
  button btnSortKey "Sort keys"
*/


