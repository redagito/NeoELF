
EDI_MENU = 0
EDI_EDIT = 1

function ediInitProperties()
	editor.gui.properties = {}
	local properties = editor.gui.properties

	properties.tab = EDI_MENU

	properties.object = CreateScreen(editor.gui.object, "WinProperties", GetWindowWidth()-256, 0, 256, GetWindowHeight())

	properties.menu = CreateButton(properties.object, "Menu", 1, 1, 123, 18, "Menu")
	properties.edit = CreateButton(properties.object, "Menu", 124, 1, 123, 18, "Edit")

	properties.minimize = CreateButton(properties.object, "Minimize", 249, 0, 6, GetWindowHeight(), "")

	ediInitWidFile()
	ediInitWidOptions()
	ediInitWidCreate()

	ediInitWidActor()
	ediInitWidPhysics()
	ediInitWidLight()
	ediInitWidShaft()
	ediInitWidCamera()
	ediInitWidEntity()
	ediInitWidMaterials()
	ediInitWidParticles()

	ediSetPropertiesTab(EDI_MENU)

	ediPackScreensVer(properties, 25)
end

function ediSetPropertiesTab(tab)
	local properties = editor.gui.properties

	properties.tab = tab

	ediHidePropertiesWidgets()

	if properties.tab == EDI_MENU then
		SetGuiObjectActive(properties.menu, false)
		SetGuiObjectActive(properties.edit, true)
		SetGuiObjectVisible(properties.widFile.object, true)
		SetGuiObjectVisible(properties.widOptions.object, true)
		SetGuiObjectVisible(properties.widCreate.object, true)
	elseif properties.tab == EDI_EDIT then
		local sel = ediGetSelection()
		SetGuiObjectActive(properties.menu, true)
		SetGuiObjectActive(properties.edit, false)
		SetGuiObjectVisible(properties.widActor.object, true)
		SetGuiObjectVisible(properties.widPhysics.object, true)
		if GetObjectType(sel) == CAMERA then
			SetGuiObjectVisible(properties.widCamera.object, true)
		elseif GetObjectType(sel) == ENTITY then
			SetGuiObjectVisible(properties.widEntity.object, true)
			SetGuiObjectVisible(properties.widMaterials.object, true)
		elseif GetObjectType(sel) == LIGHT then
			SetGuiObjectVisible(properties.widLight.object, true)
			SetGuiObjectVisible(properties.widShaft.object, true)
		elseif GetObjectType(sel) == PARTICLES then
			SetGuiObjectVisible(properties.widParticles.object, true)
		elseif GetObjectType(sel) == SPRITE then
		end
	end

	ediPackScreensVer(properties, 25)
end

function ediUpdatePropertiesSelection()
	local sel = ediGetSelection()

	ediUpdateWidActorSelection()
	ediUpdateWidPhysicsSelection()

	if GetObjectType(sel) == CAMERA then
		ediUpdateWidCameraSelection()
	elseif GetObjectType(sel) == ENTITY then
		ediUpdateWidEntitySelection()
		ediUpdateWidMaterialsSelection()
	elseif GetObjectType(sel) == LIGHT then
		ediUpdateWidLightSelection()
		ediUpdateWidShaftSelection()
	elseif GetObjectType(sel) == PARTICLES then
		ediUpdateWidParticlesSelection()
	elseif GetObjectType(sel) == SPRITE then
	end

	if editor.gui.properties.tab == EDI_EDIT then
		ediSetPropertiesTab(EDI_EDIT)
	end
end

function ediUpdatePropertiesTab()
	local properties = editor.gui.properties

	if properties.tab == EDI_MENU then
		ediUpdateWidFile()
		ediUpdateWidOptions()
		ediUpdateWidCreate()
	elseif properties.tab == EDI_EDIT then
		ediUpdateWidActor()
		ediUpdateWidPhysics()
		if GetObjectType(editor.scene.selection) == CAMERA then
			ediUpdateWidCamera()
		elseif GetObjectType(editor.scene.selection) == ENTITY then
			ediUpdateWidEntity()
			ediUpdateWidMaterials()
		elseif GetObjectType(editor.scene.selection) == LIGHT then
			ediUpdateWidLight()
			ediUpdateWidShaft()
		elseif GetObjectType(editor.scene.selection) == PARTICLES then
			ediUpdateWidParticles()
		elseif GetObjectType(editor.scene.selection) == SPRITE then
		end
	end
end

function ediHidePropertiesWidgets()
	local properties = editor.gui.properties

	for k, v in pairs(properties) do
		if type(v) == 'table' and v.object ~= nil then
			SetGuiObjectVisible(v.object, false)
		end
	end
end

function ediMinimizeProperties()
	local properties = editor.gui.properties

	SetScreenSize(properties.object, 8, GetWindowHeight())
	SetGuiObjectPosition(properties.object, GetWindowWidth()-8, 0)
	SetGuiObjectPosition(properties.minimize, 1, 0)

	ediHidePropertiesWidgets()
end

function ediExpandProperties()
	local properties = editor.gui.properties

	SetScreenSize(properties.object, 256, GetWindowHeight())
	SetGuiObjectPosition(properties.object, GetWindowWidth()-256, 0)
	SetGuiObjectPosition(properties.minimize, 249, 0)

	ediSetPropertiesTab(properties.tab)
end

function ediUpdateProperties()
	local properties = editor.gui.properties

	if GetGuiObjectEvent(properties.minimize) == CLICKED or
		(GetGuiActiveTextField(editor.gui.object) == nil and GetKeyState(KEY_2) == PRESSED) then
		if GetGuiObjectSize(properties.object).x > 8 then
			ediMinimizeProperties()
		else ediExpandProperties() end
	end

	if GetGuiObjectEvent(properties.menu) == CLICKED then
		ediSetPropertiesTab(EDI_MENU)
	elseif GetGuiObjectEvent(properties.edit) == CLICKED then
		if ediGetSelection() ~= nil then
			ediSetPropertiesTab(EDI_EDIT)
		end
	end

	ediUpdatePropertiesTab()
end

