
function ediInitWidFiles()
	editor.gui.toolbox.widFiles = {}
	local widFiles = editor.gui.toolbox.widFiles

	widFiles.prevMouseWheel = 0
	widFiles.prevDragging = false

	widFiles.object = CreateScreen(editor.gui.toolbox.object, "WidFiles", 8, GetWindowHeight()/2, 248, GetWindowHeight()/2)

	widFiles.minimize = CreateButton(widFiles.object, "Files", 1, 1, 246, 14, "Files")

	widFiles.filePath = CreateTextField(widFiles.object, "FilePath", 4, 22, 239, "")
	widFiles.fileList = CreateTextList(widFiles.object, "FileList", 4, 42, (GetWindowHeight()/2-44)/15, 229)
	widFiles.scrollBar = CreateSlider(widFiles.object, "ScrollBar", 237, 46, 0, GetWindowHeight()/2-53, 1.0)

	SetTextFieldCursorPosition(widFiles.filePath, 0)
	SetTextListItemDrag(widFiles.fileList, true)

	ediSetWidFilesDirectory(GetCurrentDirectory())
end

function ediGetWidFilesDirectory()
	return editor.gui.toolbox.widFiles.path
end

function ediSetWidFilesDirectory(path)
	local widFiles = editor.gui.toolbox.widFiles

	local dir = ReadDirectory(path)

	if dir ~= nil then
		RemoveTextListItems(widFiles.fileList)
		for i=0, GetDirectoryItemCount(dir)-1 do
			local item = GetDirectoryItem(dir, i)
			if GetDirectoryItemType(item) == DIR and i > 1 then
				AddTextListItem(widFiles.fileList, " " .. GetDirectoryItemName(item) .. "/")
			else
				AddTextListItem(widFiles.fileList, " " .. GetDirectoryItemName(item))
			end
		end
		widFiles.path = ediMakeDirectoryPath(path)
		widFiles.dir = dir
		SetTextFieldText(widFiles.filePath, widFiles.path)
		SetSliderValue(widFiles.scrollBar, 1.0)
	end
end

function ediUpdateWidFiles()
	local widFiles = editor.gui.toolbox.widFiles

	if GetGuiObjectEvent(widFiles.filePath) == LOSE_FOCUS then
		if GetKeyState(KEY_ENTER) == PRESSED then
			path = GetTextFieldText(widFiles.filePath)
			ediSetWidFilesDirectory(path)
		end
	end

	if GetGuiObjectEvent(widFiles.minimize) == CLICKED then
		if GetGuiObjectSize(widFiles.object).y > 16 then
			SetScreenSize(widFiles.object, 248, 16)
		else
			SetScreenSize(widFiles.object, 248, GetWindowHeight()/2)
		end
		ediPackToolboxScreens(editor.gui.toolbox, 0)
	end

	if GetGuiObjectEvent(widFiles.fileList) == SELECTION_CHANGED then
		if GetTextListSelectionIndex(widFiles.fileList) ~= -1 then
			local item = GetDirectoryItem(widFiles.dir, GetTextListSelectionIndex(widFiles.fileList))
			if GetDirectoryItemType(item) == DIR then
				if GetDirectoryItemName(item) == "." then
					ediSetWidFilesDirectory(widFiles.path)
				elseif GetDirectoryItemName(item) == ".." then
					ediSetWidFilesDirectory(ediGetParentDirectory(widFiles.path))
				else
					ediSetWidFilesDirectory(ediMakeDirectoryPath(widFiles.path .. GetDirectoryItemName(item)))
				end
			end
		end
	end

	local diff = GetTextListItemCount(widFiles.fileList)- GetTextListRowCount(widFiles.fileList)
	if diff > 0 then
		if GetGuiObjectEvent(widFiles.scrollBar) == VALUE_CHANGED then
			SetTextListOffset(widFiles.fileList, diff*(1.0-GetSliderValue(widFiles.scrollBar)))
		end

		if GetGuiTrace(editor.gui.object) ~= nil and
			GetGuiObjectName(GetGuiTrace(editor.gui.object)) ==
			GetGuiObjectName(widFiles.fileList) then

			local wheelDiff = -(GetMouseWheel() - widFiles.prevMouseWheel)
			if wheelDiff ~= 0 then
				offset = GetTextListOffset(widFiles.fileList)+wheelDiff
				if offset < 0 then offset = 0 end
				if offset > diff then offset = diff end

				SetTextListOffset(widFiles.fileList, offset)
				SetSliderValue(widFiles.scrollBar, 1.0-(offset/diff))
			end
		end
	else
		SetSliderValue(widFiles.scrollBar, 1.0)
	end

	if widFiles.prevDragging == true and GetGuiDragging(editor.gui.object) == false and
		GetGuiObjectName(GetGuiDragObject(editor.gui.object)) == GetGuiObjectName(widFiles.fileList) and
		GetGuiTrace(editor.gui.object) == nil then
		local path = string.sub(GetGuiDragContent(editor.gui.object), 2)
		if path ~= "." and path ~= ".." then
			ediImportScene(widFiles.path .. path)
		end
	end

	widFiles.prevMouseWheel = GetMouseWheel()
	widFiles.prevDragging = GetGuiDragging(editor.gui.object)
end

