
function ediInitWidMaterials()
	editor.gui.properties.widMaterials = {}
	local widMaterials = editor.gui.properties.widMaterials

	widMaterials.prevMouseWheel = 0

	widMaterials.object = CreateScreen(editor.gui.properties.object, "WidMaterials", 0, 25, 248, 411)

	widMaterials.addMaterialBut = CreateButton(widMaterials.object, "AddMaterialBut", 10, 22, 228, 18, "Add Material")
	widMaterials.removeMaterialBut = CreateButton(widMaterials.object, "RemoveMaterialBut", 10, 42, 228, 18, "Remove Material")

	widMaterials.materialsTxl = CreateTextList(widMaterials.object, "MaterialsTxl", 10, 64, 4, 218)
	widMaterials.materialsScrollBar = CreateSlider(widMaterials.object, "ScrollBar", 232, 68, 0, 52, 1.0)

	widMaterials.nameLab = CreateLabel(widMaterials.object, "NameLab", 4, 132, "Name")

	widMaterials.diffuseAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Diffuse", 4, 148, 0, nil, "", 4, "SetMaterialDiffuseColor")
	widMaterials.specularAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Specular", 4, 168, 0, nil, "", 4, "SetMaterialSpecularColor")
	widMaterials.ambientAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Ambient", 4, 188, 0, nil, "", 4, "SetMaterialAmbientColor")
	widMaterials.specPowerAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Spec pow", 4, 208, 0, nil, "", 1, "SetMaterialSpecularPower")
	widMaterials.lightingAttr = ediCreateBooleanAttribute(widMaterials.object, "Lighting", 4, 228, false, "SetMaterialLighting")
	widMaterials.alphaTestAttr = ediCreateBooleanAttribute(widMaterials.object, "Alpha test", 4, 248, false, "SetMaterialAlphaTest")
	widMaterials.alphaThrsAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Alpha thrs", 4, 268, 0, nil, "", 1, "SetMaterialAlphaThreshold")
	widMaterials.prlxScaleAttr = ediCreateFloatGroupAttribute(widMaterials.object, "Prlx scale", 4, 348, 0, nil, "", 1, "SetMaterialParallaxScale")

	widMaterials.diffuseMapLab = CreateLabel(widMaterials.object, "DiffuseMapLab", 4, 292, "Diff Map")
	widMaterials.normalMapLab = CreateLabel(widMaterials.object, "NormalMapLab", 4, 312, "Norm Map")
	widMaterials.heightMapLab = CreateLabel(widMaterials.object, "HeightMapLab", 4, 332, "Heig Map")
	widMaterials.specularMapLab = CreateLabel(widMaterials.object, "SpecularMapLab", 4, 372, "Spec Map")
	widMaterials.lightMapLab = CreateLabel(widMaterials.object, "LightMapLab", 4, 392, "LightMap")

	SetGuiObjectColor(widMaterials.nameLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widMaterials.diffuseMapLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widMaterials.normalMapLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widMaterials.heightMapLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widMaterials.specularMapLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widMaterials.lightMapLab, 1.0, 1.0, 1.0, 0.6)

	widMaterials.nameTxf = CreateTextField(widMaterials.object, "NameTxf", 80, 128, 164, "")

	widMaterials.diffuseMapTxf = CreateTextField(widMaterials.object, "DiffuseMapTxf", 80, 288, 144, "")
	widMaterials.normalMapTxf = CreateTextField(widMaterials.object, "NormalMapTxf", 80, 308, 144, "")
	widMaterials.heightMapTxf = CreateTextField(widMaterials.object, "HeightMapTxf", 80, 328, 144, "")
	widMaterials.specularMapTxf = CreateTextField(widMaterials.object, "SpecularMapTxf", 80, 368, 144, "")
	widMaterials.lightMapTxf = CreateTextField(widMaterials.object, "LightMapTxf", 80, 388, 144, "")

	widMaterials.openDiffuseMap = CreateButton(widMaterials.object, "OpenDiffuseMap", 226, 288, 18, 18, "..")
	widMaterials.openNormalMap = CreateButton(widMaterials.object, "OpenNormalMap", 226, 308, 18, 18, "..")
	widMaterials.openHeightMap = CreateButton(widMaterials.object, "OpenHeightMap", 226, 328, 18, 18, "..")
	widMaterials.openSpecularMap = CreateButton(widMaterials.object, "OpenSpecularMap", 226, 368, 18, 18, "..")
	widMaterials.openLightMap = CreateButton(widMaterials.object, "OpenLightMap", 226, 388, 18, 18, "..")

	widMaterials.minimize = CreateButton(widMaterials.object, "Materials", 1, 1, 246, 14, "Materials")

	SetScreenSize(widMaterials.object, 248, 16)
