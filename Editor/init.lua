
function ediInit()
	editor = {}

	SetTitle("EditorELF 0.9 Beta 4")

	dofile("scripts/gui.lua")
	dofile("scripts/scene.lua")
	dofile("scripts/camera.lua")

	dofile("scripts/attribute.lua")

	dofile("scripts/wintoolbox.lua")
	dofile("scripts/widfiles.lua")
	dofile("scripts/widtree.lua")

	dofile("scripts/winproperties.lua")
	dofile("scripts/widfile.lua")
	dofile("scripts/widoptions.lua")
	dofile("scripts/widcreate.lua")
	dofile("scripts/widactor.lua")
	dofile("scripts/widphysics.lua")
	dofile("scripts/widlight.lua")
	dofile("scripts/widshaft.lua")
	dofile("scripts/widcamera.lua")
	dofile("scripts/widentity.lua")
	dofile("scripts/widmaterials.lua")
	dofile("scripts/widparticles.lua")

	dofile("scripts/winfiledialog.lua")

	dofile("scripts/util.lua")

	ediInitGui()
	ediInitScene()
end

function ediRun()
	while Run() == true do
		if GetKeyState(KEY_ESC) == PRESSED then Quit() end

		-- save a screenshot on F5
		if GetKeyState(KEY_F5) == PRESSED then
			if SaveScreenShot("screenshot.png") == true then
				print("screen shot saved to " .. GetCurrentDirectory() .. "/screenshot.png")
			end
		end

		-- hide debug draw and menu when space is pressed (for screen shots ;))
		SetSceneDebugDraw(editor.scene.object, true)
		SetGuiObjectVisible(editor.gui.object, true)
		if GetKeyState(KEY_SPACE) ~= UP then
			SetSceneDebugDraw(editor.scene.object, false)
			SetGuiObjectVisible(editor.gui.object, false)
		end

		ediUpdateScene()
		ediUpdateGui()
	end
end

function ediDeinit()
	editor = nil
end

-- initialize and run EditorELF

ediInit()
ediRun()
ediDeinit()

