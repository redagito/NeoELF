
function ediInitWidOptions()
	editor.gui.properties.widOptions = {}
	local widOptions = editor.gui.properties.widOptions

	widOptions.object = CreateScreen(editor.gui.properties.object, "WidOptions", 0, 25, 248, 128)

	widOptions.resolutionLab = CreateLabel(widOptions.object, "ResolutionLab", 4, 26, "Resolution")
	widOptions.multisamplesLab = CreateLabel(widOptions.object, "MultisamplesLab", 4, 46, "Multisamp.")
	widOptions.textureAniLab = CreateLabel(widOptions.object, "TextureAnisotropyLab", 4, 66, "TextureAni.")
	widOptions.texCompLab = CreateLabel(widOptions.object, "TextureAnisotropyLab", 4, 86, "TexComp.")

	SetGuiObjectColor(widOptions.resolutionLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widOptions.multisamplesLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widOptions.textureAniLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widOptions.texCompLab, 1.0, 1.0, 1.0, 0.6)

	widOptions.resolutionWidth = CreateTextField(widOptions.object, "ResolutionWidth", 80, 22, 80, GetWindowWidth())
	widOptions.resolutionHeight = CreateTextField(widOptions.object, "ResolutionHeight", 164, 22, 80, GetWindowHeight())
	widOptions.multisamplesTxf = CreateTextField(widOptions.object, "MultisamplesTxf", 80, 42, 164, GetMultisamples())
	widOptions.textureAniTxf = CreateTextField(widOptions.object, "TextureAnisotropyTxf", 80, 62, 164, GetTextureAnisotropy())
	widOptions.texCompCbx = CreateCheckBox(widOptions.object, "TextureCompressCbx", 80, 85, GetTextureCompress())

	widOptions.save = CreateButton(widOptions.object, "Save", 10, 106, 228, 18, "Save")

	widOptions.minimize = CreateButton(widOptions.object, "Options", 1, 1, 246, 14, "Options")
end

function ediUpdateWidOptions()
	local widOptions = editor.gui.properties.widOptions

	if GetGuiObjectEvent(widOptions.minimize) == CLICKED then
		if GetGuiObjectSize(widOptions.object).y > 16 then
			SetScreenSize(widOptions.object, 248, 16)
		else
			SetScreenSize(widOptions.object, 248, 128)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widOptions.resolutionWidth) == LOSE_FOCUS then
		ediCheckTextFieldInt(widOptions.resolutionWidth, 640, nil)
	end

	if GetGuiObjectEvent(widOptions.resolutionHeight) == LOSE_FOCUS then
		ediCheckTextFieldInt(widOptions.resolutionHeight, 480, nil)
	end

	if GetGuiObjectEvent(widOptions.multisamplesTxf) == LOSE_FOCUS then
		ediCheckTextFieldInt(widOptions.multisamplesTxf, 0, 16)
	end

	if GetGuiObjectEvent(widOptions.textureAniTxf) == LOSE_FOCUS then
		ediCheckTextFieldInt(widOptions.textureAniTxf, 1, 16)
	end

	if GetGuiObjectEvent(widOptions.save) == CLICKED then
		local f = io.open("config.txt", "w")
		if f ~= nil then
			f:write("windowSize " .. GetTextFieldText(widOptions.resolutionWidth) .. " " .. GetTextFieldText(widOptions.resolutionHeight) .. "\n")
			f:write("multisamples " .. GetTextFieldText(widOptions.multisamplesTxf) .. "\n")
			f:write("textureAnisotropy " .. GetTextFieldText(widOptions.textureAniTxf) .. "\n")
			f:write("textureCompress ")
			if GetCheckBoxState(widOptions.texCompCbx) then f:write("TRUE\n") else f:write("FALSE\n") end
			f:close()
			SetButtonText(widOptions.save, "Save [Restart Required]")
		end
	end
end

