print("Malom")

-- palya (L)
--		mezo (T)
--			id (int)
--			szin (int)
--			b,f,j,l (int)

-- gbabu (int) -- bábuk száma a kezében
-- glbabu (int) -- bábuk száma a játékban
-- szin (int) -- saját szine
-- eszin (int) -- jatekos szine

hova=-1
honnan=-1
leutotte=-1

allasok = {}
tart = 0

function malome(hanyadik,irany,mszin,ezt)

	if (ezt[ezt[hanyadik].b] and ezt[ezt[hanyadik].j] and (irany==0 or irany==1 or irany==3) and ezt[ezt[hanyadik].b].szin==mszin and ezt[ezt[hanyadik].j].szin==mszin) then return true end
	--if ezt[ezt[hanyadik].b] and ezt[ezt[hanyadik].j] then print("bj") end
	--if (irany==0 or irany==1 or irany==3) then print("bji") end
	--if ezt[ezt[hanyadik].b] and ezt[ezt[hanyadik].j] and (ezt[ezt[hanyadik].b].szin==mszin and ezt[ezt[hanyadik].j].szin==mszin) then print("bjszin") end

	if (ezt[ezt[hanyadik].f] and ezt[ezt[hanyadik].l] and (irany==0 or irany==2 or irany==4) and ezt[ezt[hanyadik].f].szin==mszin and ezt[ezt[hanyadik].l].szin==mszin) then return true end
	--if ezt[ezt[hanyadik].f] and ezt[ezt[hanyadik].l] then print("fl") end
	--if (irany==0 or irany==2 or irany==4) then print("fli") end
	--if ezt[ezt[hanyadik].f] and ezt[ezt[hanyadik].l] and (ezt[ezt[hanyadik].f].szin==mszin and ezt[ezt[hanyadik].l].szin==mszin) then print("flszin") end

	if (( irany==0 or irany==1) and ezt[ezt[hanyadik].b] and ezt[ezt[hanyadik].b].szin==mszin) then if ( malome(ezt[hanyadik].b,1,mszin,ezt) ) then return true end end
	if (( irany==0 or irany==2) and ezt[ezt[hanyadik].f] and ezt[ezt[hanyadik].f].szin==mszin) then if ( malome(ezt[hanyadik].f,2,mszin,ezt) ) then return true end end
	if (( irany==0 or irany==3) and ezt[ezt[hanyadik].j] and ezt[ezt[hanyadik].j].szin==mszin) then if ( malome(ezt[hanyadik].j,3,mszin,ezt) ) then return true end end
	if (( irany==0 or irany==4) and ezt[ezt[hanyadik].l] and ezt[ezt[hanyadik].l].szin==mszin) then if ( malome(ezt[hanyadik].l,4,mszin,ezt) ) then return true end end

	return false
end

function szomszede(hanyadik,szom,ezt)
	if ezt[szom].b==hanyadik then return true end
	if ezt[szom].f==hanyadik then return true end
	if ezt[szom].j==hanyadik then return true end
	if ezt[szom].l==hanyadik then return true end
	return false
end

function szomszedokszama(hanyadik,ezt)
	local szi=0
	if ezt[ezt[hanyadik].b] and ezt[ezt[hanyadik].b].szin==6 then szi=szi+1 end
	if ezt[ezt[hanyadik].f] and ezt[ezt[hanyadik].f].szin==6 then szi=szi+1 end
	if ezt[ezt[hanyadik].j] and ezt[ezt[hanyadik].j].szin==6 then szi=szi+1 end
	if ezt[ezt[hanyadik].l] and ezt[ezt[hanyadik].l].szin==6 then szi=szi+1 end
	return szi
end

function vanenemmalom(mszin,ezt)
	for i,mezo in ipairs(ezt) do
		if (mezo.szin==mszin and not malome(i,0,mszin,ezt)) then return true end
	end
	return false
end

