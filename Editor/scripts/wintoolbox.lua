
function ediInitToolbox()
	editor.gui.toolbox = {}
	local toolbox = editor.gui.toolbox

	toolbox.object = CreateScreen(editor.gui.object, "WinToolbox", 0, 0, 256, GetWindowHeight())

	toolbox.minimize = CreateButton(toolbox.object, "Minimize", 1, 0, 6, GetWindowHeight(), "")

	ediInitWidFiles()
	ediInitWidTree()

	ediPackToolboxScreens()
end

function ediPackToolboxScreens()
	local toolbox = editor.gui.toolbox

	if GetGuiObjectSize(toolbox.widTree.object).y == 16 then
		SetScreenSize(toolbox.widFiles.object, 248, GetWindowHeight()-16)
		SetGuiObjectPosition(toolbox.widFiles.object, 8, 16)
		SetTextListSize(toolbox.widFiles.fileList, (GetWindowHeight()-60)/15, 229)
		SetTextListOffset(toolbox.widFiles.fileList, 0)
		SetSliderSize(toolbox.widFiles.scrollBar, 0, GetWindowHeight()-69)
		SetSliderValue(toolbox.widFiles.scrollBar, 1.0)
	elseif GetGuiObjectSize(toolbox.widFiles.object).y == 16 then
		SetScreenSize(toolbox.widTree.object, 248, GetWindowHeight()-16)
		SetGuiObjectPosition(toolbox.widFiles.object, 8, GetWindowHeight()-16)
		SetTextListOffset(toolbox.widFiles.fileList, 0)
		SetSliderValue(toolbox.widFiles.scrollBar, 1.0)
	else
		SetScreenSize(toolbox.widTree.object, 248, GetWindowHeight()/2)
		SetScreenSize(toolbox.widFiles.object, 248, GetWindowHeight()/2)
		SetGuiObjectPosition(toolbox.widFiles.object, 8, GetWindowHeight()/2)
		SetTextListSize(toolbox.widFiles.fileList, (GetWindowHeight()/2-44)/15, 229)
		SetTextListOffset(toolbox.widFiles.fileList, 0)
		SetSliderSize(toolbox.widFiles.scrollBar, 0, GetWindowHeight()/2-53)
		SetSliderValue(toolbox.widFiles.scrollBar, 1.0)
	end
end

function ediUpdateToolbox()
	local toolbox = editor.gui.toolbox

	if GetGuiObjectEvent(toolbox.minimize) == CLICKED or
		(GetGuiActiveTextField(editor.gui.object) == nil and GetKeyState(KEY_1) == PRESSED) then
		if GetGuiObjectSize(toolbox.object).x > 8 then
			SetScreenSize(toolbox.object, 8, GetWindowHeight())
			SetGuiObjectPosition(editor.gui.version, 12, GetWindowHeight()-12)
		else
			SetScreenSize(toolbox.object, 256, GetWindowHeight())
			SetGuiObjectPosition(editor.gui.version, 260, GetWindowHeight()-12)
		end
	end

	ediUpdateWidFiles()
	ediUpdateWidTree()
end

