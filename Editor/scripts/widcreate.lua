
function ediInitWidCreate()
	editor.gui.properties.widCreate = {}
	local widCreate = editor.gui.properties.widCreate

	widCreate.object = CreateScreen(editor.gui.properties.object, "WidCreate", 0, 25, 248, 104)

	widCreate.camera = CreateButton(widCreate.object, "Camera", 10, 22, 228, 18, "Camera")
	widCreate.entity = CreateButton(widCreate.object, "Entity", 10, 42, 228, 18, "Entity")
	widCreate.light = CreateButton(widCreate.object, "Light", 10, 62, 228, 18, "Light")
	widCreate.particles = CreateButton(widCreate.object, "Particles", 10, 82, 228, 18, "Particles")

	widCreate.minimize = CreateButton(widCreate.object, "Create", 1, 1, 246, 14, "Create")
end

function ediUpdateWidCreate()
	local widCreate = editor.gui.properties.widCreate

	if GetGuiObjectEvent(widCreate.minimize) == CLICKED then
		if GetGuiObjectSize(widCreate.object).y > 16 then
			SetScreenSize(widCreate.object, 248, 16)
		else
			SetScreenSize(widCreate.object, 248, 104)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widCreate.camera) == CLICKED then
		cam = CreateCamera("Camera")
		SetActorRotation(cam, 90.0, 0.0, 0.0)
		AddSceneCamera(editor.scene.object, cam)
		ediSetSelection(cam)
	end

	if GetGuiObjectEvent(widCreate.entity) == CLICKED then
		ent = CreateEntity("Entity")
		AddSceneEntity(editor.scene.object, ent)
		ediSetSelection(ent)
	end

	if GetGuiObjectEvent(widCreate.light) == CLICKED then
		lig = CreateLight("Light")
		AddSceneLight(editor.scene.object, lig)
		ediSetSelection(lig)
	end

	if GetGuiObjectEvent(widCreate.particles) == CLICKED then
		par = CreateParticles("Particles", 150)
		AddSceneParticles(editor.scene.object, par)
		ediSetSelection(par)
	end
end