function kiertekel(ezt)
	local p = {1,1}
	for i,mezo in ipairs(ezt) do
		local id = 0

		if mezo.szin~=6 then 
			if mezo.szin==szin then id=2 mszin=szin else id=1 mszin=eszin end
			
			-- A mellete lévő mezők közül az ér pontot amin ő vagy senki nem áll
			if ezt[mezo.b] then
				if ezt[mezo.b].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.b].szin==mszin then p[id]=p[id]+1 end 
			end
			if ezt[mezo.f] then
				if ezt[mezo.f].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.f].szin==mszin then p[id]=p[id]+1 end 
			end
			if ezt[mezo.j] then
				if ezt[mezo.j].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.j].szin==mszin then p[id]=p[id]+1 end 
			end
			if ezt[mezo.l] then
				if ezt[mezo.l].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.l].szin==mszin then p[id]=p[id]+1 end 
			end

			if malome(i,0,mszin,ezt) then p[id]=p[id]+1 end -- A malmot 3x számolja
		end
	end
	return (p[2]/p[1])
end

function jolepes(hanyadik,felszedve,uthet,ezt,babu,lbabu,mszin,meszin)
	if not ezt[hanyadik] then return false, felszedve, uthet end
	ijsz = ezt[hanyadik].szin

	if (ijsz==6 and babu>0 and (felszedve==0 or szomszede(hanyadik,felszedve,ezt) or lbabu==3) and not uthet ) then
		--print("hova "..hanyadik)
		ezt[hanyadik].szin=mszin
		babu=babu-1
		if (malome(hanyadik,0,mszin,ezt)) then uthet=true end
		--print(uthet)
		return true, felszedve, uthet, babu, lbabu
	elseif (ijsz==mszin and felszedve==0 and not uthet) then
		if (szomszedokszama(hanyadik,ezt)>0 or lbabu==3) then
			--print("honnan "..hanyadik.." "..szomszedokszama(hanyadik,ezt))
			felszedve=hanyadik
			babu=babu+1
			ezt[hanyadik].szin=6
			return true, felszedve, uthet, babu, lbabu
		end
	elseif (uthet and ijsz==meszin) then
		if (not malome(hanyadik,0,mszin,ezt) or not vanenemmalom(meszin,ezt) ) then
			--print("uthet "..hanyadik)
			ezt[hanyadik].szin=6
			return true, felszedve, uthet, babu, lbabu
		end
	end
	--print("semmi "..hanyadik)
	return false, felszedve, uthet, babu, lbabu
end


