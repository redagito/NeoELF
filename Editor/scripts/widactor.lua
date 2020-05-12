
function ediInitWidActor()
	editor.gui.properties.widActor = {}
	local widActor = editor.gui.properties.widActor

	widActor.object = CreateScreen(editor.gui.properties.object, "WidActor", 0, 25, 248, 105)

	widActor.nameLab = CreateLabel(widActor.object, "nameLab", 4, 26, "Name")

	widActor.positionAttr = ediCreateFloatGroupAttribute(widActor.object, "Position", 4, 42, nil, nil, "", 3, "SetActorPosition")
	widActor.rotationAttr = ediCreateFloatGroupAttribute(widActor.object, "Rotation", 4, 62, nil, nil, "", 3, "SetActorRotation")

	widActor.scriptLab = CreateLabel(widActor.object, "scriptLab", 4, 86, "Script")

	SetGuiObjectColor(widActor.nameLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widActor.scriptLab, 1.0, 1.0, 1.0, 0.6)

	widActor.nameTxf = CreateTextField(widActor.object, "NameTxf", 80, 22, 164, "")
	widActor.scriptTxf = CreateTextField(widActor.object, "ScriptTxf", 80, 82, 144, "")
	widActor.openScript = CreateButton(widActor.object, "Script", 226, 82, 18, 18, "..")

	widActor.minimize = CreateButton(widActor.object, "Actor", 1, 1, 246, 14, "Actor")	
end

function ediUpdateWidActorSelection()
	local widActor = editor.gui.properties.widActor

	local sel = ediGetSelection()
	local pos = GetActorPosition(sel)
	local rot = GetActorRotation(sel)
	local script = GetActorScript(sel)

	SetTextFieldText(widActor.nameTxf, GetActorName(sel))

	ediSetFloatGroupAttributeValues(widActor.positionAttr, {pos.x, pos.y, pos.z})
	ediSetFloatGroupAttributeValues(widActor.rotationAttr, {rot.x, rot.y, rot.z})

	if script ~= nil then SetTextFieldText(widActor.scriptTxf, GetScriptName(script))
	else SetTextFieldText(widActor.scriptTxf, "") end

	SetTextFieldCursorPosition(widActor.nameTxf, 0)
	SetTextFieldCursorPosition(widActor.scriptTxf, 0)
end

function ediWidActorOpenScript(path)
	if path == "" then return true end

	scr = CreateScriptFromFile(GetFileFromPath(path), path)

	if scr ~= nil then
		SetActorScript(ediGetSelection(), scr)
		ediUpdateWidActorSelection()
		return true
	end

	return false
end

function ediUpdateWidActor()
	local widActor = editor.gui.properties.widActor
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widActor.minimize) == CLICKED then
		if GetGuiObjectSize(widActor.object).y > 16 then
			SetScreenSize(widActor.object, 248, 16)
		else
			SetScreenSize(widActor.object, 248, 105)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widActor.nameTxf) == LOSE_FOCUS then
		if string.len(GetTextFieldText(widActor.nameTxf)) < 1 then
			SetTextFieldText(widActor.nameTxf, GetActorName(ediGetSelection()))
		else
			SetActorName(ediGetSelection(), GetTextFieldText(widActor.nameTxf))
		end
	end

	ediUpdateAttribute(widActor.positionAttr, sel)
	ediUpdateAttribute(widActor.rotationAttr, sel)

	if GetGuiObjectEvent(widActor.scriptTxf) == LOSE_FOCUS then
		if GetActorScript(sel) ~= nil then
			if string.len(GetTextFieldText(widActor.scriptTxf)) < 1 then
				ClearActorScript(sel)
			else
				SetScriptName(GetActorScript(sel), GetTextFieldText(widActor.scriptTxf))
			end
		else
			local scripts = GetSceneScripts(editor.scene.object)
			local scr = BeginList(scripts)
			local found = false
			while scr ~= nil do
				if GetScriptName(scr) == GetTextFieldText(widActor.scriptTxf) then
					SetActorScript(sel, scr)
					found = true
					break
				end
				scr = GetListNext()
			end
			if found == false then SetTextFieldText(widActor.scriptTxf, "") end
		end
	elseif GetGuiObjectEvent(widActor.scriptTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local scr = CreateScriptFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
				ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if scr ~= nil then
				SetActorScript(sel, scr)
				SetTextFieldText(widActor.scriptTxf, GetScriptName(scr))
			end
		end
	end

	if GetGuiObjectEvent(widActor.openScript) == CLICKED then
		ediOpenFileDialog(nil, EDI_OPEN, ediWidActorOpenScript)
	end
end

