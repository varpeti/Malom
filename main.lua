print("Malom")

-- palya (L)
--		mezo (T)
--			id (int)
--			szin (int)
--			b,f,j,l (int)

-- db (int) -- bábuk száma a kezében
-- lbabu (int) -- bábuk száma a játékban
-- szin (int) -- saját szine
-- eszin (int) -- jatekos szine

hova=-1
honnan=-1
leutotte=-1

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
			if mezo.b then
				if ezt[mezo.b].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.b].szin==mszin then p[id]=p[id]+1 end 
			end
			if mezo.f then
				if ezt[mezo.f].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.f].szin==mszin then p[id]=p[id]+1 end 
			end
			if mezo.j then
				if ezt[mezo.j].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.j].szin==mszin then p[id]=p[id]+1 end 
			end
			if mezo.l then
				if ezt[mezo.l].szin==6 then p[id]=p[id]+1 end 
				if ezt[mezo.l].szin==mszin then p[id]=p[id]+1 end 
			end

			if malome(i,0,mszin,ezt) then p[id]=p[id]+1 end -- A malmot 3x számolja
		end
	end
	return p[2]/p[1]
end

function jolepes(hanyadik,felszedve,uthet,ezt)
	if not ezt[hanyadik] then return false, felszedve, uthet end
	ijsz = ezt[hanyadik].szin

	if (ijsz==6 and db>0 and (felszedve==0 or szomszede(hanyadik,felszedve,ezt) or lbabu==3) and not uthet ) then
		--print("hova "..hanyadik)
		ezt[hanyadik].szin=szin
		db=db-1
		if (malome(hanyadik,0,szin,ezt)) then uthet=true end
		--print(uthet)
		return true, felszedve, uthet
	elseif (ijsz==szin and felszedve==0 and not uthet) then
		if (szomszedokszama(hanyadik,ezt)>0 or lbabu==3) then
			--print("honnan "..hanyadik.." "..szomszedokszama(hanyadik,ezt))
			felszedve=hanyadik
			db=db+1
			ezt[hanyadik].szin=6
			return true, felszedve, uthet
		end
	elseif (uthet and ijsz==eszin) then
		if (not malome(hanyadik,0,szin,ezt) or not vanenemmalom(eszin,ezt) ) then
			--print("uthet "..hanyadik)
			ezt[hanyadik].szin=6
			return true, felszedve, uthet
		end
	end
	--print("semmi "..hanyadik)
	return false, felszedve, uthet
end


function lephet()
	hova=hova+1
	honnan=honnan+1
	leutotte=leutotte+1
	print(hova.." "..honnan.." "..leutotte)
	palya[hova].szin=eszin
	if honnan>0 then palya[honnan].szin=6 end
	if leutotte>0 then palya[leutotte].szin=6 lbabu=lbabu-1 end

	hova = -1
	honnan = -1
	leutotte = -1

	local id = 1

	local jo, felszedve, uthet = false, 0, false
	while (not jo) do
		id=math.random(0,#palya)
		jo, felszedve, uthet = jolepes(id,felszedve,uthet,palya)
	end

	if felszedve>0 then
		honnan=id-1
		jo = false
		while (not jo) do
			id=math.random(0,#palya)
			jo, felszedve, uthet = jolepes(id,felszedve,uthet,palya)
		end
	end
	hova=id-1

	if uthet then
		jo = false
		while (not jo) do
			id=math.random(0,#palya)
			jo, felszedve, uthet = jolepes(id,felszedve,uthet,palya)
		end
		leutotte = id-1;
	end

	print(hova.." "..honnan.." "..leutotte)
end

function szamol()

end