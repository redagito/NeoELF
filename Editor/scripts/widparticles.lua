
function ediInitWidParticles()
	editor.gui.properties.widParticles = {}
	local widParticles = editor.gui.properties.widParticles

	widParticles.object = CreateScreen(editor.gui.properties.object, "WidParticles", 0, 25, 248, 425)

	widParticles.maxCountAttr = ediCreateIntGroupAttribute(widParticles.object, "Max Count", 4, 22, 0, nil, "", 1, "SetParticlesMaxCount")
	widParticles.spawnAttr = ediCreateBooleanAttribute(widParticles.object, "Spawn", 4, 122, false, "SetParticlesSpawn")
	widParticles.spawnSecAttr = ediCreateIntGroupAttribute(widParticles.object, "Spawn/Sec", 4, 142, 0, nil, "", 1, "SetParticlesSpawnCount")
	widParticles.gravityAttr = ediCreateFloatGroupAttribute(widParticles.object, "Gravity", 4, 162, nil, nil, "", 3, "SetParticlesGravity")
	widParticles.sizeAttr = ediCreateFloatGroupAttribute(widParticles.object, "Size", 4, 182, 0, nil, "", 2, "SetParticlesSize")
	widParticles.sizeGrowAttr = ediCreateFloatGroupAttribute(widParticles.object, "Size Grow", 4, 202, 0, nil, "", 2, "SetParticlesSizeGrowth")
	widParticles.rotationAttr = ediCreateFloatGroupAttribute(widParticles.object, "Rotation", 4, 222, nil, nil, "", 2, "SetParticlesRotation")
	widParticles.rotGrowAttr = ediCreateFloatGroupAttribute(widParticles.object, "Rot Grow", 4, 242, nil, nil, "", 2, "SetParticlesRotationGrowth")
	widParticles.lifeSpanAttr = ediCreateFloatGroupAttribute(widParticles.object, "Life Span", 4, 262, 0, nil, "", 2, "SetParticlesLifeSpan")
	widParticles.fadeSpeedAttr = ediCreateFloatGroupAttribute(widParticles.object, "FadeSpeed", 4, 282, 0, nil, "", 2, "SetParticlesFadeSpeed")
	widParticles.posMinAttr = ediCreateFloatGroupAttribute(widParticles.object, "Pos Min", 4, 302, nil, nil, "", 3, "SetParticlesPositionMin")
	widParticles.posMaxAttr = ediCreateFloatGroupAttribute(widParticles.object, "Pos Max", 4, 322, nil, nil, "", 3, "SetParticlesPositionMax")
	widParticles.velMinAttr = ediCreateFloatGroupAttribute(widParticles.object, "Vel Min", 4, 342, nil, nil, "", 3, "SetParticlesVelocityMin")
	widParticles.velMaxAttr = ediCreateFloatGroupAttribute(widParticles.object, "Vel Max", 4, 362, nil, nil, "", 3, "SetParticlesVelocityMax")
	widParticles.colorMinAttr = ediCreateFloatGroupAttribute(widParticles.object, "Color Min", 4, 382, 0, nil, "", 4, "SetParticlesColorMin")
	widParticles.colorMaxAttr = ediCreateFloatGroupAttribute(widParticles.object, "Color Max", 4, 402, 0, nil, "", 4, "SetParticlesColorMax")

	widParticles.drawModeLab = CreateLabel(widParticles.object, "DrawModeLab", 4, 46, "Draw Mode")
	widParticles.textureLab = CreateLabel(widParticles.object, "TextureLab", 4, 66, "Texture")
	widParticles.modelLab = CreateLabel(widParticles.object, "ModelLab", 4, 86, "Model")
	widParticles.entityLab = CreateLabel(widParticles.object, "EntityLab", 4, 106, "Entity")

	SetGuiObjectColor(widParticles.drawModeLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widParticles.textureLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widParticles.modelLab, 1.0, 1.0, 1.0, 0.6)
	SetGuiObjectColor(widParticles.entityLab, 1.0, 1.0, 1.0, 0.6)

	widParticles.transBut = CreateButton(widParticles.object, "TransBut", 80, 42, 82, 18, "Trans")
	widParticles.addBut = CreateButton(widParticles.object, "AddBut", 162, 42, 82, 18, "Add")
	widParticles.textureTxf = CreateTextField(widParticles.object, "TextureTxf", 80, 62, 144, "")
	widParticles.openTexture = CreateButton(widParticles.object, "OpenTexture", 226, 61, 18, 18, "..")
	widParticles.modelTxf = CreateTextField(widParticles.object, "ModelTxf", 80, 82, 164, "")
	widParticles.entityTxf = CreateTextField(widParticles.object, "EntityTxf", 80, 102, 164, "")

	widParticles.minimize = CreateButton(widParticles.object, "Particles", 1, 1, 246, 14, "Particles")

	SetScreenSize(widParticles.object, 248, 16)
end