function lephet()
	hova=hova+1
	honnan=honnan+1
	leutotte=leutotte+1
	print(hova.." "..honnan.." "..leutotte)
	palya[hova].szin=eszin
	if honnan>0 then palya[honnan].szin=6 end
	if leutotte>0 then palya[leutotte].szin=6 glbabu=glbabu-1 end

	while(true) do
		local gyid,megvan = 0, true;
		for i,mezo in ipairs(allasok[1].palya) do
			if mezo.szin~=palya[i].szin then megvan=false break end
		end
		if megvan then
			local max=0;
			for i,gy in ipairs(allasok[1].gyerekek) do
				if gy.ertek>max then 
					max=gy.ertek
					hova=gy.p.hova-1
					honnan=gy.p.honnan-1
					leutotte=gy.p.leutotte-1
				end
			end
			break
		end
		--print("-tart: ",tart,#allasok)
		table.remove(allasok,1)
		tart=tart-1
		if (#allasok==0) then init() szamol() end
	end

	if hova>-1 then palya[hova+1].szin=szin end
	if honnan>-1 then palya[honnan+1].szin=6 end
	if leutotte>-1 then palya[leutotte+1].szin=6 end
	print(hova.." "..honnan.." "..leutotte)
end

function init()
	allas = {}
	allas.palya = {}
	for i,mezo in ipairs(palya) do
		local id = mezo.id
		local szin = mezo.szin
		local f = mezo.f
		local l = mezo.l
		local b = mezo.b
		local j = mezo.j
		table.insert(allas.palya,{id=id,szin=szin,f=f,l=l,b=b,j=j})
	end
	allas.babu=gbabu;
	allas.lbabu=glbabu;
	allas.p={hova=0,honnan=0,leutotte=0}
	allas.ertek=kiertekel(allas.palya)
	allas.ki=1 -- kié volt ez az állás
	allas.gyerekek={}
	table.insert(allasok,allas)
	tart = 1
end

function lep(la,ln,lu,ezt,babu,lbabu,mszin,meszin)

	local bla = 0
	local id = 0
	if ln~=0 then 
		id=ln-1 --ugyanott
		bla=la
	else 
		id=la 
	end

	local jo, felszedve, uthet = false, 0, false
	while (not jo and id<=#ezt) do
		id=id+1
		jo, felszedve, uthet, babu, lbabu = jolepes(id,felszedve,uthet,ezt,babu,lbabu,mszin,meszin)
	end
	if id>#ezt then return 0, 0, 0, babu, lbabu end

	if felszedve>0 then
		ln=id
		id=bla
		jo = false
		while (not jo and id<=#ezt) do
			id=id+1
			jo, felszedve, uthet, babu, lbabu = jolepes(id,felszedve,uthet,ezt,babu,lbabu,mszin,meszin)
		end
		if id>#ezt then return 0, 0, 0, babu, lbabu end
	end
	la=id
	id=lu

	if uthet then
		jo = false
		while (not jo and id<=#ezt) do
			id=id+1
			jo, felszedve, uthet, babu, lbabu = jolepes(id,felszedve,uthet,ezt,babu,lbabu,mszin,meszin)
		end
		if id>#ezt then return 0, 0, 0, babu, lbabu end
		lu = id;
	end
	return la, ln, lu, babu, lbabu
end

function szamol()

	if #allasok>100000 then return end

	local la, ln, lu = 0,0,0

	while (true) do
		local megvan = true
		for i,mezo in ipairs(allasok[1].palya) do
			if mezo.szin~=palya[i].szin then megvan=false break end
		end 
		if megvan then break end
		table.remove(allasok,1)
	end

	while (true) do

		local mszin, meszin = szin, eszin
		if allasok[tart].ki==1 then mszin=eszin meszin=szin end
	
		allas = {}
		allas.palya = {}
		for i,mezo in ipairs(allasok[tart].palya) do
			local id = mezo.id
			local szin = mezo.szin
			local f = mezo.f
			local l = mezo.l
			local b = mezo.b
			local j = mezo.j
			table.insert(allas.palya,{id=id,szin=szin,f=f,l=l,b=b,j=j})
		end
		allas.babu=allasok[tart].babu;
		allas.lbabu=allasok[tart].lbabu;
		allas.p={hova=0,honnan=0,leutotte=0}
		if allasok[tart].ki==1 then allas.ki=0 else allas.ki=1 end -- a következő lépő más lesz.
		allas.gyerekek = {}

		la, ln, lu, allas.babu, allas.lbabu = lep(la,ln,lu,allas.palya,allas.babu,allas.lbabu,mszin,eszin)

		allas.p.hova, allas.p.honnan, allas.p.leutotte = la, ln, lu

		if allas.p.hova==0 then break end

		allas.ertek=kiertekel(allas.palya)
		table.insert(allasok,allas)
		table.insert(allasok[tart].gyerekek,allasok[#allasok]) -- elmenti hogy ő belőle lépték

		--[[for i,mezo in ipairs(allas.palya) do
			if mezo.szin~=6 then print(i,mezo.szin) end
		end
		print("ertek: ",allas.ertek)
		print("mozgas: ",allas.p.hova, allas.p.honnan, allas.p.leutotte)]]

	end
	tart = tart+1
	print("tart:",tart)
end