
function ediInitGui()
	editor.gui = {}
	local gui = editor.gui

	gui.object = CreateGui()
	SetGui(gui.object)

	gui.version = CreateLabel(gui.object, "Version", 260, GetWindowHeight()-12, "EditorELF 0.9 Beta 4, FPS: 0")
	SetGuiObjectColor(gui.version, 1.0, 1.0, 1.0, 0.4)

	ediInitToolbox()
	ediInitProperties()
	ediInitFileDialog()
end

function ediUpdateGui()
	local gui = editor.gui

	SetLabelText(gui.version, "EditorELF 0.9 Beta 4, FPS: " .. GetFps())

	ediUpdateToolbox()
	ediUpdateProperties()
	ediUpdateFileDialog()
end

function ediPackScreensVer(screens, preOffset)
	local offset = preOffset
	for k, v in pairs(screens) do
		if type(v) == "table" and v.object ~= nil and GetObjectType(v.object) == SCREEN and GetGuiObjectVisible(v.object) then
			size = GetGuiObjectSize(v.object)
			pos = GetGuiObjectPosition(v.object)
			SetGuiObjectPosition(v.object, pos.x, offset)
			offset = offset + size.y
		end
	end
end