function ediUpdateWidParticlesSelection()
	local widParticles = editor.gui.properties.widParticles
	local sel = ediGetSelection()

	local gravity = GetParticlesGravity(sel)
	local posMin = GetParticlesPositionMin(sel)
	local posMax = GetParticlesPositionMax(sel)
	local velMin = GetParticlesVelocityMin(sel)
	local velMax = GetParticlesVelocityMax(sel)
	local colorMin = GetParticlesColorMin(sel)
	local colorMax = GetParticlesColorMax(sel)

	ediSetIntGroupAttributeValues(widParticles.maxCountAttr, {GetParticlesMaxCount(sel)})
	ediSetBooleanAttributeValue(widParticles.spawnAttr, GetParticlesSpawn(sel))
	ediSetIntGroupAttributeValues(widParticles.spawnSecAttr, {GetParticlesSpawnCount(sel)})
	ediSetFloatGroupAttributeValues(widParticles.gravityAttr, {gravity.x, gravity.y, gravity.z})
	ediSetFloatGroupAttributeValues(widParticles.sizeAttr, {GetParticlesSizeMin(sel), GetParticlesSizeMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.sizeGrowAttr, {GetParticlesSizeGrowthMin(sel), GetParticlesSizeGrowthMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.rotationAttr, {GetParticlesRotationMin(sel), GetParticlesRotationMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.rotGrowAttr, {GetParticlesRotationGrowthMin(sel), GetParticlesRotationGrowthMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.lifeSpanAttr, {GetParticlesLifeSpanMin(sel), GetParticlesLifeSpanMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.fadeSpeedAttr, {GetParticlesFadeSpeedMin(sel), GetParticlesFadeSpeedMax(sel)})
	ediSetFloatGroupAttributeValues(widParticles.posMinAttr, {posMin.x, posMin.y, posMin.z})
	ediSetFloatGroupAttributeValues(widParticles.posMaxAttr, {posMax.x, posMax.y, posMax.z})
	ediSetFloatGroupAttributeValues(widParticles.velMinAttr, {velMin.x, velMin.y, velMin.z})
	ediSetFloatGroupAttributeValues(widParticles.velMaxAttr, {velMax.x, velMax.y, velMax.z})
	ediSetFloatGroupAttributeValues(widParticles.colorMinAttr, {colorMin.r, colorMin.g, colorMin.b, colorMin.a})
	ediSetFloatGroupAttributeValues(widParticles.colorMaxAttr, {colorMax.r, colorMax.g, colorMax.b, colorMax.a})

	SetGuiObjectActive(widParticles.transBut, true)
	SetGuiObjectActive(widParticles.addBut, true)

	if GetParticlesDrawMode(sel) == TRANSPARENT then SetGuiObjectActive(widParticles.transBut, false)
	elseif GetParticlesDrawMode(sel) == ADD then SetGuiObjectActive(widParticles.addBut, false) end
end

function ediUpdateWidParticles()
	local widParticles = editor.gui.properties.widParticles
	local sel = ediGetSelection()

	if GetGuiObjectEvent(widParticles.minimize) == CLICKED then
		if GetGuiObjectSize(widParticles.object).y > 16 then
			SetScreenSize(widParticles.object, 248, 16)
		else
			SetScreenSize(widParticles.object, 248, 425)
		end
		ediPackScreensVer(editor.gui.properties, 25)
	end

	if GetGuiObjectEvent(widParticles.transBut) == CLICKED then
		SetParticlesDrawMode(sel, TRANSPARENT)
		SetGuiObjectActive(widParticles.transBut, false)
		SetGuiObjectActive(widParticles.addBut, true)
	end
	if GetGuiObjectEvent(widParticles.addBut) == CLICKED then
		SetParticlesDrawMode(sel, ADD)
		SetGuiObjectActive(widParticles.transBut, true)
		SetGuiObjectActive(widParticles.addBut, false)
	end

	ediUpdateAttribute(widParticles.maxCountAttr, sel)
	ediUpdateAttribute(widParticles.spawnAttr, sel)
	ediUpdateAttribute(widParticles.spawnSecAttr, sel)
	ediUpdateAttribute(widParticles.gravityAttr, sel)
	ediUpdateAttribute(widParticles.sizeAttr, sel)
	ediUpdateAttribute(widParticles.sizeGrowAttr, sel)
	ediUpdateAttribute(widParticles.rotationAttr, sel)
	ediUpdateAttribute(widParticles.rotGrowAttr, sel)
	ediUpdateAttribute(widParticles.lifeSpanAttr, sel)
	ediUpdateAttribute(widParticles.fadeSpeedAttr, sel)
	ediUpdateAttribute(widParticles.posMinAttr, sel)
	ediUpdateAttribute(widParticles.posMaxAttr, sel)
	ediUpdateAttribute(widParticles.velMinAttr, sel)
	ediUpdateAttribute(widParticles.velMaxAttr, sel)
	ediUpdateAttribute(widParticles.colorMinAttr, sel)
	ediUpdateAttribute(widParticles.colorMaxAttr, sel)
end

