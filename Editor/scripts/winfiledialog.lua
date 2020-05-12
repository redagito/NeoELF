
EDI_OPEN = 0
EDI_SAVE = 1

function ediInitFileDialog()
	editor.gui.fileDialog = {}
	local fileDialog = editor.gui.fileDialog

	fileDialog.prevMouseWheel = 0
	fileDialog.prevDragging = false
	fileDialog.defaultPath = GetCurrentDirectory()

	fileDialog.object = CreateScreen(editor.gui.object, "WinFileDialog", 0, 0, GetWindowWidth(), GetWindowHeight())

	fileDialog.filePath = CreateTextField(fileDialog.object, "FilePath", 4, 4, GetWindowWidth()-118, "")
	fileDialog.action = CreateButton(fileDialog.object, "Action", GetWindowWidth()-112, 4, 108, 18, "Open")

	fileDialog.fileList = CreateTextList(fileDialog.object, "FileList", 4, 24, (GetWindowHeight()-54)/15, GetWindowWidth()-18)
	fileDialog.scrollBar = CreateSlider(fileDialog.object, "ScroolBar", GetWindowWidth()-10, 28, 0, GetWindowHeight()-58, 1.0)
	fileDialog.cancel = CreateButton(fileDialog.object, "Cancel", 4, GetWindowHeight()-22, GetWindowWidth()-8, 18, "Cancel")

	SetGuiObjectVisible(fileDialog.object, false)	
end

function ediSetFileDialogDirectory(path)
	local fileDialog = editor.gui.fileDialog

	local dir = ReadDirectory(path)

	if dir ~= nil then
		RemoveTextListItems(fileDialog.fileList)
		for i=0, GetDirectoryItemCount(dir)-1 do
			local item = GetDirectoryItem(dir, i)
			if GetDirectoryItemType(item) == DIR and i > 1 then
				AddTextListItem(fileDialog.fileList, " " .. GetDirectoryItemName(item) .. "/")
			else
				AddTextListItem(fileDialog.fileList, " " .. GetDirectoryItemName(item))
			end
		end
		fileDialog.path = ediMakeDirectoryPath(path)
		fileDialog.dir = dir
		SetTextFieldText(fileDialog.filePath, fileDialog.path)
		SetSliderValue(fileDialog.scrollBar, 1.0)

		return true
	end

	return false
end

function ediOpenFileDialog(path, mode, callback)
	local fileDialog = editor.gui.fileDialog

	if path == nil then path = fileDialog.defaultPath end

	if ediSetFileDialogDirectory(path) ~= true then
		ediSetFileDialogDirectory(GetCurrentDirectory())
	end

	fileDialog.callback = callback

	if mode == EDI_OPEN then
		SetButtonText(fileDialog.action, "Open")
	elseif mode == EDI_SAVE then
		SetButtonText(fileDialog.action, "Save")
	end

	SetGuiObjectVisible(fileDialog.object, true)	
	SetScreenToTop(fileDialog.object)
	ForceScreenFocus(fileDialog.object)
end

function ediUpdateFileDialog()
	local fileDialog = editor.gui.fileDialog

	if GetGuiObjectEvent(fileDialog.filePath) == LOSE_FOCUS then
		ediSetFileDialogDirectory(GetTextFieldText(fileDialog.filePath))
	end

	if GetGuiObjectEvent(fileDialog.action) == CLICKED then
		if fileDialog.callback ~= nil then
			if fileDialog.callback(GetTextFieldText(fileDialog.filePath)) == true then
				ReleaseScreenFocus(fileDialog.object)
				fileDialog.defaultPath = fileDialog.path
				SetGuiObjectVisible(fileDialog.object, false)
			end
		end
	end

	if GetGuiObjectEvent(fileDialog.fileList) == SELECTION_CHANGED and GetTextListSelectionIndex(fileDialog.fileList) ~= -1 then
		local item = GetDirectoryItem(fileDialog.dir, GetTextListSelectionIndex(fileDialog.fileList))
		if GetDirectoryItemType(item) == DIR then
			if GetDirectoryItemName(item) == "." then
				ediSetFileDialogDirectory(fileDialog.path)
			elseif GetDirectoryItemName(item) == ".." then
				ediSetFileDialogDirectory(ediGetParentDirectory(fileDialog.path))
			else
				ediSetFileDialogDirectory(ediMakeDirectoryPath(fileDialog.path .. GetDirectoryItemName(item)))
			end
		elseif GetDirectoryItemType(item) == FILE then
			SetTextFieldText(fileDialog.filePath, fileDialog.path .. GetDirectoryItemName(item))
		end
	end

	local diff = GetTextListItemCount(fileDialog.fileList)- GetTextListRowCount(fileDialog.fileList)
	if diff > 0 then
		if GetGuiObjectEvent(fileDialog.scrollBar) == VALUE_CHANGED then
			SetTextListOffset(fileDialog.fileList, diff*(1.0-GetSliderValue(fileDialog.scrollBar)))
		end

		if GetGuiTrace(editor.gui.object) ~= nil and
			GetGuiObjectName(GetGuiTrace(editor.gui.object)) ==
			GetGuiObjectName(fileDialog.fileList) then

			local wheelDiff = -(GetMouseWheel() - fileDialog.prevMouseWheel)
			if wheelDiff ~= 0 then
				offset = GetTextListOffset(fileDialog.fileList)+wheelDiff
				if offset < 0 then offset = 0 end
				if offset > diff then offset = diff end

				SetTextListOffset(fileDialog.fileList, offset)
				SetSliderValue(fileDialog.scrollBar, 1.0-(offset/diff))
			end
		end
	else
		SetSliderValue(fileDialog.scrollBar, 1.0)
	end

	if GetGuiObjectEvent(fileDialog.cancel) == CLICKED then
		if fileDialog.callback ~= nil then
			if fileDialog.callback("") == true then
				ReleaseScreenFocus(fileDialog.object)
				SetGuiObjectVisible(fileDialog.object, false)
			end
		else
			ReleaseScreenFocus(fileDialog.object)
			SetGuiObjectVisible(fileDialog.object, false)
		end
	end

	fileDialog.prevMouseWheel = GetMouseWheel()
	fileDialog.prevDragging = GetGuiDragging(editor.gui.object)
end

