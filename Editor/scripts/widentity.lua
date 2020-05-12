function ediInitWidEntity()
	editor.gui.properties.widEntity = {}
	local widEntity = editor.gui.properties.widEntity

	widEntity.object = CreateScreen(editor.gui.properties.object, "WidEntity", 0, 25, 248, 84)

	widEntity.scaleAttr = ediCreateFloatGroupAttribute(widEntity.object, "Scale", 4, 42, 0, nil, "", 3, "SetEntityScale")
	widEntity.occluderAttr = ediCreateBooleanAttribute(widEntity.object, "Occluder", 4, 62, false, "SetEntityOccluder")

	widEntity.modelLab = CreateLabel(widEntity.object, "ModelLab", 4, 26, "Model")

	SetGuiObjectColor(widEntity.modelLab, 1.0, 1.0, 1.0, 0.6)

	widEntity.modelTxf = CreateTextField(widEntity.object, "ModelTxf", 80, 22, 164, "")

	widEntity.minimize = CreateButton(widEntity.object, "Entity", 1, 1, 246, 14, "Entity")

	SetScreenSize(widEntity.object, 248, 16)
end

function ediUpdateWidEntitySelection()
	local widEntity = editor.gui.properties.widEntity
	local sel = ediGetSelection()

	local mdl = GetEntityModel(sel)
	if mdl ~= nil then
		SetTextFieldText(widEntity.modelTxf, GetModelName(mdl))
	else
		SetTextFieldText(widEntity.modelTxf, "")
	end

	local scale = GetEntityScale(sel)
	ediSetFloatGroupAttributeValues(widEntity.scaleAttr, {scale.x, scale.y, scale.z})
	ediSetBooleanAttributeValue(widEntity.occluderAttr, GetEntityOccluder(sel))

	SetTextFieldCursorPosition(widEntity.modelTxf, 0)
end

function ediUpdateWidEntity()
	local widEntity = editor.gui.properties.widEntity
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widEntity.minimize) == CLICKED then
		if GetGuiObjectSize(widEntity.object).y > 16 then
			SetScreenSize(widEntity.object, 248, 16)
		else
			SetScreenSize(widEntity.object, 248, 84)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widEntity.modelTxf) == LOSE_FOCUS then
		if GetEntityModel(sel) ~= nil then
			if string.len(GetTextFieldText(widEntity.modelTxf)) < 1 then
				ClearEntityModel(sel)
			else
				SetModelName(GetEntityModel(sel), GetTextFieldText(widEntity.modelTxf))
			end
		else
			local models = GetSceneModels(editor.scene.object)
			local mdl = BeginList(models)
			local found = false
			while mdl ~= nil do
				if GetModelName(mdl) == GetTextFieldText(widEntity.modelTxf) then
					SetEntityModel(sel, mdl)
					foudn = true
				end
				mdl = GetListNext(models)
			end
			if found == false then SetTextFieldText(widEntity.modelTxf, "") end
		end
	end

	ediUpdateAttribute(widEntity.scaleAttr, sel)
	ediUpdateAttribute(widEntity.occluderAttr, sel)
end