end

function ediClearWidMaterialsSpecs()
	local widMaterials = editor.gui.properties.widMaterials

	SetTextFieldText(widMaterials.nameTxf, "")

	ediClearFloatGroupAttribute(widMaterials.diffuseAttr)
	ediClearFloatGroupAttribute(widMaterials.specularAttr)
	ediClearFloatGroupAttribute(widMaterials.ambientAttr)
	ediClearFloatGroupAttribute(widMaterials.specPowerAttr)
	ediClearBooleanAttribute(widMaterials.lightingAttr)
	ediClearBooleanAttribute(widMaterials.alphaTestAttr)
	ediClearFloatGroupAttribute(widMaterials.alphaThrsAttr)
	ediClearFloatGroupAttribute(widMaterials.prlxScaleAttr)

	SetTextFieldText(widMaterials.diffuseMapTxf, "")
	SetTextFieldText(widMaterials.normalMapTxf, "")
	SetTextFieldText(widMaterials.heightMapTxf, "")
	SetTextFieldText(widMaterials.specularMapTxf, "")
	SetTextFieldText(widMaterials.lightMapTxf, "")

	widMaterials.material = nil
end

function ediSetWidMaterialsMaterial(idx)
	local widMaterials = editor.gui.properties.widMaterials
	local sel = ediGetSelection()

	widMaterials.material = idx
	if idx > GetEntityMaterialCount(sel)-1 then
		widMaterials.material = nil
		return
	end

	local mat = GetEntityMaterial(sel, idx)

	local diffuse = GetMaterialDiffuseColor(mat)
	local specular = GetMaterialSpecularColor(mat)
	local ambient = GetMaterialAmbientColor(mat)

	SetTextFieldText(widMaterials.nameTxf, GetMaterialName(mat))

	ediSetFloatGroupAttributeValues(widMaterials.diffuseAttr, {diffuse.r, diffuse.g, diffuse.b, diffuse.a})
	ediSetFloatGroupAttributeValues(widMaterials.specularAttr, {specular.r, specular.g, specular.b, specular.a})
	ediSetFloatGroupAttributeValues(widMaterials.ambientAttr, {ambient.r, ambient.g, ambient.b, ambient.a})
	ediSetFloatGroupAttributeValues(widMaterials.specPowerAttr, {GetMaterialSpecularPower(mat)})
	ediSetBooleanAttributeValue(widMaterials.lightingAttr, GetMaterialLighting(mat))
	ediSetBooleanAttributeValue(widMaterials.alphaTestAttr, GetMaterialAlphaTest(mat))
	ediSetFloatGroupAttributeValues(widMaterials.alphaThrsAttr, {GetMaterialAlphaThreshold(mat)})
	ediSetFloatGroupAttributeValues(widMaterials.prlxScaleAttr, {GetMaterialParallaxScale(mat)})

	local map = GetMaterialDiffuseMap(mat)
	if map ~= nil then SetTextFieldText(widMaterials.diffuseMapTxf, GetTextureName(map))
	else SetTextFieldText(widMaterials.diffuseMapTxf, "") end
	map = GetMaterialNormalMap(mat)
	if map ~= nil then SetTextFieldText(widMaterials.normalMapTxf, GetTextureName(map))
	else SetTextFieldText(widMaterials.normalMapTxf, "") end
	map = GetMaterialHeightMap(mat)
	if map ~= nil then SetTextFieldText(widMaterials.heightMapTxf, GetTextureName(map))
	else SetTextFieldText(widMaterials.heightMapTxf, "") end
	map = GetMaterialSpecularMap(mat)
	if map ~= nil then SetTextFieldText(widMaterials.specularMapTxf, GetTextureName(map))
	else SetTextFieldText(widMaterials.specularMapTxf, "") end
	map = GetMaterialLightMap(mat)
	if map ~= nil then SetTextFieldText(widMaterials.lightMapTxf, GetTextureName(map))
	else SetTextFieldText(widMaterials.lightMapTxf, "") end

	SetTextFieldCursorPosition(widMaterials.nameTxf, 0)
	SetTextFieldCursorPosition(widMaterials.diffuseMapTxf, 0)
	SetTextFieldCursorPosition(widMaterials.normalMapTxf, 0)
	SetTextFieldCursorPosition(widMaterials.heightMapTxf, 0)
	SetTextFieldCursorPosition(widMaterials.specularMapTxf, 0)
	SetTextFieldCursorPosition(widMaterials.lightMapTxf, 0)

