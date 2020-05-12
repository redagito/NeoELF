
function ediInitWidFile()
	editor.gui.properties.widFile = {}
	local widFile = editor.gui.properties.widFile

	widFile.object = CreateScreen(editor.gui.properties.object, "WidFile", 0, 25, 248, 105)
	SetGuiObjectColor(widFile.object, 0.0, 0.0, 0.0, 0.0)

	widFile.open = CreateButton(widFile.object, "Open", 10, 22, 228, 18, "Open...")
	widFile.save = CreateButton(widFile.object, "Save", 10, 42, 228, 18, "Save")
	widFile.saveAs = CreateButton(widFile.object, "SaveAs", 10, 62, 228, 18, "Save As...")
	widFile.import = CreateButton(widFile.object, "Import", 10, 82, 228, 18, "Import...")

	widFile.minimize = CreateButton(widFile.object, "File", 1, 1, 246, 14, "File")
end

function ediWidFileOpen(path)
	if path == "" then return true end
	
	return ediOpenScene(path)
end

function ediWidFileSaveAs(path)
	if path == "" then return true end

	return ediSaveScene(path)
end

function ediWidFileImport(path)
	if path == "" then return true end

	return ediImportScene(path)
end

function ediUpdateWidFile()
	local widFile = editor.gui.properties.widFile

	if GetGuiObjectEvent(widFile.minimize) == CLICKED then
		if GetGuiObjectSize(widFile.object).y > 16 then
			SetScreenSize(widFile.object, 248, 16)	
		else
			SetScreenSize(widFile.object, 248, 105)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widFile.open) == CLICKED then
		ediOpenFileDialog(nil, EDI_OPEN, ediWidFileOpen)
	end

	if GetGuiObjectEvent(widFile.save) == CLICKED then
		if GetSceneFilePath(editor.scene.object) ~= nil then
			ediSaveScene(GetSceneFilePath(editor.scene.object))
		else
			ediSaveScene("scene.pak")
		end
	end

	if GetGuiObjectEvent(widFile.saveAs) == CLICKED then
		ediOpenFileDialog(nil, EDI_SAVE, ediWidFileSaveAs)
	end

	if GetGuiObjectEvent(widFile.import) == CLICKED then
		ediOpenFileDialog(nil, EDI_OPEN, ediWidFileImport)
	end
end

