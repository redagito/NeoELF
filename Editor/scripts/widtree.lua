
function ediInitWidTree()
	editor.gui.toolbox.widTree = {}
	local widTree = editor.gui.toolbox.widTree

	widTree.object = CreateScreen(editor.gui.toolbox.object, "Tree", 8, 0, 248, GetWindowHeight()/2)

	widTree.minimize = CreateButton(widTree.object, "Tree", 1, 1, 246, 14, "Tree")
end

function ediUpdateWidTree()
	local widTree = editor.gui.toolbox.widTree

	if GetGuiObjectEvent(widTree.minimize) == CLICKED then
		if GetGuiObjectSize(widTree.object).y > 16 then
			SetScreenSize(widTree.object, 248, 16)	
		else
			SetScreenSize(widTree.object, 248, GetWindowHeight()/2)
		end
		ediPackToolboxScreens()
	end
end

