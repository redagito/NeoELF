
EDI_ATTR_STRING = 1
EDI_ATTR_INT_GROUP = 2
EDI_ATTR_FLOAT_GROUP = 3
EDI_ATTR_BOOLEAN = 4

function ediInitAtt(att, t, name, x, obj)
	att.type = t
	att.name = name
	att.pos = {}
	att.pos.x = x
	att.pos.y = y
end

function ediCreateStringAttribute(parent, name, x, y, default, callback)
	local att = {}

	ediInitAtt(att, EDI_ATTR_STRING, x, y)

	att.label = CreateLabel(parent, name, x, y+4, name)
	SetGuiObjectColor(att.label, 1.0, 1.0, 1.0, 0.6)

	att.textField = CreateTextField(parent, name, x+80, y, 164, default)

	att.callback = callback

	return att
end

function ediSetStringAttributeValue(att, text)
	SetTextFieldText(att.textField, text)
end

function ediGetStringAttributeValue(att)
	return GetTextFieldText(att.textField)
end

function ediCreateIntGroupAttribute(parent, name, x, y, min, max, default, count, callback)
	local att = {}

	ediInitAtt(att, EDI_ATTR_INT_GROUP, x, y)

	att.min = min
	att.max = max
	att.count = count

	att.label = CreateLabel(parent, name, x, y+4, name)
	SetGuiObjectColor(att.label, 1.0, 1.0, 1.0, 0.6)

	offset = 76
	width = math.floor((164-3*(count-1))/count)
	step = width + 3
	att.textFields = {}

	for i=0, count-1 do
		if i == count-1 then width = 248-8-offset end
		att.textFields[i] = CreateTextField(parent, name, x+offset, y, width, default)
		offset = offset + step
	end

	att.callback = callback

	return att
end

function ediClearInitGroupAttribute(att)
	for i=0, att.count-1 do SetTextFieldText(att.textFields[i], "") end
end

function ediSetIntGroupAttributeValues(att, vals)
	for i=0, att.count-1 do
		SetTextFieldText(att.textFields[i], tostring(vals[i+1]))
		SetTextFieldCursorPosition(att.textFields[i], 0)
	end
end

function ediCreateFloatGroupAttribute(parent, name, x, y, min, max, default, count, callback)
	local att = {}

	ediInitAtt(att, EDI_ATTR_FLOAT_GROUP, x, y)

	att.min = min
	att.max = max
	att.count = count

	att.label = CreateLabel(parent, name, x, y+4, name)
	SetGuiObjectColor(att.label, 1.0, 1.0, 1.0, 0.6)

	offset = 76
	width = math.floor((164-3*(count-1))/count)
	step = width + 3
	att.textFields = {}

	for i=0, count-1 do
		if i == count-1 then width = 248-8-offset end
		att.textFields[i] = CreateTextField(parent, name, x+offset, y, width, default)
		offset = offset + step
	end

	att.callback = callback

	return att
end

function ediClearFloatGroupAttribute(att)
	for i=0, att.count-1 do SetTextFieldText(att.textFields[i], "") end
end

function ediSetFloatGroupAttributeValues(att, vals)
	for i=0, att.count-1 do
		SetTextFieldText(att.textFields[i], tostring(vals[i+1]))
		SetTextFieldCursorPosition(att.textFields[i], 0)
	end
end

function ediCreateBooleanAttribute(parent, name, x, y, default, callback)
	local att = {}

	ediInitAtt(att, EDI_ATTR_BOOLEAN, x, y)

	att.min = min
	att.max = max
	att.count = count

	att.label = CreateLabel(parent, name, x, y+4, name)
	SetGuiObjectColor(att.label, 1.0, 1.0, 1.0, 0.6)

	att.checkBox = CreateCheckBox(parent, name, x+76, y+2, default)

	att.callback = callback

	return att
end

function ediClearBooleanAttribute(att)
	SetCheckBoxState(att.checkBox, false)
end

function ediSetBooleanAttributeValue(att, val)
	SetCheckBoxState(att.checkBox, val)
end

function ediUpdateAttribute(att, obj)
	if att.type == EDI_ATTR_STRING then
		if GetGuiObjectEvent(att.textField) ~= NONE then
			if type(att.callback) == "string" then
				if GetGuiObjectEvent(att.textField) == LOSE_FOCUS then
					RunString(att.callback .. "(obj, " .. GetTextFieldText() .. ")")
				end
			elseif type(att.callback) == "function" then
				att.callback(att, obj)
			end
		end
	elseif att.type == EDI_ATTR_INT_GROUP then
		local found = false
		for i=0, att.count-1 do
			if GetGuiObjectEvent(att.textFields[i]) == LOSE_FOCUS then
				found = true
				break
			end
		end
		if found then
			for i=0, att.count-1 do ediCheckTextFieldInt(att.textFields[i], att.min, att.max) end
			if type(att.callback) == "string" then
				vals = ""
				for i=0, att.count-1 do
					vals = vals .. GetTextFieldText(att.textFields[i])
					if i < att.count-1 then vals = vals .. ", " end
				end
				gobj = obj
				RunString(att.callback .. "(gobj, " .. vals .. ")")
				gobj = nil
			elseif type(att.callback) == "function" then
				att.callback(att, obj)
			end
		end
	elseif att.type == EDI_ATTR_FLOAT_GROUP then
		local found = false
		for i=0, att.count-1 do
			if GetGuiObjectEvent(att.textFields[i]) == LOSE_FOCUS then
				found = true
				break
			end
		end
		if found then
			for i=0, att.count-1 do ediCheckTextFieldFloat(att.textFields[i], att.min, att.max) end
			if type(att.callback) == "string" then
				vals = ""
				for i=0, att.count-1 do
					vals = vals .. GetTextFieldText(att.textFields[i])
					if i < att.count-1 then vals = vals .. ", " end
				end
				gobj = obj
				RunString(att.callback .. "(gobj, " .. vals .. ")")
				gobj = nil
			elseif type(att.callback) == "function" then
				att.callback(att, obj)
			end
		end
	elseif att.type == EDI_ATTR_BOOLEAN then
		if GetGuiObjectEvent(att.checkBox) == STATE_CHANGED then
			if type(att.callback) == "string" then
				gobj = obj
				RunString(att.callback .. "(gobj, " .. tostring(GetCheckBoxState(att.checkBox)) .. ")")
				gobj = nil
			elseif type(att.callback) == "function" then
				att.callback(att, obj)
			end
		end
	end
end