end

function ediUpdateWidMaterialsSelection()
	local widMaterials = editor.gui.properties.widMaterials
	local sel = ediGetSelection()

	RemoveTextListItems(widMaterials.materialsTxl)
	SetSliderValue(widMaterials.materialsScrollBar, 1.0)

	if GetEntityMaterialCount(sel) > 0 then
		for i=0, GetEntityMaterialCount(sel)-1 do
			AddTextListItem(widMaterials.materialsTxl, GetMaterialName(GetEntityMaterial(sel, i)))
		end
		SetTextListSelection(widMaterials.materialsTxl, 0)
		ediSetWidMaterialsMaterial(0)
	else
		ediClearWidMaterialsSpecs()
	end
end

function ediUpdateWidMaterials()
	local widMaterials = editor.gui.properties.widMaterials

	if GetGuiObjectEvent(widMaterials.minimize) == CLICKED then
		if GetGuiObjectSize(widMaterials.object).y > 16 then
			SetScreenSize(widMaterials.object, 248, 16)
		else
			SetScreenSize(widMaterials.object, 248, 411)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	local sel = ediGetSelection()

	if GetGuiObjectEvent(widMaterials.addMaterialBut) == CLICKED then
		local nmat = CreateMaterial("Material")
		AddEntityMaterial(sel, nmat)
		AddTextListItem(widMaterials.materialsTxl, "Material")
		SetTextListSelection(widMaterials.materialsTxl, GetEntityMaterialCount(sel)-1)
		ediSetWidMaterialsMaterial(GetEntityMaterialCount(sel)-1)
	end

	local diff = GetTextListItemCount(widMaterials.materialsTxl)- GetTextListRowCount(widMaterials.materialsTxl)
	if diff > 0 then
		if GetGuiObjectEvent(widMaterials.materialsScrollBar) == VALUE_CHANGED then
			SetTextListOffset(widMaterials.materialsTxl, diff*(1.0-GetSliderValue(widMaterials.materialsScrollBar)))
		end

		if GetGuiTrace(editor.gui.object) ~= nil and
			GetGuiObjectName(GetGuiTrace(editor.gui.object)) ==
			GetGuiObjectName(widMaterials.materialsTxl) then

			local wheelDiff = -(GetMouseWheel() - widMaterials.prevMouseWheel)
			if wheelDiff ~= 0 then
				offset = GetTextListOffset(widMaterials.materialsTxl)+wheelDiff
				if offset < 0 then offset = 0 end
				if offset > diff then offset = diff end

				SetTextListOffset(widMaterials.materialsTxl, offset)
				SetSliderValue(widMaterials.materialsScrollBar, 1.0-(offset/diff))
			end
		end
	else
		SetSliderValue(widMaterials.materialsScrollBar, 1.0)
	end

	widMaterials.prevMouseWheel = GetMouseWheel()

	if widMaterials.material == nil then return end

	if GetGuiObjectEvent(widMaterials.removeMaterialBut) == CLICKED then
		RemoveEntityMaterial(sel, widMaterials.material)
		RemoveTextListItem(widMaterials.materialsTxl, widMaterials.material)
		if GetEntityMaterialCount(sel) == 0 then
			widMaterials.material = nil
			ediClearWidMaterialsSpecs()
			return
		end

		if widMaterials.material <= GetEntityMaterialCount(sel)-1 then
			ediSetWidMaterialsMaterial(widMaterials.material)
			SetTextListSelection(widMaterials.materialsTxl, widMaterials.material)
		else
			ediSetWidMaterialsMaterial(GetEntityMaterialCount(sel)-1)
			SetTextListSelection(widMaterials.materialsTxl, GetEntityMaterialCount(sel)-1)
		end

		diff = GetTextListItemCount(widMaterials.materialsTxl)- GetTextListRowCount(widMaterials.materialsTxl)
		SetTextListOffset(widMaterials.materialsTxl, diff*(1.0-GetSliderValue(widMaterials.materialsScrollBar)))
	end

	local mat = GetEntityMaterial(sel, widMaterials.material)

	if GetGuiObjectEvent(widMaterials.materialsTxl) == SELECTION_CHANGED then
		if GetTextListSelectionIndex(widMaterials.materialsTxl) == -1 then
			SetTextListSelection(widMaterials.materialsTxl, widMaterials.material)
		else
			ediSetWidMaterialsMaterial(GetTextListSelectionIndex(widMaterials.materialsTxl))
		end
	end

	if GetGuiObjectEvent(widMaterials.nameTxf) == LOSE_FOCUS then
		if string.len(GetTextFieldText(widMaterials.nameTxf)) < 1 then
			SetTextFieldText(widMaterials.nameTxf, GetMaterialName(mat))
		else
			SetMaterialName(mat, GetTextFieldText(widMaterials.nameTxf))
			SetTextListItem(widMaterials.materialsTxl, widMaterials.material, GetMaterialName(mat))
		end
	end

	ediUpdateAttribute(widMaterials.diffuseAttr, mat)
	ediUpdateAttribute(widMaterials.specularAttr, mat)
	ediUpdateAttribute(widMaterials.ambientAttr, mat)
	ediUpdateAttribute(widMaterials.specPowerAttr, mat)
	ediUpdateAttribute(widMaterials.lightingAttr, mat)
	ediUpdateAttribute(widMaterials.alphaTestAttr, mat)
	ediUpdateAttribute(widMaterials.alphaThrsAttr, mat)
	ediUpdateAttribute(widMaterials.prlxScaleAttr, mat)

	if GetGuiObjectEvent(widMaterials.diffuseMapTxf) == LOSE_FOCUS then
		local map = GetMaterialDiffuseMap(mat)
		if map ~= nil then
			if string.len(GetTextFieldText(widMaterials.diffuseMapTxf)) < 1 then
				ClearMaterialDiffuseMap(mat)
			else
				SetTextureName(map, GetTextFieldText(widMaterials.diffuseMapTxf))
			end
		else
			local textures = GetSceneTextures(editor.scene.object)
			local tex = BeginList(textures)
			local found = false
			while tex ~= nil do
				if GetTextureName(tex) == GetTextFieldText(widMaterials.diffuseMapTxf) then
					SetMaterialDiffuseMap(mat, tex)
					found = true
					break
				end
				tex = GetListNext(textures)
			end
			if found == false then SetTextFieldText(widMaterials.diffuseMapTxf, "") end
		end
	elseif GetGuiObjectEvent(widMaterials.diffuseMapTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local tex = CreateTextureFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
					ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if tex ~= nil then
				SetMaterialDiffuseMap(mat, tex)
				SetTextFieldText(widMaterials.diffuseMapTxf, GetTextureName(tex))
			end
		end
	end

	if GetGuiObjectEvent(widMaterials.normalMapTxf) == LOSE_FOCUS then
		local map = GetMaterialNormalMap(mat)
		if map ~= nil then
			if string.len(GetTextFieldText(widMaterials.normalMapTxf)) < 1 then
				ClearMaterialNormalMap(mat)
			else
				SetTextureName(map, GetTextFieldText(widMaterials.normalMapTxf))
			end
		else
			local textures = GetSceneTextures(editor.scene.object)
			local tex = BeginList(textures)
			local found = false
			while tex ~= nil do
				if GetTextureName(tex) == GetTextFieldText(widMaterials.normalMapTxf) then
					SetMaterialNormalMap(mat, tex)
					GenerateEntityTangents(sel)
					found = true
					break
				end
				tex = GetListNext(textures)
			end
			if found == false then SetTextFieldText(widMaterials.normalMapTxf, "") end
		end
	elseif GetGuiObjectEvent(widMaterials.normalMapTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local tex = CreateTextureFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
					ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if tex ~= nil then
				SetMaterialNormalMap(mat, tex)
				GenerateEntityTangents(sel)
				SetTextFieldText(widMaterials.normalMapTxf, GetTextureName(tex))
			end
		end
	end

	if GetGuiObjectEvent(widMaterials.heightMapTxf) == LOSE_FOCUS then
		local map = GetMaterialHeightMap(mat)
		if map ~= nil then
			if string.len(GetTextFieldText(widMaterials.heightMapTxf)) < 1 then
				ClearMaterialHeightMap(mat)
			else
				SetTextureName(map, GetTextFieldText(widMaterials.heightMapTxf))
			end
		else
			local textures = GetSceneTextures(editor.scene.object)
			local tex = BeginList(textures)
			local found = false
			while tex ~= nil do
				if GetTextureName(tex) == GetTextFieldText(widMaterials.heightMapTxf) then
					SetMaterialHeightMap(mat, tex)
					GenerateEntityTangents(sel)
					found = true
					break
				end
				tex = GetListNext(textures)
			end
			if found == false then SetTextFieldText(widMaterials.heightMapTxf, "") end
		end
	elseif GetGuiObjectEvent(widMaterials.heightMapTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local tex = CreateTextureFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
					ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if tex ~= nil then
				SetMaterialHeightMap(mat, tex)
				GenerateEntityTangents(sel)
				SetTextFieldText(widMaterials.heightMapTxf, GetTextureName(tex))
			end
		end
	end

	if GetGuiObjectEvent(widMaterials.specularMapTxf) == LOSE_FOCUS then
		local map = GetMaterialSpecularMap(mat)
		if map ~= nil then
			if string.len(GetTextFieldText(widMaterials.specularMapTxf)) < 1 then
				ClearMaterialSpecularMap(mat)
			else
				SetTextureName(map, GetTextFieldText(widMaterials.specularMapTxf))
			end
		else
			local textures = GetSceneTextures(editor.scene.object)
			local tex = BeginList(textures)
			local found = false
			while tex ~= nil do
				if GetTextureName(tex) == GetTextFieldText(widMaterials.specularMapTxf) then
					SetMaterialSpecularMap(mat, tex)
					found = true
					break
				end
				tex = GetListNext(textures)
			end
			if found == false then SetTextFieldText(widMaterials.specularMapTxf, "") end
		end
	elseif GetGuiObjectEvent(widMaterials.specularMapTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local tex = CreateTextureFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
					ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if tex ~= nil then
				SetMaterialSpecularMap(mat, tex)
				SetTextFieldText(widMaterials.specularMapTxf, GetTextureName(tex))
			end
		end
	end

	if GetGuiObjectEvent(widMaterials.lightMapTxf) == LOSE_FOCUS then
		local map = GetMaterialLightMap(mat)
		if map ~= nil then
			if string.len(GetTextFieldText(widMaterials.lightMapTxf)) < 1 then
				ClearMaterialLightMap(mat)
			else
				SetTextureName(map, GetTextFieldText(widMaterials.lightMapTxf))
			end
		else
			local textures = GetSceneTextures(editor.scene.object)
			local tex = BeginList(textures)
			local found = false
			while tex ~= nil do
				if GetTextureName(tex) == GetTextFieldText(widMaterials.lightMapTxf) then
					SetMaterialLightMap(mat, tex)
					found = true
					break
				end
				tex = GetListNext(textures)
			end
			if found == false then SetTextFieldText(widMaterials.lightMapTxf, "") end
		end
	elseif GetGuiObjectEvent(widMaterials.lightMapTxf) == DROP then
		if GetGuiObjectName(editor.gui.toolbox.widFiles.fileList) == GetGuiObjectName(GetGuiDragObject(editor.gui.object)) then
			local tex = CreateTextureFromFile(string.sub(GetGuiDragContent(editor.gui.object), 2),
					ediGetWidFilesDirectory() .. string.sub(GetGuiDragContent(editor.gui.object), 2))
			if tex ~= nil then
				SetMaterialLightMap(mat, tex)
				SetTextFieldText(widMaterials.lightMapTxf, GetTextureName(tex))
			end
		end
	end
end

