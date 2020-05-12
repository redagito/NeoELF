
function ediRound(num, idp)
	local mult = 10^(idp or 0)
	return math.floor(num * mult + 0.5) / mult
end

function ediCheckTextFieldFloat(txf, min, max)
	local val = GetTextFieldText(txf)

	if val == nil then val = 0 end
	val = tonumber(val)
	if val == nil then
		val = 0
	else
		if min ~= nil and val < min then val = min end
		if max ~= nil and val > max then val = max end
	end

	SetTextFieldText(txf, tostring(val))
	SetTextFieldCursorPosition(txf, 0)
end

function ediCheckTextFieldInt(txf, min, max)
	local val = GetTextFieldText(txf)

	if val == nil then val = 0 end
	val = tonumber(val)
	if val == nil then
		val = 0
	else
		val = ediRound(val)
		if min ~= nil then min = ediRound(min) end
		if max ~= nil then max = ediRound(max) end
		if min ~= nil and val < min then val = min end
		if max ~= nil and val > max then val = max end
	end

	SetTextFieldText(txf, tostring(val))
	SetTextFieldCursorPosition(txf, 0)
end

function ediGetParentDirectory(path)
	if string.len(path) < 2 then return path end

	local rpath = string.reverse(path)

	if string.sub(rpath, 1, 1) == "\\" then rpath = string.sub(rpath, 2) end
	if string.sub(rpath, 1, 1) == "/" then rpath = string.sub(rpath, 2) end

	local idx = string.find(rpath, "\\")
	if idx == nil then idx = string.find(rpath, "/") end
	if idx == nil then return string.reverse(rpath) end

	return string.reverse(rpath.sub(rpath, idx))
end

function ediMakeDirectoryPath(path)
	local postfix = nil

	if string.len(path) < 2 then return path end

	local rpath = string.reverse(path)

	if string.sub(rpath, 1, 1) == "\\" then return string.reverse(rpath) end
	if string.sub(rpath, 1, 1) == "/" then return string.reverse(rpath) end

	if GetPlatform() == "windows" then postfix = "\\" else postfix = "/" end

	return string.reverse(rpath) .. postfix
end

